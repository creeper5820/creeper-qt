from __future__ import annotations

import argparse
import sys

from core.config import load_config


def register(sub: argparse._SubParsersAction) -> None:
    parser = sub.add_parser("fetch", help="按锁定的 commit 拉取 Compose 源码到缓存")
    parser.set_defaults(run=run)


def run(args: argparse.Namespace) -> int:
    config = load_config()
    print(f"fetch: not implemented (commit {config.androidx.commit})", file=sys.stderr)
    return 2
