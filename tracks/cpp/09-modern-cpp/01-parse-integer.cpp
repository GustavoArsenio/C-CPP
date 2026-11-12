#include <iostream>
#include <new>
#include <stdexcept>
#include <string>

int main() {
    try {
        std::string input;
        bool overlong = false;
        while (true) {
            const int character = std::cin.get();
            if (character == std::char_traits<char>::eof() ||
                character == '\n') {
                break;
            }
            if (input.size() < 128) {
                input.push_back(static_cast<char>(character));
            } else {
                overlong = true;
            }
        }
        if (std::cin.bad()) {
            std::cerr << "error: unable to read input\n";
            return 1;
        }
        if (input.empty() || overlong) {
            std::cout << "error: invalid integer\n";
            return 1;
        }

        std::size_t first_digit =
            (!input.empty() && (input[0] == '+' || input[0] == '-')) ? 1 : 0;
        if (first_digit == input.size()) {
            throw std::invalid_argument("missing digits");
        }
        for (std::size_t index = first_digit; index < input.size(); ++index) {
            if (input[index] < '0' || input[index] > '9') {
                throw std::invalid_argument("not a decimal integer");
            }
        }

        std::size_t parsed_characters = 0;
        const int value = std::stoi(input, &parsed_characters, 10);
        if (parsed_characters != input.size()) {
            throw std::invalid_argument("trailing characters");
        }
        std::cout << "value=" << value << '\n';
    } catch (const std::invalid_argument &) {
        std::cout << "error: invalid integer\n";
        return 1;
    } catch (const std::out_of_range &) {
        std::cout << "error: integer out of range\n";
        return 1;
    } catch (const std::bad_alloc &) {
        std::cerr << "error: unable to read input\n";
        return 1;
    }
}
