#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>

#include <common.h>

int main() {
    const auto input = aoc::read_file("day01/input.txt");
    std::vector<std::size_t> left_numbers;
    std::vector<std::size_t> right_numbers;
    for (const auto& line : aoc::split(input, '\n')) {
        const auto entries = aoc::split(line, ' ');
        if (entries.size() != 2) {
            throw std::runtime_error("2 entries are expected on every line");
        }
        left_numbers.emplace_back(std::atoi(std::string(entries[0]).c_str()));
        right_numbers.emplace_back(std::atoi(std::string(entries[1]).c_str()));
    }
    std::sort(left_numbers.begin(), left_numbers.end());
    std::sort(right_numbers.begin(), right_numbers.end());
    std::size_t result = 0;
    for (std::size_t i = 0; i < left_numbers.size(); ++i) {
        const auto left = left_numbers.at(i);
        const auto right = right_numbers.at(i);
        result += std::llabs(left - right);
    }
    std::cout << result << std::endl;
    return 0;
}