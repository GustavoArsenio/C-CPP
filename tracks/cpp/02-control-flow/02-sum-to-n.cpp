#include <iostream>

int main() {
    int limit;
    if (!(std::cin >> limit) || limit < 0 || limit > 10000) {
        std::cerr << "Expected an integer from 0 through 10000.\n";
        return 1;
    }

    long long sum = 0;
    for (int value = 1; value <= limit; ++value) {
        sum += value;
    }
    std::cout << "Sum: " << sum << '\n';
    return 0;
}
