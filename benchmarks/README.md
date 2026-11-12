# Sorting benchmark

Run the local comparison with:

```sh
python3 tools/benchmarking/compare_sort.py \
  --count 100000 --seed 20261112 --warmups 2 --repetitions 9 \
  --output benchmarks/results/sort-comparison-local.json
```

The harness compiles a C17 program that calls `qsort` and a C++20 program that
calls `std::sort` and `std::stable_sort`. All use `-O3 -DNDEBUG -Wall -Wextra
-Wpedantic`. It exercises random, already-sorted, reverse-sorted, and
duplicate-heavy inputs. The fixed-width generator uses state = state *
6364136223846793005 + 1 modulo 2^64; random values use the upper 32 bits, and
duplicate-heavy values use state modulo 16. Ordered datasets use integer
ranges. All cases receive the same seed and count.

Each program checks nondecreasing order after timing; the harness also requires
deterministic checksums across implementations for each distribution. The timed
region contains only the sorting call. Allocation, generation, process startup,
sortedness checking, and checksum calculation are excluded. Each case gets the
specified number of warmups followed by measured repetitions; results report
all samples and their median. There are no timing thresholds in CI.

The checked-in result is one observation from the recorded WSL2 environment,
not a general performance claim. `qsort` is a C library function with a
comparator callback; the C++ standard algorithms have different implementations
and storage behavior. This compares algorithm/API and compiler optimization
differences, not language-runtime cost. Process creation is outside the timed
region but adds machine noise; CPU load, scheduling, thermal state, cache state,
compiler, and C library also affect the results. Rerun on the target machine
before drawing conclusions.
