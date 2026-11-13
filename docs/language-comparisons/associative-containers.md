# Associative containers: C open addressing and C++ `std::map`

The C lesson at `tracks/c/09-data-structures/hash-based/01-word-count.c` counts
exact whitespace-delimited tokens with the reusable
`data-structures/hash-based/string_counter` module. Its opaque table owns a copy
of every key, compares case-sensitively, and uses deterministic hashing with
open addressing and linear probing. Operations take expected O(1) time and
worst-case O(u) per operation for u distinct keys. Storage is O(u).

The C++ lesson at `tracks/cpp/08-stl/04-map-word-count.cpp` uses
`std::map<std::string, std::size_t>`. Lookup and insertion take O(log u), and
iteration naturally yields lexicographic order. The hash table has no ordering
guarantee; the C frontend visits its keys, sorts references in O(u log u), and
then prints them to match the C++ lesson.

Both programs preserve token case and punctuation in text input, split on
whitespace, and read through EOF, including a final token without a newline.
The table visitor returns borrowed read-only key pointers; callers must keep
the table alive and must not mutate it during a visit. The C API stores
NUL-terminated strings, so the word-count frontend rejects input containing
embedded NUL bytes; ordinary text tokens are otherwise unbounded by a fixed
buffer size.
