#include <utility>
#include <unordered_set>
#include <stdexcept>
#include <iostream>
#include <type_traits>

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

enum class Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

struct Collision {

    Position position;
    Direction direction;

    bool operator==(const Collision& other) const {
        return position == other.position && direction == other.direction;
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

    template<>
    struct hash<Collision> {
        std::size_t operator()(const Collision& collision) const {
            std::hash<Position> hasher;
            return aoc::hash_combine(hasher(collision.position), static_cast<std::underlying_type_t<Direction>>(collision.direction));
        }
    };

}

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

    // The smart way to do this is to try to find quadrants of obstructions that are missing one corner
    // The unga-bunga brute force way though is to try to place an obstruction in every single cell
    // and then detect if the guard got into an infinite loop or not. :)
    const auto map_width = static_cast<std::int64_t>(lines[0].size());
    const auto map_height = static_cast<std::int64_t>(lines.size());
    auto is_guard_out_of_bounds = [&]() {
        return
            guard_position.x < 0 ||
            guard_position.x >= map_width ||
            guard_position.y < 0 ||
            guard_position.y >= map_height;
    };

    std::size_t result = 0;
    Position initial_guard_position = guard_position;
    Direction initial_guard_direction = guard_direction;
    for (std::int64_t y = 0; y < map_height; ++y) {
        for (std::int64_t x = 0; x < map_width; ++x) {
            const Position position{ x, y };
            // Skip cells that already contain obstructions
            {
                const auto it = obstructions.find(position);
                if (it != obstructions.cend()) {
                    continue;
                }
            }
            
            // Reset the guard position, rotation and add the new obstruction
            guard_position = initial_guard_position;
            guard_direction = initial_guard_direction;
            auto altered_obstructions = obstructions;
            altered_obstructions.emplace(position);

            bool entered_infinite_loop = false;
            std::unordered_set<Collision> guard_collisions;
            std::function<void()> step_guard = [&]() {
                const auto maybe_new_position = guard_position + direction_to_offset(guard_direction);
                // If the potential new position is obstructed rotate the guard and try again
                if (altered_obstructions.find(maybe_new_position) != altered_obstructions.cend()) {
                    const Collision collision { guard_position, guard_direction };
                    if (guard_collisions.find(collision) != guard_collisions.cend()) {
                        entered_infinite_loop = true;
                        return;
                    }
                    guard_collisions.emplace(collision);
                    guard_direction = rotate_direction(guard_direction);
                    return step_guard();
                }
                // Otherwise step the guard to the new position
                guard_position = maybe_new_position;
            };
            while (!is_guard_out_of_bounds() && !entered_infinite_loop) {
                step_guard();
            }

            if (entered_infinite_loop) {
                ++result;
            }
        }
    }
    std::cout << result << std::endl;

    return 0;
}
