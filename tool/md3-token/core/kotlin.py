from __future__ import annotations

import difflib
import re
from dataclasses import dataclass
from pathlib import Path

from core.config import ConfigError
from core.model import (
    Dp,
    Float,
    Sp,
    Unknown,
    Value,
    classify,
)


class KotlinError(ConfigError):
    pass


@dataclass(frozen=True)
class Location:
    path: Path
    line: int


@dataclass(frozen=True)
class Declaration:
    name: str
    value: Value
    raw: str
    location: Location


@dataclass(frozen=True)
class KotlinFile:
    path: Path
    version: str | None
    declarations: tuple[Declaration, ...]

    def get(self, name: str) -> Declaration:
        matches = [
            declaration for declaration in self.declarations
            if declaration.name == name
        ]
        if not matches:
            names = sorted({declaration.name for declaration in self.declarations})
            suggestions = difflib.get_close_matches(
                name, names, n=3, cutoff=0.6)
            message = f"{self.path}: symbol {name} not found"
            if suggestions:
                message += f" (did you mean {', '.join(suggestions)}?)"
            raise KotlinError(message)
        if len(matches) > 1:
            lines = ", ".join(
                str(declaration.location.line) for declaration in matches)
            raise KotlinError(
                f"{self.path}: symbol {name} is ambiguous (lines {lines})")
        return matches[0]

    def require(self, name: str, *kinds: type[Value]) -> Declaration:
        declaration = self.get(name)
        if isinstance(declaration.value, kinds):
            return declaration
        if isinstance(declaration.value, Unknown):
            detail = "unsupported Kotlin expression"
        else:
            expected = ", ".join(kind.__name__ for kind in kinds)
            detail = f"expected {expected}, got {type(declaration.value).__name__}"
        raise KotlinError(
            f"{self.path}:{declaration.location.line}: {name} = "
            f"{declaration.raw}: {detail}")


_VERSION = re.compile(r"^\s*//\s*VERSION:\s*(\S+)\s*$", re.MULTILINE)
_DECLARATION = re.compile(r"\bval\s+([A-Za-z_]\w*)")
_GETTER = re.compile(
    r"[ \t\r\n]*"
    r"(?P<prefix>(?:(?:@[A-Za-z_][\w.]*(?:\([^)]*\))?"
    r"|inline|public|private|internal|protected)\s+)*)"
    r"get\s*\(\s*\)\s*=",
)
_CAPTURE_NUMBER = re.compile(r"[+-]?\d+(?:\.\d+)?")
_CAPTURE_UNIT = re.compile(r"\s*\.(dp|sp)\b")
_FLOAT_SUFFIX = re.compile(r"f(?![A-Za-z0-9_])")


def _mask(text: str) -> tuple[str, str]:
    commentless = list(text)
    masked = list(text)

    def cover(start: int, end: int) -> None:
        for index in range(start, end):
            if text[index] != "\n":
                masked[index] = "_"

    def blank_comments(start: int, end: int) -> None:
        for index in range(start, end):
            if text[index] != "\n":
                commentless[index] = " "
                masked[index] = " "

    length = len(text)
    position = 0
    while position < length:
        if text.startswith("//", position):
            end = text.find("\n", position)
            if end < 0:
                end = length
            blank_comments(position, end)
            position = end
        elif text.startswith("/*", position):
            depth = 1
            end = position + 2
            while end < length and depth:
                if text.startswith("/*", end):
                    depth += 1
                    end += 2
                elif text.startswith("*/", end):
                    depth -= 1
                    end += 2
                else:
                    end += 1
            blank_comments(position, end)
            position = end
        elif text.startswith('"""', position):
            end = position + 3
            while end < length and not text.startswith('"""', end):
                end += 1
            if end < length:
                end += 3
            cover(position, end)
            position = end
        elif text[position] == '"' or text[position] == "'":
            quote = text[position]
            end = position + 1
            while end < length:
                if text[end] == "\\":
                    end += 2
                elif text[end] == quote:
                    end += 1
                    break
                elif text[end] == "\n":
                    break
                else:
                    end += 1
            cover(position, end)
            position = end
        else:
            position += 1
    return "".join(commentless), "".join(masked)


