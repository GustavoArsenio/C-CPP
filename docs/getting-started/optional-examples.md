# Optional Examples

The default build avoids non-core platform and library requirements.

## POSIX TCP client

On UNIX-like systems, configure with:

```sh
cmake -S . -B build-posix -DLEARNING_BUILD_POSIX_NETWORKING=ON
cmake --build build-posix --parallel
ctest --test-dir build-posix --output-on-failure
```

The integration test uses a loopback TCP server. Environments that prohibit
local sockets report that check as skipped.

## OpenSSL SHA-256 example

Install OpenSSL development headers/libraries, then configure with:

```sh
cmake -S . -B build-openssl -DLEARNING_BUILD_OPENSSL_EXAMPLES=ON
cmake --build build-openssl --parallel
ctest --test-dir build-openssl --output-on-failure
```

The source uses OpenSSL EVP and is not part of the dependency-free default
build.

## Sanitized optional targets

With GCC or Clang and OpenSSL development files installed, build and test both
optional examples under AddressSanitizer and UndefinedBehaviorSanitizer:

```sh
cmake -S . -B build/sanitized-optional \
  -DLEARNING_ENABLE_SANITIZERS=ON \
  -DLEARNING_BUILD_POSIX_NETWORKING=ON \
  -DLEARNING_BUILD_OPENSSL_EXAMPLES=ON
cmake --build build/sanitized-optional --parallel
ctest --test-dir build/sanitized-optional --output-on-failure
```

Some sandboxed Linux environments prevent LeakSanitizer from inspecting the
process and report that it cannot run under `ptrace`. For a diagnostic rerun,
`ASAN_OPTIONS=detect_leaks=0` disables leak detection while leaving address and
undefined-behavior checks enabled; that run does not validate memory leaks.
