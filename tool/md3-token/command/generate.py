from __future__ import annotations

import argparse
import sys

from core.config import load_config


def register(sub: argparse._SubParsersAction) -> None:
    parser = sub.add_parser("generate", help="根据 details/token/*.yaml 生成头文件")
    parser.set_defaults(run=run)


def run(args: argparse.Namespace) -> int:
    config = load_config()
    print(f"generate: not implemented (commit {config.androidx.commit})", file=sys.stderr)
    return 2
