# C Learning Path

The path follows C17. Read the short catalog statement before compiling each
program; use the catalog's status to distinguish complete examples from work
that still needs implementation.

| Stage | Material currently available | Main gaps |
| --- | --- | --- |
| Foundations | Basic types, standard input/output, nine migrated introductory array/input exercises | More exercises based on the original PDF after its text is reviewed |
| Control flow | `for` and `while` examples | Conditions, switch, nested loops, validation exercises |
| Functions | `03-functions/01-min-max.c`, bounded signed-integer parsing with `strtoimax`, and reusable `algorithms/statistics/int_min_max` API | Recursion, scope, function pointers, arrays as parameters |
| Arrays and strings | Array sum, sequence generation, Fibonacci, parity partition, matrix operations and patterns, bounded string copy, modular growable integer vector | Dynamic strings, parsing, safe interfaces, multidimensional API design |
| Pointers and memory | Pointer dereference and checked `malloc`/`free` allocation paired with a C++ vector owner | Ownership contracts, pointer arithmetic, allocation APIs, error-path exercises |
| Structures and files | `06-structures/01-calendar-date.c` and its header/source API, standard-stream line copy, checked file line counter, tree uses a struct | Binary file operations |
| Data structures and algorithms | Binary search tree, linked stacks/lists, bounded queue, linear search, `qsort` comparator, open-addressed string counter with a paired word-count lesson | Sorting implementations and broader complexity comparisons |
| Systems and advanced | Bounded POSIX queue; optional TCP and OpenSSL examples | Signals, processes, robust I/O, security threat models, portability variants |

The catalog is the canonical list. Do not infer that an empty curriculum area
is complete merely because the directory structure exists.

For a paired view of the C `qsort` interface and C++ `std::sort`, see
[`sorting.md`](../language-comparisons/sorting.md).

For a paired view of explicit C allocation and C++ Rule-of-Zero ownership, see
[`ownership.md`](../language-comparisons/ownership.md).

For a paired view of a modular C dynamic-array API and C++ `std::vector`, see
[`containers.md`](../language-comparisons/containers.md).

For a paired view of C `errno`/end-pointer handling and C++ parsing exceptions,
see [`error-handling.md`](../language-comparisons/error-handling.md).

For a paired view of C open addressing and C++ `std::map` word counting, see
[`associative-containers.md`](../language-comparisons/associative-containers.md).
