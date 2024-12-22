#include <unordered_set>
#include <functional>
#include <stdexcept>
#include <iostream>

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

    std::size_t perimeter() const {
        auto is_neighbor = [&](const Coordinate& coord) {
            const auto it = coordinates.find(coord);
            return it != coordinates.cend() ? 1 : 0;
        };
        auto num_neighbors = [&](const Coordinate& coord) {
            return is_neighbor(coord.up()) +
                is_neighbor(coord.down()) +
                is_neighbor(coord.left()) +
                is_neighbor(coord.right());
        };
        std::size_t result = 0;
        for (const auto& coord : coordinates) {
            result += 4 - num_neighbors(coord);
        }
        return result;
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
