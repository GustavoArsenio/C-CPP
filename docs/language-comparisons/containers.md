# Growable integer sequences in C and C++

The paired lessons read a count and that many integers, then print the count
and values in insertion order:

- [C opaque vector API](../../tracks/c/04-arrays-strings/arrays/02-dynamic-int-vector.c)
- [C++ `std::vector`](../../tracks/cpp/08-stl/03-dynamic-vector.cpp)

| Concern | C17 | C++20 |
| --- | --- | --- |
| Representation | Private `int_vector` structure in a separate implementation file | Standard `std::vector<int>` value |
| Growth | `realloc` with geometrically doubled capacity and checked size arithmetic | `push_back`; the standard container controls capacity growth |
| Failure | API returns `false`, retaining prior elements and storage | Growth can throw `std::bad_alloc` or `std::length_error`; the lesson reports both |
| Lifetime | Caller owns an opaque handle and calls `int_vector_destroy` | Automatic lifetime; the vector releases storage through RAII |
| Access | Checked `int_vector_get` and `int_vector_pop_back` operations | Range-based traversal and standard container operations |

The C API keeps its representation out of the public header and parses decimal
counts and elements with range checks before conversion. Its unit tests exercise
empty state, invalid handles and indices, multiple capacity growths, insertion
order, removal, and cleanup. Portable tests do not force allocator failure or
synthetic vector-capacity overflow. Both smoke suites cover normal, empty,
incomplete, negative-count, and out-of-range decimal input. These lessons compare
container interfaces and ownership; they contain no performance measurements.
