#include <iostream>

int greatest_common_divisor(int left, int right) {
    while (right != 0) {
        const int remainder = left % right;
        left = right;
        right = remainder;
    }
    return left;
}

int main() {
    int left;
    int right;
    if (!(std::cin >> left >> right) || left < 0 || right < 0) {
        std::cerr << "Expected two non-negative integers.\n";
        return 1;
    }

    std::cout << "GCD: " << greatest_common_divisor(left, right) << '\n';
    return 0;
}
