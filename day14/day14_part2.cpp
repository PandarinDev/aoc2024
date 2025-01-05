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

bool is_likely_tree(const std::vector<Robot>& robots) {
    const auto has_robot_at = [&](std::int64_t x, std::int64_t y) {
        if (x < 0 || y < 0 || x >= map_width || y >= map_height) {
            return false;
        }
        for (const auto& robot : robots) {
            if (robot.position.x == x && robot.position.y == y) {
                return true;
            }
        }
        return false;
    };
    // Looking for the top of the tree which has to be a ^ shape
    for (const auto& robot : robots) {
        if (has_robot_at(robot.position.x - 1, robot.position.y + 1) &&
            has_robot_at(robot.position.x + 1, robot.position.y + 1) &&
            has_robot_at(robot.position.x - 2, robot.position.y + 2) &&
            has_robot_at(robot.position.x + 2, robot.position.y + 2) &&
            has_robot_at(robot.position.x - 3, robot.position.y + 3) &&
            has_robot_at(robot.position.x + 3, robot.position.y + 3)) {
            return true;
        }
    }
    return false;
}

void print_tree(const std::vector<Robot>& robots, std::size_t iteration) {
    std::ofstream file_handle("day14/output_" + std::to_string(iteration) + ".txt");
    if (!file_handle) {
        throw std::runtime_error("Failed to open output file.");
    }
    const auto count_robots_at = [&](std::size_t x, std::size_t y) {
        std::size_t result = 0;
        for (const auto& robot : robots) {
            if (robot.position.x == static_cast<std::int64_t>(x) &&
                robot.position.y == static_cast<std::int64_t>(y)) {
                ++result;
            }
        }
        return result;
    };
    for (std::size_t y = 0; y < map_height; ++y) {
        for (std::size_t x = 0; x < map_width; ++x) {
            const auto num_robots = count_robots_at(x, y);
            if (num_robots > 0) {
                file_handle << num_robots;
            }
            else {
                file_handle << '.';
            }
        }
        file_handle << '\n';
    }
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

    for (std::size_t i = 0; i < 10'000; ++i) {
        if (is_likely_tree(robots)) {
            print_tree(robots, i);
        }
        simulate_second(robots);
    }

    return 0;
}