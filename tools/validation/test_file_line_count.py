#!/usr/bin/env python3
"""Check file line counting boundaries and input/open errors."""

import argparse
from pathlib import Path
import subprocess
import tempfile


def run(executable: str, path_input: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run([executable], input=path_input, text=True,
                          capture_output=True, check=False, timeout=10)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--executable", required=True)
    args = parser.parse_args()

    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        cases = ((b"", "0\n"), (b"one\ntwo\n", "2\n"),
                 (b"one\ntwo", "2\n"), (b"\n", "1\n"))
        for index, (contents, expected) in enumerate(cases):
            path = root / f"case-{index}.txt"
            path.write_bytes(contents)
            result = run(args.executable, str(path) + "\n")
            if result.returncode != 0 or result.stdout != expected:
                raise SystemExit(f"unexpected result for {contents!r}: {result}")

            if index == 0:
                crlf = run(args.executable, str(path) + "\r\n")
                if crlf.returncode != 0 or crlf.stdout != expected:
                    raise SystemExit(f"CRLF path input failed: {crlf}")
                at_eof = run(args.executable, str(path))
                if at_eof.returncode != 0 or at_eof.stdout != expected:
                    raise SystemExit(f"EOF-terminated path input failed: {at_eof}")

        missing = run(args.executable, str(root / "missing.txt") + "\n")
        if missing.returncode != 1 or not missing.stderr:
            raise SystemExit("missing file must fail with a diagnostic")

        empty_path = run(args.executable, "\n")
        if empty_path.returncode != 1 or not empty_path.stderr:
            raise SystemExit("empty path must fail with a diagnostic")

        overlong_path = run(args.executable, "x" * 1023 + "\n")
        if overlong_path.returncode != 1 or "too long" not in overlong_path.stderr:
            raise SystemExit("overlong path must fail with a diagnostic")

    print("PASS file line count boundaries and errors")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
