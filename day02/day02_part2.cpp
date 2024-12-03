#include <iostream>

#include <common.h>

using Permutations = std::vector<std::vector<std::int64_t>>;

// Create possible permutations of a vector, always leaving one entry off
Permutations create_permutations(const std::vector<std::int64_t>& entries) {
    Permutations permutations;
    for (std::size_t i = 0; i < entries.size(); ++i) {
        std::vector<std::int64_t> entry;
        for (std::size_t j = 0; j < entries.size(); ++j) {
            if (i != j) {
                entry.emplace_back(entries[j]);
            }
        }
        permutations.push_back(entry);
    }
    return permutations;
}

int main() {
    const auto input = aoc::read_file("day02/input.txt");
    std::size_t safe_measurements = 0;
    const auto is_safe = [](const std::vector<std::int64_t>& entries) {
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
        return (only_increasing || only_decreasing) && is_valid;
    };
    for (const auto line : aoc::split(input, '\n')) {
        const auto entries = aoc::map<std::string_view, std::int64_t>(aoc::split(line, ' '), aoc::str_view_to_type<std::int64_t>);
        for (const auto& permutation : create_permutations(entries)) {
            if (is_safe(permutation)) {
                ++safe_measurements;
                break;
            }
        }
    }
    std::cout << safe_measurements << std::endl;
    return 0;
}