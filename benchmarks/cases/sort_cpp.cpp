#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

static std::uint64_t checksum(const std::vector<int> &values) {
    std::uint64_t hash = UINT64_C(14695981039346656037);
    for (int value : values) {
        hash ^= static_cast<std::uint32_t>(value);
        hash *= UINT64_C(1099511628211);
    }
    return hash;
}

int main(int argc, char **argv) {
    if (argc != 5) {
        return 2;
    }
    const auto count =
        static_cast<std::size_t>(std::strtoull(argv[1], nullptr, 10));
    std::uint64_t seed = std::strtoull(argv[2], nullptr, 10);
    const std::string distribution(argv[3]);
    if (count == 0 || count > std::vector<int>().max_size() ||
        count > static_cast<std::size_t>(INT_MAX)) {
        return 2;
    }
    std::vector<int> values(count);
    for (int &value : values) {
        seed = seed * UINT64_C(6364136223846793005) + 1;
        value =
            distribution == "duplicates"
                ? static_cast<int>(seed % 16)
                : static_cast<int>((seed >> 32) % (UINT64_C(2) * INT_MAX + 1)) -
                      INT_MAX;
    }
    const std::string algorithm(argv[4]);
    if (distribution == "sorted") {
        for (std::size_t index = 0; index < count; ++index) {
            values[index] = static_cast<int>(index);
        }
    } else if (distribution == "reverse") {
        for (std::size_t index = 0; index < count; ++index) {
            values[index] = static_cast<int>(count - index);
        }
    } else if (distribution != "random" && distribution != "duplicates") {
        return 2;
    }
    const auto start = std::chrono::steady_clock::now();
    if (algorithm == "std_sort") {
        std::sort(values.begin(), values.end());
    } else if (algorithm == "stable_sort") {
        std::stable_sort(values.begin(), values.end());
    } else {
        return 2;
    }
    const auto end = std::chrono::steady_clock::now();
    const auto elapsed =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
            .count();
    const bool sorted = std::is_sorted(values.begin(), values.end());
    std::cout << elapsed << ' ' << checksum(values) << ' ' << sorted << '\n';
}
