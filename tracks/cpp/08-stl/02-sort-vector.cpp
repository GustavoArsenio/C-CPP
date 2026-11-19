#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> values{5, 3, 8, 1, 4};
    std::sort(values.begin(), values.end());

    std::cout << "Sorted:";
    for (std::size_t index = 0; index < values.size(); ++index) {
        std::cout << ' ' << values[index];
    }
    std::cout << '\n';
}
