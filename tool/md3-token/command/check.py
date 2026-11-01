from __future__ import annotations

import argparse
import sys

from core.config import load_config


def register(sub: argparse._SubParsersAction) -> None:
    parser = sub.add_parser("check", help="校验生成结果与仓库一致并检查 include 依赖")
    parser.set_defaults(run=run)


def run(args: argparse.Namespace) -> int:
    config = load_config()
    print(f"check: not implemented (commit {
          config.androidx.commit})", file=sys.stderr)
    return 2
