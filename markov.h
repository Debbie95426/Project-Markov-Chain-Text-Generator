#ifndef MARKOV_H
#define MARKOV_H

#include <string>

std::string joinWords(const std::string words[], int start, int end);
int readWordsFromFile(const std::string& filename, std::string words[], int maxWords);
int buildMarkovChain(const std::string words[], int wordCount, int order, std::string prefixes[], std::string suffixes[], int maxChainSize);
std::string getRandomSuffix(const std::string prefixes[], const std::string suffixes[], int chainSize, const std::string& prefix);
std::string getRandomPrefix(const std::string prefixes[], int chainSize);
std::string generateText(const std::string prefixes[], const std::string suffixes[], int chainSize, int order, int numWords);

#endif