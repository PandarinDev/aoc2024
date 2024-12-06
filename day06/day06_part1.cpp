#include <utility>
#include <unordered_set>
#include <stdexcept>
#include <iostream>

#include <common.h>

struct Position {

    std::int64_t x;
    std::int64_t y;

    Position operator+(const Position& other) const {
        return Position{ x + other.x, y + other.y };
    }

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

};

namespace std {

    template<>
    struct hash<Position> {
        std::size_t operator()(const Position& position) const {
            std::hash<std::int64_t> hasher;
            return aoc::hash_combine(hasher(position.x), position.y);
        }
    };

}

enum class Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

Position direction_to_offset(Direction direction) {
    switch (direction) {
        case Direction::UP: return { 0, -1 };
        case Direction::RIGHT: return { 1, 0 };
        case Direction::DOWN: return { 0, 1 };
        case Direction::LEFT: return { -1, 0 };
        default: throw std::runtime_error("Unknown direction.");
    }
}

Direction rotate_direction(Direction direction) {
    switch (direction) {
        case Direction::UP: return Direction::RIGHT;
        case Direction::RIGHT: return Direction::DOWN;
        case Direction::DOWN: return Direction::LEFT;
        case Direction::LEFT: return Direction::UP;
        default: throw std::runtime_error("Unknown direction.");
    }
}

int main() {
    std::unordered_set<Position> obstructions;
    const auto input = aoc::read_file("day06/input.txt");
    const auto lines = aoc::split(input, '\n');
    Position guard_position { 0, 0 };
    Direction guard_direction = Direction::UP;
    if (lines.empty()) {
        throw std::runtime_error("Empty input.");
    }
    // Parse the map
    for (std::size_t y = 0; y < lines.size(); ++y) {
        const auto& line = lines[y];
        for (std::size_t x = 0; x < line.size(); ++x) {
            const auto c = line[x];
            if (c == '^') {
                guard_position = { static_cast<std::int64_t>(x), static_cast<std::int64_t>(y) };
            }
            else if (c == '#') {
                obstructions.emplace(Position{ static_cast<std::int64_t>(x), static_cast<std::int64_t>(y) });
            }
        }
    }

    // Figure out unique locations visited by the guard
    std::unordered_set<Position> guard_positions;
    const auto map_width = static_cast<std::int64_t>(lines[0].size());
    const auto map_height = static_cast<std::int64_t>(lines.size());
    auto is_guard_out_of_bounds = [&]() {
        return
            guard_position.x < 0 ||
            guard_position.x >= map_width ||
            guard_position.y < 0 ||
            guard_position.y >= map_height;
    };
    std::function<void()> step_guard = [&]() {
        const auto maybe_new_position = guard_position + direction_to_offset(guard_direction);
        // If the potential new position is obstructed rotate the guard and try again
        if (obstructions.find(maybe_new_position) != obstructions.cend()) {
            guard_direction = rotate_direction(guard_direction);
            return step_guard();
        }
        // Otherwise step the guard to the new position
        guard_position = maybe_new_position;
    };
    while (!is_guard_out_of_bounds()) {
        guard_positions.emplace(guard_position);
        // Step the guard
        step_guard();
    }
    std::cout << guard_positions.size() << std::endl;

    return 0;
}
