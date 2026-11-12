#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <locale>
#include <new>
#include <string>

int main() {
    std::string path;
    try {
        path.reserve(1023);
    } catch (const std::bad_alloc &) {
        std::cerr << "error: could not allocate file path storage\n";
        return 1;
    }

    bool saw_input = false;
    bool ended_with_lf = false;
    char character = '\0';
    while (std::cin.get(character)) {
        if (character == '\n') {
            ended_with_lf = true;
            break;
        }
        saw_input = true;
        if (character == '\0') {
            std::cerr << "error: file path contains a NUL byte\n";
            return 1;
        }
        if (path.size() == 1023) {
            std::cerr << "error: file path is too long\n";
            return 1;
        }
        try {
            path.push_back(character);
        } catch (const std::bad_alloc &) {
            std::cerr << "error: could not allocate file path storage\n";
            return 1;
        }
    }

    if (std::cin.bad() || (std::cin.fail() && !std::cin.eof())) {
        std::cerr << "error: failed while reading file path\n";
        return 1;
    }
    if (!saw_input && !ended_with_lf) {
        std::cerr << "error: expected a file path on standard input\n";
        return 1;
    }
    if (ended_with_lf && !path.empty() && path.back() == '\r') {
        path.pop_back();
    }
    if (path.empty()) {
        std::cerr << "error: file path is empty\n";
        return 1;
    }
    if (path.size() > 1022) {
        std::cerr << "error: file path is too long\n";
        return 1;
    }

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "error: cannot open file\n";
        return 1;
    }

    std::uintmax_t line_count = 0;
    bool has_content = false;
    char last_character = '\n';
    while (file.get(character)) {
        has_content = true;
        last_character = character;
        if (character == '\n') {
            if (line_count == std::numeric_limits<std::uintmax_t>::max()) {
                std::cerr << "error: line count overflow\n";
                return 1;
            }
            ++line_count;
        }
    }

    if (file.bad() || !file.eof()) {
        std::cerr << "error: failed while reading file\n";
        return 1;
    }
    if (has_content && last_character != '\n') {
        if (line_count == std::numeric_limits<std::uintmax_t>::max()) {
            std::cerr << "error: line count overflow\n";
            return 1;
        }
        ++line_count;
    }

    file.clear();
    file.close();
    if (file.fail()) {
        std::cerr << "error: failed to close file\n";
        return 1;
    }

    std::cout.imbue(std::locale::classic());
    std::cout << line_count << '\n';
    if (!std::cout) {
        std::cerr << "error: failed to write line count\n";
        return 1;
    }
    return 0;
}
