# Current State Audit

Audit date: 2026-11-12

## Scope and evidence

The initial audit compared baseline
`ea1241027a98552c2b9d065948d0a24f0c16142b` with the then-current working tree,
which contained 84 legacy path deletions and new `beginner/`, misspelled
`intermidiate/`, and `advanced/` examples. That migration is committed on
`codex/educational-repo-refactor`; removed source files remain recoverable from
the baseline commit in Git history. Assignment PDFs and selected fixtures are
retained under `docs/references/`.

The current tree has 16 C sources and no C++ source. The original tree has 32
C sources, 4 headers, 11 PDFs, input/output fixtures and shell scripts, plus
generated binaries, object files, and editor cache files. The directory named
`C++/` contains C, not C++.

## Initial working-tree architecture

The current layout groups examples by difficulty and topic, but does not have
exercise metadata, a catalog, tests, or documentation beyond a short README.
The README promises challenge descriptions and starter/solution files that
are not present. The Makefile uses `$(wildcard **/*.c)`, which GNU Make does
not expand recursively by default; its `BIN` substitution is malformed, and
the clean target only removes root object files. Consequently the advertised
build is not a reliable repository-wide build.

The initial sources were standalone C programs. They included beginner I/O,
loops and strings; intermediate arrays, file I/O, memory, pointers, search and
sorting; and advanced trees, sockets, hashing and a mutex queue. The advanced
networking and hashing examples depend on nonstandard/platform libraries and
are not portable as written. The socket example has invalid headers/structure
usage and omits error handling. The hash source includes SHA APIs but refers
to MD5 symbols/constants. The queue does not represent empty/full states,
can overwrite unread elements, ignores pthread errors, and has a consumer
function with no return. The tree example does not check allocation failure
or free nodes. The string example omits `<stdio.h>`. The sorting example
omits `<stdlib.h>` for `qsort`. Several files use compressed formatting and
the initial intermediate directory misspelled `intermediate`.

## Historical content inventory and disposition

`KEEP` means preserve the educational idea and inputs; `REFACTOR` means retain
the idea while correcting correctness, safety, portability, or style issues;
`RELOCATE` means move to the canonical location chosen in the target
architecture; `SPLIT`/`MERGE` mean reorganize related material; `DEPRECATE`
means retain the original as archived/reference material while documenting a
replacement. These are preliminary classifications; migration must record
each final destination in `migration-map.md`.

| Historical path(s) | Classification | Audit notes / likely destination |
| --- | --- | --- |
| `C++/Matriz.c`, `C++/algebra.c`, `C++/algebra.h`, `C++/in.txt` | REFACTOR, RELOCATE | Matrix arithmetic and menu project are C, with shared operations exposed by `tracks/c/10-algorithms/linear-algebra/matrix-operations.h`. The collection frontend supports listing/lookup, appended sum/product, selected transpose and exact determinant; the repeated interactive menu restores the historical add/arithmetic/factorial/determinant/transpose/display/exit workflow. Appended values are bounded to signed 32-bit entries. |
| `C++/ReadMe.txt`, `C++/start.bat` | RELOCATE | Build/use notes and platform-specific launcher; translate useful instructions to portable documentation and build targets. |
| `Lista_1/Ex1.c`–`Ex9.c` | KEEP, REFACTOR, RELOCATE | Introductory input, arithmetic, conditionals, loops, and arrays. Assign individual exercise IDs and retain Portuguese prompts where they carry context. Fix unchecked input, type/format mismatches, and missing return paths. |
| `Lista_1/Lista_1 (1).pdf` | KEEP, RELOCATE | Original assignment statement; preserve as reference or transcribe its requirements into exercise metadata. |
| `Lista_1/ReadMe.txt`, `Lista_1/compilarParam.sh` | RELOCATE | Build instructions/scripts; replace with documented portable commands if still useful. |
| `Lista_2/ex02.c`, `ex05.c`, `ex07.c`, `ex08.c`, `ex10.c`, `ex13.c` | KEEP, REFACTOR, RELOCATE | Array, Fibonacci, classification, and matrix exercises. Multiple unchecked reads; `ex05` allocates too few elements and indexes out of bounds; `ex08` may print uninitialized values; `ex10`/`ex13` pass a `char *` incorrectly to `%s`; validate limits and arithmetic. |
| `Lista_2/matrizFill I.c`, `matrizFill III.c` | KEEP, REFACTOR, RELOCATE | Matrix pattern exercises. VLAs need explicit portability constraints; validate dimensions and prevent integer overflow. `pow` introduces floating-point conversion for an integer pattern. |
| `Lista_2/*.pdf` | KEEP, RELOCATE | Eight assignment/problem statements are source material for exercise descriptions; keep source attribution/context and fixtures where relevant. |
| `Lista_2/Entrada.txt` | KEEP, RELOCATE | Input fixture; associate it with the exercise(s) it serves and document expected use. |
| `Lista_2/ReadMe.txt`, `Lista_2/compilarParam.sh` | RELOCATE | Legacy build notes/scripts; assess before replacing. |
| `Lista_3/Ex1.c`–`Ex6.c`, `Lista_3/main.c` | KEEP, REFACTOR, SPLIT, RELOCATE | Stack/list, base conversion, string reversal, and palindrome exercises are mixed with globals and commented-out exercises. Preserve each task separately; check empty-stack behavior, allocation failures, string input, and cleanup. |
| `Lista_3/lista.c`, `Lista_3/lista.h` | REFACTOR, RELOCATE | Reusable linked-list/stack implementation. Review ownership, empty operations, allocation sizing, API naming, and duplicate char/int implementations. |
| `Lista_3/ReadMe.txt`, `compilar.sh`, `compilarParam.sh` | RELOCATE | Historical build guidance; replace with reproducible build instructions. |
| `Lista_4/lista.c`, `Lista_4/lista.h` | MERGE, REFACTOR, RELOCATE | Byte-identical-looking duplicate list implementation; verify hashes during migration, then keep one canonical implementation with links/references. |
| `Lista_4/lista4.pdf`, `Lista_4/ReadMe.txt`, `Lista_4/compilarParam.sh` | KEEP, RELOCATE | Assignment specification and build context; retain or transcribe into exercises. |
| `ProgBahia/MinimosQuadradosRegLinear.c` | KEEP, REFACTOR, RELOCATE | Migrated to a bounded least-squares example with checked input, `double` calculations, singular-data rejection, and exact-fit smoke coverage. |
| `ProgBahia/compilarParam.sh`, `ProgBahia/saida` | RELOCATE | Build script replaced by the CMake target. The historical output contained `nan` from a zero-point run; corrected deterministic cases are maintained beside the regression example. |
| `ProjetoCartas/deck.c`, `deck.h`, `main.c` | KEEP, REFACTOR, SPLIT, RELOCATE | A portable seeded deck shuffle and three-card deal is now covered by smoke checks. Original file-backed game rules and card rendering are not yet migrated. |
| `ProjetoCartas/templateCarta.c`, `cartas.txt`, `nipes.txt` | KEEP, RELOCATE | Rendering example and data fixtures; associate with the card project and clarify encoding/portability. |
| `ProjetoCartas/compilarParam.sh` | RELOCATE | Replace with portable build target after project boundaries are defined. |
| `Trabalho/Trabalho1.pdf` | KEEP, RELOCATE | Assignment document; inspect and classify its educational content before deciding whether it becomes exercises or a reference artifact. |
| `Listas_Gustavo_Arsenio_de_Sousa_74830.zip` | DEPRECATE | Archive duplicate of assignment material; inspect archive manifest/content and avoid keeping redundant binary archive if its contents are represented elsewhere. Do not delete until verified. |
| `C++/*.o`, `*.exe`, `*.execls`; `ProgBahia/*.o`, `Executaveis/*.exe`; `ProjetoCartas/*.o`, `*.exe`; `ProjetoCartas/.vscode/ipch/**` | DEPRECATE | Generated build/editor artifacts, not canonical learning content. Exclude from new build and ignore rules; retain source and fixtures. |
| Root `ReadMe.txt` | DEPRECATE | Empty file; supersede with maintained README after migration is complete. |