def _read(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        raise KotlinError(f"{path}: cannot read: {error}") from None


def _value_start(masked: str, match: re.Match[str]) -> tuple[bool, int] | None:
    position = match.end()
    length = len(masked)
    while position < length and masked[position] in " \t":
        position += 1
    if position < length and masked[position] == ":":
        position += 1
        while position < length and masked[position] not in "=\n":
            position += 1
        while position < length and masked[position] in " \t\r":
            position += 1
    if position < length and masked[position] == "=":
        return False, position + 1
    getter = _GETTER.match(masked, position)
    if getter is None:
        return None
    annotated = "@" in getter.group("prefix")
    return annotated, getter.end()


def _read_expression(masked: str, start: int) -> tuple[int, int]:
    length = len(masked)
    position = start
    while position < length and masked[position].isspace():
        position += 1
    begin = position
    depth = {"(": 0, "[": 0, "{": 0}
    closers = {")": "(", "]": "[", "}": "{"}
    while position < length:
        character = masked[position]
        if character in depth:
            depth[character] += 1
        elif character in closers:
            opener = closers[character]
            if depth[opener] == 0:
                break
            depth[opener] -= 1
        elif character == "\n" and not any(depth.values()):
            break
        position += 1
    end = position
    while end > begin and (masked[end - 1].isspace() or masked[end - 1] == ";"):
        end -= 1
    return begin, end


def parse_file(path: Path) -> KotlinFile:
    text = _read(path)
    commentless, masked = _mask(text)
    version_match = _VERSION.search(text)
    version = version_match.group(1) if version_match is not None else None

    declarations: list[Declaration] = []
    parenthesis_depth = 0
    scanned = 0
    for match in _DECLARATION.finditer(masked):
        for character in masked[scanned:match.start()]:
            if character == "(":
                parenthesis_depth += 1
            elif character == ")":
                parenthesis_depth = max(0, parenthesis_depth - 1)
        scanned = match.start()
        if parenthesis_depth > 0:
            continue
        header = _value_start(masked, match)
        if header is None:
            continue
        annotated, value_start = header
        begin, end = _read_expression(masked, value_start)
        raw = " ".join(commentless[begin:end].split())
        value = Unknown(raw) if annotated else classify(raw)
        line = masked.count("\n", 0, match.start(1)) + 1
        declarations.append(
            Declaration(
                name=match.group(1),
                value=value,
                raw=raw,
                location=Location(path=path, line=line),
            )
        )
    return KotlinFile(
        path=path,
        version=version,
        declarations=tuple(declarations),
    )


def extract(path: Path, pattern: str) -> tuple[Value, Location]:
    try:
        compiled = re.compile(pattern, re.MULTILINE)
    except re.error as error:
        raise KotlinError(
            f"{path}: invalid pattern {pattern!r}: {error}") from None
    if compiled.groups != 1:
        raise KotlinError(
            f"{path}: pattern {pattern!r} must have exactly 1 capturing group, "
            f"got {compiled.groups}")
    text = _read(path)
    matches = list(compiled.finditer(text))
    if not matches:
        raise KotlinError(f"{path}: pattern {pattern!r} matched nothing")
    if len(matches) > 1:
        lines = ", ".join(
            str(text.count("\n", 0, match.start()) + 1) for match in matches)
        raise KotlinError(
            f"{path}: pattern {pattern!r} matched {len(matches)} times "
            f"at lines {lines}")
    match = matches[0]
    captured = match.group(1).strip()
    if _CAPTURE_NUMBER.fullmatch(captured):
        tail = text[match.end(1):]
        unit = _CAPTURE_UNIT.match(tail)
        if unit is not None:
            number = float(captured)
            value: Value = Dp(number) if unit.group(1) == "dp" else Sp(number)
        elif _FLOAT_SUFFIX.match(tail):
            value = Float(float(captured))
        else:
            value = classify(captured)
    else:
        value = classify(captured)
    line = text.count("\n", 0, match.start()) + 1
    return value, Location(path=path, line=line)
