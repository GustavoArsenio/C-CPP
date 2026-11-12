# C and C++ Learning Repository

An open educational repository for learning C and C++ through progressive
lessons, exercises, reusable algorithms, projects, and benchmark methodology.
The curriculum and migration are being built in phases; the architecture
status and known gaps are documented below.

## Start here

1. Install C and C++ compilers, CMake 3.20+, and Make.
2. Configure and build the portable examples:

   ```sh
   cmake -S . -B build
   cmake --build build
   ```

3. Run automated checks with `ctest --test-dir build --output-on-failure` or
   `make test`.

The default build compiles the portable examples. The POSIX TCP and OpenSSL
examples are optional; see the [optional build guide](docs/getting-started/optional-examples.md).
Catalog and output checks run through CTest when Python 3 is available.

## Learning material

- [Current-state audit](docs/architecture/current-state.md)
- [Target architecture](docs/architecture/target-architecture.md)
- [Migration map](docs/architecture/migration-map.md)
- [Exercise catalog](exercises/catalog.json)
- [Preserved assignment PDFs](docs/references/assignments/README.md)
- [License proposal and status](docs/architecture/licensing.md)
- [C learning path](docs/learning-paths/c.md)
- [C++ learning path](docs/learning-paths/cpp.md)
- [Benchmark guide and local result](benchmarks/README.md)
- [Roadmap](docs/roadmap.md)

The intended canonical organization separates C and C++ tracks, reusable
algorithms and data structures, integrated projects, paired language
comparisons, and benchmarks. Historical assignment solutions and statements
are mapped from the original repository; see the migration map for their
destinations and remaining gaps. The directory named `C++/` in the old
repository contained C programs and is not a C++ curriculum.

## Current status

- C examples exist from introductory through advanced topics.
- The C++ track includes foundations, control flow, functions, classes, STL,
  exception-based parsing, RAII, and a function template; advanced topics
  remain.
- The exercise catalog is the single source of status and coverage counts;
  run `python3 tools/validation/catalog.py` to validate it and print totals.
- Existing and historical examples are under review for correctness,
  portability, safety, and pedagogy.
- CI checks GCC/Clang builds, catalog-driven checks, formatting, GCC's C
  analyzer, sanitizers, and optional examples. The linked-stack API has a unit
  test, and a local C/C++ sorting comparison is available under `benchmarks/`.

Uma introdução em português está disponível em [README.pt-BR.md](README.pt-BR.md).

See [AGENTS.md](AGENTS.md) for contribution rules. Please do not treat an
example as a verified reference solution until its exercise status and
validation command say so.
