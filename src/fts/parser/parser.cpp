#include <fts/parser.hpp>
#include <string>

Ngrams NgramParser(Words& text, const json& config)
{
    Ngrams AllNgrams;
    for (auto& word : text) {
        word.erase(
                std::remove_if(
                        word.begin(),
                        word.end(),
                        [](unsigned char letter) {
                            return std::ispunct(letter);
                        }),
                word.end());
        word.erase(
                std::remove_if(
                        word.begin(),
                        word.end(),
                        [](unsigned char letter) {
                            return std::isspace(letter);
                        }),
                word.end());
        std::transform(
                word.begin(),
                word.end(),
                word.begin(),
                [](unsigned char letter) { return std::tolower(letter); });
    }

    for (const auto& word : text) {
        const auto& stop_words = config["stop_words"];
        if (std::find(stop_words.begin(), stop_words.end(), word)
                    == stop_words.end()
            && word.size() >= config["ngram_min_length"]) {
            Words Ngrams;
            for (size_t i = static_cast<size_t>(config["ngram_min_length"]) - 1;
                 i < config["ngram_max_length"] && word[i] != '\0';
                 ++i) {
                std::string ngram = word;
                ngram.erase(i + 1);
                Ngrams.push_back(ngram);
            }
            AllNgrams.push_back(Ngrams);
        }
    }

    return AllNgrams;
}
