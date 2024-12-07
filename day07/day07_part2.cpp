#include <stdexcept>
#include <vector>
#include <iostream>
#include <numeric>

#include <common.h>

enum class OperatorType {
    PLUS,
    MULTIPLY,
    CONCATENATION
};

std::int64_t apply_operator(OperatorType op, std::int64_t first, std::int64_t second) {
    switch (op) {
        case OperatorType::PLUS: return first + second;
        case OperatorType::MULTIPLY: return first * second;
        case OperatorType::CONCATENATION: return first * static_cast<std::int64_t>(
            std::pow(10, (static_cast<std::int64_t>(std::log10(second)) + 1))) + second;
        default: throw std::runtime_error("Unknown operator.");
    }
}

int main() {
    const auto input = aoc::read_file("day07/input.txt");

    std::vector<OperatorType> operator_types { OperatorType::PLUS, OperatorType::MULTIPLY, OperatorType::CONCATENATION };
    auto is_solvable = [&](std::int64_t test_value, const std::vector<std::int64_t>& numbers) {
        // There is one less operator than numbers
        const auto num_operators = numbers.size() - 1;
        const auto maybe_result = aoc::combinate_until<OperatorType>(operator_types, num_operators, [&](const auto& operators) {
            std::int64_t result = numbers[0];
            for (std::size_t i = 1; i < numbers.size(); ++i) {
                const auto op = operators[i - 1];
                const auto number = numbers[i];
                result = apply_operator(op, result, number);
            }
            return result == test_value;
        });
        return maybe_result.has_value();
    };

    std::size_t result = 0;
    for (const auto line : aoc::split(input, '\n')) {
        const auto data = aoc::split(line, ':');
        if (data.size() != 2) {
            throw std::runtime_error("Invalid input line.");
        }
        const auto test_value = aoc::str_view_to_type<std::int64_t>(data[0]);
        const auto numbers = aoc::map<std::string_view, std::int64_t>(
            aoc::split(data[1], ' '), aoc::str_view_to_type<std::int64_t>);
        if (is_solvable(test_value, numbers)) {
            result += test_value;
        }
    }
    std::cout << result << std::endl;

    return 0;
}