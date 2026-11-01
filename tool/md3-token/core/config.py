from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path, PurePosixPath
from typing import Any, NoReturn

import yaml

TOOL_ROOT = Path(__file__).resolve().parents[1]
REPO_ROOT = TOOL_ROOT.parents[1]
CONFIG_PATH = TOOL_ROOT / "config.yaml"

TOP_LEVEL_KEYS = frozenset(
    {"androidx", "cache", "sparse", "modules", "output"})
ANDROIDX_KEYS = frozenset({"remote", "commit"})
COMMIT_PATTERN = re.compile(r"[0-9a-f]{40}")


class ConfigError(Exception):
    pass


@dataclass(frozen=True)
class Androidx:
    remote: str
    commit: str


@dataclass(frozen=True)
class Config:
    androidx: Androidx
    cache: Path
    sparse: tuple[str, ...]
    modules: dict[str, PurePosixPath]
    output: Path

    def module_path(self, alias: str) -> Path:
        try:
            relative = self.modules[alias]
        except KeyError:
            available = ", ".join(sorted(self.modules))
            raise ConfigError(f"unknown module alias {
                              alias!r} (available: {available})") from None
        return self.cache / relative

    def resolve(self, ref: str) -> Path:
        alias, separator, relative = ref.partition(":")
        if not separator or not alias or not relative:
            raise ConfigError(f"invalid reference {
                              ref!r} (expected <alias>:<relative file>)")
        relative_path = PurePosixPath(relative)
        if relative_path.is_absolute() or ".." in relative_path.parts:
            raise ConfigError(f"invalid reference {
                              ref!r} (file must stay in module)")
        return self.module_path(alias) / relative_path


def _fail(path: Path, field: str, problem: str) -> NoReturn:
    raise ConfigError(f"{path}: {field}: {problem}")


def _is_relative(value: str) -> bool:
    pure_path = PurePosixPath(value)
    return not pure_path.is_absolute() and ".." not in pure_path.parts


def _validate_path(path: Path, field: str, value: Any) -> str:
    if not isinstance(value, str) or not value:
        _fail(path, field, "must be a non-empty string")
    if not _is_relative(value):
        _fail(path, field, f"must be a relative path without '..': {value!r}")
    return value


def _validate_keys(path: Path, field: str, value: Any, expected: frozenset[str]) -> None:
    if not isinstance(value, dict):
        _fail(path, field, "must be a mapping")
    keys = set(value)
    if keys != expected:
        missing = sorted(str(key) for key in expected - keys)
        unexpected = sorted(str(key) for key in keys - expected)
        details = []
        if missing:
            details.append(f"missing keys: {missing}")
        if unexpected:
            details.append(f"unexpected keys: {unexpected}")
        _fail(path, field, "; ".join(details))


def load_config(path: Path = CONFIG_PATH) -> Config:
    path = Path(path)
    try:
        raw = yaml.safe_load(path.read_text(encoding="utf-8"))
    except OSError as error:
        _fail(path, "file", f"cannot read: {error}")
    except yaml.YAMLError as error:
        _fail(path, "yaml", f"cannot parse: {error}")

    _validate_keys(path, "top-level", raw, TOP_LEVEL_KEYS)

    _validate_keys(path, "androidx", raw["androidx"], ANDROIDX_KEYS)
    remote = raw["androidx"]["remote"]
    if not isinstance(remote, str) or not remote:
        _fail(path, "androidx.remote", "must be a non-empty string")
    commit = raw["androidx"]["commit"]
    if not isinstance(commit, str) or not COMMIT_PATTERN.fullmatch(commit):
        _fail(path, "androidx.commit",
              "must be a 40-character lowercase hex string")

    cache_value = _validate_path(path, "cache", raw["cache"])

    sparse_value = raw["sparse"]
    if not isinstance(sparse_value, list) or not sparse_value:
        _fail(path, "sparse", "must be a non-empty list of strings")
    sparse = tuple(
        _validate_path(path, f"sparse[{index}]", entry) for index, entry in enumerate(sparse_value)
    )

    modules_value = raw["modules"]
    if not isinstance(modules_value, dict) or not modules_value:
        _fail(path, "modules", "must be a non-empty mapping of strings")
    modules: dict[str, PurePosixPath] = {}
    for alias, module in modules_value.items():
        if not isinstance(alias, str) or not alias:
            _fail(path, "modules",
                  f"alias must be a non-empty string: {alias!r}")
        module_value = _validate_path(path, f"modules.{alias}", module)
        if not any(module_value.startswith(prefix) for prefix in sparse):
            _fail(
                path,
                f"modules.{alias}",
                f"module {module_value!r} is not under any sparse path",
            )
        modules[alias] = PurePosixPath(module_value)

    output_value = _validate_path(path, "output", raw["output"])

    return Config(
        androidx=Androidx(remote=remote, commit=commit),
        cache=REPO_ROOT / cache_value,
        sparse=sparse,
        modules=modules,
        output=REPO_ROOT / output_value,
    )
