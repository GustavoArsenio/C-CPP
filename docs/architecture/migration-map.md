# Migration Map

Status: partial. Mappings are based on original baseline
`ea1241027a98552c2b9d065948d0a24f0c16142b`. The migration is committed on
`codex/educational-repo-refactor`; removed legacy source files remain
recoverable with `git show ea1241027a98552c2b9d065948d0a24f0c16142b:<path>`.
This ledger records destinations and remaining gaps so migration does not
erase educational provenance.

| Source path | Planned destination | Treatment |
| --- | --- | --- |
| `Lista_1/Ex1.c`–`Ex4.c` | `tracks/c/01-foundations/exercises/lista-1/01-...`–`04-...` | MIGRATED as corrected solutions with metadata and output smoke tests; all nine required items were checked against the PDF and linked from the catalog. Six complementary items are catalogued with planned status. |
| `Lista_1/Ex5.c`–`Ex9.c` | `tracks/c/01-foundations/exercises/lista-1/05-...`–`09-...` | MIGRATED as corrected array exercises with metadata and fixture-backed output smoke tests. |
| `Lista_1/Lista_1 (1).pdf` | `docs/references/assignments/lista-1.pdf` | COPIED byte-for-byte; requirements transcribed to `docs/references/assignments/specifications.md`, with all required exercise records linked to the PDF. |
| `Lista_2/ex02.c`, `ex05.c`, `ex07.c`, `ex08.c`, `ex10.c`, `ex13.c` | `tracks/c/04-arrays-strings/exercises/lista-2/01-...`–`06-...` | MIGRATED and reconciled against their PDFs; catalog links preserve the source, and smoke fixtures cover multi-case input, exact formatting, floating-point operations, and boundaries. |
| Lista 2 exercise PDFs | `docs/references/assignments/` | COPIED byte-for-byte; text reviewed and summarized in `specifications.md`; Lista 2 records link to source PDFs. |
| `Lista_2/matrizFill I.c`, `matrizFill III.c` | `tracks/c/04-arrays-strings/exercises/lista-2/07-concentric-matrix.c`, `08-powers-of-two-matrix.c` | MIGRATED; define and check supported dimensions and integer width. |
| `Lista_2/Entrada.txt` | `docs/references/fixtures/lista-2-entrada.txt` | COPIED byte-for-byte as a raw matrix payload: it contains only values 1 through 144. The payload is linked from Lista 2 ex10 and ex13; it is not a complete runnable input because it has no row/operation selector. Their executable smoke fixtures add the required selectors while retaining the same 144 values. The original commit added this file alongside unrelated card-game changes, so its exact original intended exercise remains inferred rather than proven. |
| `Lista_3/Ex1.c`, `lista.c`, `lista.h` | `data-structures/linear/int_stack.{h,c}`, `data-structures/linear/char_stack.{h,c}`, `tracks/c/09-data-structures/stack/01-stack-demo.c`, `tests/unit/test_int_stack.c`, `tests/unit/test_char_stack.c` | MIGRATED: the duplicated integer/character stack behavior is represented by type-safe opaque APIs with unit tests; the two assignment demonstrations retain separate contexts. |
| `Lista_3/Ex2.c` | `tracks/c/09-data-structures/stack/05-transfer-between-stacks.c` | MIGRATED with output smoke test. |
| `Lista_3/Ex3.c`, `Ex4.c` | `tracks/c/09-data-structures/stack/02-convert-base.c` | MERGED into one parameterized base-conversion exercise for bases 2–36, including binary zero and hexadecimal smoke examples. |
| `Lista_3/Ex5.c`, `Ex6.c` | `tracks/c/09-data-structures/stack/03-reverse-line.c`, `04-palindrome.c` | MIGRATED with bounded line input and tests covering spaces, palindromes, and non-palindromes. |
| `Lista_3/main.c` | no direct source destination | Commented duplicate exercise snippets were split into the canonical individual exercises above. |
| `Lista_4/lista.c`, `Lista_4/lista.h` | `data-structures/linear/` | Verified byte-identical to Lista 3; existing stack APIs preserve shared implementation. Distinct Lista 4 linked-list requirements are transcribed separately; exercises 1–8 use opaque integer/double APIs and exercises 9–12 use the focused `lista4_lists` API for character copies, circular conversion, and doubly linked mutation. |
| `Lista_3/ReadMe.txt`, `compilar.sh`, `compilarParam.sh`; `Lista_4/ReadMe.txt`, `compilarParam.sh` | `docs/references/legacy-build-notes.md` | Preserve relevant build history, supersede scripts with CMake/Make targets. |
| `Lista_1/ReadMe.txt`, `Lista_2/ReadMe.txt`, root `ReadMe.txt` | `docs/references/legacy-build-notes.md` or archive manifest | Extract useful context; empty/root duplicates may be dropped only after mapping. |
| `C++/Matriz.c`, `C++/algebra.c`, `C++/algebra.h`, `C++/ReadMe.txt` | `tracks/c/10-algorithms/linear-algebra/01-matrix-operations.c`, `tracks/c/10-algorithms/linear-algebra/matrix-operations.h`, `tracks/c/10-algorithms/linear-algebra/02-matrix-menu.c` | MIGRATED: the bounded linked collection supports listing/lookup, appended sum/product, selected transpose/exact determinant, and a repeated interactive menu for add, arithmetic, factorial, determinant, transpose, display, and exit. Menu input is checked and collection storage is released at session end. Appended arithmetic results must fit signed 32-bit entries. |
| `C++/in.txt` | `docs/references/fixtures/matrix-input.txt` | COPIED byte-for-byte and retained as a historical interactive matrix-menu transcript. It starts by adding two matrices, then uses the legacy selection `2 0 1` and quoted result name `"SomaA+B"`; the current menu uses one-based selections and unquoted names. The remaining tokens do not form a valid current-menu session, so this fixture is not used as a current test input. |
| `C++/start.bat` | `docs/references/legacy-build-notes.md` | Record Windows build context; replace with cross-platform CMake usage. |
| `ProgBahia/MinimosQuadradosRegLinear.c` | `tracks/c/10-algorithms/regression/01-linear-regression.c` | MIGRATED as bounded least-squares fit with checked input, singular-case rejection and fixture-backed smoke tests. |
| `ProgBahia/saida` | `docs/references/fixtures/regression-output.txt` | COPIED byte-for-byte as historical evidence of the old zero-point `nan` defect; corrected deterministic fixtures are maintained beside the replacement. |
| `ProgBahia/compilarParam.sh` | `docs/references/legacy-build-notes.md` | Replace script with project target. |
| `ProjetoCartas/deck.c`, `deck.h`, `main.c` | `tracks/c/09-data-structures/card-deck/01-deal-three-cards.c` | PARTIALLY MIGRATED: the historical generic distributor remains as a separate exercise; a canonical 40-card linked-list Truco model now provides seeded shuffle, four hands, vira, manilha ranking, circular player order, a wager-integrated hand coordinator that resolves in up to three tricks, scorekeeping, hand-of-eleven behavior, face-down plays after trick one and opaque partner signals. The hidden-card ranking interpretation and signal-code convention are explicit policies because the assignment leaves them undefined. Deterministic automatic and user-controlled match frontends use the shared state machine. The historical renderer is migrated to a portable ASCII module and CLI. |
| `ProjetoCartas/templateCarta.c` | `projects/card-game-c/card_renderer.{h,c}`, `render-card.c` | MIGRATED as a portable C17 renderer and CLI; golden/unit tests cover width, rows, enum mappings, invalid values, and unchanged output. The original OEM glyph rendering is replaced with printable ASCII. |
| Historical matrix and card functionality | `tracks/c/10-algorithms/linear-algebra/01-matrix-operations.c`, `tracks/c/09-data-structures/card-deck/01-deal-three-cards.c` | Catalog entries cover matrix addition, multiplication, exact integer determinant, transpose, factorial, generic dealing, and Truco deck/dealing; smoke cases include arithmetic overflow rejection, determinant row-swap/singular and wide exact cases, factorial boundaries, invalid deck size, seeded Truco dealing, and unique cards. |
| `ProjetoCartas/cartas.txt`, `nipes.txt` | `docs/references/fixtures/card-values.txt`, `card-suits.txt` | COPIED byte-for-byte and retained as historical-only fixtures. The tested ASCII renderer maps the canonical Truco enums directly and does not consume these legacy text files. |
| `ProjetoCartas/compilarParam.sh` | `docs/references/legacy-build-notes.md` | Replace script with project target. |
| `Lista_4/lista4.pdf`, `Trabalho/Trabalho1.pdf` | `docs/references/assignments/lista-4.pdf`, `trabalho-1.pdf` | COPIED byte-for-byte; both visually reviewed and summarized in `specifications.md`. All Lista 4 exercises are implemented and tested; Truco ambiguities have documented engine or frontend policies and test coverage. The renderer has portable ASCII output and CLI coverage. |
| `Listas_Gustavo_Arsenio_de_Sousa_74830.zip` | `docs/references/archive-manifest.md` | REVIEWED: 26/30 entries are byte-identical; four differ only by a format bug or whitespace. No unique useful content; deprecate archive. |
| `C++/*.o`, `*.exe`, `*.execls`; `ProgBahia/*.o`, `Executaveis/*.exe`; `ProjetoCartas/*.o`, `*.exe`; `.vscode/ipch/**` | no destination | Generated artifacts; exclude from source migration, document removal/ignore policy. |
| `beginner/basic/01-hello.c`, `02-var.c`; `beginner/io/01-input.c` | `tracks/c/01-foundations/{basic,io}/` | MOVED; indexed in the partial exercise catalog. |
| `beginner/loops/01-for-loop.c`, `02-while.c` | `tracks/c/02-control-flow/loops/` | MOVED; indexed in the partial exercise catalog. |
| `beginner/strings/01-dup.c`, `intermidiate/arrays/01-sum.c` | `tracks/c/04-arrays-strings/{strings,arrays}/` | MOVED; added missing stdio include; indexed in catalog. |
| `intermidiate/memory/01-malloc.c`, `intermidiate/pointers/01-ptr.c` | `tracks/c/05-pointers-memory/{memory,pointers}/` | MOVED; indexed in the partial exercise catalog. |
| New C++ Rule-of-Zero ownership lesson | `tracks/cpp/09-modern-cpp/04-vector-ownership.cpp` | ADDED as the C++ counterpart to the existing `c-memory-allocate-array` lesson; both own five integers and print the same sum. The C example uses explicit `malloc`/`free`; the C++ aggregate owns a `std::vector` without custom special members. |
| `intermidiate/fileio/01-fileio.c` | `tracks/c/07-file-io/fileio/` | MOVED; indexed in the partial exercise catalog. |
| New paired file line-count lessons | `tracks/c/07-file-io/line-count/01-count-lines.c`, `tracks/cpp/09-modern-cpp/03-count-file-lines.cpp` | ADDED as C17 and C++20 lessons with one shared byte-wise LF contract, bounded path input, path terminator handling, empty/final unterminated files, checked counter overflow, and file cleanup/error handling. The shared harness covers readable files, open failures, and path boundaries; portable read/close failure injection is not included. |
| `intermidiate/searching/01-search.c`, `intermidiate/sorting/01-qs.c` | `algorithms/{searching,sorting}/` | MOVED; added missing stdlib include; corrected description to identify standard `qsort`, not quicksort. |
| `advanced/data-structures/01-btree.c` | `data-structures/trees/01-btree.c` | MOVED; now checks allocation and frees all nodes. |
| `advanced/concurrency/05-queue.c` | `tracks/c/11-systems-programming/concurrency/05-queue.c` | MOVED and rewritten as a bounded blocking queue; build/run checked. |
| New `data-structures/linear/int_stack.{h,c}`, `char_stack.{h,c}`, `tracks/c/09-data-structures/stack/01-stack-demo.c`, and unit tests | Canonical linked stacks | Type-safe integer and character APIs based on historical exercises; CTest covers empty operations, LIFO order, peek, size, null arguments, and destruction. |
| `advanced/networking/01-socket.c` | `tracks/c/11-systems-programming/networking/01-socket.c` | MOVED and rewritten using POSIX `getaddrinfo`, checked connect/send/close, and a configurable endpoint. Optional build passes; local integration is SKIPPED because this sandbox denies socket creation; CI covers loopback. |
| `advanced/security/01-hash.c` | `tracks/c/12-advanced/security/01-hash.c` | MOVED and rewritten with OpenSSL EVP SHA-256. Optional build and known digest output test pass. |
| Sources under `tracks/cpp/01-foundations`, `02-control-flow`, `03-functions`, `04-functions`, `05-classes`, `06-classes`, `08-stl`, and `09-modern-cpp` | C++ track | Sixteen lesson sources are catalogued as tested; function templates, exception parsing, RAII, vector ownership, and growable-vector input have catalog smoke coverage. |
| `cpp-exception-parse` catalog exercise | `tracks/cpp/09-modern-cpp/01-parse-integer.cpp` | IMPLEMENTED: strict decimal input parsing distinguishes malformed input from `int` range errors and shares a 128-byte line contract with its C counterpart. Catalog smoke cases cover valid signs, exact line boundary, EOF, empty/missing input, malformed syntax, embedded NUL, overlong input, and range errors. |
| New C integer parsing counterpart | `tracks/c/03-functions/02-parse-integer.c` | ADDED as a C17 comparison to `cpp-exception-parse`; both use the same signed-decimal grammar, 128-byte line limit, complete-input checks, and output/error cases. C reports conversion status via `errno`, end pointer, and an enum; C++ catches `std::stoi` exceptions. |

## Migration invariants

- Every historical source has a destination or a documented reason for
  deprecation.
- PDFs and input fixtures are not discarded until their relationship to
  exercises is established.
- The directory `C++/` is not treated as a C++ track; no old source is
  relabeled as C++.
- Duplicate linked-list code is merged only after comparing contents and
  preserving the distinct exercise context.
- Build artifacts are excluded only after source and reproducibility needs
  are accounted for.
- At migration completion, replace this plan with verified old-to-new paths,
  including any deliberate removals and reasons.
- The catalog includes all required Lista 1 and Lista 2 exercises, the six Lista 1 complementary exercises as planned, and all twelve Lista 4 exercises. All twelve have tested implementations.
