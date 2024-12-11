#include <iostream>
#include <vector>
#include <functional>

#include <common.h>

int main() {
    const auto input = aoc::read_file("day10/input.txt");
    const auto lines = aoc::split(input, '\n');
    if (lines.empty()) {
        throw std::runtime_error("Empty input.");
    }

    // Build a map from the input
    std::vector<std::vector<std::int64_t>> map;
    for (std::size_t y = 0; y < lines.size(); ++y) {
        std::vector<std::int64_t> row;
        for (std::size_t x = 0; x < lines[y].size(); ++x) {
            char c = lines[y][x];
            const auto value = static_cast<std::int64_t>(std::atoi(std::string(1, c).c_str()));
            row.emplace_back(value);
        }
        map.emplace_back(std::move(row));
    }

    const auto map_width = static_cast<std::int64_t>(lines[0].size());
    const auto map_height = static_cast<std::int64_t>(lines.size());
    std::function<std::int64_t(std::int64_t, std::int64_t, std::int64_t)> traverse = [&](
        std::int64_t x,
        std::int64_t y,
        std::int64_t expected_value) -> std::int64_t {
        if (x < 0 || x >= map_width || y < 0 || y >= map_height) {
            return 0;
        }
        const auto value = map[y][x];
        if (value != expected_value) {
            return 0;
        }
        // Check if we reached the end of a trail
        if (expected_value == 9) {
            return 1;
        }
        return
            traverse(x - 1, y, expected_value + 1) +
            traverse(x + 1, y, expected_value + 1) +
            traverse(x, y + 1, expected_value + 1) +
            traverse(x, y - 1, expected_value + 1);
    };

    // Traverse every cell in the map
    std::size_t result = 0;
    for (std::int64_t y = 0; y < map_height; ++y) {
        for (std::int64_t x = 0; x < map_width; ++x) {
            result += traverse(x, y, 0);
        }
    }
    std::cout << result << std::endl;
    return 0;
}