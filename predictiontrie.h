﻿#ifndef PREDICTIONTRIE_PREDICTIONTRIE_H
#define PREDICTIONTRIE_PREDICTIONTRIE_H

#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class PredictionTrie {
    using MatchedPair = std::tuple<std::string, unsigned int>;

    struct PredictionTrieNode
    {
        enum class Type
        {
            Root,
            Regular,
            Leaf,
        };

        Type type = Type::Regular;
        std::unordered_map<char, PredictionTrieNode*> children;
        unsigned int count = 0;
    };

    PredictionTrieNode* _root;

    const PredictionTrieNode* find(const std::string& word) const;

    void collectAllNodes(
            const std::unordered_map<char, PredictionTrieNode*>& letterLayer,
            std::vector<PredictionTrieNode*>& result);

    void collectAllWordsStartedWith(
            const std::string& wordPart,
            const std::unordered_map<char, PredictionTrieNode*>& letterLayer,
            std::vector<MatchedPair>& result) const;

public:
    PredictionTrie();
    ~PredictionTrie();

    PredictionTrie(const PredictionTrie& other) = delete;
    PredictionTrie(PredictionTrie&& other) = delete;

    void insert(const std::string& word);

    bool isPresented(const std::string& word) const;

    std::vector<MatchedPair> allWordsStartedWith(const std::string& wordBegin) const;
    std::string findBestMatch(const std::string& wordBegin) const;
    std::vector<std::string> findBestMatches(const std::string& wordBegin, unsigned count) const;
};

#endif //PREDICTIONTRIE_PREDICTIONTRIE_H
