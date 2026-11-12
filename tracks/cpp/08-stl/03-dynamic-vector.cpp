#include <charconv>
#include <iostream>
#include <new>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
    try {
        std::string count_token;
        if (!(std::cin >> count_token)) {
            std::cerr << "error: expected an element count\n";
            return 1;
        }
        std::size_t count = 0;
        const auto [end, error] = std::from_chars(
            count_token.data(), count_token.data() + count_token.size(), count);
        if (error != std::errc{} ||
            end != count_token.data() + count_token.size()) {
            std::cerr << "error: expected a nonnegative element count\n";
            return 1;
        }

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
