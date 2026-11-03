from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
from typing import cast

from core.config import Config
from core.kotlin import Declaration, KotlinError, KotlinFile, parse_file
from core.model import (
    Alias,
    Circle,
    CubicBezier,
    Dp,
    Double,
    Float,
    FontWeightName,
    Int,
    Millis,
    Rectangle,
    Ref,
    RoundedCorner,
    RoundedCornerPerCorner,
    Sp,
    Value,
    Weight,
)

MAX_ALIAS_DEPTH = 8


@dataclass(frozen=True)
class SysRef:
    kind: str
    name: str


@dataclass(frozen=True)
class TypeScale:
    size: Sp
    line_height: Sp
    tracking: Sp
    weight: int


@dataclass(frozen=True)
class SysEntry:
    kind: str
    name: str
    value: Value
    files: tuple[KotlinFile, ...]


@dataclass(frozen=True)
class Resolved:
    value: Value | SysRef
    files: tuple[KotlinFile, ...]


class Resolver:
    def __init__(self, config: Config) -> None:
        self.config = config
        self.entries: dict[tuple[str, str], SysEntry] = {}
        self._files: dict[Path, KotlinFile] = {}

    def file(self, path: Path) -> KotlinFile:
        cached = self._files.get(path)
        if cached is None:
            cached = parse_file(path)
            self._files[path] = cached
        return cached

    def by_reference(self, reference: str) -> KotlinFile:
        return self.file(self.config.resolve(reference))

    def resolve(self, declaration: Declaration) -> Resolved | None:
        value = declaration.value
        if isinstance(value, Ref):
            return self._reference(declaration, value)
        if isinstance(value, RoundedCorner):
            return Resolved(Dp(value.size.value), ())
        if isinstance(value, (Dp, Sp, Float, Int, Millis, Double, CubicBezier)):
            return Resolved(value, ())
        raise self._unsupported(declaration, f"unsupported value {type(value).__name__}")

    def _unsupported(self, declaration: Declaration, detail: str) -> KotlinError:
        return KotlinError(
            f"{declaration.location.path}:{declaration.location.line}: "
            f"{declaration.name} = {declaration.raw}: {detail}")

    def _reference(self, declaration: Declaration, ref: Ref) -> Resolved | None:
        if ref.owner == "ColorSchemeKeyTokens":
            return None
        if ref.owner == "ShapeKeyTokens":
            return self._shape(declaration, ref)
        if ref.owner == "ElevationTokens":
            return self._elevation(ref)
        if ref.owner == "TypographyKeyTokens":
            return self._typography(ref)
        raise self._unsupported(declaration, "unsupported reference")

    def _shape(self, declaration: Declaration, ref: Ref) -> Resolved:
        shape_file = self.by_reference("tokens:ShapeTokens.kt")
        target = shape_file.get(ref.name)
        value = target.value
        if isinstance(value, RoundedCorner):
            entry = self._entry("shape", ref.name, Dp(value.size.value), (shape_file,))
            return Resolved(SysRef("shape", ref.name), entry.files)
        if isinstance(value, (Circle, Rectangle, RoundedCornerPerCorner)):
            raise KotlinError(
                f"{declaration.location.path}:{declaration.location.line}: "
                f"shape {ref.name} ({type(value).__name__}) is not supported yet")
        raise KotlinError(
            f"{shape_file.path}:{target.location.line}: {ref.name} = {target.raw}: "
            "expected RoundedCorner")

    def _elevation(self, ref: Ref) -> Resolved:
        elevation_file = self.by_reference("tokens:ElevationTokens.kt")
        target = elevation_file.require(ref.name, Dp)
        entry = self._entry("elevation", ref.name, target.value, (elevation_file,))
        return Resolved(SysRef("elevation", ref.name), entry.files)

    def _typography(self, ref: Ref) -> Resolved:
        scale_file = self.by_reference("tokens:TypeScaleTokens.kt")
        size = cast(Sp, scale_file.require(f"{ref.name}Size", Sp).value)
        line_height = cast(Sp, scale_file.require(f"{ref.name}LineHeight", Sp).value)
        tracking = cast(Sp, scale_file.require(f"{ref.name}Tracking", Sp).value)

        weight_declaration = scale_file.get(f"{ref.name}Weight")
        weight_value = weight_declaration.value
        if not isinstance(weight_value, Ref) or weight_value.owner != "TypefaceTokens":
            raise KotlinError(
                f"{scale_file.path}:{weight_declaration.location.line}: "
                f"{weight_declaration.name} = {weight_declaration.raw}: "
                "expected TypefaceTokens.<name>")

        typeface_file = self.by_reference("tokens:TypefaceTokens.kt")
        font_name_declaration = typeface_file.get(weight_value.name)
        font_name = font_name_declaration.value
        if not isinstance(font_name, FontWeightName):
            raise KotlinError(
                f"{typeface_file.path}:{font_name_declaration.location.line}: "
                f"{font_name_declaration.name} = {font_name_declaration.raw}: "
                "expected FontWeight.<name>")

        font_file = self.by_reference("text:FontWeight.kt")
        weight = self._weight(font_file, font_name.name)

        scale = TypeScale(
            size=size,
            line_height=line_height,
            tracking=tracking,
            weight=weight,
        )
        files = (scale_file, typeface_file, font_file)
        entry = self._entry("typography", ref.name, scale, files)
        return Resolved(SysRef("typography", ref.name), entry.files)

    def _weight(self, font_file: KotlinFile, name: str) -> int:
        current = name
        for _ in range(MAX_ALIAS_DEPTH + 1):
            declaration = font_file.get(current)
            value = declaration.value
            if isinstance(value, Weight):
                return value.value
            if isinstance(value, Alias):
                current = value.name
                continue
            raise KotlinError(
                f"{font_file.path}:{declaration.location.line}: "
                f"{declaration.name} = {declaration.raw}: expected FontWeight or alias")
        raise KotlinError(
            f"{font_file.path}: alias chain for {name} exceeds {MAX_ALIAS_DEPTH} levels")

    def _entry(
            self, kind: str, name: str, value: Value,
            files: tuple[KotlinFile, ...]) -> SysEntry:
        key = (kind, name)
        entry = self.entries.get(key)
        if entry is None:
            entry = SysEntry(kind=kind, name=name, value=value, files=files)
            self.entries[key] = entry
        return entry
