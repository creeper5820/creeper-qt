from __future__ import annotations

import argparse
import difflib
from pathlib import Path

from command import generate
from core import cache, log as log_module, rules
from core.config import REPO_ROOT, Config, ConfigError, load_config

log = log_module.get("check")

DIFF_LIMIT = 40


def register(sub: argparse._SubParsersAction) -> None:
    parser = sub.add_parser("check", help="校验生成结果与仓库一致并检查 include 依赖")
    parser.set_defaults(run=run)


def _relative(path: Path) -> str:
    try:
        return path.relative_to(REPO_ROOT).as_posix()
    except ValueError:
        return str(path)


def _diff(label: str, disk: str, generated: str) -> list[str]:
    lines = list(difflib.unified_diff(
        disk.splitlines(),
        generated.splitlines(),
        fromfile=f"{label} (disk)",
        tofile=f"{label} (generated)",
        lineterm="",
    ))
    if len(lines) <= DIFF_LIMIT:
        return lines
    return [*lines[:DIFF_LIMIT], f"... {len(lines) - DIFF_LIMIT} more lines"]


def _compare(
        targets: dict[Path, str]
) -> tuple[list[Path], list[tuple[Path, str, str]]]:
    missing: list[Path] = []
    outdated: list[tuple[Path, str, str]] = []
    for path in sorted(targets):
        try:
            disk = path.read_text(encoding="utf-8")
        except FileNotFoundError:
            missing.append(path)
            continue
        except (OSError, UnicodeError) as error:
            raise ConfigError(f"{path}: cannot read: {error}") from None
        if disk != targets[path]:
            outdated.append((path, disk, targets[path]))
    return missing, outdated


def check(config: Config) -> int:
    targets = generate.build_all(config)
    missing, outdated = _compare(targets)
    stale = generate.stale_files(config, targets)
    violations = rules.verify(config)
    scanned = len(rules.headers(config))

    for path in missing:
        log.info("missing %s", _relative(path))
    for path, disk, generated in outdated:
        label = _relative(path)
        log.info("outdated %s", label)
        for line in _diff(label, disk, generated):
            log.info("%s", line)
    for path in stale:
        log.info("stale %s", _relative(path))
    for violation in violations:
        log.info(
            "%s:%d: #include %s: %s",
            _relative(violation.path),
            violation.line,
            violation.include,
            violation.reason,
        )

    if not (missing or outdated or stale or violations):
        print(
            f"check passed ({len(targets)} generated files, {scanned} headers scanned)",
            flush=True)
        return 0
    print(
        f"check failed: {len(missing)} missing, {len(outdated)} outdated, "
        f"{len(stale)} stale, {len(violations)} include violations",
        flush=True)
    return 1


def run(args: argparse.Namespace) -> int:
    config = load_config()
    cache.require(config)
    return check(config)
