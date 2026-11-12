# Roadmap

Priorities are based on the remaining requirements in the refactor brief.

## P0 — preserve and complete the foundation

- All twelve Lista 4 exercises now have tested implementations, including both character-copy variants and circular/doubly linked-list operations.
- The bounded matrix collection now has a repeated interactive menu for add,
  appended sum/product, factorial, selected determinant/transpose, display,
  and exit. Determinant, addition, multiplication, transpose and factorial have
  catalog examples, including exact determinant
  output for 1x1 through 8x8 integer matrices. The Truco engine and
  interactive frontend now implement face-down plays after trick one and
  opaque partner signals under documented policies. The seeded automatic and
  user-controlled matches share the tested hand, wager, and score-to-12 engine.
- Review the regression method and historical fixtures; a corrected bounded
  least-squares implementation and deterministic cases now exist.
- All readable assignment PDFs are reconciled with catalog records; the remaining Trabalho 1 card-game behavior is tracked explicitly.
- A dedicated C structures and enum lesson now validates calendar-date
  invariants, including Gregorian leap-year boundaries. The min/max API
  teaches function parameters, return status, output pointers, and separate
  header/source compilation. File line counting covers lifecycle, LF/CRLF/EOF
  path input, and unterminated final lines.
- The C++ track has 13 tested lesson sources, including a generic maximum
  function template, RAII, classes, overloads, `std::sort`, and exception-based
  parsing. Advanced topics remain.
- The binary search tree and bounded queue now have CTest unit coverage for
  ordering, duplicate insertion, empty/full states, FIFO draining, and close;
  queue tests avoid timing-dependent waits. Opaque stack/list APIs, Lista 4
  collections and Truco state transitions also have unit coverage.
- Resolve ownership/provenance and apply a license only after maintainer
  confirmation; see `architecture/licensing.md`.
- Confirm the POSIX loopback integration in hosted CI and update its catalog
  state based on that result.

## P1 — broaden engineering coverage

- The historical card renderer is migrated as a portable C17 library module and CLI. Golden and unit tests verify 12 rows of 19 columns, newline/NUL termination, all 40 enum combinations, and invalid lower/upper enum values with unchanged output. CLI catalog smoke cases cover valid, invalid, and missing input. The byte-preserved `card-values.txt` and `card-suits.txt` remain historical-only fixtures; the renderer uses canonical enums directly.
- Paired C17/C++20 line-count lessons now share byte-wise LF semantics and boundary tests, while comparing `FILE *` cleanup with `std::ifstream` RAII. Read/close failure branches are implemented but not injected by the portable test harness.
- The C `qsort` and C++ `std::sort` lessons now use the same dataset and byte-identical output, with API and complexity differences documented separately from benchmark observations.
- The C `malloc`/`free` array lesson is now paired with a C++ `std::vector` Rule-of-Zero owner using the same data and output; allocation-failure handling is implemented, but that branch is not injected by the smoke suite.
- Legacy fixture associations are recorded in the migration map: the 1–144
  values in `Lista_2/Entrada.txt` are a shared raw matrix payload for Lista 2
  ex10/ex13, while `C++/in.txt` remains historical-only because its old menu
  tokens do not satisfy the current one-based, unquoted-name protocol.
- Paired ownership and sorting examples now exist without custom algorithm
  duplication.
- A C opaque growable-vector API and a C++ `std::vector` lesson now share
  input/output examples; C unit coverage checks API boundaries and capacity
  growth. Allocator failure injection remains outside the portable suite.
- Paired container examples now exist. A signed-integer parsing pair compares
  C `errno`/end-pointer/status handling with C++ exceptions; add further
  error-handling comparisons for APIs with different failure models.
- Add data structures and algorithms with complexity discussions and boundary
  tests.
- Expand the sorting benchmark with larger datasets, additional algorithms,
  order-balanced runs, and reruns on native (non-WSL) machines. The current
  harness already covers random, sorted, reverse, and duplicate-heavy inputs.
- Add sanitizer and static-analysis coverage for optional targets and project
  code.
- Add contribution and learning documentation in Portuguese and English.
- The catalog is the authoritative backlog for planned and scaffolded
  exercises; run `python3 tools/validation/catalog.py` for current totals.

## P2 — deepen advanced topics

- Add design-pattern examples only where they clarify a real language or
  systems trade-off.
- Add advanced concurrency, memory-model, profiling, and performance lessons.
- Add compiler/platform matrix coverage beyond Linux when maintainers can
  validate those environments.
