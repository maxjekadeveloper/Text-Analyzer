#include "file_writer.hpp"

namespace FileHandling {
FileWriter::FileWriter(const std::string &file) : offile{file}
{
    if(!offile)
        throw std::runtime_error("Could not open file for writing: " + file);
}

void FileWriter::write(const std::string& content)
{
    offile << content;
    if (!offile) {
        throw std::runtime_error("Error writing to file");
    }
}
}