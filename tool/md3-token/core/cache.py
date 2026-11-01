from __future__ import annotations

from pathlib import Path

from core.config import Config, ConfigError

COMMIT_FILE = ".commit"


class CacheError(ConfigError):
    pass


def commit_file(config: Config) -> Path:
    return config.cache / COMMIT_FILE


def recorded_commit(config: Config) -> str | None:
    try:
        return commit_file(config).read_text(encoding="utf-8").strip()
    except FileNotFoundError:
        return None


def require(config: Config) -> None:
    recorded = recorded_commit(config)
    if recorded is None:
        raise CacheError(
            f"cache not found at {config.cache}; run `generator.py fetch` first")
    if recorded != config.androidx.commit:
        raise CacheError(
            f"cache is at {recorded}, config wants {config.androidx.commit}; "
            "run `generator.py fetch`")
    missing = sorted(
        alias for alias in config.modules
        if not config.module_path(alias).is_dir()
    )
    if missing:
        raise CacheError(
            f"cache is missing modules: {', '.join(missing)}; "
            "sparse checkout is incomplete, run `generator.py fetch` again")
