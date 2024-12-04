#include <stdexcept>
#include <optional>
#include <functional>
#include <iostream>

#include <common.h>

int main() {
    static const std::string mas = "MAS";
    static const std::string mas_reversed = "SAM";

    const auto input = aoc::read_file("day04/input.txt");
    const auto lines = aoc::split(input, '\n');
    if (lines.empty()) {
        throw std::runtime_error("The input must contain at least one line.");
    }
    // This solution assumes that the length of lines is consistent in the input
    const auto num_lines = lines.size();
    const auto line_length = lines[0].size();

    const auto is_safe = [&](std::size_t x, std::size_t y) {
        return (line_length - x) >= mas.size() && (num_lines - y) >= mas.size();
    };
    const auto get_block_diagonal_down_right = [&](std::size_t x, std::size_t y)  -> std::optional<std::string> {
        if (!is_safe(x, y)) {
            return std::nullopt;
        }
        std::string result(mas.size(), ' ');
        for (std::size_t offset = 0; offset < mas.size(); ++offset) {
            result[offset] = lines[y + offset][x + offset];
        }
        return result;
    };
    const auto get_block_diagonal_down_left = [&](std::size_t x, std::size_t y) -> std::optional<std::string> {
        if (!is_safe(x, y)) {
            return std::nullopt;
        }
        std::string result(mas.size(), ' ');
        for (std::size_t offset = 0; offset < mas.size(); ++offset) {
            result[offset] = lines[y + offset][x + mas.size() - 1 - offset];
        }
        return result;
    };

    auto is_match = [](const std::optional<std::string>& str) {
        if (!str.has_value()) {
            return false;
        }
        return str.value() == mas || str.value() == mas_reversed;
    };

    std::size_t matches = 0;
    for (std::size_t y = 0; y < num_lines; ++y) {
        for (std::size_t x = 0; x < line_length; ++x) {
            const auto first_diagonal = get_block_diagonal_down_right(x, y);
            const auto second_diagonal = get_block_diagonal_down_left(x, y);
            if (is_match(first_diagonal) && is_match(second_diagonal)) {
                ++matches;
            }
        }
    }
    std::cout << matches << std::endl;

    return 0;
}
