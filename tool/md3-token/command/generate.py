from __future__ import annotations

import argparse
import difflib
from dataclasses import dataclass
from pathlib import Path
from typing import NoReturn

from core import cache, kotlin, log as log_module, render, scheme
from core.config import (
    REPO_ROOT,
    Component,
    Config,
    ConfigError,
    load_components,
    load_config,
)
from core.kotlin import Declaration, Location, KotlinError, KotlinFile
from core.model import Ref, Unknown, Value
from core.resolve import Resolver, SysRef

log = log_module.get("generate")


@dataclass(frozen=True)
class Field:
    name: str
    value: Value | SysRef


@dataclass(frozen=True)
class CollectedValue:
    value: Value | SysRef | None
    file: KotlinFile


@dataclass(frozen=True)
class Collected:
    component: Component
    fields: tuple[Field, ...]
    values: dict[str, CollectedValue]
    files: tuple[KotlinFile, ...]
    skipped_colors: int


@dataclass(frozen=True)
class Prepared:
    collected: Collected
    measurements: tuple[render.MeasurementField, ...] | None
    measurement_files: tuple[KotlinFile, ...]
    colors: tuple[render.ColorField, ...] | None
    color_files: tuple[KotlinFile, ...]


def register(sub: argparse._SubParsersAction) -> None:
    parser = sub.add_parser("generate", help="根据 details/token/*.yaml 生成头文件")
    parser.set_defaults(run=run)


def _relative(path: Path) -> str:
    try:
        return path.relative_to(REPO_ROOT).as_posix()
    except ValueError:
        return str(path)


def _raw(value: Value) -> str:
    if isinstance(value, Unknown):
        return value.text
    inner = getattr(value, "value", None)
    return "" if inner is None else str(inner)


def _tokens_declarations(
        resolver: Resolver, component: Component) -> list[tuple[KotlinFile, Declaration]]:
    if component.tokens is None:
        return []
    kotlin_file = resolver.by_reference(component.tokens.file)
    names = component.tokens.include
    if names is None:
        return [(kotlin_file, declaration) for declaration in kotlin_file.declarations]
    for name in names:
        kotlin_file.get(name)
    included = set(names)
    return [
        (kotlin_file, declaration) for declaration in kotlin_file.declarations
        if declaration.name in included
    ]


def _symbol_declarations(
        resolver: Resolver, component: Component) -> list[tuple[KotlinFile, Declaration]]:
    declarations = []
    for source in component.symbols:
        kotlin_file = resolver.by_reference(source.file)
        declarations.extend(
            (kotlin_file, kotlin_file.get(name)) for name in source.names)
    return declarations


def _extract_declarations(
        resolver: Resolver, component: Component) -> list[tuple[KotlinFile, Declaration]]:
    declarations = []
    for source in component.extract:
        kotlin_file = resolver.by_reference(source.file)
        value, location = kotlin.extract(kotlin_file.path, source.match)
        declarations.append((kotlin_file, Declaration(
            name=source.name,
            value=value,
            raw=_raw(value),
            location=location,
        )))
    return declarations


def _reject_duplicates(declarations: list[tuple[KotlinFile, Declaration]]) -> None:
    seen: dict[str, Location] = {}
    for _, declaration in declarations:
        previous = seen.get(declaration.name)
        if previous is not None:
            raise KotlinError(
                f"{declaration.location.path}:{declaration.location.line}: "
                f"duplicate token {declaration.name} "
                f"(also at {previous.path}:{previous.line})")
        seen[declaration.name] = declaration.location


def _add_file(files: list[KotlinFile], kotlin_file: KotlinFile) -> None:
    if all(existing.path != kotlin_file.path for existing in files):
        files.append(kotlin_file)


def _collect(resolver: Resolver, component: Component) -> Collected:
    declarations = [
        *_tokens_declarations(resolver, component),
        *_symbol_declarations(resolver, component),
        *_extract_declarations(resolver, component),
    ]
    _reject_duplicates(declarations)

    fields = []
    files: list[KotlinFile] = []
    values: dict[str, CollectedValue] = {}
    skipped_colors = 0
    for kotlin_file, declaration in declarations:
        resolved = resolver.resolve(declaration)
        values[declaration.name] = CollectedValue(
            value=None if resolved is None else resolved.value,
            file=kotlin_file,
        )
        if resolved is None:
            skipped_colors += 1
            continue
        _add_file(files, kotlin_file)
        fields.append(Field(
            name=declaration.name,
            value=resolved.value,
        ))
    return Collected(
        component=component,
        fields=tuple(fields),
        values=values,
        files=tuple(files),
        skipped_colors=skipped_colors,
    )


