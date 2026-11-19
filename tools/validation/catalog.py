#!/usr/bin/env python3
"""Validate exercise metadata and print catalog totals."""

from collections import Counter
import json
from pathlib import Path
import sys
from typing import Any


ROOT = Path(__file__).resolve().parents[2]
CATALOG = ROOT / "exercises" / "catalog.json"
REQUIRED_FIELDS = {
    "id", "title", "language", "difficulty", "topics", "prerequisites",
    "learning_objectives", "statement", "examples", "constraints", "status",
    "source", "validation", "acceptance_criteria", "complexity", "references",
}
LANGUAGES = {"C", "C++"}
DIFFICULTIES = {"beginner", "intermediate", "advanced"}
STATUSES = {"planned", "scaffolded", "implemented", "tested", "reviewed"}
EXAMPLE_CHECKS = {"stdout_empty", "stderr_nonempty", "unique_cards_per_player"}
STRING_FIELDS = ("id", "title", "language", "difficulty", "statement",
                 "status", "validation")
ARRAY_STRING_FIELDS = ("topics", "prerequisites", "learning_objectives",
                       "constraints", "references")


def validate_catalog(data: Any, root: Path = ROOT) -> list[str]:
    """Return deterministic diagnostics for malformed catalog data."""
    if not isinstance(data, dict):
        return ["catalog must be an object"]
    version = data.get("schema_version")
    exercises = data.get("exercises")
    if type(version) is not int or version != 1:
        return ["schema_version must be integer 1"]
    if not isinstance(exercises, list):
        return ["exercises must be an array"]

    errors: list[str] = []
    identifiers: set[str] = set()
    root_path = Path(root).resolve()
    for index, exercise in enumerate(exercises):
        prefix = f"exercises[{index}]"
        if not isinstance(exercise, dict):
            errors.append(f"{prefix} must be an object")
            continue
        missing = REQUIRED_FIELDS - exercise.keys()
        if missing:
            errors.append(f"{prefix} missing fields: {', '.join(sorted(missing))}")
            continue

        for key in STRING_FIELDS:
            if not isinstance(exercise[key], str) or not exercise[key]:
                errors.append(f"{prefix}.{key} must be a non-empty string")
        identifier = exercise["id"]
        if isinstance(identifier, str) and identifier:
            if identifier in identifiers:
                errors.append(f"duplicate exercise id: {identifier}")
            else:
                identifiers.add(identifier)

        for key, choices in (("language", LANGUAGES),
                             ("difficulty", DIFFICULTIES),
                             ("status", STATUSES)):
            value = exercise[key]
            if isinstance(value, str) and value not in choices:
                errors.append(f"{prefix}.{key} is unsupported")

        for key in ARRAY_STRING_FIELDS:
            value = exercise[key]
            if not isinstance(value, list):
                errors.append(f"{prefix}.{key} must be an array")
            elif any(not isinstance(item, str) or not item for item in value):
                errors.append(f"{prefix}.{key} must contain non-empty strings")

        criteria = exercise["acceptance_criteria"]
        if (not isinstance(criteria, list) or not criteria or
                any(not isinstance(item, str) or not item for item in criteria)):
            errors.append(
                f"{prefix}.acceptance_criteria must contain non-empty strings")
        source = exercise["source"]
        status = exercise["status"]
        # The existing catalog uses null for a few unstarted exercises.
        # Accept that legacy spelling only where an empty source is allowed.
        legacy_empty_source = (source is None and status in {"planned", "scaffolded"}
                               if isinstance(status, str) else False)
        if not isinstance(source, str) and not legacy_empty_source:
            errors.append(f"{prefix}.source must be a string")
        if (exercise["complexity"] is not None and
                not isinstance(exercise["complexity"], str)):
            errors.append(f"{prefix}.complexity must be a string or null")

        if (status in {"implemented", "tested", "reviewed"}
                if isinstance(status, str) else False) and not source:
            errors.append(f"{prefix}.source is required for implemented work")
        elif isinstance(source, str) and source:
            try:
                source_path = (root_path / source).resolve()
                inside_root = source_path == root_path or root_path in source_path.parents
            except (OSError, RuntimeError, ValueError):
                inside_root = False
                source_path = root_path
            if not inside_root or not source_path.is_file():
                errors.append(f"{prefix}.source does not exist inside repository")
        elif (((isinstance(source, str) and source == "") or legacy_empty_source)
              and isinstance(status, str)
              and status not in {"planned", "scaffolded"}):
            # The status-specific diagnostic above is sufficient for known
            # implemented statuses; invalid statuses also require a source.
            if status not in STATUSES:
                errors.append(f"{prefix}.source must be non-empty for this status")

        examples = exercise["examples"]
        if not isinstance(examples, list):
            errors.append(f"{prefix}.examples must be an array")
        else:
            for example_index, example in enumerate(examples):
                example_prefix = f"{prefix}.examples[{example_index}]"
                if not isinstance(example, dict):
                    errors.append(f"{example_prefix} must be an object")
                    continue
                for inline_key, file_key in (("input", "input_file"),
                                             ("output", "output_file")):
                    has_inline = inline_key in example
                    has_file = file_key in example
                    if has_inline == has_file:
                        errors.append(
                            f"{example_prefix} needs exactly one of "
                            f"{inline_key} or {file_key}")
                    if has_inline and not isinstance(example[inline_key], str):
                        errors.append(f"{example_prefix}.{inline_key} must be a string")
                    if has_file and (not isinstance(example[file_key], str) or
                                     not example[file_key]):
                        errors.append(
                            f"{example_prefix}.{file_key} must be a non-empty string")
                if ("expected_exit_code" in example and
                        (type(example["expected_exit_code"]) is not int)):
                    errors.append(
                        f"{example_prefix}.expected_exit_code must be an integer")
                checks = example.get("checks")
                if "checks" in example:
                    if not isinstance(checks, list):
                        errors.append(f"{example_prefix}.checks must be an array")
                    elif any(not isinstance(check, str) or check not in EXAMPLE_CHECKS
                             for check in checks):
                        errors.append(f"{example_prefix}.checks contains unknown checks")

    for exercise in exercises:
        if not isinstance(exercise, dict):
            continue
        prerequisites = exercise.get("prerequisites")
        if not isinstance(prerequisites, list):
            continue
        identifier = exercise.get("id")
        for prerequisite in prerequisites:
            if (isinstance(prerequisite, str) and
                    prerequisite not in identifiers):
                errors.append(
                    f"{identifier if isinstance(identifier, str) else '<unknown>'} "
                    f"has unknown prerequisite {prerequisite}")
    return errors


def print_summary(exercises: list[dict[str, Any]]) -> None:
    """Print the stable derived totals for a valid catalog."""
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


def main() -> int:
    try:
        catalog = json.loads(CATALOG.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError) as error:
        print(f"catalog error: {error}", file=sys.stderr)
        return 1

    errors = validate_catalog(catalog, ROOT)
    if errors:
        print("catalog error: validation failed", file=sys.stderr)
        for error in errors:
            print(f"- {error}", file=sys.stderr)
        return 1
    print_summary(catalog["exercises"])
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
