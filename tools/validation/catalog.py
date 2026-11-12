#!/usr/bin/env python3
"""Validate exercise metadata and print catalog totals."""

from collections import Counter
import json
from pathlib import Path
import sys


ROOT = Path(__file__).resolve().parents[2]
CATALOG = ROOT / "exercises" / "catalog.json"
REQUIRED_FIELDS = {
    "id",
    "title",
    "language",
    "difficulty",
    "topics",
    "prerequisites",
    "learning_objectives",
    "statement",
    "examples",
    "constraints",
    "status",
    "source",
    "validation",
    "acceptance_criteria",
    "complexity",
    "references",
}
LANGUAGES = {"C", "C++"}
DIFFICULTIES = {"beginner", "intermediate", "advanced"}
STATUSES = {"planned", "scaffolded", "implemented", "tested", "reviewed"}


def main() -> int:
    try:
        catalog = json.loads(CATALOG.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as error:
        print(f"catalog error: {error}", file=sys.stderr)
        return 1

    exercises = catalog.get("exercises")
    if catalog.get("schema_version") != 1 or not isinstance(exercises, list):
        print("catalog error: expected schema_version 1 and exercises array",
              file=sys.stderr)
        return 1

    errors: list[str] = []
    identifiers: set[str] = set()
    for index, exercise in enumerate(exercises):
        prefix = f"exercises[{index}]"
        if not isinstance(exercise, dict):
            errors.append(f"{prefix} must be an object")
            continue
        missing = REQUIRED_FIELDS - exercise.keys()
        if missing:
            errors.append(f"{prefix} missing fields: {', '.join(sorted(missing))}")
            continue

        identifier = exercise["id"]
        if not isinstance(identifier, str) or not identifier:
            errors.append(f"{prefix}.id must be a non-empty string")
        elif identifier in identifiers:
            errors.append(f"duplicate exercise id: {identifier}")
        else:
            identifiers.add(identifier)

        if exercise["language"] not in LANGUAGES:
            errors.append(f"{prefix}.language is unsupported")
        if exercise["difficulty"] not in DIFFICULTIES:
            errors.append(f"{prefix}.difficulty is unsupported")
        if exercise["status"] not in STATUSES:
            errors.append(f"{prefix}.status is unsupported")

        for key in ("topics", "prerequisites", "learning_objectives",
                    "examples", "constraints"):
            if not isinstance(exercise[key], list):
                errors.append(f"{prefix}.{key} must be an array")
        if isinstance(exercise["examples"], list):
            for example_index, example in enumerate(exercise["examples"]):
                example_prefix = f"{prefix}.examples[{example_index}]"
                if not isinstance(example, dict):
                    errors.append(f"{example_prefix} must be an object")
                    continue
                if not ("input" in example or "input_file" in example):
                    errors.append(f"{example_prefix} needs input or input_file")
                if not ("output" in example or "output_file" in example):
                    errors.append(f"{example_prefix} needs output or output_file")
                if ("expected_exit_code" in example and
                        not isinstance(example["expected_exit_code"], int)):
                    errors.append(f"{example_prefix}.expected_exit_code must be an integer")
        if not isinstance(exercise["statement"], str) or not exercise["statement"]:
            errors.append(f"{prefix}.statement must be non-empty")
        criteria = exercise["acceptance_criteria"]
        if (not isinstance(criteria, list) or not criteria or
                not all(isinstance(item, str) and item for item in criteria)):
            errors.append(f"{prefix}.acceptance_criteria must contain text")
        if (exercise["complexity"] is not None and
                not isinstance(exercise["complexity"], str)):
            errors.append(f"{prefix}.complexity must be text or null")
        if not isinstance(exercise["references"], list):
            errors.append(f"{prefix}.references must be an array")

        source = exercise["source"]
        if (exercise["status"] in {"implemented", "tested", "reviewed"} and
                not source):
            errors.append(f"{prefix}.source is required for implemented work")
        if source:
            source_path = (ROOT / source).resolve()
            if ROOT not in source_path.parents or not source_path.is_file():
                errors.append(f"{prefix}.source does not exist inside repository")

    for exercise in exercises:
        if not isinstance(exercise, dict):
            continue
        for prerequisite in exercise.get("prerequisites", []):
            if prerequisite not in identifiers:
                errors.append(
                    f"{exercise.get('id', '<unknown>')} has unknown prerequisite "
                    f"{prerequisite}"
                )

    if errors:
        print("Catalog validation failed:", file=sys.stderr)
        for error in errors:
            print(f"- {error}", file=sys.stderr)
        return 1

    print(f"Exercises: {len(exercises)}")
    for label, key in (("Language", "language"),
                       ("Difficulty", "difficulty"),
                       ("Status", "status")):
        counts = Counter(item[key] for item in exercises)
        print(f"{label}: " + ", ".join(
            f"{name}={counts[name]}" for name in sorted(counts)))
    topic_counts = Counter(topic for item in exercises for topic in item["topics"])
    print("Topics: " + ", ".join(
        f"{name}={topic_counts[name]}" for name in sorted(topic_counts)))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
