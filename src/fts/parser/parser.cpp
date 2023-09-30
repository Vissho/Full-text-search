#include <cstring>
#include <fts/parser.hpp>

Ngrams NgramParser(Words& text, const json& config)
{
    Ngrams MainNgrams;
    for (auto& word : text) {
        word.erase(std::remove_if(word.begin(),
                              word.end(),
                              [](unsigned char letter) { return std::ispunct(letter); }),
               word.end());
        word.erase(std::remove_if(word.begin(),
                              word.end(),
                              [](unsigned char letter) { return std::isspace(letter); }),
               word.end());
	std::transform(word.begin(), word.end(), word.begin(),
                   [](unsigned char letter){ return std::tolower(letter); });
    }

    for (const auto& word : text) {
	// word - провереям, является ли стоп-словом
	// config["stop_words"]
	const auto& stop_words = config["stop_words"];
	auto it = std::find(stop_words.begin(), stop_words.end(), word);
	if (it == stop_words.end()) { 
		// не нашли стоп слово
	}
//[&config](unsigned char word){ for (const auto& stop_word : config["stop_words"]) {if (word == stop_word) {return word; }}}
        if (std::find_if(word.begin(), word.end(), ) && word.size() >= config["ngram_min_length"]) {
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
