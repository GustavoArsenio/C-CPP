# C++ Learning Path

The current examples target C++20 and use standard streams, functions, and
standard containers. Follow the progression below; unimplemented areas remain
explicit curriculum gaps.

| Stage | Material currently available | Main gaps |
| --- | --- | --- |
| Foundations | Hello world and basic value types | Input validation, constants, types and initialization in depth |
| Control flow | Sign classification and bounded summation | Loops, switch, nested control flow, error cases |
| Functions | Euclidean greatest common divisor, overloads, const string reference, a maximum function template | Recursion, callable objects |
| Types and objects | Point and Vector2D classes, constructor, const method and operator overload | Invariants, destructors, copy/move behavior |
| Memory and RAII | A move-only owner wraps `FILE*` with `std::unique_ptr` and a custom deleter; a temporary file is closed automatically on every exit path | Ownership composition, smart pointers, move semantics, resource cleanup |
| Templates and STL | `std::vector`, range traversal, `std::sort` with iterators | Associative containers |
| Modern C++ | C++20 is the default build standard; integer parsing demonstrates standard exceptions and complete-input validation; temporary-file processing demonstrates RAII for a C resource | Concepts, ranges, chrono, filesystem, modules where supported |
| Patterns, concurrency, advanced | No dedicated C++ lessons yet | Idiomatic examples and paired C/C++ comparisons |

See the [exercise catalog](../../exercises/catalog.json) for current source
paths and verification status.
