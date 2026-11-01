from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path

from core import render
from core.config import Config, ConfigError

ALLOWED_INCLUDES = frozenset({
    "creeper-qt/utility/theme/color-scheme.hh",
    "creeper-qt/utility/math/cubic-bezier.hh",
})
COMPONENT_PREFIXES = (
    "creeper-qt/widget/",
    "creeper-qt/layout/",
)
COMPONENT_REASON = "details must not depend on components"
UNLISTED_REASON = "not in the allowed include list"
INCLUDE_PATTERN = re.compile(r'^\s*#\s*include\s*(?P<include><[^>]*>|"[^"]*")')


@dataclass(frozen=True)
class Violation:
    path: Path
    line: int
    include: str
    reason: str


def headers(config: Config) -> list[Path]:
    return sorted(config.output.rglob("*.hh"))


def _strip_comments(line: str, in_block: bool) -> tuple[str, bool]:
    kept: list[str] = []
    index = 0
    while index < len(line):
        if in_block:
            end = line.find("*/", index)
            if end < 0:
                return "".join(kept), True
            index = end + 2
            in_block = False
            continue
        block = line.find("/*", index)
        comment = line.find("//", index)
        if comment >= 0 and (block < 0 or comment < block):
            kept.append(line[index:comment])
            return "".join(kept), False
        if block < 0:
            kept.append(line[index:])
            return "".join(kept), False
        kept.append(line[index:block])
        index = block + 2
        in_block = True
    return "".join(kept), in_block


def _reason(specifier: str, prefix: str) -> str | None:
    if specifier.startswith("<"):
        return None
    inner = specifier[1:-1]
    if inner.startswith(COMPONENT_PREFIXES):
        return COMPONENT_REASON
    if inner in ALLOWED_INCLUDES or inner.startswith(prefix):
        return None
    return UNLISTED_REASON


def verify(config: Config) -> list[Violation]:
    prefix = f"{render.include_path(config)}/"
    violations: list[Violation] = []
    for path in headers(config):
        try:
            lines = path.read_text(encoding="utf-8").splitlines()
        except (OSError, UnicodeError) as error:
            raise ConfigError(f"{path}: cannot read: {error}") from None
        in_block = False
        for number, line in enumerate(lines, start=1):
            code, in_block = _strip_comments(line, in_block)
            match = INCLUDE_PATTERN.match(code)
            if match is None:
                continue
            specifier = match.group("include")
            reason = _reason(specifier, prefix)
            if reason is None:
                continue
            violations.append(Violation(
                path=path,
                line=number,
                include=specifier,
                reason=reason,
            ))
    return violations