## Quality and safety findings

The historical sources contain common beginner mistakes that are valuable
teaching material only when the learning objective is explicit and unsafe
behavior is not silently presented as a correct reference solution. Observed
issues include unchecked `scanf`/`fopen`, buffer and array overruns, use of
uninitialized values, format-string/type mismatches, leaks, unchecked
allocation, integer overflow, and algorithms with incorrect edge cases. The
matrix, linked-list, regression, and card projects require focused review and
should not be compiled as a single translation unit.

No C++ implementation exists in the historical source inventory. The migrated
tree has a separate C++20 track with 13 tested lesson sources; it continues to
expand independently rather than relabeling historical C examples.

## Initial architectural recommendation

Use language-specific learning tracks as the canonical home for lessons and
exercises. Give every exercise stable metadata and validate it with the
catalog tool. Put reusable algorithms/data structures in a shared topic
library only when they are independently reusable; lessons link to those
canonical implementations. Use project directories when an integrated
application needs its own build/test boundary. Avoid copying implementations
into parallel `algorithms/`, `data-structures/`, and `exercises/` trees.

## Audit status and subsequent work

Phase 1 established the baseline, recorded defects, and classified historical
paths. The classifications are provisional where PDFs, archives, or fixtures
require content-level review. Subsequent work moved the initial C examples
to `tracks/c/`, `algorithms/`, and `data-structures/`; corrected the tree and
bounded queue; migrated seventeen exercises from the first two lists; added
a linked-stack library with unit tests, matrix arithmetic and exact integer
determinant, linear regression, card dealing, and the C++ lessons. CMake,
catalog validation, and catalog-driven output smoke tests are in place.
Catalog counts are calculated by `tools/validation/catalog.py`.
The POSIX TCP client builds, but
its loopback integration was skipped locally because the sandbox denies
socket creation; CI runs that case on its hosted runner.

The historical source inventory is recoverable from the baseline commit in
Git history, but not every old source or fixture has a direct replacement.
The catalog exceeds the brief's minimum of eighty records. Assignment PDFs
have been reviewed and summarized, and the matrix project now has a dynamic
collection workflow and interactive menu.
The card implementation does not load the historical rank/suit files or
reproduce its original renderer. It has automatic and user-controlled matches,
with explicit policies for rules omitted by the assignment. The C++ track has
lessons across six topics; unit coverage exercises the linked stacks and Truco
state machine. Several legacy fixtures and the historical card renderer still
need explicit disposition. A reproducible sorting comparison covers four
distributions and records environment metadata, but the suite still lacks
larger datasets, additional algorithms, and native non-WSL measurements. CI checks
GCC/Clang builds, optional OpenSSL/POSIX examples, smoke tests, formatting,
GCC analysis, and sanitizers. Locally, LeakSanitizer requires
`ASAN_OPTIONS=detect_leaks=0` because the sandbox runs under ptrace; address
and undefined-behavior checks pass with that setting. See
`migration-map.md` for the per-path migration ledger and remaining work.
