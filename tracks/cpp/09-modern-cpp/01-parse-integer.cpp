#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    std::string input;
    if (!std::getline(std::cin, input)) {
        std::cout << "error: invalid integer\n";
        return 1;
    }

    try {
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
    }
}
