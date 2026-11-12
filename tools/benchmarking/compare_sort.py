#!/usr/bin/env python3
"""Compare C qsort, C++ std::sort, and C++ std::stable_sort."""

import argparse
from functools import lru_cache
import json
import os
from pathlib import Path
import platform
import statistics
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
DISTRIBUTIONS = ("random", "sorted", "reverse", "duplicates")
DEFAULT_COUNT = 100_000
# The native benchmark takes count as an int and caps it at INT_MAX.
MAX_COUNT = 2_147_483_647


@lru_cache(maxsize=None)
def version(compiler):
    result = subprocess.run([compiler, "--version"], text=True,
                            capture_output=True, check=True)
    return result.stdout.splitlines()[0]


def machine_details():
    cpu = platform.processor() or platform.machine()
    cpuinfo = Path("/proc/cpuinfo")
    if cpuinfo.is_file():
        for line in cpuinfo.read_text(encoding="utf-8", errors="replace").splitlines():
            if line.lower().startswith("model name"):
                cpu = line.split(":", 1)[1].strip()
                break
    memory_bytes = None
    if hasattr(os, "sysconf"):
        try:
            memory_bytes = os.sysconf("SC_PAGE_SIZE") * os.sysconf("SC_PHYS_PAGES")
        except (ValueError, OSError):
            pass
    return {"cpu": cpu, "memory_bytes": memory_bytes,
            "kernel": platform.release(), "python": platform.python_version()}


def run_case(binary, count, seed, distribution, algorithm):
    arguments = [str(binary), str(count), str(seed), distribution]
    if algorithm != "qsort":
        arguments.append(algorithm)
    output = subprocess.run(arguments, text=True, capture_output=True,
                            check=True).stdout.split()
    elapsed, checksum, sorted_flag = output
    if sorted_flag not in ("1", "true"):
        raise RuntimeError(f"{algorithm} did not sort {distribution} input")
    return int(elapsed), checksum


def positive_count(value):
    if not value or any(character < "0" or character > "9"
                        for character in value):
        raise argparse.ArgumentTypeError("count must be a positive decimal")
    try:
        count = int(value)
    except ValueError as error:
        raise argparse.ArgumentTypeError("count is too large") from error
    if count < 1 or count > MAX_COUNT:
        raise argparse.ArgumentTypeError(
            f"count must be between 1 and {MAX_COUNT}")
    return count


def parse_counts(value):
    counts = [positive_count(part) for part in value.split(",")]
    if len(set(counts)) != len(counts):
        raise argparse.ArgumentTypeError("counts must not contain duplicates")
    return counts


