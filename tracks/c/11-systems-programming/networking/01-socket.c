#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static int send_all(int socket_fd, const char *message, size_t length) {
    size_t sent = 0;
    while (sent < length) {
        const ssize_t result =
            send(socket_fd, message + sent, length - sent, 0);
        if (result < 0 && errno == EINTR) {
            continue;
        }
        if (result <= 0) {
            return -1;
        }
        sent += (size_t)result;
    }
    return 0;
}

int main(int argc, char **argv) {
    const char *host = argc > 1 ? argv[1] : "127.0.0.1";
    const char *port = argc > 2 ? argv[2] : "8080";
    if (argc > 3) {
        fprintf(stderr, "Usage: %s [host [port]]\n", argv[0]);
        return 2;
    }

    struct addrinfo hints = {0};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *addresses = NULL;
    const int lookup = getaddrinfo(host, port, &hints, &addresses);
    if (lookup != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(lookup));
        return 1;
    }

    int socket_fd = -1;
    for (const struct addrinfo *address = addresses; address != NULL;
         address = address->ai_next) {
        socket_fd = socket(address->ai_family, address->ai_socktype,
                           address->ai_protocol);
        if (socket_fd < 0) {
            continue;
        }
        if (connect(socket_fd, address->ai_addr, address->ai_addrlen) == 0) {
            break;
        }
        close(socket_fd);
        socket_fd = -1;
    }
    freeaddrinfo(addresses);

    if (socket_fd < 0) {
        perror("connect");
        return 1;
    }

    static const char message[] = "hello\n";
    if (send_all(socket_fd, message, sizeof(message) - 1) != 0) {
        perror("send");
        close(socket_fd);
        return 1;
    }
    if (close(socket_fd) != 0) {
        perror("close");
        return 1;
    }

    printf("Sent %zu bytes\n", sizeof(message) - 1);
    return 0;
}
