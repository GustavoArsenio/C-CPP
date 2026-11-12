#include <iostream>
#include <string>

int describe(int value) {
    return value * 2;
}

std::string describe(const std::string &value) {
    return "Hello, " + value;
}

int main() {
    std::cout << describe(21) << '\n' << describe(std::string("C++")) << '\n';
}
