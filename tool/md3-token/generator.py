#!/usr/bin/env python3

from __future__ import annotations
from core.config import ConfigError
from command import check, fetch, generate

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))


def main() -> int:
    parser = argparse.ArgumentParser(prog="generator.py")
    sub = parser.add_subparsers(dest="command", required=True)
    for module in (fetch, generate, check):
        module.register(sub)
    args = parser.parse_args()
    try:
        return args.run(args)
    except ConfigError as error:
        print(f"error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
