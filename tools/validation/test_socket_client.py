#!/usr/bin/env python3
"""Exercise the POSIX TCP client against an ephemeral loopback server."""

import argparse
import socket
import subprocess
import threading


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--executable", required=True)
    args = parser.parse_args()

    try:
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    except PermissionError:
        print("SKIP loopback sockets are unavailable in this environment")
        return 77
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server.bind(("127.0.0.1", 0))
    server.listen(1)
    server.settimeout(5)
    port = server.getsockname()[1]
    received: list[bytes] = []
    server_errors: list[BaseException] = []

    def receive_message() -> None:
        try:
            connection, _ = server.accept()
            with connection:
                connection.settimeout(5)
                chunks = []
                while True:
                    chunk = connection.recv(128)
                    if not chunk:
                        break
                    chunks.append(chunk)
                received.append(b"".join(chunks))
        except BaseException as error:  # Propagate thread failures to main.
            server_errors.append(error)

    thread = threading.Thread(target=receive_message, daemon=True)
    thread.start()
    result = subprocess.run(
        [args.executable, "127.0.0.1", str(port)],
        text=True,
        capture_output=True,
        timeout=8,
        check=False,
    )
    thread.join(timeout=6)
    server.close()

    if result.returncode != 0:
        raise SystemExit(f"client failed ({result.returncode}): {result.stderr}")
    if thread.is_alive():
        raise SystemExit("server thread did not finish")
    if server_errors:
        raise SystemExit(f"server failed: {server_errors[0]}")
    if received != [b"hello\n"]:
        raise SystemExit(f"unexpected TCP payload: {received!r}")
    if result.stdout != "Sent 6 bytes\n":
        raise SystemExit(f"unexpected client output: {result.stdout!r}")
    print("PASS POSIX TCP client loopback integration")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
