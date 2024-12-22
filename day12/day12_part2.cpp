#include <unordered_set>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <array>
#include <limits>

#include <common.h>

struct Coordinate {
    
    std::int64_t x;
    std::int64_t y;

    Coordinate(std::int64_t x, std::int64_t y) : x(x), y(y) {}

    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }

    Coordinate up() const { return Coordinate(x, y - 1); }
    Coordinate down() const { return Coordinate(x, y + 1); }
    Coordinate left() const { return Coordinate(x - 1, y); }
    Coordinate right() const { return Coordinate(x + 1, y); }

};

namespace std {

    template<>
    struct hash<Coordinate> {
        std::size_t operator()(const Coordinate& coordinate) const {
            std::hash<std::int64_t> hasher;
            return aoc::hash_combine(hasher(coordinate.x), coordinate.y);
        }
    };

}

struct Plot {

    char type;
    std::unordered_set<Coordinate> coordinates;

    Plot(char type) : type(type) {}

    std::size_t area() const {
        return coordinates.size();
    }

    // Returns [min_x, max_x, min_y, max_y]
    std::array<std::int64_t, 4> boundaries() const {
        std::int64_t min_x, max_x, min_y, max_y;
        min_x = min_y = std::numeric_limits<std::int64_t>::max();
        max_x = max_y = std::numeric_limits<std::int64_t>::min();
        for (const auto& coordinate : coordinates) {
            if (coordinate.x < min_x) min_x = coordinate.x;
            if (coordinate.x > max_x) max_x = coordinate.x;
            if (coordinate.y < min_y) min_y = coordinate.y;
            if (coordinate.y > max_y) max_y = coordinate.y;
        }
        return { min_x, max_x, min_y, max_y };
    }

    std::size_t perimeter() const {
        auto contains = [&](const std::unordered_set<Coordinate>& container, const Coordinate& coord) {
            const auto it = container.find(coord);
            return it != container.cend() ? 1 : 0;
        };
        // This solution is probably way too complicated and there's an easier way, but oh well
        std::size_t perimeter = 0;
        std::unordered_set<Coordinate> left_fences, right_fences, up_fences, down_fences;
        auto perimeter_check = [&](const std::unordered_set<Coordinate>& fences, const Coordinate& first, const Coordinate& second) {
            if (!contains(fences, first) && !contains(fences, second)) {
                ++perimeter;
            }
        };
        auto [min_x, max_x, min_y, max_y] = boundaries();
        for (std::int64_t y = min_y; y <= max_y; ++y) {
            for (std::int64_t x = min_x; x <= max_x; ++x) {
                Coordinate coordinate(x, y);
                if (!contains(coordinates, coordinate)) {
                    continue;
                }
                if (const auto up = coordinate.up(); !contains(coordinates, up)) {
                    up_fences.emplace(coordinate);
                    perimeter_check(up_fences, coordinate.left(), coordinate.right());
                }
                if (const auto down = coordinate.down(); !contains(coordinates, down)) {
                    down_fences.emplace(coordinate);
                    perimeter_check(down_fences, coordinate.left(), coordinate.right());
                }
                if (const auto left = coordinate.left(); !contains(coordinates, left)) {
                    left_fences.emplace(coordinate);
                    perimeter_check(left_fences, coordinate.up(), coordinate.down());
                }
                if (const auto right = coordinate.right(); !contains(coordinates, right)) {
                    right_fences.emplace(coordinate);
                    perimeter_check(right_fences, coordinate.up(), coordinate.down());
                }
            }
        }
        return perimeter;
    }

};

int main() {
    const auto input = aoc::read_file("day12/input.txt");
    const auto map = aoc::split(input, '\n');
    if (map.empty()) {
        throw std::runtime_error("Input empty.");
    }

    const auto map_height = static_cast<std::int64_t>(map.size());
    const auto map_width = static_cast<std::int64_t>(map[0].size());

    std::vector<Plot> plots;
    std::unordered_set<Coordinate> processed_coordinates;
    auto is_valid_coord = [&](const Coordinate& coord) {
        return coord.x >= 0 && coord.x < map_width && coord.y >= 0 && coord.y < map_height;
    };
    std::function<void(const Coordinate&, Plot&)> process = [&](const Coordinate& coord, Plot& plot) {
        // Ignore invalid coordinates
        if (!is_valid_coord(coord)) {
            return;
        }
        // Skip already processed plots
        if (const auto it = processed_coordinates.find(coord); it != processed_coordinates.cend()) {
            return;
        }
        if (map[coord.y][coord.x] != plot.type) {
            return;
        }
        plot.coordinates.emplace(coord);
        processed_coordinates.emplace(coord);
        process(coord.up(), plot);
        process(coord.down(), plot);
        process(coord.left(), plot);
        process(coord.right(), plot);
    };

    for (std::int64_t y = 0; y < map_height; ++y) {
        for (std::int64_t x = 0; x < map_width; ++x) {
            const Coordinate coord(x, y);
            if (const auto it = processed_coordinates.find(coord); it != processed_coordinates.cend()) {
                continue;
            }
            char type = map[y][x];
            auto& plot = plots.emplace_back(type);
            process(coord, plot);
        }
    }

    std::size_t result = 0;
    for (const auto& plot : plots) {
        result += plot.area() * plot.perimeter();
    }
    std::cout << result << std::endl;
    return 0;
}
