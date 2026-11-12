#include <iostream>

int main() {
    const int answer = 42;
    const double ratio = 2.5;
    const char grade = 'A';
    const bool ready = true;

    std::cout << "answer=" << answer << ", ratio=" << ratio
              << ", grade=" << grade << ", ready=" << std::boolalpha << ready
              << '\n';
    return 0;
}
