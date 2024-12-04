#include <stdexcept>
#include <optional>
#include <functional>
#include <iostream>

#include <common.h>

int main() {
    static const std::string xmas = "XMAS";
    static const std::string xmas_reversed = "SAMX";

    const auto input = aoc::read_file("day04/input.txt");
    const auto lines = aoc::split(input, '\n');
    if (lines.empty()) {
        throw std::runtime_error("The input must contain at least one line.");
    }
    // This solution assumes that the length of lines is consistent in the input
    const auto num_lines = lines.size();
    const auto line_length = lines[0].size();

    // Safety check functions
    const auto is_safe_horizontal = [&line_length](std::size_t x) {
        return (line_length - x) >= xmas.size();
    };
    const auto is_safe_vertical = [&num_lines](std::size_t y) {
        return (num_lines - y) >= xmas.size();
    };
    const auto is_safe_diagonal_down_right = [&](std::size_t x, std::size_t y) {
        return is_safe_horizontal(x) && is_safe_vertical(y);
    };
    const auto is_safe_diagonal_down_left = [&](std::size_t x, std::size_t y) {
        return x >= xmas.size() - 1 && is_safe_vertical(y);
    };

    // String providers
    const auto get_horizontal = [&](std::size_t x, std::size_t y) {
        const auto& line = lines[y];
        return is_safe_horizontal(x) ? std::make_optional<std::string>(line.data() + x, xmas.size()) : std::nullopt;
    };
    const auto get_vertical = [&](std::size_t x, std::size_t y) -> std::optional<std::string> {
        if (!is_safe_vertical(y)) {
            return std::nullopt;
        }
        std::string result(xmas.size(), ' ');
        for (std::size_t y_offset = 0; y_offset < xmas.size(); ++y_offset) {
            result[y_offset] = lines[y + y_offset][x];
        }
        return result;
    };
    const auto get_diagonal_down_right = [&](std::size_t x, std::size_t y)  -> std::optional<std::string> {
        if (!is_safe_diagonal_down_right(x, y)) {
            return std::nullopt;
        }
        std::string result(xmas.size(), ' ');
        for (std::size_t offset = 0; offset < xmas.size(); ++offset) {
            result[offset] = lines[y + offset][x + offset];
        }
        return result;
    };
    const auto get_diagonal_down_left = [&](std::size_t x, std::size_t y) -> std::optional<std::string> {
        if (!is_safe_diagonal_down_left(x, y)) {
            return std::nullopt;
        }
        std::string result(xmas.size(), ' ');
        for (std::size_t offset = 0; offset < xmas.size(); ++offset) {
            result[offset] = lines[y + offset][x - offset];
        }
        return result;
    };

    auto is_match = [](const std::optional<std::string>& str) {
        if (!str.has_value()) {
            return false;
        }
        return str.value() == xmas || str.value() == xmas_reversed;
    };
    using MaybeStringProvider = std::function<std::optional<std::string>(std::size_t, std::size_t)>;
    std::vector<MaybeStringProvider> providers {
        get_horizontal,
        get_vertical,
        get_diagonal_down_right,
        get_diagonal_down_left
    };

    std::size_t matches = 0;
    for (std::size_t y = 0; y < num_lines; ++y) {
        for (std::size_t x = 0; x < line_length; ++x) {
            for (const auto& provider : providers) {
                if (is_match(provider(x, y))) {
                    ++matches;
                }
            }
        }
    }
    std::cout << matches << std::endl;

    return 0;
}
