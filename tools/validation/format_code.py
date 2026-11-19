#!/usr/bin/env python3
"""Run the same versioned C/C++ formatter locally and in CI."""

import argparse
import os
from pathlib import Path
import re
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[2]
SOURCE_ROOTS = (
    "tracks", "algorithms", "data-structures", "projects", "tests",
    "comparisons", "benchmarks",
)
EXTENSIONS = {".c", ".h", ".cpp", ".hpp", ".cc", ".hh", ".cxx", ".hxx"}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--clang-format", default="clang-format-18")
    parser.add_argument("--fix", action="store_true")
    args = parser.parse_args()

    try:
        version = subprocess.check_output(
            [args.clang_format, "--version"], text=True,
        ).strip()
        if not re.search(r"\bclang-format version 18\.", version):
            print(f"error: clang-format 18 is required; found {version}",
                  file=sys.stderr)
            return 1
        print(version, flush=True)

        tracked = subprocess.check_output(
            ["git", "ls-files", "-z", "--", *SOURCE_ROOTS], cwd=ROOT,
        )
        files = sorted(
            os.fsdecode(path) for path in tracked.split(b"\0")
            if path and Path(os.fsdecode(path)).suffix in EXTENSIONS
        )
        if not files:
            print("error: no tracked C/C++ files found", file=sys.stderr)
            return 1

        print(f"{'Formatting' if args.fix else 'Checking'} {len(files)} "
              "tracked C/C++ source and header files", flush=True)
        options = ["-i"] if args.fix else ["--dry-run", "--Werror"]
        return subprocess.call(
            [args.clang_format, "--style=file", *options, *files], cwd=ROOT,
        )
    except (OSError, subprocess.CalledProcessError) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