def _resolved_type(resolved: Value | SysRef | None, resolver: Resolver) -> str:
    if resolved is None:
        return "ColorToken"
    if isinstance(resolved, SysRef):
        entry = resolver.entries.get((resolved.kind, resolved.name))
        if entry is None:
            return resolved.kind
        return type(entry.value).__name__
    return type(resolved).__name__


def _missing_value(
        path: Path, field: str, name: str, available: list[str]) -> NoReturn:
    suggestions = difflib.get_close_matches(name, available, n=3, cutoff=0.6)
    detail = f"unknown value {name}"
    if suggestions:
        detail += f" (did you mean {', '.join(suggestions)}?)"
    raise ConfigError(f"{path}: {field}: {detail}")


def _measurement_fields(
        resolver: Resolver, collected: Collected
) -> tuple[tuple[render.MeasurementField, ...] | None, tuple[KotlinFile, ...]]:
    component = collected.component
    if component.measurements is None:
        return None, ()
    measurements = []
    files: list[KotlinFile] = []
    available = sorted(collected.values)
    for measurement in component.measurements:
        info = collected.values.get(measurement.value)
        if info is None:
            _missing_value(
                component.path,
                f"measurements.{measurement.name}",
                measurement.value,
                available,
            )
        type_name = _resolved_type(info.value, resolver)
        if type_name not in ("Dp", "Sp"):
            raise ConfigError(
                f"{component.path}: measurements.{measurement.name}: "
                f"{measurement.value} has type {type_name}, expected Dp or Sp")
        if isinstance(info.value, SysRef):
            expression = render.render_value(info.value)
        else:
            expression = render.cpp_name(measurement.value)
        measurements.append(render.MeasurementField(
            name=measurement.name,
            type_name=type_name,
            value=expression,
        ))
        _add_file(files, info.file)
    return tuple(measurements), tuple(files)


def _color_token(
        component: Component, tokens: KotlinFile, field: str, token: str,
        scheme_fields: frozenset[str]) -> str:
    try:
        declaration = tokens.get(token)
    except KotlinError as error:
        raise ConfigError(f"{component.path}: {field}: {error}") from None
    value = declaration.value
    if not isinstance(value, Ref) or value.owner != "ColorSchemeKeyTokens":
        if isinstance(value, Ref):
            actual = f"{value.owner}.{value.name}"
        else:
            actual = type(value).__name__
        raise ConfigError(
            f"{component.path}: {field}: {token} = {declaration.raw}: "
            f"expected ColorSchemeKeyTokens.<name>, got {actual}")
    scheme_field = scheme.snake_case(value.name)
    if scheme_field not in scheme_fields:
        raise ConfigError(
            f"{declaration.location.path}:{declaration.location.line}: "
            f"{token} = {declaration.raw}: ColorScheme has no field {scheme_field}")
    return scheme_field


def _color_fields(
        resolver: Resolver, collected: Collected, scheme_fields: frozenset[str]
) -> tuple[tuple[render.ColorField, ...] | None, tuple[KotlinFile, ...]]:
    component = collected.component
    if component.colors is None:
        return None, ()
    if component.tokens is None:
        raise ConfigError(f"{component.path}: colors: requires a tokens section")
    tokens = resolver.by_reference(component.tokens.file)
    colors = []
    for entry in component.colors:
        field = f"colors.{entry.name}"
        if entry.token is None:
            colors.append(render.ColorField(
                name=entry.name,
                token=None,
                scheme_field=None,
                opacity=None,
            ))
            continue
        scheme_field = _color_token(
            component, tokens, field, entry.token, scheme_fields)
        if entry.opacity is not None:
            info = collected.values.get(entry.opacity)
            if info is None:
                available = sorted(
                    name for name, value in collected.values.items()
                    if _resolved_type(value.value, resolver) in ("Float", "Double")
                )
                _missing_value(
                    component.path, f"{field}.opacity", entry.opacity, available)
            opacity_type = _resolved_type(info.value, resolver)
            if opacity_type not in ("Float", "Double"):
                raise ConfigError(
                    f"{component.path}: {field}.opacity: {entry.opacity} "
                    f"has type {opacity_type}, expected Float or Double")
        colors.append(render.ColorField(
            name=entry.name,
            token=entry.token,
            scheme_field=scheme_field,
            opacity=entry.opacity,
        ))
    return tuple(colors), (tokens,)


