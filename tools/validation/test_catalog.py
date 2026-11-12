#!/usr/bin/env python3
"""Regression tests for exercise catalog validation."""

import copy
import json
from pathlib import Path
import tempfile
import unittest

import catalog


def minimal_catalog(root: Path, **changes: object) -> dict:
    source = root / "example.c"
    source.write_text("int main(void) { return 0; }\n", encoding="utf-8")
    record = {
        "id": "example-one", "title": "Example", "language": "C",
        "difficulty": "beginner", "topics": ["arrays"], "prerequisites": [],
        "learning_objectives": ["Learn arrays"], "statement": "Do a task.",
        "examples": [{"input": "", "output": ""}], "constraints": ["Small"],
        "status": "implemented", "source": "example.c", "validation": "run it",
        "acceptance_criteria": ["It works"], "complexity": None, "references": [],
    }
    record.update(changes)
    return {"schema_version": 1, "exercises": [record]}


class CatalogValidationTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)

    def tearDown(self) -> None:
        self.temp.cleanup()

    def errors(self, data: object) -> list[str]:
        return catalog.validate_catalog(data, self.root)

    def test_valid_minimal_catalog(self) -> None:
        self.assertEqual(self.errors(minimal_catalog(self.root)), [])

    def test_real_catalog(self) -> None:
        data = json.loads(catalog.CATALOG.read_text(encoding="utf-8"))
        self.assertEqual(catalog.validate_catalog(data, catalog.ROOT), [])

    def test_non_object_roots_are_diagnostics(self) -> None:
        for value in ([], None, "catalog"):
            with self.subTest(value=value):
                self.assertTrue(self.errors(value))

    def test_wrong_schema_and_exercises_type(self) -> None:
        data = minimal_catalog(self.root)
        data["schema_version"] = True
        self.assertIn("schema_version must be integer 1", self.errors(data))
        data = minimal_catalog(self.root)
        data["exercises"] = {}
        self.assertIn("exercises must be an array", self.errors(data))

    def test_records_missing_fields_and_duplicate_id(self) -> None:
        data = minimal_catalog(self.root)
        data["exercises"] = [None, {"id": "incomplete"}]
        errors = self.errors(data)
        self.assertTrue(any("must be an object" in error for error in errors))
        self.assertTrue(any("missing fields" in error for error in errors))
        data = minimal_catalog(self.root)
        data["exercises"].append(copy.deepcopy(data["exercises"][0]))
        self.assertTrue(any("duplicate exercise id" in error
                            for error in self.errors(data)))

    def test_enum_and_scalar_types(self) -> None:
        for value in ([], {}):
            data = minimal_catalog(self.root, language=value)
            self.assertTrue(self.errors(data))
        data = minimal_catalog(self.root, status={}, source="")
        self.assertTrue(self.errors(data))
        data = minimal_catalog(self.root, title=3, statement=None, validation=[])
        self.assertTrue(self.errors(data))
        data = minimal_catalog(self.root, complexity=[])
        self.assertTrue(any("complexity" in error for error in self.errors(data)))

    def test_array_member_types_and_prerequisites(self) -> None:
        for field in ("topics", "prerequisites", "learning_objectives",
                      "constraints", "references", "acceptance_criteria"):
            with self.subTest(field=field):
                data = minimal_catalog(self.root, **{field: [[], ""]})
                self.assertTrue(self.errors(data))
        data = minimal_catalog(self.root, prerequisites=[["unhashable"]])
        self.assertTrue(any("prerequisites" in error
                            for error in self.errors(data)))
        data = minimal_catalog(self.root, prerequisites=["missing-id"])
        self.assertTrue(any("unknown prerequisite" in error
                            for error in self.errors(data)))

    def test_examples_validate_alternatives_and_metadata(self) -> None:
        examples = [
            {"input": "", "input_file": "fixture.in", "output": ""},
            {"input_file": "", "output_file": "fixture.out"},
            {"input": [], "output": "", "expected_exit_code": True,
             "checks": ["not-supported"]},
        ]
        data = minimal_catalog(self.root, examples=examples)
        errors = self.errors(data)
        self.assertTrue(any("exactly one" in error for error in errors))
        self.assertTrue(any("non-empty string" in error for error in errors))
        self.assertTrue(any("input must be a string" in error for error in errors))
        self.assertTrue(any("expected_exit_code" in error for error in errors))
        self.assertTrue(any("unknown checks" in error for error in errors))

    def test_source_required_exists_and_confined(self) -> None:
        data = minimal_catalog(self.root, source="")
        self.assertTrue(any("source is required" in error
                            for error in self.errors(data)))
        data = minimal_catalog(self.root, source="missing.c")
        self.assertTrue(any("does not exist" in error for error in self.errors(data)))
        data = minimal_catalog(self.root, source="../outside.c")
        self.assertTrue(any("does not exist inside" in error
                            for error in self.errors(data)))

    def test_symlink_escape(self) -> None:
        outside = self.root.parent / (self.root.name + "-outside.c")
        outside.write_text("outside\n", encoding="utf-8")
        link = self.root / "escape.c"
        try:
            link.symlink_to(outside)
        except (NotImplementedError, OSError) as error:
            outside.unlink(missing_ok=True)
            self.skipTest(f"symlinks unavailable: {error}")
        try:
            data = minimal_catalog(self.root, source="escape.c")
            self.assertTrue(any("does not exist inside" in error
                                for error in self.errors(data)))
        finally:
            outside.unlink(missing_ok=True)

    def test_malformed_inputs_never_escape_as_exceptions(self) -> None:
        cases = [[], None, "bad", {"schema_version": True},
                 {"schema_version": 1, "exercises": [dict(id=[], language={})]},
                 minimal_catalog(self.root, prerequisites=[{}])]
        for value in cases:
            with self.subTest(value=value):
                self.assertIsInstance(self.errors(value), list)


if __name__ == "__main__":
    unittest.main()
