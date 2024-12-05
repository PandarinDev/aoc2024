#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>

#include <common.h>

using Edges = std::unordered_map<std::size_t, std::unordered_set<std::size_t>>;

int main() {
    const auto input = aoc::read_file("day05/input.txt");
    
    // First split the lines into dependencies and updates
    Edges edges;
    std::vector<std::vector<std::size_t>> updates;
    bool reading_edges = true;
    for (const auto& line : aoc::split(input, '\n', true)) {
        if (line.empty()) {
            reading_edges = false;
            continue;
        }
        if (reading_edges) {
            const auto edge = aoc::map<std::string_view, std::size_t>(
                aoc::split(line, '|'), aoc::str_view_to_type<std::size_t>);
            const auto from = edge.at(0);
            const auto to = edge.at(1);
            edges[from].emplace(to);
        }
        else {
            auto update = aoc::map<std::string_view, std::size_t>(
                aoc::split(line, ','), aoc::str_view_to_type<std::size_t>);
            updates.emplace_back(std::move(update));
        }
    }

    // Process the updates
    auto is_parent_of = [&edges](std::size_t parent, std::size_t child) {
        const auto it = edges.find(parent);
        if (it == edges.cend()) {
            return false;
        }
        const auto children = it->second;
        return children.find(child) != children.cend();
    };

    // Collect the incorrect updates
    std::vector<std::vector<std::size_t>> updates_to_sort;
    for (const auto& update : updates) {
        bool is_correct = true;
        for (std::size_t i = 1; i < update.size(); ++i) {
            const auto previous = update[i - 1];
            const auto current = update[i];
            if (!is_parent_of(previous, current)) {
                is_correct = false;
                break;
            }
        }
        if (!is_correct) {
            updates_to_sort.emplace_back(update);
        }
    }

    // Sort the incorrect updates and calculate result
    std::size_t result = 0;
    for (auto update : updates_to_sort) {
        std::sort(update.begin(), update.end(), is_parent_of);
        result += update[update.size() / 2];
    }
    std::cout << result << std::endl;

    return 0;
}
