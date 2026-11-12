# Benchmark Methodology

The repository includes one sorting comparison. This document defines the
minimum method for future benchmark work so results remain reproducible.

Each case must state:

- The question being measured and the competing implementations.
- Compiler name/version, language standard, flags, and build type.
- CPU, memory, operating system, and relevant power/virtualization state.
- Dataset source, size, generation seed, and checksum where practical.
- Warm-up count, timed repetitions, aggregation method, and timing API.
- Whether allocation, parsing, setup, and output are inside the timed region.
- Known sources of noise and limitations.

Use deterministic inputs, validate outputs before timing, and repeat runs
under comparable conditions. Report raw samples or enough summary data to
reproduce the calculation. Treat measurements as observations tied to that
environment. CI must not fail because a benchmark is slower than an arbitrary
threshold.

When the run order supports matched rounds, keep the pairing in the result and
define ratio direction explicitly. Report the median of per-round ratios as a
separate statistic from the ratio of medians. Preserve unavailable ratios
instead of dropping their positions, and document the denominator policy. A
paired ratio describes these observations; it does not establish statistical
significance or isolate one implementation detail by itself.

A first runnable case is available in `benchmarks/`. It compares C `qsort`
with C++ `std::sort`, verifies matching result checksums, records raw samples,
and stores compiler and platform metadata. The checked-in local result is a
single WSL2 observation; it is not a CI threshold or a broad claim about C and
C++ performance. The comparison includes different library/API designs and
must be interpreted with that limitation.
