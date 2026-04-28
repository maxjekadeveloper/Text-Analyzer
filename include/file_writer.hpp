#pragma once
#include <string>

namespace FileHandling {
    
class FileWriter
{
public:
    static void write(const std::string &file, const std::string& content);
};

}