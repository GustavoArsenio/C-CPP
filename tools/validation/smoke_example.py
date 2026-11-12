#!/usr/bin/env python3
"""Run and compare every catalog example for one source file."""

import argparse
import json
from pathlib import Path
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[2]


def read_fixture(relative_path: str, exercise_id: str) -> str:
    path = (ROOT / relative_path).resolve()
    if ROOT not in path.parents or not path.is_file():
        raise ValueError(f"Invalid fixture for {exercise_id}: {relative_path}")
    return path.read_text(encoding="utf-8")


def check_example(exercise: dict, number: int, executable: str) -> None:
    example = exercise["examples"][number - 1]
    input_text = (read_fixture(example["input_file"], exercise["id"])
                  if "input_file" in example else example.get("input", ""))
    result = subprocess.run(
        [executable], input=input_text, text=True, capture_output=True,
        check=False, timeout=10,
    )
    expected_code = example.get("expected_exit_code", 0)
    if result.returncode != expected_code:
        raise ValueError(
            f"{exercise['id']} example {number} exited {result.returncode}; "
            f"expected {expected_code}. stderr: {result.stderr}"
        )
    expected = (read_fixture(example["output_file"], exercise["id"])
                if "output_file" in example else example["output"])
    if result.stdout != expected:
        raise ValueError(
            f"Output mismatch for {exercise['id']} example {number}: "
            f"expected {expected!r}, got {result.stdout!r}"
        )
    for check in example.get("checks", []):
        if check != "unique_cards_per_player":
            raise ValueError(f"Unknown smoke check {check!r}")
        dealt_cards = []
        for line in result.stdout.splitlines():
            if ":" not in line:
                raise ValueError(f"Invalid hand output in {exercise['id']}")
            cards = line.split(":", 1)[1].strip().split()
            if len(cards) != 3 or len(set(cards)) != 3:
                raise ValueError(f"Invalid hand in {exercise['id']}")
            dealt_cards.extend(cards)
        if len(dealt_cards) != len(set(dealt_cards)):
            raise ValueError(f"Duplicate card in {exercise['id']}")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", required=True)
    parser.add_argument("--executable", required=True)
    args = parser.parse_args()

    catalog = json.loads((ROOT / "exercises" / "catalog.json").read_text(
        encoding="utf-8"))
    exercises = [item for item in catalog["exercises"]
                 if item["source"] == args.source]
    if not exercises:
        print(f"No catalog record for {args.source}", file=sys.stderr)
        return 1

    tested = 0
    try:
        for exercise in exercises:
            if exercise["status"] not in {"implemented", "tested", "reviewed"}:
                continue
            if not exercise["examples"]:
                raise ValueError(f"{exercise['id']} has no smoke examples")
            for number in range(1, len(exercise["examples"]) + 1):
                check_example(exercise, number, args.executable)
                tested += 1
            print(f"PASS {exercise['id']} ({len(exercise['examples'])} example(s))")
    except (OSError, ValueError, KeyError, subprocess.SubprocessError) as error:
        print(error, file=sys.stderr)
        return 1

    if tested == 0:
        print(f"No implemented examples found for {args.source}", file=sys.stderr)
        return 77
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
