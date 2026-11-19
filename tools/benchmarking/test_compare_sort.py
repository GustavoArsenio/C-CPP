#!/usr/bin/env python3
"""Tests for sorting benchmark scheduling and result structure."""

import argparse
from collections import Counter
import importlib.util
import json
from pathlib import Path
import statistics
import subprocess
import sys
import unittest

sys.dont_write_bytecode = True


SCRIPT = Path(__file__).with_name("compare_sort.py")
SPEC = importlib.util.spec_from_file_location("compare_sort", SCRIPT)
compare_sort = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(compare_sort)


class CompareSortTests(unittest.TestCase):
    def test_paired_ratio_summary_handles_values_and_zero_times(self):
        summary = compare_sort.paired_ratio_summary([100, 200], [50, 100])
        self.assertEqual(summary["ratio_samples"], [2.0, 2.0])
        self.assertEqual(summary["median_ratio"], 2.0)
        self.assertEqual(summary["valid_samples"], 2)

        summary = compare_sort.paired_ratio_summary(
            [100, 60, 50], [0, 30, 0])
        self.assertEqual(summary["ratio_samples"], [None, 2.0, None])
        self.assertEqual(summary["median_ratio"], 2.0)
        self.assertEqual(summary["valid_samples"], 1)

    def test_paired_ratio_summary_handles_empty_and_unmatched_samples(self):
        summary = compare_sort.paired_ratio_summary([], [])
        self.assertEqual(summary, {
            "ratio_samples": [], "median_ratio": None,
            "valid_samples": 0})
        summary = compare_sort.paired_ratio_summary([10], [0])
        self.assertIsNone(summary["median_ratio"])
        self.assertEqual(summary["valid_samples"], 0)
        with self.assertRaises(ValueError):
            compare_sort.paired_ratio_summary([10], [])

    def test_counts_require_positive_unique_decimal_values(self):
        self.assertEqual(compare_sort.positive_count("17"), 17)
        self.assertEqual(compare_sort.parse_counts("17,29"), [17, 29])
        for value in ("0", "-1", "+1", "1.0", "2147483648", "",
                      "9" * 5000):
            with self.subTest(value=value):
                with self.assertRaises(argparse.ArgumentTypeError):
                    compare_sort.positive_count(value)
        with self.assertRaises(argparse.ArgumentTypeError):
            compare_sort.parse_counts("17,17")

    def test_cyclic_schedule_is_repeatable_and_balanced(self):
        labels = ["first", "second", "third"]
        for rounds in (1, 2, 3, 5, 6, 9, 10):
            with self.subTest(rounds=rounds):
                orders = compare_sort.orders_by_round(labels, rounds, offset=2)
                self.assertEqual(
                    orders,
                    compare_sort.orders_by_round(labels, rounds, offset=2))
                self.assertTrue(all(sorted(order) == sorted(labels)
                                    for order in orders))
                for position in range(len(labels)):
                    positions = Counter(order[position] for order in orders)
                    self.assertLessEqual(max(positions.values()) -
                                         min(positions.values()), 1)
                    if rounds % len(labels) == 0:
                        self.assertEqual(len(set(positions.values())), 1)

    def test_real_harness_sweep_reports_cases_and_checksums(self):
        completed = subprocess.run(
            [sys.executable, str(SCRIPT), "--counts", "40,80", "--seed",
             "1234", "--warmups", "1", "--repetitions", "3"],
            check=True, capture_output=True, text=True)
        result = json.loads(completed.stdout)

        self.assertIsNone(result["count"])
        self.assertEqual(result["counts"], [40, 80])
        self.assertEqual(len(result["results"]), 2 * 4 * 3)
        self.assertEqual(len(result["run_orders"]), 2 * 4)
        self.assertEqual(len(result["comparisons"]), 2 * 4)
        self.assertIn("not significance tests",
                      result["paired_ratio_definition"])
        labels = result["implementation_order"]
        for count in result["counts"]:
            for distribution in result["distributions"]:
                key = f"{count} / {distribution}"
                schedule = result["run_orders"][key]
                self.assertEqual(len(schedule["warmups"]), 1)
                self.assertEqual(len(schedule["measured"]), 3)
                for order in schedule["measured"]:
                    self.assertEqual(sorted(order), sorted(labels))
                for position in range(len(labels)):
                    starts = Counter(order[position]
                                     for order in schedule["measured"])
                    self.assertEqual(set(starts.values()), {1})
                checksums = set()
                for label in labels:
                    sample = result["results"][f"{key} / {label}"]
                    self.assertEqual(sample["count"], count)
                    self.assertEqual(sample["distribution"], distribution)
                    self.assertEqual(len(sample["samples_ns"]), 3)
                    checksums.add(sample["checksum"])
                self.assertEqual(len(checksums), 1)

                comparison = result["comparisons"][key]
                self.assertEqual(comparison["baseline"], "C qsort")
                baseline_samples = result["results"][
                    f"{key} / C qsort"]["samples_ns"]
                self.assertEqual(set(comparison["candidates"]),
                                 set(labels) - {"C qsort"})
                for label, summary in comparison["candidates"].items():
                    candidate_samples = result["results"][
                        f"{key} / {label}"]["samples_ns"]
                    ratios = [
                        None if candidate == 0 else baseline / candidate
                        for baseline, candidate in zip(
                            baseline_samples, candidate_samples)
                    ]
                    valid_ratios = [
                        ratio for ratio in ratios if ratio is not None]
                    self.assertEqual(summary["ratio_samples"], ratios)
                    self.assertEqual(summary["valid_samples"],
                                     len(valid_ratios))
                    self.assertEqual(
                        summary["median_ratio"],
                        statistics.median(valid_ratios)
                        if valid_ratios else None)

    def test_count_options_are_mutually_exclusive(self):
        completed = subprocess.run(
            [sys.executable, str(SCRIPT), "--count", "10", "--counts",
             "10,20"], capture_output=True, text=True)
        self.assertEqual(completed.returncode, 2)
        self.assertIn("not allowed with argument", completed.stderr)

    def test_single_count_option_retains_count_metadata(self):
        completed = subprocess.run(
            [sys.executable, str(SCRIPT), "--count", "30", "--seed",
             "1234", "--warmups", "0", "--repetitions", "1"],
            check=True, capture_output=True, text=True)
        result = json.loads(completed.stdout)
        self.assertEqual(result["count"], 30)
        self.assertEqual(result["counts"], [30])
        self.assertEqual(len(result["results"]), 4 * 3)


if __name__ == "__main__":
    unittest.main()
