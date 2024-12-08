#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <iostream>

#include <common.h>

struct Location {

    std::int64_t x;
    std::int64_t y;

    Location(std::int64_t x, std::int64_t y) : x(x), y(y) {}

    bool operator==(const Location& other) const {
        return x == other.x && y == other.y;
    }

    Location operator+(const Location& other) const {
        return Location(x + other.x, y + other.y);
    }

    Location& operator+=(const Location& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Location& operator-=(const Location& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Location operator-(const Location& other) const {
        return Location(x - other.x, y - other.y);
    }

};

namespace std {

    template<>
    struct hash<Location> {
        std::size_t operator()(const Location& loc) const {
            std::hash<decltype(Location::x)> hasher;
            return aoc::hash_combine(hasher(loc.x), loc.y);
        }
    };

}

int main() {
    const auto input = aoc::read_file("day08/input.txt");
    const auto lines = aoc::split(input, '\n');
    if (lines.empty()) {
        throw std::runtime_error("Input is empty.");
    }
    std::unordered_map<char, std::vector<Location>> antennas;
    std::unordered_set<Location> antinodes;
    for (std::size_t y = 0; y < lines.size(); ++y) {
        const auto& line = lines[y];
        for (std::size_t x = 0; x < line.size(); ++x) {
            char c = line[x];
            if (c != '.') {
                antennas[c].emplace_back(
                    static_cast<std::int64_t>(x), static_cast<std::int64_t>(y));
            }
        }
    }
    auto map_width = static_cast<std::int64_t>(lines[0].size());
    auto map_height = static_cast<std::int64_t>(lines.size());
    auto is_within_bounds = [&](const Location& loc) {
        return loc.x >= 0 && loc.x < map_width && loc.y >= 0 && loc.y < map_height;
    };
    auto get_antinodes_for = [&](const Location& first, const Location& second) {
        std::vector<Location> result;
        // Test in both directions until we go out of bounds
        const auto offset = second - first;
        // Positive offset direction
        Location testing_location = second;
        while (is_within_bounds(testing_location)) {
            result.emplace_back(testing_location);
            testing_location += offset;
        }

        // Negative offset direction
        testing_location = first;
        while (is_within_bounds(testing_location)) {
            result.emplace_back(testing_location);
            testing_location -= offset;
        }
        return result;
    };
    for (const auto& [freq, positions] : antennas) {
        const auto pairs = aoc::unique_pairs(positions);
        for (const auto& [first, second] : pairs) {
            const auto antinodes_for_pair = get_antinodes_for(first, second);
            antinodes.insert(antinodes_for_pair.cbegin(), antinodes_for_pair.cend());
        }
    }
    std::cout << antinodes.size() << std::endl;
    return 0;
}