def build_all(config: Config) -> dict[Path, str]:
    components = load_components(config)
    resolver = Resolver(config)
    scheme_fields = scheme.color_scheme_fields()
    collected = []
    for component in components:
        result = _collect(resolver, component)
        collected.append(result)
        log.info("component %s (%s)", component.namespace, _relative(component.path))
        for kotlin_file in result.files:
            log.info("  source %s", render.source_label(config, kotlin_file.path))
        log.info("  skipped %d color tokens", result.skipped_colors)

    targets: dict[Path, str] = {}
    prepared = []
    for result in collected:
        measurements, measurement_files = _measurement_fields(resolver, result)
        colors, color_files = _color_fields(resolver, result, scheme_fields)
        prepared.append(Prepared(
            collected=result,
            measurements=measurements,
            measurement_files=measurement_files,
            colors=colors,
            color_files=color_files,
        ))

    for item in prepared:
        result = item.collected
        namespace = result.component.namespace
        path = render.component_path(config, namespace)
        targets[path] = render.render_component(
            config,
            path,
            namespace,
            tuple((field.name, field.value) for field in result.fields),
            result.files,
        )

        if item.measurements is not None:
            path = render.measurements_path(config, namespace)
            targets[path] = render.render_measurements(
                config, path, namespace, item.measurements, item.measurement_files)

        if item.colors is not None:
            path = render.colors_path(config, namespace)
            targets[path] = render.render_colors(
                config, path, namespace, item.colors, item.color_files)

        sources = list(result.files)
        for kotlin_file in item.color_files:
            _add_file(sources, kotlin_file)
        path = render.defaults_path(config, namespace)
        targets[path] = render.render_defaults(
            config,
            path,
            namespace,
            tuple(field.name for field in result.fields),
            item.colors,
            tuple(sources),
        )

    for kind in sorted({entry.kind for entry in resolver.entries.values()}):
        entries = tuple(
            entry for entry in resolver.entries.values() if entry.kind == kind)
        files: list[KotlinFile] = []
        for entry in entries:
            for kotlin_file in entry.files:
                _add_file(files, kotlin_file)
        targets[render.sys_path(config, kind)] = render.render_sys(
            config, kind, entries, tuple(files))
    return targets


def _write(targets: dict[Path, str]) -> tuple[int, int]:
    written = 0
    unchanged = 0
    for path in sorted(targets):
        text = targets[path]
        try:
            existing = path.read_text(encoding="utf-8")
        except FileNotFoundError:
            existing = None
        except (OSError, UnicodeError) as error:
            raise ConfigError(f"{path}: cannot read: {error}") from None
        if existing == text:
            unchanged += 1
            log.info("unchanged %s", _relative(path))
            continue
        try:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text, encoding="utf-8")
        except OSError as error:
            raise ConfigError(f"{path}: cannot write: {error}") from None
        written += 1
        log.info("wrote %s", _relative(path))
    return written, unchanged


def _clean(config: Config, targets: dict[Path, str]) -> int:
    removed = 0
    for path in sorted(config.output.glob("*/*.hh")):
        if path in targets:
            continue
        try:
            with path.open(encoding="utf-8") as stream:
                first_line = stream.readline().rstrip("\n")
        except (OSError, UnicodeError):
            continue
        if first_line != render.GENERATED_MARKER:
            continue
        try:
            path.unlink()
        except OSError as error:
            raise ConfigError(f"{path}: cannot remove: {error}") from None
        removed += 1
        log.info("removed %s", _relative(path))
    return removed


def run(args: argparse.Namespace) -> int:
    config = load_config()
    cache.require(config)
    targets = build_all(config)
    written, unchanged = _write(targets)
    removed = _clean(config, targets)
    print(
        f"generated {len(targets)} files "
        f"({written} written, {unchanged} unchanged, {removed} removed)")
    return 0
