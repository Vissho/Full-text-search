#include <cxxopts.hpp>
#include <fstream>
#include <fts/parser.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

int main(int argc, char** argv)
{
    cxxopts::Options options("Text");
    options.add_options()("text", "text", cxxopts::value<fts::Words>());
    options.parse_positional({"text"});

    auto result = options.parse(argc, argv);
    fts::Words text;
    try {
        text = result["text"].as<fts::Words>();
    } catch (const std::exception& e) {
        std::cerr << "\x1B[31mText no found:\033[0m " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    int index = 0;
    std::ifstream filename("ConfigParser.json");

    fts::Json config;
    try {
        config = fts::Json::parse(filename);
    } catch (const std::exception& e) {
        std::cerr << "\x1B[31mJson:\033[0m " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    for (const auto& word : text) {
        std::cout << word << ' ';
    }
    std::cout << "\n";

    fts::Ngrams MainNgrams;
    try {
        MainNgrams = fts::ngram_parser(text, config);
    } catch (const std::exception& e) {
        std::cerr << "\x1B[31mNgramParser:\033[0m " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    for (const auto& first : MainNgrams) {
        for (const auto& second : first) {
            std::cout << second << ' ' << index << ' ';
        }
        index++;
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
