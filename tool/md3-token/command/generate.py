from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path

from core import cache, kotlin, log as log_module, render
from core.config import (
    REPO_ROOT,
    Component,
    Config,
    ConfigError,
    load_components,
    load_config,
)
from core.kotlin import Declaration, Location, KotlinError, KotlinFile
from core.model import Unknown, Value
from core.resolve import Resolver, SysRef

log = log_module.get("generate")


@dataclass(frozen=True)
class Field:
    name: str
    value: Value | SysRef


@dataclass(frozen=True)
class Collected:
    component: Component
    fields: tuple[Field, ...]
    files: tuple[KotlinFile, ...]
    skipped_colors: int


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
    skipped_colors = 0
    for kotlin_file, declaration in declarations:
        resolved = resolver.resolve(declaration)
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
        files=tuple(files),
        skipped_colors=skipped_colors,
    )


def build_all(config: Config) -> dict[Path, str]:
    components = load_components(config)
    resolver = Resolver(config)
    collected = []
    for component in components:
        result = _collect(resolver, component)
        collected.append(result)
        log.info("component %s (%s)", component.namespace, _relative(component.path))
        for kotlin_file in result.files:
            log.info("  source %s", render.source_label(config, kotlin_file.path))
        log.info("  skipped %d color tokens", result.skipped_colors)

    targets: dict[Path, str] = {}
    for result in collected:
        path = render.component_path(config, result.component.namespace)
        targets[path] = render.render_component(
            config,
            path,
            result.component.namespace,
            tuple((field.name, field.value) for field in result.fields),
            result.files,
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
