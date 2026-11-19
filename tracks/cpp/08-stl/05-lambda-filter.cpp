#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <new>
#include <vector>

int main() {
    constexpr int max_value_count = 10000;

    int value_count = 0;
    if (!(std::cin >> value_count) || value_count < 0 ||
        value_count > max_value_count) {
        std::cerr << "error: invalid input\n";
        return 1;
    }

    try {
        std::vector<int> values;
        values.reserve(static_cast<std::size_t>(value_count));
        for (int index = 0; index < value_count; ++index) {
            int value = 0;
            if (!(std::cin >> value)) {
                std::cerr << "error: invalid input\n";
                return 1;
            }
            values.push_back(value);
        }

        std::vector<int> even_values;
        std::copy_if(values.cbegin(), values.cend(),
                     std::back_inserter(even_values),
                     [](int value) { return value % 2 == 0; });

        std::cout << "Even:";
        for (const int value : even_values) {
            std::cout << ' ' << value;
        }
        std::cout << '\n';
    } catch (const std::bad_alloc&) {
        std::cerr << "error: unable to allocate memory\n";
        return 1;
    }
}
