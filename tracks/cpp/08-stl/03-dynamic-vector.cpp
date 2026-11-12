#include <iostream>
#include <limits>
#include <new>
#include <stdexcept>
#include <vector>

int main() {
    long long requested_count = 0;
    if (!(std::cin >> requested_count) || requested_count < 0 ||
        static_cast<unsigned long long>(requested_count) >
            std::numeric_limits<std::size_t>::max()) {
        std::cerr << "error: expected an element count\n";
        return 1;
    }
    const std::size_t count = static_cast<std::size_t>(requested_count);

    try {
        std::vector<int> values;
        for (std::size_t index = 0; index < count; ++index) {
            int value = 0;
            if (!(std::cin >> value)) {
                std::cerr << "error: expected an integer value\n";
                return 1;
            }
            values.push_back(value);
        }

        std::cout << "Count: " << values.size() << ", values:";
        for (const int value : values) {
            std::cout << ' ' << value;
        }
        std::cout << '\n';
    } catch (const std::bad_alloc &) {
        std::cerr << "error: unable to allocate integer vector\n";
        return 1;
    } catch (const std::length_error &) {
        std::cerr << "error: integer vector exceeds maximum size\n";
        return 1;
    }
    return 0;
}
