#include <iostream>

#include <common.h>

int main() {
    const auto input = aoc::read_file("day02/input.txt");
    std::size_t safe_measurements = 0;
    for (const auto line : aoc::split(input, '\n')) {
        const auto entries = aoc::map<std::string_view, std::int64_t>(aoc::split(line, ' '), aoc::str_view_to_type<std::int64_t>);
        bool only_increasing = true;
        bool only_decreasing = true;
        bool is_valid = true;
        for (std::size_t i = 1; i < entries.size(); ++i) {
            const auto previous = entries[i - 1];
            const auto current = entries[i];
            if (current < previous) {
                only_increasing = false;
            }
            if (current > previous) {
                only_decreasing = false;
            }
            const auto difference = std::abs(current - previous);
            if (difference < 1 || difference > 3) {
                is_valid = false;
            }
        }
        if ((only_increasing || only_decreasing) && is_valid) {
            ++safe_measurements;
        }
    }
    std::cout << safe_measurements << std::endl;
    return 0;
}