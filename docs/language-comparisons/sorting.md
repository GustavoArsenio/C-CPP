# Sorting with `qsort` and `std::sort`

The paired examples
[`C qsort`](../../algorithms/sorting/01-qs.c) and
[`C++ std::sort`](../../tracks/cpp/08-stl/02-sort-vector.cpp) sort the same
five integers and print the same result:

```text
Sorted: 1 3 4 5 8
```

| Concern | C17 `qsort` | C++20 `std::sort` |
| --- | --- | --- |
| Range description | Array pointer, element count, and element size | Half-open iterator range `[begin, end)` |
| Comparison | Callback receives `const void *` element pointers; the example casts to `int` and returns negative, zero, or positive | Template operates on typed iterators and uses the element ordering by default |
| Type information | Element type is not expressed in the callback signature | Iterator and element types are available to the template |
| Mutation | Reorders the supplied array in place | Reorders the supplied range in place |
| Comparison complexity | C17 specifies no complexity bound for `qsort`; this depends on the library implementation | `std::sort` requires O(n log n) comparisons |

The C comparator uses relational comparisons rather than subtracting the two
integers, so its result cannot overflow. The C standard specifies the effect of
`qsort`, not a particular internal sorting algorithm. These examples teach the
two APIs and do not establish a performance difference between the languages.

The separate [sorting benchmark](../../benchmarks/README.md) compares library
implementations under a documented local methodology. Its timings are
environment-specific observations and do not change the behavior or guarantees
demonstrated by these small examples.
