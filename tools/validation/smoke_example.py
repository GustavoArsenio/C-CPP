#!/usr/bin/env python3
"""Run and compare every catalog example for one source file."""

import argparse
import json
from collections.abc import Mapping
from pathlib import Path
import re
import subprocess
import sys


ROOT = Path(__file__).resolve().parents[2]
_HAND_PATTERN = re.compile(r"Player ([1-9][0-9]*): (\S+) (\S+) (\S+)")
_KNOWN_CHECKS = {"stdout_empty", "stderr_nonempty", "unique_cards_per_player"}


def read_fixture(relative_path: str, exercise_id: str) -> str:
    if not isinstance(relative_path, str):
        raise ValueError(f"Invalid fixture for {exercise_id}: {relative_path!r}")
    try:
        path = (ROOT / relative_path).resolve(strict=True)
        if ROOT not in path.parents or not path.is_file():
            raise ValueError
        return path.read_text(encoding="utf-8")
    except (OSError, RuntimeError, ValueError) as error:
        raise ValueError(
            f"Invalid fixture for {exercise_id}: {relative_path}"
        ) from error


def _validate_example(exercise: Mapping, example: object, number: int) -> Mapping:
    exercise_id = exercise.get("id", "<unknown>")
    if not isinstance(example, Mapping):
        raise ValueError(f"{exercise_id} example {number} must be an object")
    for file_field, inline_field in (("input_file", "input"),
                                     ("output_file", "output")):
        if file_field in example and inline_field in example:
            raise ValueError(
                f"{exercise_id} example {number} cannot specify both "
                f"{file_field} and {inline_field}"
            )
        if file_field in example and not isinstance(example[file_field], str):
            raise ValueError(
                f"{exercise_id} example {number} {file_field} must be a string"
            )
    if "input_file" not in example and "input" not in example:
        raise ValueError(
            f"{exercise_id} example {number} needs input or input_file"
        )
    if "input" in example and not isinstance(example["input"], str):
        raise ValueError(f"{exercise_id} example {number} input must be a string")
    if "output" not in example and "output_file" not in example:
        raise ValueError(f"{exercise_id} example {number} needs expected output")
    if "output" in example and not isinstance(example["output"], str):
        raise ValueError(f"{exercise_id} example {number} output must be a string")
    expected_code = example.get("expected_exit_code", 0)
    if isinstance(expected_code, bool) or not isinstance(expected_code, int):
        raise ValueError(
            f"{exercise_id} example {number} expected_exit_code must be an integer"
        )
    checks = example.get("checks", [])
    if not isinstance(checks, list) or any(not isinstance(item, str)
                                           for item in checks):
        raise ValueError(f"{exercise_id} example {number} checks must be strings")
    for check in checks:
        if check not in _KNOWN_CHECKS:
            raise ValueError(f"Unknown smoke check {check!r}")
    return example


def _check_unique_cards(stdout: str, exercise_id: str) -> None:
    lines = stdout.splitlines()
    if not lines:
        raise ValueError(f"Expected at least one player hand in {exercise_id}")
    dealt_cards = []
    player_ids = set()
    for line in lines:
        match = _HAND_PATTERN.fullmatch(line)
        if match is None:
            raise ValueError(f"Invalid hand output in {exercise_id}: {line!r}")
        player_id, *cards = match.groups()
        if player_id in player_ids:
            raise ValueError(f"Duplicate player hand in {exercise_id}")
        player_ids.add(player_id)
        if len(set(cards)) != 3:
            raise ValueError(f"Invalid hand in {exercise_id}: duplicate cards")
        dealt_cards.extend(cards)
    if len(dealt_cards) != len(set(dealt_cards)):
        raise ValueError(f"Duplicate card in {exercise_id}")


def check_example(exercise: Mapping, number: int, executable: str) -> None:
    exercise_id = exercise.get("id", "<unknown>")
    examples = exercise.get("examples")
    if not isinstance(examples, list) or number < 1 or number > len(examples):
        raise ValueError(f"Invalid examples for {exercise_id}")
    example = _validate_example(exercise, examples[number - 1], number)
    input_text = (read_fixture(example["input_file"], exercise_id)
                  if "input_file" in example else example.get("input", ""))
    result = subprocess.run(
        [executable], input=input_text, text=True, capture_output=True,
        check=False, timeout=10,
    )
    expected_code = example.get("expected_exit_code", 0)
    if result.returncode != expected_code:
        raise ValueError(
            f"{exercise_id} example {number} exited {result.returncode}; "
            f"expected {expected_code}. stderr: {result.stderr}"
        )
    expected = (read_fixture(example["output_file"], exercise_id)
                if "output_file" in example else example["output"])
    if result.stdout != expected:
        raise ValueError(
            f"Output mismatch for {exercise_id} example {number}: "
            f"expected {expected!r}, got {result.stdout!r}"
        )
    for check in example.get("checks", []):
        if check == "stdout_empty" and result.stdout:
            raise ValueError(
                f"Expected empty stdout for {exercise_id} example {number}"
            )
        if check == "stderr_nonempty" and not result.stderr.strip():
            raise ValueError(
                f"Expected useful stderr for {exercise_id} example {number}"
            )
        if check == "unique_cards_per_player":
            _check_unique_cards(result.stdout, exercise_id)


def _load_catalog() -> dict:
    try:
        catalog = json.loads((ROOT / "exercises" / "catalog.json").read_text(
            encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as error:
        raise ValueError(f"Unable to read exercise catalog: {error}") from error
    if not isinstance(catalog, Mapping):
        raise ValueError("Exercise catalog must be an object")
    records = catalog.get("exercises")
    if not isinstance(records, list):
        raise ValueError("Exercise catalog exercises must be a list")
    if any(not isinstance(record, Mapping) for record in records):
        raise ValueError("Every exercise catalog record must be an object")
    return catalog


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", required=True)
    parser.add_argument("--executable", required=True)
    args = parser.parse_args()

    try:
        catalog = _load_catalog()
        exercises = [item for item in catalog["exercises"]
                     if item.get("source") == args.source]
        if not exercises:
            print(f"No catalog record for {args.source}", file=sys.stderr)
            return 1

        tested = 0
        for exercise in exercises:
            exercise_id = exercise.get("id")
            if not isinstance(exercise_id, str):
                raise ValueError("Exercise record id must be a string")
            status = exercise.get("status")
            if not isinstance(status, str):
                raise ValueError(f"{exercise_id} status must be a string")
            if status not in {"implemented", "tested", "reviewed"}:
                continue
            examples = exercise.get("examples")
            if not isinstance(examples, list) or not examples:
                raise ValueError(f"{exercise_id} has no smoke examples")
            for number in range(1, len(examples) + 1):
                check_example(exercise, number, args.executable)
                tested += 1
            print(f"PASS {exercise_id} ({len(examples)} example(s))")
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        print(error, file=sys.stderr)
        return 1

    if tested == 0:
        print(f"No implemented examples found for {args.source}", file=sys.stderr)
        return 77
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
