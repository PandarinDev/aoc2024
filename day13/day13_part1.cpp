#include <vector>
#include <stdexcept>
#include <iostream>

#include <common.h>

struct Coord {
    
    std::int64_t x;
    std::int64_t y;

    Coord() = default;
    Coord(std::int64_t x, std::int64_t y) : x(x), y(y) {}

};

struct Entry {
    Coord button_a;
    Coord button_b;
    Coord prize;
};

enum class LineType {
    BUTTON_A,
    BUTTON_B,
    PRIZE,
    EMPTY
};

std::int64_t solve(const Entry& entry) {
    // Both the formula for a and b come from solving the system of linear equations from
    // X = a * x_a + b * x_b
    // Y = a * y_a + b * y_b
    const float X = entry.prize.x;
    const float Y = entry.prize.y;
    const float x_a = entry.button_a.x;
    const float x_b = entry.button_b.x;
    const float y_a = entry.button_a.y;
    const float y_b = entry.button_b.y;

    const auto b = (x_a * Y - X * y_a) / (x_a * y_b - x_b * y_a);
    const auto round_b = std::roundf(b);
    const auto epsilon_b = std::abs(round_b - b);
    
    const auto a = (X - b * x_b) / x_a;
    const auto round_a = std::roundf(a);
    const auto epsilon_a = std::abs(round_a - a);
    static constexpr auto epsilon = 0.0000001f;
    if (epsilon_a > epsilon || epsilon_b > epsilon) {
        return 0;
    }
    return 3 * static_cast<std::int64_t>(round_a) + static_cast<std::int64_t>(round_b);
}

int main() {
    const auto input = aoc::read_file("day13/input.txt");
    const auto lines = aoc::split(input, '\n', true);
    // Parse entries
    std::vector<Entry> entries;
    Entry entry{};
    for (std::size_t i = 0; i < lines.size(); ++i) {
        const auto type = static_cast<LineType>(i % 4);
        if (type == LineType::EMPTY) {
            entries.emplace_back(entry);
            entry = {};
            continue;
        }
        const auto& line = lines[i];
        const auto [first, second] = aoc::cut(aoc::cut(line, ": ").second, ", ");
        if (type == LineType::PRIZE) {
            const auto x = aoc::str_view_to_type<std::int64_t>(aoc::cut(first, "=").second);
            const auto y = aoc::str_view_to_type<std::int64_t>(aoc::cut(second, "=").second);
            entry.prize.x = x;
            entry.prize.y = y;
            continue;
        }
        else {
            const auto x = aoc::str_view_to_type<std::int64_t>(aoc::cut(first, "+").second);
            const auto y = aoc::str_view_to_type<std::int64_t>(aoc::cut(second, "+").second);
            auto& value_to_set = (type == LineType::BUTTON_A) ? entry.button_a : entry.button_b;
            value_to_set.x = x;
            value_to_set.y = y;
        }
    }

    std::size_t tokens = 0;
    for (const auto& entry : entries) {
        tokens += solve(entry);
    }
    std::cout << tokens << std::endl;
    return 0;
}
