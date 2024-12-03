#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <functional>
#include <charconv>

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

    std::vector<std::string_view> split(std::string_view input, char delimiter) {
        std::vector<std::string_view> result;
        std::size_t start_idx = 0;
        for (std::size_t i = 0; i < input.size(); ++i) {
            char c = input[i];
            if (c == delimiter) {
                const auto size = i - start_idx;
                if (size > 0) {
                    result.emplace_back(input.substr(start_idx, size));
                }
                start_idx = i + 1;
            }
        }
        const auto size = input.size() - start_idx;
        if (size > 0) {
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

}