#include <fts/searcher.hpp>

#include <cxxopts.hpp>

#include <iostream>

int main(int argc, char** argv)
{
    cxxopts::Options options(
            "IndexCommand", "The program indexes the csv file");

    options.add_options()(
            "index", "Path to the index file", cxxopts::value<std::string>())(
            "query",
            "Search query in the index file",
            cxxopts::value<std::string>())("h,help", "Print usage");

    cxxopts::ParseResult result;
    try {
        result = options.parse(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    if (result.count("help") != 0) {
        std::cout << options.help() << std::endl;
        return EXIT_SUCCESS;
    }

    if (result.count("query") == 0 || result.count("index") == 0) {
        std::cout << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    const std::string query = result["query"].as<std::string>();
    const std::string index_name = result["index"].as<std::string>();

    if (query.empty() || index_name.empty()) {
        std::cout << "Invalid name for index or query" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        const fts::Json config
                = fts::Json::parse(std::ifstream("ConfigParser.json"));
        const fts::IndexAccessor index_acc(index_name, config);
        const fts::Result result = fts::search(query, index_acc);
        fts::print_result(query, index_acc, result);

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}