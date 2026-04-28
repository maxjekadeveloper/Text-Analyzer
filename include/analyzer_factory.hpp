#pragma once
#include <memory>
#include "analyzer.hpp"

namespace FileHandling {
// Factory for creating analyzers
class AnalyzerFactory {
public:
    /**
     * Creates an analyzer based on the specified type.
     * 
     * @param type The type of analyzer to create
     * @return A unique pointer to the created analyzer
     * @throws std::runtime_error if the type is not recognized
     */
    std::unique_ptr<TextAnalyzer> createAnalyzer(const std::string& type);
};
}