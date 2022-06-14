#include "predictiontrie.h"

#include <algorithm>
#include <fstream>
#include <map>


PredictionTrie::PredictionTrie() {
    _root = new PredictionTrie::PredictionTrieNode;
    _root->count = 0u;
    _root->type = PredictionTrie::PredictionTrieNode::Type::Root;
    std::string line;
    std::ifstream in("..\\PredictionTrie\\words.txt");
    if (in.is_open()) {
        while (getline(in, line)) {
            insert(line);
        }
    }
    else {
        throw std::out_of_range("File not found!");
    }
}


PredictionTrie::~PredictionTrie() {
    std::vector<PredictionTrieNode*> result = {_root};
    collectAllNodes(_root->children, result);
    for (auto&& node: result) {
        delete node;
    }
}


void PredictionTrie::insert(const std::string& word) {
    auto* current = _root;
    for (auto letter : word) {
        auto foundIt = current->children.find(letter);
        if (foundIt == current->children.end()) {
            auto [it, _] = current->children.emplace(letter, new PredictionTrieNode);
            foundIt = it;
        }
        current = foundIt->second;
    }

    current->type = PredictionTrie::PredictionTrieNode::Type::Leaf;
    current->count += 1u;
}

void PredictionTrie::remove(const std::string& word) {
    PredictionTrieNode* found = find(word);
    found->type = PredictionTrie::PredictionTrieNode::Type::Regular;
    found->count = 0u;
}

PredictionTrie::PredictionTrieNode* PredictionTrie::find(const std::string& word) const {
    auto* current = _root;
    for (auto letter : word) {
        auto foundIt = current->children.find(letter);
        if (foundIt == current->children.end()) {
            return nullptr;
        }
        current = foundIt->second;
    }
    return current;
}

void PredictionTrie::collectAllNodes(
        const std::unordered_map<char, PredictionTrieNode*>& letterLayer,
        std::vector<PredictionTrieNode*>& result) {
    for (auto&& [letter, node] : letterLayer) {
        result.emplace_back(node);
        collectAllNodes(node->children, result);
    }
}

bool PredictionTrie::isPresented(const std::string& word) const {
    auto* found = find(word);
    return found && found->type == PredictionTrie::PredictionTrieNode::Type::Leaf;
}

std::vector<PredictionTrie::MatchedPair> PredictionTrie::allWordsStartedWith(const std::string& wordBegin) const {
    auto* found = find(wordBegin);
    if (!found) {
        return {};
    }

    std::vector<MatchedPair> result;
    collectAllWordsStartedWith(wordBegin, found->children, result);

    return result;
}

void PredictionTrie::collectAllWordsStartedWith(
        const std::string& wordPart,
        const std::unordered_map<char, PredictionTrieNode*>& letterLayer,
        std::vector<PredictionTrie::MatchedPair>& result) const {
    for (auto&& [letter, node] : letterLayer) {
        std::string word = wordPart + letter;
        if (node->type == PredictionTrieNode::Type::Leaf) {
            result.emplace_back(word, node->count);
        }
        collectAllWordsStartedWith(word, node->children, result);
    }
}

std::string PredictionTrie::findBestMatch(const std::string& wordBegin) const {
    return findBestMatches(wordBegin, 1).front();
}

std::vector<std::string> PredictionTrie::findBestMatches(const std::string& wordBegin, unsigned count) const {
    auto allWords = allWordsStartedWith(wordBegin);
    count = allWords.size() < count ? allWords.size() : count;
    std::partial_sort(
            std::begin(allWords),
            allWords.begin() + count,
            std::end(allWords),
            [](const auto& first, const auto& second) {
                return std::get<1>(first) > std::get<1>(second);
            }
    );
    std::vector<std::string> result;
    for (int i = 0; i < count; ++i) {
        result.push_back(std::get<0>(allWords[i]));
    }
    return result;
}
