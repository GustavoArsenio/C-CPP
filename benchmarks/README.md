# Sorting benchmark

The harness compares C `qsort`, C++ `std::sort`, and C++ `std::stable_sort`
over reproducible random, sorted, reverse-sorted, and duplicate-heavy integer
datasets. Each executable uses `-O3 -DNDEBUG -Wall -Wextra -Wpedantic` with
C17 or C++20. The timed region contains only the sorting call; allocation,
input generation, process startup, sortedness checks, and checksum calculation
are outside the measured interval.

## Smoke run

Run a small sweep to check that the compilers and harness work:

```sh
python3 tools/benchmarking/compare_sort.py \
  --counts 100,1000 --seed 20261112 --warmups 1 --repetitions 3
```

The harness reports JSON to standard output. A focused no-threshold test covers
the schedule and executes this same harness with small datasets:

```sh
python3 tools/benchmarking/test_compare_sort.py
```

## Larger workloads

For a multi-size observation, use the same seed and a larger dataset sweep:

```sh
python3 tools/benchmarking/compare_sort.py \
  --counts 10000,100000,1000000 --seed 20261112 \
  --warmups 2 --repetitions 9 \
  --output /tmp/sort-comparison-sweep.json
```

This command is a suggested workload, not a reported measurement. The native
benchmark programs cap `count` at their implementation's `INT_MAX` and also
need enough memory for the arrays. The Python interface limits a requested
count to 2,147,483,647; a platform with a smaller `int` range may reject a
lower count in the native program. Avoid workloads that cause swapping or
exhaust available memory.

`--count N` remains available for one dataset size; if neither count option is
given, the default is 100000. `--counts` accepts unique, positive, comma-
separated decimal values. Warmups may be zero; measured repetitions must be
positive. No timing threshold is used as a test gate.

## Ordering and reproducibility

Each round runs every implementation once. The base order is rotated by
`(count index + distribution index + round index) modulo implementation
count`. Warmup and measured rounds are rotated separately and the emitted JSON
records the actual order for each count and distribution; rotation continues
across the warmup/measured boundary. Measured start and position counts differ
by at most one, and are equal when the repetition count is divisible by the
number of implementations. The schedule is deterministic;
repeating the same arguments reproduces dataset generation and execution
order, though not timing values.

The fixed-width generator uses the 64-bit LCG
`state = state * 6364136223846793005 + 1 modulo 2^64`; random values use the
upper 32 bits, and duplicate-heavy values use `state modulo 16`. Ordered
datasets use integer ranges. Every implementation receives the same count and
seed. Native programs check nondecreasing order, and the harness requires
checksums to match across all implementations and samples for a given count
and distribution. JSON includes compiler versions, flags, standards, raw
samples, medians, environment details, and per-round order.

## Interpreting results

These observations do not isolate language runtime cost. `qsort` uses a
comparator callback; the C++ algorithms may use different library
implementations and storage strategies. C timing uses
`clock_gettime(CLOCK_MONOTONIC)` and C++ uses `std::chrono::steady_clock`.
Process startup is excluded from the timed interval but still affects machine
load and cache state. CPU scheduling, background load, thermals, compiler,
standard library, and C library contribute noise. The harness has no timing
thresholds in CI.

The checked-in [`sort-comparison-local.json`](results/sort-comparison-local.json)
is a historical observation from WSL2 using the earlier fixed implementation
order. It is not a result from the current cyclic schedule, nor a native
Windows or Linux measurement. Rerun the full command on the exact target
machine and preserve its emitted environment metadata before drawing claims.
Do not compare results collected on WSL with native results as if only the
algorithm changed.
