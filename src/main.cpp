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
        size_t numThreads = (argc > 2) ? std::stoul(argv[2]) : std::thread::hardware_concurrency();
        
        // Create and run the text analysis system
        FileHandling::TextAnalysisSystem system(numThreads);
        system.batchProcess(directory, ".txt", directory);
    }
    catch(const std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "duration = " << duration << std::endl;
}