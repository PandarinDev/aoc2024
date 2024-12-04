#include <regex>
#include <iostream>

#include <common.h>

int main() {
    const auto input = aoc::read_file("day03/input.txt");

    // First discover ranges where multiplication is enabled
    std::vector<std::pair<std::size_t, std::size_t>> enabled_ranges;
    {
        bool enabled = true;
        std::regex do_dont_pattern("do\\(\\)|don't\\(\\)");
        auto begin = std::sregex_iterator(input.cbegin(), input.cend(), do_dont_pattern);
        auto end = std::sregex_iterator();
        std::pair<std::size_t, std::size_t> current_range{ 0, 0 };
        for (auto i = begin; i != end; ++i) {
            const auto str = i->str();
            // Start a new range
            if (str == "do()" && !enabled) {
                current_range.first = i->position() + i->length();
                enabled = true;
            }
            else if (str == "don't()" && enabled) {
                current_range.second = i->position() - 1;
                enabled_ranges.emplace_back(current_range);
                current_range = { 0, 0 };
                enabled = false;
            }
        }
        // If we ended with a do() add a final range
        if (enabled) {
            current_range.second = input.size() - 1;
            enabled_ranges.emplace_back(current_range);
        }
    }

    // Helper to check if a subrange is enabled or not
    const auto is_enabled = [&enabled_ranges](std::size_t start, std::size_t end) {
        for (const auto& range : enabled_ranges) {
            if (range.first <= start && range.second >= end) {
                return true;
            }
        }
        return false;
    };

    // Then look for multiplications and check that they are in an enabled range
    std::regex mul_pattern("mul\\((\\d{1,3}),(\\d{1,3})\\)");
    auto begin = std::sregex_iterator(input.cbegin(), input.cend(), mul_pattern);
    auto end = std::sregex_iterator();
    std::size_t result = 0;
    for (auto i = begin; i != end; ++i) {
        const auto pattern_start = i->position();
        const auto pattern_end = i->position() + i->length() - 1;
        if (is_enabled(pattern_start, pattern_end)) {
            const auto first = std::atoi((*i)[1].str().c_str());
            const auto second = std::atoi((*i)[2].str().c_str());
            result += first * second;
        }
    }
    std::cout << result << std::endl;
    return 0;
}