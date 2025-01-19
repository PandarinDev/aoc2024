#include <common.h>

#include <unordered_map>
#include <stdexcept>
#include <stack>
#include <functional>
#include <iostream>

enum class Cell {
    EMPTY, ROBOT, BOX, WALL
};

Cell parse_cell(char c) {
    switch (c) {
        case '.': return Cell::EMPTY;
        case '@': return Cell::ROBOT;
        case 'O': return Cell::BOX;
        case '#': return Cell::WALL;
        default: throw std::runtime_error("Unknown cell.");
    }
}

struct Coord {

    std::int64_t x;
    std::int64_t y;

    Coord() : x(0), y(0) {}
    Coord(std::int64_t x, std::int64_t y) : x(x), y(y) {}

    bool operator==(const Coord& other) const {
        return x == other.x && y == other.y;
    }

    Coord operator+(const Coord& other) const {
        return Coord(x + other.x, y + other.y);
    }

};

namespace std {

    template<>
    struct hash<Coord> {
        std::size_t operator()(const Coord& coord) const {
            std::hash<std::int64_t> hasher;
            return aoc::hash_combine(hasher(coord.x), coord.y);
        }
    };

}

struct Map {
    std::unordered_map<Coord, Cell> cells;

    Cell get_cell(const Coord& coord) {
        const auto it = cells.find(coord);
        if (it != cells.cend()) {
            return it->second;
        }
        throw std::runtime_error("Cell out of bounds.");
    }

    Coord get_robot_coord() const {
        for (const auto& [coord, cell] : cells) {
            if (cell == Cell::ROBOT) {
                return coord;
            }
        }
        throw std::runtime_error("No robot on map.");
    }

    std::vector<Coord> get_box_coordinates() const {
        std::vector<Coord> result;
        for (const auto& [coord, cell] : cells) {
            if (cell == Cell::BOX) {
                result.emplace_back(coord);
            }
        }
        return result;
    }

};

enum class Move {
    LEFT, RIGHT, UP, DOWN
};

Move parse_move(char c) {
    switch (c) {
        case '^': return Move::UP;
        case 'v': return Move::DOWN;
        case '<': return Move::LEFT;
        case '>': return Move::RIGHT;
        default: throw std::runtime_error("Unknown move.");
    }
}

Coord move_to_offset(Move move) {
    switch (move) {
        case Move::UP: return Coord(0, -1);
        case Move::DOWN: return Coord(0, 1);
        case Move::LEFT: return Coord(-1, 0);
        case Move::RIGHT: return Coord(1, 0);
        default: throw std::runtime_error("Unhandled move.");
    }
}

struct CellMove {
    Cell cell;
    Coord from;
    Coord to;
};

void simulate_move(Move move, Map& map) {
    const auto robot_coord = map.get_robot_coord();
    const auto offset = move_to_offset(move);
    std::stack<CellMove> cell_moves;
    const std::function<bool(const Coord&)> try_cell_move = [&](const Coord& coord) {
        const auto from = map.get_cell(coord);
        const auto target_coord = coord + offset;
        cell_moves.emplace(CellMove{ from, coord, target_coord });
        const auto to = map.get_cell(target_coord);
        switch (to) {
            case Cell::EMPTY: return true;
            case Cell::WALL: return false;
            case Cell::BOX: return try_cell_move(target_coord);
            default: throw std::runtime_error("Unhandled cell type during simulation.");
        }
    };
    // If we cannot execute the move just return
    if (!try_cell_move(robot_coord)) {
        return;
    }
    // Otherwise execute the cell move stack
    while (!cell_moves.empty()) {
        const auto move = cell_moves.top();
        cell_moves.pop();
        map.cells.at(move.from) = Cell::EMPTY;
        map.cells.at(move.to) = move.cell;
    }
}

std::size_t coord_to_gps_value(const Coord& coord) {
    return 100 * coord.y + coord.x;
}

int main() {
    const auto input = aoc::read_file("day15/input.txt");
    bool is_reading_moves = false;
    Map map;
    std::vector<Move> moves;
    std::int64_t y = 0;
    for (const auto& line : aoc::split(input, '\n', true)) {
        // Reading an empty line means that we are now reading moves
        if (line.empty()) {
            is_reading_moves = true;
            continue;
        }
        if (is_reading_moves) {
            // Parse moves
            for (char c : line) {
                moves.emplace_back(parse_move(c));
            }
        }
        else {
            // Parse map
            std::int64_t x = 0;
            for (char c : line) {
                const auto coord = Coord(x, y);
                const auto cell = parse_cell(c);
                map.cells.emplace(coord, cell);
                ++x;
            }
            ++y;
        }
    }

    // Simulate moves
    for (const auto move : moves) {
        simulate_move(move, map);
    }

    // Calculate the sum of all box GPS values
    std::size_t result = 0;
    for (const auto& box_coord : map.get_box_coordinates()) {
        result += coord_to_gps_value(box_coord);
    }
    std::cout << result << std::endl;
    return 0;
}