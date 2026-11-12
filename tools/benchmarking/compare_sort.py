#!/usr/bin/env python3
"""Compare C qsort, C++ std::sort, and C++ std::stable_sort."""

import argparse
import json
import os
from pathlib import Path
import platform
import statistics
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
DISTRIBUTIONS = ("random", "sorted", "reverse", "duplicates")


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


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--count", type=int, default=100_000)
    parser.add_argument("--seed", type=int, default=20261112)
    parser.add_argument("--warmups", type=int, default=2)
    parser.add_argument("--repetitions", type=int, default=9)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    if args.count < 1 or args.warmups < 0 or args.repetitions < 1:
        parser.error("count/repetitions must be positive and warmups non-negative")

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
        for distribution in DISTRIBUTIONS:
            expected = None
            for label, (binary, algorithm, compiler, flags, standard) in implementations.items():
                samples = []
                checksums = set()
                for iteration in range(args.warmups + args.repetitions):
                    elapsed, checksum = run_case(binary, args.count, args.seed,
                                                 distribution, algorithm)
                    checksums.add(checksum)
                    if iteration >= args.warmups:
                        samples.append(elapsed)
                if len(checksums) != 1:
                    raise RuntimeError(f"{label} output is nondeterministic")
                checksum = checksums.pop()
                if expected is not None and checksum != expected:
                    raise RuntimeError(f"sorted checksums differ for {distribution}")
                expected = checksum
                results[f"{distribution} / {label}"] = {
                    "compiler": version(compiler), "flags": flags,
                    "standard": standard, "checksum": checksum,
                    "samples_ns": samples,
                    "median_ns": int(statistics.median(samples)),
                }

    result = {
        "case": "C qsort vs C++ std::sort and std::stable_sort",
        "count": args.count, "seed": args.seed,
        "distributions": list(DISTRIBUTIONS),
        "generator": "64-bit LCG: state = state * 6364136223846793005 + 1 modulo 2^64; random uses high 32 bits, duplicates uses state modulo 16; sorted/reverse use ordered integer ranges",
        "warmups_per_case": args.warmups,
        "repetitions_per_case": args.repetitions,
        "timed_region": "sort call only; allocation, generation, process startup, and sortedness/checksum checks excluded",
        "timing_method": "C clock_gettime(CLOCK_MONOTONIC); C++ std::chrono::steady_clock",
        "implementation_order": ["C qsort", "C++ std::sort", "C++ std::stable_sort"],
        "correctness": "each native program verifies nondecreasing order; harness requires matching deterministic checksums across implementations",
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
