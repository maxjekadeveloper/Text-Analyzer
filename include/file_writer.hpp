#pragma once
#include <string>
#include <fstream>

namespace FileHandling {
    
class FileWriter
{
public:
    FileWriter(const std::string &file);
    void write(const std::string& content);

private:
    std::ofstream offile;    
};

}