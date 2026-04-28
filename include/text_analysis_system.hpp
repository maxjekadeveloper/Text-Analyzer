#pragma once
#include <thread>
#include <unordered_map>
#include <any>
#include <vector>
#include <memory>
#include "analyzer.hpp"
#include "analyzer_factory.hpp"
#include "thread_pool.hpp"

namespace FileHandling {
// Main class for the text analysis system
class TextAnalysisSystem {
public:
    TextAnalysisSystem(size_t numThreads = std::thread::hardware_concurrency());

    /**
     * Analyzes a text file with all available analyzers.
     * 
     * @param filename The path to the file to analyze
     * @return A map of analyzer name to analysis result
     */
    std::unordered_map<std::string, std::any> analyzeFile(const std::string& filename);
    
    /**
     * Analyzes a string with all available analyzers.
     * 
     * @param text The text to analyze
     * @return A map of analyzer name to analysis result
     */
    std::unordered_map<std::string, std::any> analyzeText(const std::string& text);
    
    /**
     * Batch processes multiple files.
     * 
     * @param directory The directory containing the files to process
     * @param extension The extension of files to process (e.g., ".txt")
     * @param outputDir The directory where to save the results
     */
    void batchProcess(const std::string& directory, const std::string& extension, const std::string& outputDir);
    
private:
    ThreadPool threadPool;
    AnalyzerFactory factory;
    std::vector<std::unique_ptr<TextAnalyzer>> analyzers;
    
    /**
     * Generates a formatted report from analysis results.
     * 
     * @param results The analysis results
     * @return A formatted string report
     */

    void saveResults(const std::string &directory, const std::string &report) const;
    std::string generateReport(const std::unordered_map<std::string, std::any>& results) const;
};
}