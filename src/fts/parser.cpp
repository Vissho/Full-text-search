#include <cstring>
#include <fts/parser.hpp>

Ngrams NgramParser(Words& text, json config)
{
    Ngrams MainNgrams;
    for (auto& word : text) {
        for (size_t i = 0; word[i] != '\0'; ++i) {
            if (std::ispunct(word[i]) != 0) {
                word.erase(i, 1);
            }
            word[i] = static_cast<char>(tolower(word[i]));
        }
    }

    for (const auto& word : text) {
        int flag = 0;
        for (const auto& stop_word : config["stop_words"]) {
            if (word == stop_word) {
                flag = 1;
            }
        }
        if (flag == 0 && word.size() >= config["ngram_min_length"]) {
            Words temp;
            for (size_t i = static_cast<size_t>(config["ngram_min_length"]) - 1;
                 i < config["ngram_max_length"] && word[i] != '\0';
                 ++i) {
                std::string ngram = word;
                ngram.erase(i + 1);
                temp.push_back(ngram);
            }
            MainNgrams.push_back(temp);
        }
    }

    return MainNgrams;
}
