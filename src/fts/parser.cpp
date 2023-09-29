#include <cstring>
#include <fts/parser.hpp>

Ngrams NgramParser(
        Words& text,
        size_t ngram_min_length,
        const Words& stop_words,
        size_t ngram_max_length)
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
        for (const auto& stop_word : stop_words) {
            if (word == stop_word) {
                flag = 1;
            }
        }
        if (flag == 0 && word.size() >= ngram_min_length) {
            Words temp;
            for (std::size_t i = ngram_min_length - 1;
                 i < ngram_max_length && word[i] != '\0';
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
