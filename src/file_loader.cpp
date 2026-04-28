
#include <fstream>
#include "file_loader.hpp"

namespace FileHandling {
std::optional<std::string> FileLoader::read(const std::string &file)
{
    std::ifstream iffile(file);
    if(iffile)
        return std::string{std::istreambuf_iterator<char>{iffile}, std::istreambuf_iterator<char>{}};

    return std::nullopt;
}
}