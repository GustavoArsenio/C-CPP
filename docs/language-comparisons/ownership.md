# Dynamic Array Ownership in C and C++

The paired examples
[`C malloc/free`](../../tracks/c/05-pointers-memory/memory/01-malloc.c) and
[`C++ Rule of Zero`](../../tracks/cpp/09-modern-cpp/04-vector-ownership.cpp)
initialize the same five integers and print `Sum = 15`.

| Concern | C17 | C++20 |
| --- | --- | --- |
| Owning storage | `malloc(count * sizeof *values)` returns a pointer the caller owns | A `std::vector<int>` member owns its allocated elements |
| Allocation failure | Check for `NULL` before using the pointer | `std::vector` reports allocation failure with `std::bad_alloc`; the example catches it |
| Cleanup | The owner calls `free` exactly once after the last use | The vector destructor releases its storage automatically |
| Copy and move | The raw pointer has no automatic ownership-aware copy or move behavior | Compiler-generated special members use the vector member's copy and move behavior |

The C example makes the allocation and release points explicit. In C++ the
aggregate does not define a destructor or copy/move operations; its vector
member manages the resource. The C++ example changes a copy and moves it into
a new value, checking that the original remains unchanged and the moved value
retains its contents. These fixed, small values illustrate ownership and make
no performance claim. The examples' smoke tests verify the common result and
the copy/move behavior. Allocation failure is handled in both implementations
but is not injected by the portable test suite. No benchmark is part of this
lesson.
