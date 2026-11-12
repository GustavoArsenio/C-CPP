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
