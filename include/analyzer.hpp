#pragma once
#include <unordered_set>
#include <any>
#include <string>
#include <ranges>

namespace FileHandling {
// Base class for all text analyzers
class TextAnalyzer {
public:
    virtual ~TextAnalyzer() = default;
    
    /**
     * Analyzes the given text and returns the result.
     * 
     * @param text The text to analyze
     * @return The analysis result as std::any
     */
    virtual std::any analyze(const std::string& text) = 0;
    
    /**
     * Gets the name of the analyzer.
     * 
     * @return The name of the analyzer
     */
    virtual std::string getName() const = 0;
};

class WordFrequencyAnalyzer : public TextAnalyzer {
public:
    std::any analyze(const std::string& text) override;
    std::string getName() const override;
    
protected:
    // Helper function to clean a word (remove punctuation, convert to lowercase)
    auto cleanWord(const std::string& text) const
    {
        auto words = text 
        | std::views::transform([](unsigned char c){
            return std::isalpha(c) ? (char)std::tolower(c) : ' '; 
        }) 
        | std::views::split(' ')
        | std::views::filter([](auto&& r) { return !std::ranges::empty(r); });

        return words;
    }
};

// Readability analyzer implementation
class ReadabilityAnalyzer : public WordFrequencyAnalyzer {
public:
    struct ReadabilityMetrics {
        double fleschReadingEase = 0.0;
        double fleschKincaidGrade = 0.0;
        double averageSentenceLength = 0.0;
        double averageWordLength = 0.0;
        int totalWords = 0;
        int totalSentences = 0;
        int totalSyllables = 0;
    };
    
    std::any analyze(const std::string& text) override;
    std::string getName() const override;
    
private:
    double countAverageWordLength(const std::string& text) const;
    double countAverageSentenceLength(const std::string& text) const;

    // Helper function to count words
    int countWords(const std::string& text) const;
    // Helper function to count syllables in a word
    int countSyllables(const std::string& word) const;
    
    // Helper function to count sentences in text
    int countSentences(const std::string& text) const;
    // Helper
    bool isVowel(char c) const;
    // Helper
    int countWordSyllables(std::string_view word) const;
};

// Sentiment analyzer implementation
class SentimentAnalyzer : public WordFrequencyAnalyzer {
public:
    SentimentAnalyzer();
    
    std::any analyze(const std::string& text) override;
    std::string getName() const override;
    
private:
    std::unordered_set<std::string> positiveWords;
    std::unordered_set<std::string> negativeWords;
};
}