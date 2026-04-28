#include <fstream>
#include "file_writer.hpp"

namespace FileHandling {
void FileWriter::write(const std::string &file, const std::string& content)
{
    std::ofstream offile(file);
    if(!offile)
        throw std::runtime_error("Could not open file for writing: " + file);

    offile << content;
    if (!offile) {
        throw std::runtime_error("Error writing to file: " + file);
    }
}
}