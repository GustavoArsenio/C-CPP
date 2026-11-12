#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> values{7, 2, 9, 1, 5};
    std::sort(values.begin(), values.end());

    for (std::size_t index = 0; index < values.size(); ++index) {
        std::cout << (index == 0 ? "" : " ") << values[index];
    }
    std::cout << '\n';
}
