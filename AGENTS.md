# Contributor and Agent Instructions

## Repository purpose and layout

This repository teaches C and C++ through progressive lessons, exercises,
reusable topic implementations, paired comparisons, and small projects.
`docs/architecture/target-architecture.md` defines canonical ownership.

- `tracks/c/` and `tracks/cpp/` own lessons and exercise statements.
- `algorithms/` and `data-structures/` own reusable implementations.
- `projects/` owns integrated examples, fixtures, and project-specific tests.
- `comparisons/` contains paired examples; do not copy canonical algorithms.
- `benchmarks/` contains methodology and benchmark drivers, not duplicate
  algorithm implementations.
- `tests/` contains unit, integration, and regression tests.
- `docs/` contains maintained architecture, learning, reference, and
  contribution documentation.

Migration is in progress. `docs/architecture/current-state.md` records the
audited baseline and `migration-map.md` records planned historical paths.
Update the map when moving or intentionally deprecating material. Do not
silently discard legacy exercises, assignment statements, or useful fixtures.

## C and C++ requirements

- Write C examples as standard C, not C++ compiled as C. Use C17 for new C
  targets unless the lesson explicitly teaches another version.
- Write C++ examples as idiomatic C++20 by default. Do not relabel C sources
  or force object-oriented style where another style teaches the concept
  better.
- Use descriptive lower-case `snake_case` identifiers and file names with
  stable exercise IDs. Use four spaces; keep lines under 80 columns where
  practical. Apply the repository `.clang-format` when present.
- Include only required headers. Check input, allocation, arithmetic bounds,
  empty cases, and all resource cleanup paths.
- In C, document ownership and provide matching destruction functions for
  allocated resources. In C++, prefer value types, standard containers, and
  RAII; avoid naked owning pointers.
- Keep examples deterministic unless nondeterminism is the lesson. Label
  intentionally incorrect examples clearly and exclude them from default
  builds.
- Do not claim portability for OS-specific code. Put optional platform or
  external-library examples behind explicit build options.

## Exercise contract

Each exercise needs a stable ID, title, language, difficulty, topics,
prerequisites, learning objectives, statement, constraints, acceptance
criteria, references, and a completion state (`planned`, `scaffolded`,
`implemented`, `tested`, or `reviewed`). Include examples when input/output
applies and complexity when relevant. Record source and verification command
for implemented work. Keep status and counts in the catalog as the single
source of truth; never hand-maintain derived totals. Do not add empty
skeletons unless they support a concrete learning task.

## Build, tests, and validation

CMake is the canonical build system. Root `make`, `make test`, and `make clean`
are convenience entry points. A default build must not require optional
networking/crypto dependencies. Add each independent executable/library as an
explicit target with the correct language standard and warning options.

Tests must assert behavior, including boundary and error cases where relevant.
Use CTest and keep test-only entry points separate from lesson `main`
functions. Add regression tests for repaired historical defects. Do not state
that code is tested unless the actual declared command passed. Run available
format, metadata, build, and test checks affected by a change; report skipped
checks and why. Never add benchmark numbers that were not measured.

## Benchmarks

Every benchmark must define its question, compiler/version, flags, machine/OS,
dataset, warm-up and repetition policy, timing method, and known noise. Keep
inputs reproducible. Treat measurements as environment-specific observations,
not guarantees. Never use noisy runtime thresholds as CI gates.

## Documentation and catalog maintenance

Update README navigation when top-level architecture changes. Cross-link
lessons to canonical shared implementations rather than copying source.
Update exercise metadata and regenerate/validate catalogs after adding,
moving, or changing exercise status. Keep commands and compatibility claims
aligned with the actual build configuration.

## Completion criteria

A change is complete when its source, tests, metadata, documentation, and
migration references agree; applicable build/tests pass; and limitations are
recorded. Before finalizing, inspect `git diff --check` and `git status`, avoid
reverting unrelated user changes, and summarize evidence without overstating
validation.
