#!/usr/bin/env python3
"""Check stateful commands in the matrix menu frontend."""

import argparse
import subprocess


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--executable", required=True)
    args = parser.parse_args()
    data = "\n".join(
        [
            "1", "A", "2", "2", "1", "2", "3", "4",
            "1", "B", "2", "2", "5", "6", "7", "8",
            "2", "1", "2",       # appends sum at position 3
            "3", "1", "2",       # appends product at position 4
            "5", "1",             # exact determinant
            "6", "1",             # transpose
            "7", "3",             # display appended sum
            "4", "5",             # factorial
            "7", "99",            # invalid matrix selection
            "99",                  # invalid menu selection, session continues
            "0", "",
        ]
    )
    result = subprocess.run(
        [args.executable], input=data, text=True, capture_output=True, check=False
    )
    expected = [
        "Added matrix 1",
        "Added matrix 2",
        "3 sum 2 2\n6 8\n10 12",
        "4 product 2 2\n19 22\n43 50",
        "-2",
        "1 3\n2 4",
        "120",
        "Invalid matrix selection",
        "Invalid option",
    ]
    if result.returncode != 0:
        raise SystemExit(f"matrix menu exited {result.returncode}: {result.stderr}")
    for fragment in expected:
        if fragment not in result.stdout:
            raise SystemExit(f"missing output {fragment!r}\n{result.stdout}")

    eof = subprocess.run(
        [args.executable], input="1\n", text=True, capture_output=True, check=False
    )
    if eof.returncode != 1:
        raise SystemExit(f"mid-command EOF returned {eof.returncode}, expected 1")
    clean_eof = subprocess.run(
        [args.executable], input="", text=True, capture_output=True, check=False
    )
    if clean_eof.returncode != 0:
        raise SystemExit("EOF between commands should end the session cleanly")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
