
#include "analyzer_factory.hpp"
#include <exception>

namespace FileHandling {
std::unique_ptr<TextAnalyzer> AnalyzerFactory::createAnalyzer(const std::string& type) {
        if (type == "frequency") {
            return std::make_unique<WordFrequencyAnalyzer>();
        }
        else if (type == "readability") {
            return std::make_unique<ReadabilityAnalyzer>();
        }
        else if (type == "sentiment") {
            return std::make_unique<SentimentAnalyzer>();
        }
        
        throw std::runtime_error("Unknown analyzer type: " + type);
}
}