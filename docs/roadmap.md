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
- Add broader C++ template coverage; a generic maximum function template now
  accompanies classes, overloads, `std::sort`, and exception-based parsing.
- The binary search tree and bounded queue now have CTest unit coverage for
  ordering, duplicate insertion, empty/full states, FIFO draining, and close;
  queue tests avoid timing-dependent waits. Opaque stack/list APIs, Lista 4
  collections and Truco state transitions also have unit coverage.
- Resolve ownership/provenance and apply a license only after maintainer
  confirmation; see `architecture/licensing.md`.
- Confirm the POSIX loopback integration in hosted CI and update its catalog
  state based on that result.

## P1 — broaden engineering coverage

- Add C/C++ paired examples for ownership, containers, algorithms, and error
  handling without duplicating canonical implementations.
- Add data structures and algorithms with complexity discussions and boundary
  tests.
- Expand the first reproducible sorting benchmark with distributions, larger
  datasets, multiple algorithms, and reruns on native (non-WSL) machines.
- Add sanitizer and static-analysis coverage for optional targets and project
  code.
- Add contribution and learning documentation in Portuguese and English.

## P2 — deepen advanced topics

- Add design-pattern examples only where they clarify a real language or
  systems trade-off.
- Add advanced concurrency, memory-model, profiling, and performance lessons.
- Add compiler/platform matrix coverage beyond Linux when maintainers can
  validate those environments.