def orders_by_round(labels, rounds, offset=0):
    """Return deterministic cyclic rotations, one complete order per round."""
    if not labels:
        raise ValueError("at least one implementation is required")
    return [
        [labels[(offset + round_index + position) % len(labels)]
         for position in range(len(labels))]
        for round_index in range(rounds)
    ]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    count_group = parser.add_mutually_exclusive_group()
    count_group.add_argument("--count", type=positive_count,
                             help="one dataset size (default: 100000)")
    count_group.add_argument("--counts", type=parse_counts,
                             help="comma-separated dataset sizes")
    parser.add_argument("--seed", type=int, default=20261112)
    parser.add_argument("--warmups", type=int, default=2)
    parser.add_argument("--repetitions", type=int, default=9)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    if args.warmups < 0 or args.repetitions < 1:
        parser.error("repetitions must be positive and warmups non-negative")
    counts = (args.counts if args.counts is not None else
              [args.count if args.count is not None else DEFAULT_COUNT])

    compiler_c = os.environ.get("CC", "cc")
    compiler_cpp = os.environ.get("CXX", "c++")
    flags_c = ["-std=c17", "-O3", "-DNDEBUG", "-Wall", "-Wextra", "-Wpedantic"]
    flags_cpp = ["-std=c++20", "-O3", "-DNDEBUG", "-Wall", "-Wextra", "-Wpedantic"]
    results = {}
    with tempfile.TemporaryDirectory(prefix="sort-benchmark-") as temporary:
        c_binary = Path(temporary) / "sort-c"
        cpp_binary = Path(temporary) / "sort-cpp"
        subprocess.run([compiler_c, *flags_c,
                        str(ROOT / "benchmarks/cases/sort_c.c"),
                        "-o", str(c_binary)], check=True)
        subprocess.run([compiler_cpp, *flags_cpp,
                        str(ROOT / "benchmarks/cases/sort_cpp.cpp"),
                        "-o", str(cpp_binary)], check=True)
        implementations = {
            "C qsort": (c_binary, "qsort", compiler_c, flags_c, "c17"),
            "C++ std::sort": (cpp_binary, "std_sort", compiler_cpp,
                              flags_cpp, "c++20"),
            "C++ std::stable_sort": (cpp_binary, "stable_sort", compiler_cpp,
                                     flags_cpp, "c++20"),
        }
        base_order = list(implementations)
        run_orders = {}
        for count_index, count in enumerate(counts):
            for distribution_index, distribution in enumerate(DISTRIBUTIONS):
                expected = None
                samples = {label: [] for label in base_order}
                checksums = {label: set() for label in base_order}
                schedule_offset = count_index + distribution_index
                warmup_orders = orders_by_round(
                    base_order, args.warmups, schedule_offset)
                measured_orders = orders_by_round(
                    base_order, args.repetitions,
                    schedule_offset + args.warmups)
                run_orders[f"{count} / {distribution}"] = {
                    "warmups": warmup_orders,
                    "measured": measured_orders,
                }
                all_orders = warmup_orders + measured_orders
                for iteration, order in enumerate(all_orders):
                    measured = iteration >= args.warmups
                    for label in order:
                        binary, algorithm, *_ = implementations[label]
                        elapsed, checksum = run_case(
                            binary, count, args.seed, distribution, algorithm)
                        checksums[label].add(checksum)
                        if measured:
                            samples[label].append(elapsed)

                for label, (_, _, compiler, flags,
                            standard) in implementations.items():
                    if len(checksums[label]) != 1:
                        raise RuntimeError(
                            f"{label} output is nondeterministic")
                    checksum = checksums[label].pop()
                    if expected is not None and checksum != expected:
                        raise RuntimeError(
                            "sorted checksums differ for "
                            f"{count} / {distribution}")
                    expected = checksum
                    results[f"{count} / {distribution} / {label}"] = {
                        "count": count, "distribution": distribution,
                        "compiler": version(compiler), "flags": flags,
                        "standard": standard, "checksum": checksum,
                        "samples_ns": samples[label],
                        "median_ns": int(statistics.median(samples[label])),
                    }

    result = {
        "case": "C qsort vs C++ std::sort and std::stable_sort",
        "count": counts[0] if len(counts) == 1 else None,
        "counts": counts, "seed": args.seed,
        "distributions": list(DISTRIBUTIONS),
        "generator": (
            "64-bit LCG: state = state * 6364136223846793005 + 1 modulo "
            "2^64; random uses high 32 bits, duplicates uses state modulo "
            "16; sorted/reverse use ordered integer ranges"),
        "warmups_per_case": args.warmups,
        "repetitions_per_case": args.repetitions,
        "timed_region": (
            "sort call only; allocation, generation, process startup, and "
            "sortedness/checksum checks excluded"),
        "timing_method": "C clock_gettime(CLOCK_MONOTONIC); C++ std::chrono::steady_clock",
        "implementation_order": list(implementations),
        "implementation_order_policy": (
            "One run of every implementation per round; each round rotates "
            "the base order by (count index + distribution index + round "
            "index) modulo implementation count. Warmup and measured orders "
            "are recorded separately, with rotation continuous across the "
            "phases. Measured position counts differ by at most one."),
        "run_orders": run_orders,
        "correctness": (
            "each native program verifies nondecreasing order; harness "
            "requires matching deterministic checksums across implementations"),
        "platform": platform.platform(), "machine": platform.machine(),
        "machine_details": machine_details(), "results": results,
    }
    rendered = json.dumps(result, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(rendered, encoding="utf-8")
    print(rendered, end="")


if __name__ == "__main__":
    main()
