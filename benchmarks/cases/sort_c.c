#define _POSIX_C_SOURCE 200809L

#include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int compare_ints(const void *left, const void *right) {
    int a = *(const int *)left;
    int b = *(const int *)right;
    return (a > b) - (a < b);
}

static uint64_t checksum(const int *values, size_t count) {
    uint64_t hash = UINT64_C(14695981039346656037);
    for (size_t index = 0; index < count; ++index) {
        hash ^= (uint32_t)values[index];
        hash *= UINT64_C(1099511628211);
    }
    return hash;
}

static uint64_t now_ns(void) {
    struct timespec time;
    if (clock_gettime(CLOCK_MONOTONIC, &time) != 0) {
        return 0;
    }
    return (uint64_t)time.tv_sec * UINT64_C(1000000000) +
           (uint64_t)time.tv_nsec;
}

static int parse_distribution(const char *name) {
    if (strcmp(name, "sorted") == 0) return 1;
    if (strcmp(name, "reverse") == 0) return 2;
    if (strcmp(name, "duplicates") == 0) return 3;
    if (strcmp(name, "random") == 0) return 0;
    return -1;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        return 2;
    }
    size_t count = (size_t)strtoull(argv[1], NULL, 10);
    uint64_t seed = strtoull(argv[2], NULL, 10);
    int distribution = parse_distribution(argv[3]);
    if (count == 0 || count > SIZE_MAX / sizeof(int) ||
        count > (size_t)INT_MAX || distribution < 0) {
        return 2;
    }
    int *values = malloc(count * sizeof(*values));
    if (values == NULL) {
        free(values);
        return 2;
    }
    for (size_t index = 0; index < count; ++index) {
        seed = seed * UINT64_C(6364136223846793005) + 1;
        int value =
            distribution == 3
                ? (int)(seed % 16)
                : (int)((seed >> 32) % (UINT64_C(2) * INT_MAX + 1)) - INT_MAX;
        if (distribution == 1) value = (int)index;
        if (distribution == 2) value = (int)(count - index);
        values[index] = value;
    }
    uint64_t start = now_ns();
    qsort(values, count, sizeof(*values), compare_ints);
    uint64_t elapsed = now_ns() - start;
    int sorted = 1;
    for (size_t index = 1; index < count; ++index) {
        if (values[index - 1] > values[index]) sorted = 0;
    }
    printf("%" PRIu64 " %" PRIu64 " %d\n", elapsed, checksum(values, count),
           sorted);
    free(values);
}
