from __future__ import annotations

import re
from dataclasses import dataclass


@dataclass(frozen=True)
class Dp:
    value: float


@dataclass(frozen=True)
class Sp:
    value: float


@dataclass(frozen=True)
class Float:
    value: float


@dataclass(frozen=True)
class Int:
    value: int


@dataclass(frozen=True)
class Millis:
    value: int


@dataclass(frozen=True)
class Double:
    value: float


@dataclass(frozen=True)
class Ref:
    owner: str
    name: str


@dataclass(frozen=True)
class KeyId:
    kind: str
    id: int


@dataclass(frozen=True)
class RoundedCorner:
    size: Dp


@dataclass(frozen=True)
class RoundedCornerPerCorner:
    top_start: Dp
    top_end: Dp
    bottom_end: Dp
    bottom_start: Dp


@dataclass(frozen=True)
class CornerSizeValue:
    size: Dp


@dataclass(frozen=True)
class Circle:
    pass


@dataclass(frozen=True)
class Rectangle:
    pass


@dataclass(frozen=True)
class CubicBezier:
    a: float
    b: float
    c: float
    d: float


@dataclass(frozen=True)
class Rgb:
    red: int
    green: int
    blue: int


@dataclass(frozen=True)
class FontWeightName:
    name: str


@dataclass(frozen=True)
class Weight:
    value: int


@dataclass(frozen=True)
class Alias:
    name: str


@dataclass(frozen=True)
class Unknown:
    text: str


Value = (
    Dp
    | Sp
    | Float
    | Int
    | Millis
    | Double
    | Ref
    | KeyId
    | RoundedCorner
    | RoundedCornerPerCorner
    | CornerSizeValue
    | Circle
    | Rectangle
    | CubicBezier
    | Rgb
    | FontWeightName
    | Weight
    | Alias
    | Unknown
)

_NUMBER = r"[+-]?(?:\d+(?:\.\d*)?|\.\d+)"
_DP = re.compile(rf"^({_NUMBER})\.dp$")
_SP = re.compile(rf"^({_NUMBER})\.sp$")
_FLOAT = re.compile(rf"^({_NUMBER})[fF]$")
_INT = re.compile(r"^[+-]?\d+$")
_DOUBLE = re.compile(rf"^({_NUMBER})$")
_REF = re.compile(r"^([A-Za-z_]\w*)\.([A-Za-z_]\w*)$")
_KEY_ID = re.compile(
    r"^(ColorToken|ShapeToken|TypographyToken|MotionSchemeToken)\(\s*(\d+)\s*\)$")
_DP_ARG = re.compile(rf"^({_NUMBER})\.dp$")
_FLOAT_ARG = re.compile(rf"^({_NUMBER})[fF]?$")
_ROUNDED = re.compile(r"^RoundedCornerShape\((.*)\)$")
_CORNER_SIZE = re.compile(rf"^CornerSize\(\s*({_NUMBER})\.dp\s*,?\s*\)$")
_CUBIC = re.compile(r"^CubicBezierEasing\((.*)\)$")
_WEIGHT = re.compile(r"^FontWeight\(\s*(\d+)\s*\)$")
_IDENTIFIER = re.compile(r"^[A-Za-z_]\w*$")
_RGB = re.compile(
    r"^Color\(\s*red\s*=\s*(\d+)\s*,\s*green\s*=\s*(\d+)\s*,"
    r"\s*blue\s*=\s*(\d+)\s*,?\s*\)$")
_CORNER_NAMES = ("topStart", "topEnd", "bottomEnd", "bottomStart")


def _split_arguments(body: str) -> list[str]:
    arguments: list[str] = []
    depth = 0
    start = 0
    for index, character in enumerate(body):
        if character in "([{":
            depth += 1
        elif character in ")]}":
            depth -= 1
        elif character == "," and depth == 0:
            argument = body[start:index].strip()
            if argument:
                arguments.append(argument)
            start = index + 1
    argument = body[start:].strip()
    if argument:
        arguments.append(argument)
    return arguments


def _dp_literal(text: str) -> Dp | None:
    match = _DP_ARG.fullmatch(text.strip())
    if match is None:
        return None
    return Dp(float(match.group(1)))


def _per_corner(body: str) -> RoundedCornerPerCorner | None:
    sizes: dict[str, Dp] = {}
    for argument in _split_arguments(body):
        name, separator, value = argument.partition("=")
        if not separator:
            return None
        size = _dp_literal(value)
        if size is None:
            return None
        sizes[name.strip()] = size
    if set(sizes) != set(_CORNER_NAMES):
        return None
    return RoundedCornerPerCorner(
        top_start=sizes["topStart"],
        top_end=sizes["topEnd"],
        bottom_end=sizes["bottomEnd"],
        bottom_start=sizes["bottomStart"],
    )


def _cubic_bezier(body: str) -> CubicBezier | None:
    numbers: list[float] = []
    for argument in _split_arguments(body):
        match = _FLOAT_ARG.fullmatch(argument)
        if match is None:
            return None
        numbers.append(float(match.group(1)))
    if len(numbers) != 4:
        return None
    return CubicBezier(*numbers)


def classify(text: str) -> Value:
    compact = " ".join(text.split())
    if not compact:
        return Unknown("")

    match = _DP.fullmatch(compact)
    if match is not None:
        return Dp(float(match.group(1)))
    match = _SP.fullmatch(compact)
    if match is not None:
        return Sp(float(match.group(1)))
    match = _FLOAT.fullmatch(compact)
    if match is not None:
        return Float(float(match.group(1)))
    if _INT.fullmatch(compact):
        return Int(int(compact))
    match = _DOUBLE.fullmatch(compact)
    if match is not None:
        return Double(float(match.group(1)))

    if compact == "CircleShape":
        return Circle()
    if compact == "RectangleShape":
        return Rectangle()

    match = _KEY_ID.fullmatch(compact)
    if match is not None:
        return KeyId(kind=match.group(1), id=int(match.group(2)))

    match = _ROUNDED.fullmatch(compact)
    if match is not None:
        body = match.group(1)
        arguments = _split_arguments(body)
        if len(arguments) == 1:
            size = _dp_literal(arguments[0])
            if size is not None:
                return RoundedCorner(size)
        per_corner = _per_corner(body)
        if per_corner is not None:
            return per_corner

    match = _CORNER_SIZE.fullmatch(compact)
    if match is not None:
        return CornerSizeValue(Dp(float(match.group(1))))

    match = _CUBIC.fullmatch(compact)
    if match is not None:
        cubic = _cubic_bezier(match.group(1))
        if cubic is not None:
            return cubic

    match = _RGB.fullmatch(compact)
    if match is not None:
        return Rgb(
            red=int(match.group(1)),
            green=int(match.group(2)),
            blue=int(match.group(3)),
        )

    match = _REF.fullmatch(compact)
    if match is not None:
        owner, name = match.groups()
        if owner == "FontWeight":
            return FontWeightName(name)
        return Ref(owner=owner, name=name)

    match = _WEIGHT.fullmatch(compact)
    if match is not None:
        return Weight(int(match.group(1)))

    if _IDENTIFIER.fullmatch(compact):
        return Alias(compact)

    return Unknown(compact)
