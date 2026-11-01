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


COMPONENT_KEYS = frozenset({"namespace", "tokens", "symbols", "extract"})
UNSUPPORTED_COMPONENT_KEYS = frozenset({"measurements", "colors"})
NAMESPACE_PATTERN = re.compile(r"[a-z][a-z0-9_]*")
EXTRACT_NAME_PATTERN = re.compile(r"[A-Z][A-Za-z0-9]*")


@dataclass(frozen=True)
class TokensSource:
    file: str
    include: tuple[str, ...] | None


@dataclass(frozen=True)
class SymbolsSource:
    file: str
    names: tuple[str, ...]


@dataclass(frozen=True)
class ExtractSource:
    name: str
    file: str
    match: str


@dataclass(frozen=True)
class Component:
    path: Path
    namespace: str
    tokens: TokensSource | None
    symbols: tuple[SymbolsSource, ...]
    extract: tuple[ExtractSource, ...]


def _read_yaml(path: Path) -> Any:
    try:
        return yaml.safe_load(path.read_text(encoding="utf-8"))
    except OSError as error:
        _fail(path, "file", f"cannot read: {error}")
    except yaml.YAMLError as error:
        _fail(path, "yaml", f"cannot parse: {error}")


def _validate_component_keys(
        path: Path, field: str, value: Any,
        required: frozenset[str], optional: frozenset[str] = frozenset()) -> None:
    if not isinstance(value, dict):
        _fail(path, field, "must be a mapping")
    for key in value:
        if key in UNSUPPORTED_COMPONENT_KEYS:
            _fail(path, str(key), "not supported yet")
    missing = sorted(required - set(value))
    unexpected = sorted(str(key) for key in set(value) - required - optional)
    details = []
    if missing:
        details.append(f"missing keys: {missing}")
    if unexpected:
        details.append(f"unexpected keys: {unexpected}")
    if details:
        _fail(path, field, "; ".join(details))


def _validate_list(path: Path, field: str, value: Any) -> list[Any]:
    if not isinstance(value, list):
        _fail(path, field, "must be a list")
    return value


def _validate_names(path: Path, field: str, value: Any) -> tuple[str, ...]:
    if not isinstance(value, list) or not value:
        _fail(path, field, "must be a non-empty list of strings")
    for index, entry in enumerate(value):
        if not isinstance(entry, str) or not entry:
            _fail(path, f"{field}[{index}]", "must be a non-empty string")
    duplicates = sorted({entry for entry in value if value.count(entry) > 1})
    if duplicates:
        _fail(path, field, f"duplicate entries: {duplicates}")
    return tuple(value)


def _validate_reference(config: Config, path: Path, field: str, value: Any) -> str:
    if not isinstance(value, str) or not value:
        _fail(path, field, "must be a non-empty string")
    try:
        config.resolve(value)
    except ConfigError as error:
        _fail(path, field, str(error))
    return value


def _load_tokens(config: Config, path: Path, value: Any) -> TokensSource:
    _validate_component_keys(
        path, "tokens", value, frozenset({"file"}), frozenset({"include"}))
    file = _validate_reference(config, path, "tokens.file", value["file"])
    include = None
    if "include" in value:
        include = _validate_names(path, "tokens.include", value["include"])
    return TokensSource(file=file, include=include)


def _load_symbols(config: Config, path: Path, index: int, value: Any) -> SymbolsSource:
    field = f"symbols[{index}]"
    _validate_component_keys(path, field, value, frozenset({"file", "names"}))
    file = _validate_reference(config, path, f"{field}.file", value["file"])
    names = _validate_names(path, f"{field}.names", value["names"])
    return SymbolsSource(file=file, names=names)


def _load_extract(config: Config, path: Path, index: int, value: Any) -> ExtractSource:
    field = f"extract[{index}]"
    _validate_component_keys(path, field, value, frozenset({"name", "file", "match"}))
    name = value["name"]
    if not isinstance(name, str) or not EXTRACT_NAME_PATTERN.fullmatch(name):
        _fail(path, f"{field}.name", "must be a PascalCase identifier")
    file = _validate_reference(config, path, f"{field}.file", value["file"])
    match = value["match"]
    if not isinstance(match, str) or not match:
        _fail(path, f"{field}.match", "must be a non-empty string")
    return ExtractSource(name=name, file=file, match=match)


def _load_component(config: Config, path: Path) -> Component:
    raw = _read_yaml(path)
    _validate_component_keys(
        path, "top-level", raw, frozenset({"namespace"}), COMPONENT_KEYS - {"namespace"})

    namespace = raw["namespace"]
    if not isinstance(namespace, str) or not NAMESPACE_PATTERN.fullmatch(namespace):
        _fail(path, "namespace", "must match [a-z][a-z0-9_]*")
    if namespace != path.stem:
        _fail(path, "namespace", f"must equal the file name {path.stem!r}")

    tokens = None
    if "tokens" in raw:
        tokens = _load_tokens(config, path, raw["tokens"])
    symbols = tuple(
        _load_symbols(config, path, index, entry)
        for index, entry in enumerate(_validate_list(path, "symbols", raw.get("symbols", [])))
    )
    extract = tuple(
        _load_extract(config, path, index, entry)
        for index, entry in enumerate(_validate_list(path, "extract", raw.get("extract", [])))
    )
    return Component(
        path=path,
        namespace=namespace,
        tokens=tokens,
        symbols=symbols,
        extract=extract,
    )


def load_components(config: Config) -> tuple[Component, ...]:
    return tuple(
        _load_component(config, path)
        for path in sorted(config.output.glob("*.yaml"))
    )
