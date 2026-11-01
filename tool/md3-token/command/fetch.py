from __future__ import annotations

import argparse
import shlex
import subprocess
import time
from pathlib import Path

from core import cache, log as log_module
from core.cache import CacheError
from core.config import REPO_ROOT, Config, load_config

log = log_module.get("fetch")


def register(sub: argparse._SubParsersAction) -> None:
    parser = sub.add_parser("fetch", help="按锁定的 commit 拉取 Compose 源码到缓存")
    parser.set_defaults(run=run)


def _relative(path: Path) -> str:
    try:
        return str(path.relative_to(REPO_ROOT))
    except ValueError:
        return str(path)


def _display(args: list[str]) -> str:
    shown = []
    for arg in args:
        path = Path(arg)
        if path.is_absolute():
            shown.append(_relative(path))
        else:
            shown.append(arg)
    return shlex.join(shown)


def _git(args: list[str], capture: bool = False) -> str:
    log.info("$ git %s", _display(args))
    cmd = ["git", *args]
    try:
        result = subprocess.run(cmd, check=True, capture_output=capture, text=True)
    except subprocess.CalledProcessError:
        raise CacheError(f"git failed: {' '.join(cmd)}") from None
    except FileNotFoundError:
        raise CacheError("git not found") from None
    return result.stdout.strip() if capture else ""


def _checkout(config: Config) -> None:
    cache_dir = str(config.cache)
    if not (config.cache / ".git").is_dir():
        log.info("initializing cache")
        config.cache.parent.mkdir(parents=True, exist_ok=True)
        _git(["init", "-q", cache_dir])
        _git(["-C", cache_dir, "remote", "add", "origin", config.androidx.remote])
    else:
        log.info("using existing cache %s", _relative(config.cache))
        _git(["-C", cache_dir, "remote", "set-url", "origin",
             config.androidx.remote])
        log.info("origin set to %s", config.androidx.remote)
    log.info("sparse paths:")
    for sparse_path in config.sparse:
        log.info("  %s", sparse_path)
    _git(["-C", cache_dir, "sparse-checkout", "init", "--cone"])
    _git(["-C", cache_dir, "sparse-checkout", "set", *config.sparse])
    short = config.androidx.commit[:12]
    log.info("fetching %s (depth 1, blob:none)", short)
    started = time.monotonic()
    _git([
        "-C", cache_dir, "fetch", "--depth", "1", "--filter=blob:none",
        "origin", config.androidx.commit,
    ])
    log.info("fetched in %.1fs", time.monotonic() - started)
    log.info("checking out FETCH_HEAD")
    _git(["-C", cache_dir, "checkout", "-q", "--detach", "FETCH_HEAD"])
    head = _git(["-C", cache_dir, "rev-parse", "HEAD"], capture=True)
    if head != config.androidx.commit:
        raise CacheError(
            f"checked out {head}, expected {config.androidx.commit}")
    log.info("HEAD verified %s", head[:12])


def fetch(config: Config) -> None:
    relative = _relative(config.cache)
    log.info("remote %s", config.androidx.remote)
    log.info("commit %s", config.androidx.commit)
    log.info("cache %s", relative)
    _checkout(config)
    commit_path = cache.commit_file(config)
    commit_path.write_text(config.androidx.commit + "\n", encoding="utf-8")
    log.info("wrote %s", _relative(commit_path))
    cache.require(config)
    for alias, module in config.modules.items():
        entries = sum(1 for _ in config.module_path(alias).iterdir())
        log.info("module %s: %s (%d entries)", alias, module, entries)
    print(f"fetched {config.androidx.commit} into {relative}")


def run(args: argparse.Namespace) -> int:
    config = load_config()
    fetch(config)
    return 0
