from __future__ import annotations

import logging
import sys

ROOT_NAME = "md3-token"

_configured = False


class _Formatter(logging.Formatter):
    def format(self, record: logging.LogRecord) -> str:
        short_name = record.name.rsplit(".", 1)[-1]
        return f"[{short_name}] {super().format(record)}"


def _configure() -> None:
    global _configured
    if _configured:
        return
    root = logging.getLogger(ROOT_NAME)
    handler = logging.StreamHandler(sys.stderr)
    handler.setLevel(logging.INFO)
    handler.setFormatter(_Formatter())
    root.addHandler(handler)
    root.setLevel(logging.INFO)
    root.propagate = False
    _configured = True


def get(name: str) -> logging.Logger:
    _configure()
    return logging.getLogger(f"{ROOT_NAME}.{name}")
