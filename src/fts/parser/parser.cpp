#include <fts/parser.hpp>

namespace fts {

    void remove_punctuation(std::string& word)
    {
        word.erase(
                std::remove_if(
                        word.begin(),
                        word.end(),
                        [](unsigned char letter) {
                            return std::ispunct(letter);
                        }),
                word.end());
    }

    void to_lower(std::string& word)
    {
        std::transform(
                word.begin(),
                word.end(),
                word.begin(),
                [](unsigned char letter) { return std::tolower(letter); });
    }

    void remove_stop_words(Words& words, const Json& config)
    {
        const auto& stop_words = config["stop_words"];
        const auto& ngram_min_length = config["ngram_min_length"];
        words.erase(
                std::remove_if(
                        words.begin(),
                        words.end(),
                        [&stop_words,
                         &ngram_min_length](const std::string& word) {
                            return std::find(
                                           stop_words.begin(),
                                           stop_words.end(),
                                           word)
                                    != stop_words.end()
                                    || word.size() < ngram_min_length;
                        }),
                words.end());
    }

    Words str_to_vecstr(const std::string& text)
    {
        Words words;
        std::size_t start = 0;
        std::size_t end = text.find(' ');

        while (end != std::string::npos) {
            std::string word = text.substr(start, end - start);
            words.push_back(word);
            start = end + 1;
            end = text.find(' ', start);
        }

        std::string word = text.substr(start);
        if (!word.empty()) {
            words.push_back(word);
        }
        return words;
    }

    Ngrams ngram_parser(std::string text, const Json& config)
    {
        Ngrams all_ngrams;

        remove_punctuation(text);
        to_lower(text);

        Words words = str_to_vecstr(text);

        remove_stop_words(words, config);

        for (const auto& word : words) {
            Words ngrams;
            for (size_t i = static_cast<size_t>(config["ngram_min_length"]) - 1;
                 i < config["ngram_max_length"] && word[i] != '\0';
                 ++i) {
                std::string ngram = word;
                ngram.erase(i + 1);
                ngrams.push_back(ngram);
            }
            all_ngrams.push_back(ngrams);
        }

        return all_ngrams;
    }

} // namespace fts