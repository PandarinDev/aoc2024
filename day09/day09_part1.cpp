#include <variant>
#include <string>
#include <stdexcept>
#include <iostream>

#include <common.h>

int main() {
    const auto input = aoc::read_file("day09/input.txt");
    std::vector<std::optional<std::size_t>> blocks;
    bool reading_file_size = true;
    for (std::size_t i = 0; i < input.size(); ++i) {
        const auto num = std::atoi(std::string(1, input[i]).c_str());
        blocks.insert(blocks.cend(), num, reading_file_size
            ? std::make_optional(i / 2)
            : std::nullopt);

        // Alternate between reading file size and free blocks
        reading_file_size = !reading_file_size;
    }
    auto has_empty_space_between_blocks = [&blocks]() {
        bool found_value = false;
        for (std::int64_t i = blocks.size() - 1; i >= 0; --i) {
            const auto& block = blocks[i];
            if (block.has_value()) {
                found_value = true;
            }
            else if (found_value) {
                return true;
            }
        }
        return false;
    };
    auto get_first_empty_index = [&blocks]() -> std::optional<std::size_t> {
        for (std::size_t i = 0; i < blocks.size(); ++i) {
            if (!blocks[i].has_value()) {
                return i;
            }
        }
        return std::nullopt;
    };
    auto pop_last_value = [&blocks]() {
        // Pop empty values first from the back if there are any
        while (!blocks.empty() && !blocks.back().has_value()) {
            blocks.pop_back();
        }
        if (blocks.empty()) {
            throw std::runtime_error("Result vector is empty.");
        }
        // Pop the last value and return it
        const auto last_value = blocks.back();
        blocks.pop_back();
        return last_value;
    };

    while (has_empty_space_between_blocks()) {
        const auto empty_index = get_first_empty_index();
        const auto last_value = pop_last_value();
        blocks[*empty_index] = last_value;
    }

    std::size_t result = 0;
    for (std::size_t i = 0; i < blocks.size(); ++i) {
        if (!blocks[i].has_value()) {
            continue;
        }
        result += i * blocks[i].value();
    }
    std::cout << result << std::endl;

    return 0;
}