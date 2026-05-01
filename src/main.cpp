#include <iostream>
#include <string>
#include <chrono>

#include "text_analysis_system.hpp"

int main(int argc, char* argv[]) 
{
    auto startTime = std::chrono::high_resolution_clock::now();

    try {
        // Parse command line arguments
        if (argc < 2) {
            std::cout << "Usage: " << argv[0] << " <filename> [num_threads]" << std::endl;
            return 1;
        }
        
        std::string directory = argv[1];
        if(directory.back() != '\\')
            directory = directory + '\\';
        std::string extension = argv[2];
        if(extension[0] != '.')
            extension = '.' + extension;

        std::cout << "directory: " << directory << std::endl;
        std::cout << "files with an extension: " << extension << std::endl;

        size_t numThreads = (argc > 3) ? std::stoul(argv[3]) : std::thread::hardware_concurrency();
        std::cout << "threads: " << numThreads << std::endl;
        
        // Create and run the text analysis system
        const std::string outputDir = directory + "output_file.txt";
        FileHandling::TextAnalysisSystem system(outputDir, numThreads);
        system.batchProcess(directory, extension);
    }
    catch(const std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "duration = " << duration << std::endl;
}