#include "analyzer.hpp"
#include <unordered_map>
#include <iostream>

namespace {
        // Flesch Reading Ease
        constexpr double RE_BASE = 206.835;
        constexpr double RE_SENTENCE_WEIGHT = 1.015;
        constexpr double RE_WORD_WEIGHT = 84.6;

        // Flesch-Kincaid Grade Level
        constexpr double GL_SENTENCE_WEIGHT = 0.39;
        constexpr double GL_WORD_WEIGHT = 11.8;
        constexpr double GL_BASE = 15.59;
};

namespace FileHandling {
/**
 * @brief 
 * @class WordFrequencyAnalyzer
 */
std::any WordFrequencyAnalyzer::analyze(const std::string& text) {
        std::unordered_map<std::string, int> wordCounts;
        
        auto words = cleanWord(text);

        for(const auto& word : words)
        {
                std::string cleanWord = (word | std::ranges::to<std::string>());
                wordCounts[cleanWord]++;
        }
        
        return wordCounts;
}
    
std::string WordFrequencyAnalyzer::getName() const {
        return "Word Frequency Analyzer";
}

/**
 * @brief 
 * @class ReadabilityAnalyzer
 */
std::any ReadabilityAnalyzer::analyze(const std::string& text) {
        ReadabilityMetrics metrics;

        metrics.totalWords = countWords(text);
        metrics.totalSyllables = countSyllables(text);
        metrics.totalSentences = countSentences(text);

        metrics.averageSentenceLength = countAverageSentenceLength(text);
        metrics.averageWordLength = countAverageWordLength(text);

        if(!metrics.totalSentences)
                throw std::logic_error{"There is an attempt of dividing into 0. // metrics.totalSentences = 0"};
        if(!metrics.totalWords)
                throw std::logic_error{"There is an attempt of dividing into 0. // metrics.totalWords = 0"};

        metrics.fleschReadingEase = RE_BASE - RE_SENTENCE_WEIGHT * (static_cast<double>(metrics.totalWords) / static_cast<double>(metrics.totalSentences)) - 
        RE_WORD_WEIGHT * (static_cast<double>(metrics.totalSyllables) / static_cast<double>(metrics.totalWords));

        metrics.fleschKincaidGrade = GL_SENTENCE_WEIGHT * (static_cast<double>(metrics.totalWords) / static_cast<double>(metrics.totalSentences)) + 
        GL_WORD_WEIGHT * (static_cast<double>(metrics.totalSyllables) / static_cast<double>(metrics.totalWords)) - GL_BASE;
        
        return metrics;
}
    
std::string ReadabilityAnalyzer::getName() const {
        return "Readability Analyzer";
}

double ReadabilityAnalyzer::countAverageWordLength(const std::string& text) const
{
        auto view = cleanWord(text);
        double wordCounter = 0;
        double totalLength = 0;
        for(const auto& word : view)
        {
                wordCounter++;
                totalLength += std::ranges::distance(word);
        }

        return totalLength / wordCounter;
}

double ReadabilityAnalyzer::countAverageSentenceLength(const std::string& text) const
{
        double sentenceCounter = countSentences(text);
        return text.size() / sentenceCounter;
}

int ReadabilityAnalyzer::countWords(const std::string& text) const
{
        auto view = cleanWord(text);
        return std::ranges::distance(view);
}
    
// Helper function to count syllables in a word
int ReadabilityAnalyzer::countSyllables(const std::string& word) const{
        // Implement syllable counting algorithm
        auto syllable_counts = word 
        | std::views::split(' ') 
        | std::views::transform([this](auto&& rng) {
            std::string_view str(rng);
            return countWordSyllables(str);
          });

        int total_syllables = 0;
        for (int n : syllable_counts) total_syllables += n;
        return total_syllables;
}
    
// Helper function to count sentences in text
int ReadabilityAnalyzer::countSentences(const std::string& text) const{
        auto view = text | std::views::filter([](char c){
                return c == '.' || c == '!' || c == '?';
        });
        
        return std::ranges::distance(view);
}

bool ReadabilityAnalyzer::isVowel(char c) const {
    c = std::tolower(c);
    return std::string_view("aeiouy").contains(c);
}

int ReadabilityAnalyzer::countWordSyllables(std::string_view word) const {
    if (word.empty()) return 0;

    int count = 0;
    bool last_was_vowel = false;

    for (char c : word) {
        bool current_is_vowel = isVowel(c);
        if (current_is_vowel && !last_was_vowel) {
            count++;
        }
        last_was_vowel = current_is_vowel;
    }

    auto size = word.size();
    if (size > 2 && std::tolower(word.back()) == 'e' && !isVowel(word[size - 2])) {
        count--;
    }

    return std::max(count, 1);
}


/**
 * @brief 
 * @class SentimentAnalyzer
*/
SentimentAnalyzer::SentimentAnalyzer() 
        : positiveWords{"good", "handsome", "kind"}
        , negativeWords{"bad", "dirty", "evil"} {
        // Initialize positive and negative word lists
        // This could be from files or hardcoded lists
}
    
std::any SentimentAnalyzer::analyze(const std::string& text) {
        double score = 0.0;
        double positiveCounter = 0;
        double negativeCounter = 0;

        auto view = cleanWord(text);
        for(const auto& word : view)
        {
                if(positiveWords.contains({std::from_range, word}))
                        positiveCounter++;
                if(negativeWords.contains({std::from_range, word}))
                        negativeCounter++;         
        }
        
        if(!negativeCounter)
                return 0.0;

        score = positiveCounter / negativeCounter;
        return score;
}
    
std::string SentimentAnalyzer::getName() const {
        return "Sentiment Analyzer";
}
}