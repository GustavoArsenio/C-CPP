# Integer parsing errors in C and C++

The paired lessons
[C `strtoimax` parser](../../tracks/c/03-functions/02-parse-integer.c) and
[C++ `std::stoi` parser](../../tracks/cpp/09-modern-cpp/01-parse-integer.cpp)
parse the same bounded line and produce the same result for the catalog cases.
Both accept an optional ASCII sign and decimal digits, reject whitespace and
suffixes, and limit input to 128 bytes before the line terminator. A valid
final line may end at EOF without a newline.

| Concern | C17 | C++20 |
| --- | --- | --- |
| Conversion | `strtoimax` returns an `intmax_t` and an end pointer | `std::stoi` returns `int` and can report consumed characters |
| Failure signals | Reset and inspect `errno` for `ERANGE`; inspect the end pointer; return an explicit status | Catch `std::invalid_argument` for invalid input and `std::out_of_range` for a value outside `int` |
| `int` range | Check `INT_MIN` and `INT_MAX` before converting the `intmax_t` result | `std::stoi` reports values outside the `int` range with `std::out_of_range` |
| Input storage | A 129-byte array stores at most 128 input bytes plus the NUL terminator; an overlong line is drained and rejected | A `std::string` stores at most 128 bytes while the reader drains the rest of an overlong line |
| Allocation | The line reader uses fixed storage and does not allocate | The string uses RAII; `std::bad_alloc` while reading is reported on stderr |

For valid input, both print `value=<integer>` and exit 0. Invalid syntax,
missing or empty input, embedded NUL, and lines longer than 128 bytes print
`error: invalid integer` and exit 1. A syntactically valid decimal value that
does not fit `int` prints `error: integer out of range` and exits 1. These
contract errors go to stdout in both examples; only a C++ input-storage
allocation failure has a stderr diagnostic. No allocation-failure injection
or performance comparison is part of this lesson.
