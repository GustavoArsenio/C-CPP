# File Line Counting in C and C++

The paired lessons
[`C line counter`](../../tracks/c/07-file-io/line-count/01-count-lines.c)
and
[`C++ line counter`](../../tracks/cpp/09-modern-cpp/03-count-file-lines.cpp)
use the same input and counting contract. Both read a path from standard
input, open the file in binary mode, count each LF byte, and count one final
line when a nonempty file does not end in LF. An empty file has zero lines;
a file containing only LF has one.

| Concern | C | C++20 |
| --- | --- | --- |
| File handle | `FILE *` from `fopen` | `std::ifstream` value |
| Cleanup | Check `fclose` and close explicitly on error paths | The stream closes through RAII; explicit `close` exposes a close failure |
| Read state | Compare `fgetc` with `EOF`, then check `ferror` | Read with `get`, distinguish EOF from `badbit`, then check `close` state |
| Counter | `uintmax_t` with checked increments | `std::uintmax_t` with checked increments |

Binary mode keeps the byte-wise LF rule consistent across platforms. The final
unterminated-line rule is separate from counting LF bytes: after reading, add
one only when at least one byte was seen and the last byte was not LF. The C++
lesson caps its path `std::string` at 1023 raw bytes before validating the
normalized path length, then relies on `std::ifstream` destruction for cleanup
while explicitly closing on success so a close error can be reported.

The shared integration harness checks empty and readable files, path-format
boundaries, and a missing file. It does not inject operating-system read or
close failures; those error branches are checked in the implementations but
are not claimed as deterministically exercised by the harness.
