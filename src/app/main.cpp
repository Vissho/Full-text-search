#include <cxxopts.hpp>
#include <fts/parser.hpp>

int main(int argc, char** argv)
{
    cxxopts::Options options("Text");

    options.add_options()("text", "text", cxxopts::value<Words>());

    options.parse_positional({"text"});

    auto result = options.parse(argc, argv);
    auto text = result["text"].as<Words>();
    Words stop_words = {"and", "dr", "mr"};
    const int ngram_min_length = 3;
    const int ngram_max_length = 6;
    int index = 0;

    for (const auto& word : text) {
        std::cout << word << ' ';
    }
    std::cout << "\n";

    Ngrams MainNgrams = NgramParser(text, ngram_min_length, stop_words, ngram_max_length);
    for (const auto& first : MainNgrams) {
        for (const auto& second : first) {
            std::cout << second << ' ' << index << ' ';
        }
        index++;
    }
    std::cout << std::endl;

    return 0;
}
