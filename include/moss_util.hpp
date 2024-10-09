// Header only utility functions for the Moss engine

#pragma once

#include <fstream>
#include <vector>
#include <filesystem>

namespace util
{
    std::vector<char> readFile(std::filesystem::path path)
    {
        std::ifstream file(path, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + path.string());
        }

        size_t file_size = (size_t)file.tellg();
        std::vector<char> buffer(file_size);

        file.seekg(0);
        file.read(buffer.data(), static_cast<std::streamsize>(file_size));

        file.close();

        return buffer;
    }
}