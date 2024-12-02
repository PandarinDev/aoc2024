#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>
#include <unordered_map>

#include <common.h>

int main() {
    const auto input = aoc::read_file("day01/input.txt");
    std::vector<std::size_t> left_numbers;
    std::unordered_map<std::size_t, std::size_t> numbers_to_occurrences;
    for (const auto& line : aoc::split(input, '\n')) {
        const auto entries = aoc::split(line, ' ');
        if (entries.size() != 2) {
            throw std::runtime_error("2 entries are expected on every line");
        }
        left_numbers.emplace_back(std::atoi(std::string(entries[0]).c_str()));
        const auto right_number = std::atoi(std::string(entries[1]).c_str());
        auto it = numbers_to_occurrences.find(right_number);
        if (it == numbers_to_occurrences.cend()) {
            it = numbers_to_occurrences.emplace(right_number, 0).first;
        }
        it->second++;
    }
    std::sort(left_numbers.begin(), left_numbers.end());
    std::size_t result = 0;
    for (std::size_t i = 0; i < left_numbers.size(); ++i) {
        const auto left = left_numbers.at(i);
        const auto it = numbers_to_occurrences.find(left);
        const auto occurences = it != numbers_to_occurrences.cend() ? it->second : 0;
        result += left * occurences;
    }
    std::cout << result << std::endl;
    return 0;
}