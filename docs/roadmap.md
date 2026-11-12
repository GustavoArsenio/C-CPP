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

- Migrate the historical card renderer into a portable C17 example. Use only
  printable ASCII, format rank/suit through the canonical card enums, and avoid
  terminal code-page assumptions. Acceptance: deterministic fixed-width card
  output, exact golden tests for multiple ranks and suits, and rejection of
  invalid enum values without out-of-bounds lookup. Reconcile the retained
  `card-values.txt` and `card-suits.txt` fixtures with those tests.
- Resolve the remaining legacy input-fixture associations, including
  `Lista_2/Entrada.txt` and `C++/in.txt`: connect each to a named exercise and
  expected result, or document why it is retained as historical-only data.
- Add C/C++ paired examples for ownership, containers, algorithms, and error
  handling without duplicating canonical implementations.
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
