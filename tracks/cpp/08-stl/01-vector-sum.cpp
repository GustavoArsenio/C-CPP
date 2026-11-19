#include <iostream>
#include <vector>

int main() {
    const std::vector<int> values{2, 4, 6, 8};
    int sum = 0;
    for (const int value : values) {
        sum += value;
    }

    std::cout << "Count: " << values.size() << ", sum: " << sum << '\n';
    return 0;
}
