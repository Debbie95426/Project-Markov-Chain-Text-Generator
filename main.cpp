#include "markov.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    srand(static_cast<unsigned int>(time(0)));

    const int MAX_WORDS = 5000;
    std::string words[MAX_WORDS];
    std::string prefixes[MAX_WORDS];
    std::string suffixes[MAX_WORDS];

    std::string filename;
    int order;
    int maxRequestedWords;

    std::cout << "Enter input filename: ";
    if (!(std::cin >> filename)) return 0;

    std::cout << "Enter order (1, 2, or 3): ";
    if (!(std::cin >> order) || order < 1 || order > 3) {
        std::cout << "Invalid order. Must be 1, 2, or 3." << std::endl;
        return 0;
    }

    std::cout << "Enter maximum number of words to generate: ";
    if (!(std::cin >> maxRequestedWords) || maxRequestedWords < order) {
        std::cout << "Invalid word count. Must be at least equal to order." << std::endl;
        return 0;
    }

    int wordCount = readWordsFromFile(filename, words, MAX_WORDS);
    if (wordCount == -1) {
        std::cout << "Error: Failed to open file " << filename << std::endl;
        return 0;
    }

    if (wordCount <= order) {
        std::cout << "Error: File contains too few words. At least order + 1 training words are needed." << std::endl;
        return 0;
    }

    int chainSize = buildMarkovChain(words, wordCount, order, prefixes, suffixes, MAX_WORDS);
    if (chainSize <= 0) {
        std::cout << "Error: Could not build Markov chain." << std::endl;
        return 0;
    }

    std::string result = generateText(prefixes, suffixes, chainSize, order, maxRequestedWords);

    // Count words in result
    int actualWordCount = 0;
    if (!result.empty()) {
        actualWordCount = 1;
        for (size_t i = 0; i < result.length(); ++i) {
            if (result[i] == ' ') {
                actualWordCount++;
            }
        }
    }
    std::cout << "\nGenerated text:\n" << result << std::endl;
    std::cout << "Generated " << actualWordCount << " of at most " << maxRequestedWords << " words.";
    
    if (actualWordCount < maxRequestedWords) {
        std::cout << " Stopped early: no successor found." << std::endl;
    } else {
        std::cout << std::endl;
    }

    return 0;
}