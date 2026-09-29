#include "markov.h"
#include <fstream>
#include <cstdlib>
#include <iostream>

// Function 1: Join words from start to end (exclusive) with spaces
std::string joinWords(const std::string words[], int start, int end) {
    std::string result = "";
    for (int i = start; i < end; ++i) {
        result += words[i];
        if (i < end - 1) {
            result += " ";
        }
    }
    return result;
}

// Function 2: Read words from a file into an array
int readWordsFromFile(const std::string& filename, std::string words[], int maxWords) {
    std::ifstream inputFile(filename.c_str());
    if (!inputFile.is_open()) {
        return -1;
    }
    int count = 0;
    std::string word;
    while (inputFile >> word && count < maxWords) {
        words[count++] = word;
    }
    return count;
}

// Function 3: Build Markov Chain prefixes and suffixes
int buildMarkovChain(const std::string words[], int wordCount, int order, std::string prefixes[], std::string suffixes[], int maxChainSize) {
    if (wordCount <= order) {
        return 0;
    }
    int chainSize = 0;
    for (int i = 0; i <= wordCount - order - 1; ++i) {
        if (chainSize >= maxChainSize) {
            break;
        }
        prefixes[chainSize] = joinWords(words, i, i + order);
        suffixes[chainSize] = words[i + order];
        chainSize++;
    }
    return chainSize;
}

// Function 4: Get a random suffix for a given prefix
std::string getRandomSuffix(const std::string prefixes[], const std::string suffixes[], int chainSize, const std::string& prefix) {
    // Collect all matching suffixes
    std::string matches[5000];
    int matchCount = 0;
    for (int i = 0; i < chainSize; ++i) {
        if (prefixes[i] == prefix && matchCount < 5000) {
            matches[matchCount++] = suffixes[i];
        }
    }
    if (matchCount == 0) {
        return "";
    }
    int randomIndex = rand() % matchCount;
    return matches[randomIndex];
}

// Function 5: Get a random prefix from the chain
std::string getRandomPrefix(const std::string prefixes[], int chainSize) {
    if (chainSize <= 0) {
        return "";
    }
    int randomIndex = rand() % chainSize;
    return prefixes[randomIndex];
}

// Function 6: Generate text using the Markov chain
std::string generateText(const std::string prefixes[], const std::string suffixes[], int chainSize, int order, int numWords) {
    if (chainSize <= 0 || numWords <= 0) {
        return "";
    }

    std::string currentPrefix = getRandomPrefix(prefixes, chainSize);
    if (currentPrefix == "") {
        return "";
    }

    std::string generated = currentPrefix;
    int count = order;

    // Split the current prefix into words to track the rolling state (for order > 1)
    // A simple robust approach is to keep track of words array or parse the currentPrefix.
    // For simplicity, we can reconstruct the rolling context by maintaining the last 'order' words.
    std::string rollingWords[100];
    // Populate initial rolling words from currentPrefix by splitting spaces
    int wIdx = 0;
    std::string tempWord = "";
    for (size_t i = 0; i < currentPrefix.length(); ++i) {
        if (currentPrefix[i] == ' ') {
            rollingWords[wIdx++] = tempWord;
            tempWord = "";
        } else {
            tempWord += currentPrefix[i];
        }
    }
    rollingWords[wIdx++] = tempWord;

    while (count < numWords) {
        std::string nextSuffix = getRandomSuffix(prefixes, suffixes, chainSize, currentPrefix);
        if (nextSuffix == "") {
            break; // Dead end
        }
        generated += " " + nextSuffix;
        count++;
        // Shift rolling words and update currentPrefix
        for (int i = 0; i < order - 1; ++i) {
            rollingWords[i] = rollingWords[i + 1];
        }
        rollingWords[order - 1] = nextSuffix;

        // Rebuild currentPrefix
        currentPrefix = "";
        for (int i = 0; i < order; ++i) {
            currentPrefix += rollingWords[i];
            if (i < order - 1) {
                currentPrefix += " ";
            }
        }
    }

    return generated;
}