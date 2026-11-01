from __future__ import annotations

import re
from pathlib import Path

from core.config import REPO_ROOT, ConfigError

COLOR_SCHEME_PATH = REPO_ROOT / "src/creeper-qt/utility/theme/color-scheme.hh"

_STRUCT = re.compile(r"struct\s+ColorScheme\s*\{(?P<body>.*?)\n\};", re.DOTALL)
_FIELD = re.compile(r"^\s*QColor\s+([a-z][a-z0-9_]*)\s*;", re.MULTILINE)
_CAMEL_BOUNDARY = re.compile(r"(?<!^)(?=[A-Z])")


def snake_case(name: str) -> str:
    return _CAMEL_BOUNDARY.sub("_", name).lower()


def color_scheme_fields(path: Path | None = None) -> frozenset[str]:
    if path is None:
        path = COLOR_SCHEME_PATH
    try:
        text = path.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        raise ConfigError(f"{path}: cannot read: {error}") from None
    match = _STRUCT.search(text)
    if match is None:
        raise ConfigError(f"{path}: struct ColorScheme not found")
    fields = frozenset(_FIELD.findall(match.group("body")))
    if not fields:
        raise ConfigError(f"{path}: struct ColorScheme has no QColor fields")
    return fields
