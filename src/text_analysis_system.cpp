#include "text_analysis_system.hpp"
#include "file_loader.hpp"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace FileHandling {

    TextAnalysisSystem::TextAnalysisSystem(const std::string_view& output, size_t numThreads) 
        : threadPool(numThreads), fileWriter{output.data()}, output{output} {
        // Initialize analyzers
        analyzers.push_back(factory.createAnalyzer("frequency"));
        analyzers.push_back(factory.createAnalyzer("readability"));
        analyzers.push_back(factory.createAnalyzer("sentiment"));
    }

    std::unordered_map<std::string, std::any> TextAnalysisSystem::analyzeFile(const std::string& filename) {
        try {
            auto content = FileLoader::read(filename);
            if(!content)
                throw std::runtime_error{"there is no text from the file"};
                
            return analyzeText(*content);
        }
        catch (const std::exception& e) {
            std::cerr << "Error analyzing file " << filename << ": " << e.what() << std::endl;
            return {};
        }
    }

    std::unordered_map<std::string, std::any> TextAnalysisSystem::analyzeText(const std::string& text) {
        std::unordered_map<std::string, std::any> results;
        std::vector<std::pair<std::string, std::future<std::any>>> futures;
        
        for(auto &analyzer : analyzers)
        {
            auto future = threadPool.enqueue([&analyzer](const std::string& text){return analyzer->analyze(text);}, text);
            futures.push_back(std::pair{analyzer->getName(), std::move(future)});
        }

        for(auto &[first, second] : futures)
            results.try_emplace(first, std::move(second.get()));

        return results;
    }

    void TextAnalysisSystem::batchProcess(const std::string& directory, const std::string& extension) {
        if(!fs::is_directory(directory))
            throw std::runtime_error{directory + "is not directory."};
        if(!fs::exists(directory))
            throw std::runtime_error{directory + "doesn't exist."};
            
        std::unordered_map<std::string, std::any> results;
        for(const auto& file : fs::directory_iterator{directory})
        {
            auto path = file.path().string();
            if (file.is_regular_file() && file.path().extension() == extension) {
                results = analyzeFile(path);
                if(!results.empty())
                {
                    std::string report = "******    report for the file - '" + path + "'    ******\n";
                    report += generateReport(results);
                    saveResults(report);
                }
            }
        }
    }

    std::string TextAnalysisSystem::generateReport(const std::unordered_map<std::string, std::any>& results) const {
        std::string result;

        auto word_freq_it = results.find("Word Frequency Analyzer");
        auto word_map = std::any_cast<std::unordered_map<std::string, int>>(word_freq_it->second);
        for(const auto &[word, frequency] : word_map)
            result += "word: '" + word + "' appears " + std::to_string(frequency) + " times\n";

        auto readability_it = results.find("Readability Analyzer");
        auto metrics = std::any_cast<ReadabilityAnalyzer::ReadabilityMetrics>(readability_it->second);
        result += "total words: " + std::to_string(metrics.totalWords) + '\n'
                + "total syllables: " + std::to_string(metrics.totalSyllables) + '\n'
                + "total sentences: " + std::to_string(metrics.totalSentences) + '\n'
                + "average sentence length: " + std::to_string(metrics.averageSentenceLength) + '\n'
                + "average word length: " + std::to_string(metrics.averageWordLength) + '\n'
                + "flesch reading ease: " + std::to_string(metrics.fleschReadingEase) + '\n'
                + "flesch kincaid grade: " + std::to_string(metrics.fleschKincaidGrade) + '\n';

        auto score_it = results.find("Sentiment Analyzer");
        auto score = std::any_cast<double>(score_it->second);
        result += "score: " + std::to_string(score) + "\n\n";

        return result;
    }

    void TextAnalysisSystem::saveResults(const std::string &report)
    {
        fileWriter.write(report);
    }
}