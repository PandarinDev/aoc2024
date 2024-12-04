#include <regex>
#include <iostream>

#include <common.h>

int main() {
    const auto input = aoc::read_file("day03/input.txt");
    std::regex mul_pattern("mul\\((\\d{1,3}),(\\d{1,3})\\)");
    auto begin = std::sregex_iterator(input.cbegin(), input.cend(), mul_pattern);
    auto end = std::sregex_iterator();
    std::size_t result = 0;
    for (auto i = begin; i != end; ++i) {
        const auto first = std::atoi((*i)[1].str().c_str());
        const auto second = std::atoi((*i)[2].str().c_str());
        result += first * second;
    }
    std::cout << result << std::endl;
    return 0;
}