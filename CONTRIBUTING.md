# Contributing

Contributions should make a concept easier to learn or make the repository
more reliable. Open an issue for substantial curriculum additions, new
dependencies, or changes to the public exercise conventions.

## Add an exercise

1. Choose the language track and concept directory from
   `docs/architecture/target-architecture.md`.
2. Assign a stable ID and include title, language, difficulty, topics,
   prerequisites, learning objectives, problem statement, constraints,
   acceptance criteria, references, and complexity when relevant in
   `exercises/catalog.json`.
3. Keep the source self-contained unless the exercise explicitly teaches a
   reusable API. Put shared implementations in their canonical topic folder.
4. Add deterministic tests for expected behavior and important boundaries.
5. Update the migration map when retaining or moving historic material.

Do not add an empty starter or a duplicate implementation just to increase
catalog size. Use `planned` for a complete exercise specification without a
starter, `scaffolded` for an intentionally incomplete starter, `implemented`
before its checks pass, `tested` after the declared checks pass, and `reviewed`
only after a documented human review.

## Code and documentation

Use C17 and C++20 by default. Use four spaces, lower-case `snake_case` in C,
and follow `.clang-format`. Prefer standard-library facilities, explicit
resource ownership, checked input, and deterministic examples. Explain
complexity and design trade-offs when they are part of the lesson.

Keep beginner explanations direct. Do not claim a solution is correct,
portable, secure, thread-safe, or faster without evidence. For benchmark
contributions, document methodology and environment and never add unmeasured
results.

## Local validation

Formatting requires Python 3, Git, and clang-format 18. On Ubuntu 24.04,
install the formatter with `sudo apt-get install clang-format-18`. CI pins
that Ubuntu release and formatter major version and runs the same
`make format-check` command as local validation. The check covers tracked
C/C++ sources and headers in `tracks`, `algorithms`, `data-structures`,
`projects`, `tests`, `comparisons`, and `benchmarks`.

```sh
make format-check
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
python3 tools/validation/catalog.py
git diff --check
```

Use `make format` to apply the style before checking. If clang-format 18 has
a different executable name, use `make format-check CLANG_FORMAT=clang-format`
(or the same override with `make format`); other major versions are rejected.
New source files must be added to Git's index to enter the formatting scope.

Use `cmake --preset sanitized` when GCC/Clang sanitizer support is available,
or configure with `-DLEARNING_ENABLE_GCC_ANALYZER=ON` for GCC's C static
analyzer. `-DLEARNING_ENABLE_CLANG_TIDY=ON` requires `clang-tidy` on PATH and
analyzes both C and C++ targets with the checked-in `clang-analyzer-*` profile.
The Ubuntu CI job is configured to report this baseline without treating
warnings as errors; review its diagnostics before expanding checks or making
warnings blocking. To run the same profile locally:

```sh
cmake -S . -B build/clang-tidy -G Ninja \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DLEARNING_ENABLE_CLANG_TIDY=ON
cmake --build build/clang-tidy --parallel
```

If a tool or platform feature is unavailable, report it and the affected
checks. CI currently builds with GCC and Clang on Linux.

## Review checklist

- The exercise has a unique ID and accurate completion status.
- Inputs, allocations, bounds, edge cases, and cleanup are covered.
- Tests verify outputs or API behavior, not just successful compilation.
- C and C++ examples use the intended language standard idiomatically.
- Build requirements and platform limits are explicit.
- Catalog, docs, and migration map match the source tree.
