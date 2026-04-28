#pragma once

#include <string>
#include <optional>

namespace FileHandling {
    
class FileLoader
{
public:
    static std::optional<std::string> read(const std::string &file);
};

}