"""Regression tests for the catalog-driven executable smoke runner."""

from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import patch

import smoke_example


def result(stdout="", stderr="", returncode=0):
    return subprocess.CompletedProcess(["program"], returncode, stdout, stderr)


class SmokeExampleTests(unittest.TestCase):
    def setUp(self):
        self.exercise = {"id": "sample", "examples": [{"output": ""}]}

    def run_example(self, stdout="", stderr="", returncode=0, example=None):
        if example is not None:
            if (isinstance(example, dict) and "input" not in example
                    and "input_file" not in example):
                example["input"] = ""
            self.exercise["examples"] = [example]
        with patch.object(smoke_example.subprocess, "run",
                          return_value=result(stdout, stderr, returncode)):
            smoke_example.check_example(self.exercise, 1, "program")

    def test_valid_cards(self):
        stdout = "Player 1: 9/4 6/4 8/1\nPlayer 2: 6/2 7/3 8/4\n"
        self.run_example(stdout, example={
            "output": stdout,
            "checks": ["unique_cards_per_player"],
        })

    def test_empty_and_no_hand_output_rejected(self):
        for output in ("", "\n"):
            with self.subTest(output=output), self.assertRaises(ValueError):
                self.run_example(output, example={
                    "output": output,
                    "checks": ["unique_cards_per_player"],
                })

    def test_malformed_line_and_card_counts_rejected(self):
        for output in ("Player 1: 1 2 3 extra\n", "not a hand\n",
                       "Player 1: 1 2\n", "Player 1: 1 2 3 4\n",
                       "\nPlayer 1: 1 2 3\n",
                       "Player 0: 1 2 3\n", "Player 1:1 2 3\n"):
            with self.subTest(output=output), self.assertRaises(ValueError):
                self.run_example(output, example={
                    "output": output,
                    "checks": ["unique_cards_per_player"],
                })

    def test_duplicate_cards_within_and_across_hands_rejected(self):
        for output in ("Player 1: x y x\n",
                       "Player 1: x y z\nPlayer 2: q z r\n",
                       "Player 1: a b c\nPlayer 1: d e f\n"):
            with self.subTest(output=output), self.assertRaises(ValueError):
                self.run_example(output, example={
                    "output": output,
                    "checks": ["unique_cards_per_player"],
                })

    def test_exact_stdout_and_exit_status_are_checked(self):
        with self.assertRaises(ValueError):
            self.run_example(stdout="actual", example={"output": "expected"})
        with self.assertRaises(ValueError):
            self.run_example(returncode=1, example={"output": ""})
        with self.assertRaises(ValueError):
            self.run_example(returncode=0, example={
                "output": "", "expected_exit_code": 1,
            })
        self.run_example(returncode=1, example={
            "output": "", "expected_exit_code": 1,
        })

    def test_stderr_nonempty_and_unknown_check(self):
        with self.assertRaises(ValueError):
            self.run_example(example={"output": "",
                                     "checks": ["stderr_nonempty"]})
        self.run_example(stderr="failure\n", example={
            "output": "", "checks": ["stderr_nonempty"],
        })
        with self.assertRaisesRegex(ValueError, "Unknown smoke check"):
            self.run_example(example={"output": "", "checks": ["unknown"]})

    def test_fixture_paths_must_be_strings_and_inside_root(self):
        with tempfile.TemporaryDirectory(dir=smoke_example.ROOT) as temp_dir:
            fixture = Path(temp_dir) / "fixture.txt"
            fixture.write_text("fixture", encoding="utf-8")
            relative = fixture.relative_to(smoke_example.ROOT).as_posix()
            self.assertEqual(smoke_example.read_fixture(relative, "sample"),
                             "fixture")
        for value in (None, 4, "../outside", "."):
            with self.subTest(value=value), self.assertRaises(ValueError):
                smoke_example.read_fixture(value, "sample")

    def test_symlink_escape_is_rejected(self):
        with tempfile.TemporaryDirectory() as outside_dir:
            outside = Path(outside_dir) / "outside.txt"
            outside.write_text("outside", encoding="utf-8")
            with tempfile.TemporaryDirectory(dir=smoke_example.ROOT) as temp_dir:
                link = Path(temp_dir) / "escape.txt"
                try:
                    link.symlink_to(outside)
                except (NotImplementedError, OSError) as error:
                    self.skipTest(f"platform disallows symlink creation: {error}")
                try:
                    relative = link.relative_to(smoke_example.ROOT).as_posix()
                    with self.assertRaises(ValueError):
                        smoke_example.read_fixture(relative, "sample")
                finally:
                    link.unlink(missing_ok=True)

    def test_fixture_input_and_output_are_used(self):
        with tempfile.TemporaryDirectory(dir=smoke_example.ROOT) as temp_dir:
            input_path = Path(temp_dir) / "input.txt"
            output_path = Path(temp_dir) / "output.txt"
            input_path.write_text("input", encoding="utf-8")
            output_path.write_text("output", encoding="utf-8")
            relative_input = input_path.relative_to(smoke_example.ROOT).as_posix()
            relative_output = output_path.relative_to(smoke_example.ROOT).as_posix()
            with patch.object(smoke_example.subprocess, "run",
                              return_value=result("output")) as mocked_run:
                smoke_example.check_example({
                    "id": "sample",
                    "examples": [{"input_file": relative_input,
                                  "output_file": relative_output}],
                }, 1, "program")
            self.assertEqual(mocked_run.call_args.kwargs["input"], "input")

    def test_non_object_example_and_boolean_exit_code_rejected(self):
        with self.assertRaises(ValueError):
            self.run_example(example="not an object")
        with self.assertRaisesRegex(ValueError, "expected_exit_code"):
            self.run_example(example={"output": "", "expected_exit_code": True})
        with self.assertRaisesRegex(ValueError, "needs input"):
            with patch.object(smoke_example.subprocess, "run",
                              return_value=result()):
                smoke_example.check_example(
                    {"id": "sample", "examples": [{"output": ""}]},
                    1, "program",
                )

    def test_malformed_catalog_json_returns_concise_failure(self):
        with tempfile.TemporaryDirectory(dir=smoke_example.ROOT) as temp_dir:
            catalog_directory = Path(temp_dir) / "exercises"
            catalog_directory.mkdir()
            catalog_path = catalog_directory / "catalog.json"
            catalog_path.write_text("{", encoding="utf-8")
            with patch.object(smoke_example, "ROOT", Path(temp_dir)):
                with patch("sys.argv", ["smoke_example.py", "--source",
                                         "source.c", "--executable", "program"]):
                    with patch("sys.stderr") as stderr:
                        self.assertEqual(smoke_example.main(), 1)
            self.assertIn("Unable to read exercise catalog", "".join(
                str(call.args[0]) for call in stderr.write.call_args_list
            ))


if __name__ == "__main__":
    unittest.main()
