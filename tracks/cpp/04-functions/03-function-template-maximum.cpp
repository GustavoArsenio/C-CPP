#include <iostream>
#include <string>
#include <type_traits>

// Both types must support operator< and have a common_type. The selected
// value is converted to that common type before it is returned.
template <typename Left, typename Right>
constexpr std::common_type_t<Left, Right> maximum(const Left &left,
                                                  const Right &right) {
    using result_type = std::common_type_t<Left, Right>;
    return left < right ? result_type(right) : result_type(left);
}

int main() {
    std::cout << maximum(7, 3) << '\n';
    std::cout << maximum(2, 5) << '\n';
    std::cout << maximum(4, 4) << '\n';
    std::cout << maximum(3, 2.5) << '\n';
    std::cout << maximum(std::string("apple"), std::string("pear")) << '\n';
}
