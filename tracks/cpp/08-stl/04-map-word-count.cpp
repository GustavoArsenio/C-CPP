#include <cstddef>
#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, std::size_t> word_counts;
    std::string word;

    while (std::cin >> word) {
        ++word_counts[word];
    }

    for (const auto& [word_value, count] : word_counts) {
        std::cout << word_value << ' ' << count << '\n';
    }
}
