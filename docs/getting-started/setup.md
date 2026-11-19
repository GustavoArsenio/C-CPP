# Getting Started

## Requirements

- A C17 compiler and a C++20 compiler.
- CMake 3.20 or newer and Make.
- Python 3 to validate exercise metadata and run catalog-driven examples.
- POSIX threads for the bounded queue example on UNIX-like systems.

OpenSSL is optional. POSIX networking and OpenSSL targets are disabled in the
default build. See [optional examples](optional-examples.md).

## Configure and build

```sh
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

You can use the root Makefile as a shorter interface:

```sh
make
make test
make clean
```

To see current exercise counts and validate metadata:

```sh
python3 tools/validation/catalog.py
```

Each catalog entry links to its source and declares a completion status. Check
the status before using an example as a reference solution.
