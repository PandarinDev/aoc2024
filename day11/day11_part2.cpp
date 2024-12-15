#include <vector>
#include <cmath>
#include <iostream>

#include <common.h>

static constexpr auto max_iterations = 75;

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
    using ProcessFunction = std::function<std::int64_t(std::int64_t, std::int64_t)>;
    std::unordered_map<std::pair<std::int64_t, std::int64_t>, std::int64_t> cache;
    auto memoize = [&](ProcessFunction func, std::int64_t stone, std::int64_t iteration) -> std::int64_t {
        const auto func_args = std::make_pair(stone, iteration);
        if (const auto it = cache.find(func_args); it != cache.cend()) {
            return it->second;
        }
        const auto result = func(stone, iteration);
        cache.emplace(func_args, result);
        return result;
    };
    ProcessFunction process = [&](std::int64_t stone, std::int64_t iteration) -> std::int64_t {
        if (iteration == max_iterations) {
            return 1;
        }
        if (stone == 0) {
            return memoize(process, 1, iteration + 1);
        }
        else if (has_even_digits(stone)) {
            const auto split_result = split_number(stone);
            return memoize(process, split_result.first, iteration + 1) + memoize(process, split_result.second, iteration + 1);
        }
        return memoize(process, stone * 2024, iteration + 1);
    };

    std::size_t result = 0;
    for (const auto value : stones) {
        result += process(value, 0);
    }
    std::cout << result << std::endl;
    return 0;
}