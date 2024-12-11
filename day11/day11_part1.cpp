#include <vector>
#include <cmath>
#include <iostream>

#include <common.h>

int main() {
    const auto input = aoc::read_file("day11/input.txt");
    const auto entries = aoc::split(input, ' ');
    // Parse initial stones
    std::vector<std::int64_t> stones = aoc::map<std::string_view, std::int64_t>(
        entries, aoc::str_view_to_type<std::int64_t>);
    auto get_num_digits = [](std::int64_t number) {
        return (static_cast<std::int64_t>(std::log10(number)) + 1);
    };
    auto has_even_digits = [&](std::int64_t number) {
        return get_num_digits(number) % 2 == 0;
    };
    auto split_number = [&](std::int64_t number) -> std::pair<std::int64_t, std::int64_t> {
        const auto num_digits = get_num_digits(number);
        const auto quotient = std::pow(10, num_digits / 2.0);
        const auto split = number / quotient;
        double first_half;
        // We have to round because for some numbers modf gives very close to whole results
        // At this point this is hacky enough that it is tempting to convert to string, split and parse back
        const auto second_half = std::round(std::modf(split, &first_half) * quotient);
        return std::make_pair(static_cast<std::int64_t>(first_half), static_cast<std::int64_t>(second_half));
    };
    for (std::size_t i = 0; i < 25; ++i) {
        std::vector<std::int64_t> updated_stones;
        for (auto stone : stones) {
            if (stone == 0) {
                updated_stones.emplace_back(1);
            }
            else if (has_even_digits(stone)) {
                const auto split_result = split_number(stone);
                updated_stones.emplace_back(split_result.first);
                updated_stones.emplace_back(split_result.second);
            }
            else {
                updated_stones.emplace_back(stone * 2024);
            }
        }
        stones = std::move(updated_stones);
    }
    std::cout << stones.size() << std::endl;
    return 0;
}