#include <iostream>

int main() {
    int value;
    if (!(std::cin >> value)) {
        std::cerr << "Expected one integer.\n";
        return 1;
    }

    if (value < 0) {
        std::cout << "negative\n";
    } else if (value > 0) {
        std::cout << "positive\n";
    } else {
        std::cout << "zero\n";
    }
    return 0;
}
