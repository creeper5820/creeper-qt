#!/usr/bin/env python3

from __future__ import annotations

import sys

sys.dont_write_bytecode = True

from core.config import ConfigError  # noqa: E402
from command import check, fetch, generate  # noqa: E402

import argparse  # noqa: E402
from pathlib import Path  # noqa: E402

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
