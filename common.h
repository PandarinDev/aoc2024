#pragma once

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

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

}