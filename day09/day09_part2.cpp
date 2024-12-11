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

    using Range = std::pair<std::size_t, std::size_t>;
    std::size_t cursor = blocks.size();
    auto get_next_block = [&]() -> std::optional<Range> {
        // Return the next block left of the cursor
        std::optional<std::size_t> found_value;
        std::size_t block_start = 0;
        std::size_t block_end = 0;
        // Move to the first non-empty block
        for (std::int64_t i = static_cast<std::int64_t>(cursor) - 1; i >= 0; --i) {
            if (blocks[i].has_value()) {
                found_value = blocks[i];
                block_end = i;
                break;
            }
        }
        // Check if we found anything
        if (!found_value) {
            return std::nullopt;
        }
        // Keep seeking until we read the same value
        for (std::int64_t i = static_cast<std::int64_t>(block_end); i >= 0; --i) {
            if (blocks[i] != found_value) {
                break;
            }
            block_start = static_cast<std::size_t>(i);
        }
        cursor = block_start; // Adjust cursor
        return std::make_pair(block_start, block_end);
    };
    auto get_fitting_free_section = [&](std::size_t min_length) -> std::optional<Range> {
        std::size_t seek_from = 0;
        while (seek_from < cursor) {
            std::optional<std::size_t> block_start;
            for (std::size_t i = seek_from; i < cursor; ++i) {
                if (!blocks[i].has_value()) {
                    block_start = i;
                    break;
                }
            }
            // If we ran out of free blocks return empty
            if (!block_start) {
                return std::nullopt;
            }
            // Otherwise find the end of the range
            std::size_t block_end = *block_start;
            for (std::size_t i = *block_start; i < cursor; ++i) {
                if (blocks[i].has_value()) {
                    break;
                }
                block_end = i;
            }
            // If the range meets the minimum length we found our result
            const auto length = block_end - *block_start + 1;
            if (length >= min_length) {
                return std::make_pair(*block_start, block_end);
            }
            // Otherwise adjust seek from
            seek_from = block_end + 1;
        }
        return std::nullopt;
    };

    std::optional<Range> next_block;
    while ((next_block = get_next_block()).has_value()) {
        const auto min_length = next_block->second - next_block->first + 1;
        const auto maybe_fitting_free_section = get_fitting_free_section(min_length);
        // If we do not have a fitting free section just leave the block alone
        if (!maybe_fitting_free_section) {
            continue;
        }
        // Otherwise move the block to the beginning of the free section
        for (std::size_t i = 0; i < min_length; ++i) {
            const auto new_idx = static_cast<std::size_t>(maybe_fitting_free_section->first) + i;
            const auto old_idx = static_cast<std::size_t>(next_block->first) + i;
            blocks[new_idx] = blocks[old_idx];
            blocks[old_idx] = std::nullopt;
        }
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
