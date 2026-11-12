#include <iostream>
#include <new>
#include <numeric>
#include <utility>
#include <vector>

struct integer_sequence {
    std::vector<int> values;
};

int main() {
    try {
        const integer_sequence sequence{{1, 2, 3, 4, 5}};
        integer_sequence copy = sequence;
        copy.values[0] = 10;
        integer_sequence moved = std::move(copy);
        if (sequence.values[0] != 1 || moved.values[0] != 10) {
            std::cerr << "error: vector copy or move lost its value\n";
            return 1;
        }

        const int sum =
            std::accumulate(sequence.values.begin(), sequence.values.end(), 0);
        std::cout << "Sum = " << sum << '\n';
    } catch (const std::bad_alloc &) {
        std::cerr << "error: unable to allocate integer sequence\n";
        return 1;
    }
    return 0;
}
