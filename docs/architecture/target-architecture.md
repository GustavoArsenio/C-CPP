# Target Architecture

## Goals

The repository is a teaching collection, not a production library. It must
make C and C++ learning paths independent, keep every lesson discoverable,
and let examples be built and verified without requiring unrelated platform
dependencies. Existing assignment material remains traceable to its source.

## Canonical layout

```text
tracks/
  c/01-foundations/ ... 12-advanced/
  cpp/01-foundations/ ... 12-advanced/
projects/
  # Add a project directory when a multi-file project needs a distinct build
  # and test boundary; small historical programs remain in their track.
algorithms/                 # canonical reusable implementations
  searching/ sorting/ recursion/ dynamic-programming/ graphs/
data-structures/            # canonical reusable implementations
  linear/ trees/ hash-based/ graphs/
comparisons/                 # paired C/C++ examples and discussion
benchmarks/                  # cases, datasets, methodology, results
exercises/catalog.json       # generated/validated exercise index
tests/                       # unit, integration, regression
docs/
  getting-started/ learning-paths/ language-comparisons/
  architecture/ performance/ references/ contributing/
tools/                       # catalog, validation, benchmark helpers
```

Tracks own progressive lessons and exercise statements. A reusable algorithm
or data structure has one implementation in its canonical topic directory;
lessons link to it. Integrated project applications own their CLI, fixtures,
and project-specific tests when they need a distinct build boundary.
`comparisons/` holds paired implementations only when the same task is
meaningfully expressed in both languages. Benchmarks call canonical code and
do not carry separate algorithm implementations.

The proposed `examples/` root is omitted because standalone learning examples
belong in tracks, reusable implementations in topic directories, and larger
integrated examples in projects. A separate `design-patterns/` root is also
omitted initially: C and C++ pattern examples belong in their respective
tracks and can be indexed by topic. Add a dedicated directory only if the
catalog demonstrates enough content to justify another navigation axis.

## Exercise contract

Each exercise has a stable ID and a `README.md` (or `exercise.json` plus a
short statement) containing title, language, level, topics, prerequisites,
learning objectives, problem statement, input/output examples, constraints,
completion state, and references. Implemented exercises identify source and
verification commands. Pending exercises contain no fake solution or empty
starter unless a starter materially supports the task. The catalog is the
single source for counts and status; counts are derived, never hand-maintained.

Completion states are `planned`, `scaffolded`, `implemented`, `tested`, and
`reviewed`. `planned` has an exercise statement and acceptance criteria but no
starter source. `scaffolded` has an intentionally incomplete starter.
`implemented` means a solution exists but its declared checks have not all
passed. `tested` means its build and declared tests passed. `reviewed` means
the implementation, tests, and exercise wording also received a documented
human review. A runnable demo without meaningful assertions is not described
as tested.

## Build and compatibility

Use CMake as the canonical build interface, with separate C and C++ standards
and independently buildable targets. Keep root `make`, `make test`, and
`make clean` as documented convenience wrappers. Default builds include only
portable, dependency-free targets; optional networking/crypto examples must
declare and detect their platform/library requirements. C examples target
C17; C++ examples target C++20 unless an individual lesson explicitly teaches
an older standard. No third-party dependency is required for the core build.

Tests use CTest and a small dependency-free test harness where practical.
Sanitizers and static analysis are opt-in build options because compiler and
platform support varies. GCC's `-fanalyzer` option analyzes C targets;
optional `clang-tidy` integration can analyze both languages when available.
CI should cover at least GCC and Clang on Linux,
build all default targets, run tests, and validate exercise metadata. Formatting
uses checked-in `.clang-format` configuration compatible with the documented
style.

## Quality and safety

Examples must check input and allocation results, define ownership, handle
empty and boundary cases, and avoid undefined behavior. If an exercise
intentionally demonstrates a bug, label it as such and isolate it from the
default build. Prefer standard-library facilities and portable APIs. Do not
claim constant-time security, thread safety, performance superiority, or
benchmark results without appropriate evidence.

## Performance material

Every benchmark documents the question, compiler/version, flags, machine and
OS, dataset generation, warm-up/repetition policy, timing method, and known
sources of noise. Results are observations tied to a recorded environment,
not promises. Inputs and output formats are deterministic where possible.
No benchmark belongs in CI as a pass/fail speed gate.

## Trade-offs

Language/topic-specific directories create some navigation overlap, so the
catalog and links provide cross-cutting discovery without copying source.
Per-exercise metadata is more work than a flat source folder, but enables
accurate counts and progress reporting. CMake adds a tool requirement, offset
by reproducible multi-language builds and test integration; the Make wrapper
preserves a simple entry point. Historic assignment PDFs remain references
where their original context is valuable, while concise exercise statements
provide accessible current documentation.
