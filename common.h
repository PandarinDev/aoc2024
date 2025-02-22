#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <functional>
#include <charconv>
#include <optional>

namespace aoc {

    std::string read_file(const std::filesystem::path& file_path) {
        std::ifstream file_handle(file_path);
        if (!file_handle) {
            throw std::runtime_error("Failed to open file at '" + file_path.string() + "'.");
        }
        std::stringstream str_stream;
        str_stream << file_handle.rdbuf();
        return str_stream.str();
    }

    std::vector<std::string_view> split(std::string_view input, char delimiter, bool keep_empty = false) {
        std::vector<std::string_view> result;
        std::size_t start_idx = 0;
        for (std::size_t i = 0; i < input.size(); ++i) {
            char c = input[i];
            if (c == delimiter) {
                const auto size = i - start_idx;
                if (size > 0 || keep_empty) {
                    result.emplace_back(input.substr(start_idx, size));
                }
                start_idx = i + 1;
            }
        }
        const auto size = input.size() - start_idx;
        if (size > 0 || keep_empty) {
            result.emplace_back(input.substr(start_idx, size));
        }
        return result;
    }

    template<typename InType, typename OutType>
    std::vector<OutType> map(const std::vector<InType>& input, std::function<OutType(const InType&)> mapper) {
        std::vector<OutType> result;
        result.reserve(input.size());
        for (const auto& entry : input) {
            result.push_back(mapper(entry));
        }
        return result;
    }

    template<typename T>
    T str_view_to_type(std::string_view str) {
        T value{};
        const auto [_, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
        if (ec != std::errc()) {
            throw std::runtime_error("Failed to convert entry to output type.");
        }
        return value;
    }

    template<typename T>
    std::size_t hash_combine(std::size_t seed, const T& value) {
        std::hash<T> hasher;
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }

    template<typename T>
    std::optional<std::vector<T>> combinate_until(
        const std::vector<T>& inputs,
        std::size_t size,
        const std::function<bool(const std::vector<T>&)>& stop_predicate) {
        using ResultType = std::optional<std::vector<T>>;
        std::function<ResultType(std::vector<T>&)> addElement = [&](std::vector<T>& current) -> ResultType {
            // If max element size is reached test the result
            if (current.size() == size) {
                if (stop_predicate(current)) {
                    return current;
                }
                return std::nullopt;
            }
            // Otherwise keep adding elements
            for (const T& input : inputs) {
                current.push_back(input);
                if (const auto maybe_result = addElement(current); maybe_result.has_value()) {
                    return maybe_result;
                }
                current.pop_back();
            }
            return std::nullopt;
        };
        std::vector<T> current;
        return addElement(current);
    }

    template<typename T>
    std::vector<std::pair<T, T>> unique_pairs(const std::vector<T>& input) {
        std::vector<std::pair<T, T>> pairs;
        // Not very sophisticated, but works
        auto contains_pair = [&pairs](const T& first, const T& second) {
            return std::find(pairs.cbegin(), pairs.cend(), std::make_pair(first, second)) != pairs.cend() ||
                std::find(pairs.cbegin(), pairs.cend(), std::make_pair(second, first)) != pairs.cend();
        };
        for (std::size_t i = 0; i < input.size(); ++i) {
            for (std::size_t j = 0; j < input.size(); ++j) {
                if (i == j) {
                    continue;
                }
                if (!contains_pair(input[i], input[j])) {
                    pairs.emplace_back(std::make_pair(input[i], input[j]));
                }
            }
        }
        return pairs;
    }

    // Special case of split when wanting to cut an input in half, useful with destructuring
    std::pair<std::string_view, std::string_view> cut(std::string_view input, std::string_view delimiter) {
        const auto delimiter_pos = input.find(delimiter);
        if (delimiter_pos == std::string_view::npos) {
            throw std::runtime_error("Delimiter not found in input");
        }
        return std::make_pair(input.substr(0, delimiter_pos), input.substr(delimiter_pos + delimiter.size()));
    }

}

namespace std {

    template<typename FirstType, typename SecondType>
    struct hash<std::pair<FirstType, SecondType>> {

        std::size_t operator()(const std::pair<FirstType, SecondType>& pair) const {
            std::hash<FirstType> hasher;
            return aoc::hash_combine(hasher(pair.first), pair.second);
        }

    };

}