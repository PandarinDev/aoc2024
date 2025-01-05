#include <common.h>

#include <stdexcept>
#include <iostream>

struct Vec2 {
    std::int64_t x;
    std::int64_t y;

    Vec2() : x(0), y(0) {}
    Vec2(std::int64_t x, std::int64_t y) : x(x), y(y) {}

};

struct Robot {
    Vec2 position;
    Vec2 velocity;

    Robot() = default;
    Robot(const Vec2& position, const Vec2& velocity) : position(position), velocity(velocity) {}

};

struct Quadrant {
    Vec2 from;
    Vec2 to;

    Quadrant() = default;
    Quadrant(const Vec2& from, const Vec2& to) : from(from), to(to) {}
};

static constexpr std::int64_t map_width = 101;
static constexpr std::int64_t map_height = 103;

void simulate_second(std::vector<Robot>& robots) {
    for (auto& robot : robots) {
        robot.position.x = (map_width + ((robot.position.x + robot.velocity.x) % map_width)) % map_width;
        robot.position.y = (map_height + ((robot.position.y + robot.velocity.y) % map_height)) % map_height;
    }
}

std::vector<Quadrant> create_quadrants() {
    return {
        Quadrant(Vec2(0, 0), Vec2(map_width / 2, map_height / 2)),                          // Top left quadrant
        Quadrant(Vec2(0, map_height / 2 + 1), Vec2(map_width / 2, map_height)),             // Bottom left quadrant
        Quadrant(Vec2(map_width / 2 + 1, 0), Vec2(map_width, map_height / 2)),              // Top right quadrant
        Quadrant(Vec2(map_width / 2 + 1, map_height / 2 + 1), Vec2(map_width, map_height)), // Bottom right quadrant
    };
}

std::size_t calculate_safety_factor(const std::vector<Robot>& robots) {
    auto count_robots = [&](const Quadrant& quadrant) {
        std::size_t result = 0;
        for (const auto& robot : robots) {
            if (robot.position.x >= quadrant.from.x && robot.position.x < quadrant.to.x &&
                robot.position.y >= quadrant.from.y && robot.position.y < quadrant.to.y) {
                ++result;
            }
        }
        return result;
    };
    const auto quadrants = create_quadrants();
    std::size_t result = 1;
    for (const auto& quadrant : quadrants) {
        result *= count_robots(quadrant);
    }
    return result;
}

int main() {
    std::vector<Robot> robots;
    const auto input = aoc::read_file("day14/input.txt");
    for (const auto& line : aoc::split(input, '\n')) {
        const auto data = aoc::split(line, ' ');
        if (data.size() != 2) {
            throw std::runtime_error("Invalid data line.");
        }
        const auto pos_data = aoc::map<std::string_view, std::int64_t>(
            aoc::split(aoc::split(data[0], '=').at(1), ','), aoc::str_view_to_type<std::int64_t>);
        const auto vel_data = aoc::map<std::string_view, std::int64_t>(
            aoc::split(aoc::split(data[1], '=').at(1), ','), aoc::str_view_to_type<std::int64_t>);
        if (pos_data.size() != 2 || vel_data.size() != 2) {
            throw std::runtime_error("Invalid positional or velocity data.");
        }
        robots.emplace_back(Vec2{ pos_data[0], pos_data[1] }, Vec2{ vel_data[0], vel_data[1] });
    }

    for (std::size_t i = 0; i < 100; ++i) {
        simulate_second(robots);
    }
    std::cout << calculate_safety_factor(robots) << std::endl;

    return 0;
}