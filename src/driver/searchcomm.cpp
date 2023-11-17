#include <fts/searcher.hpp>

#include <cxxopts.hpp>
#include <replxx.hxx>

#include <iostream>

int main(int argc, char** argv)
{
    cxxopts::Options options(
            "SearchCommand",
            "The program searches for a query in a index file");

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

    if (result.count("index") == 0) {
        std::cout << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    const std::string index_name = result["index"].as<std::string>();

    if (index_name.empty()) {
        std::cout << "Invalid name for index" << std::endl;
        return EXIT_FAILURE;
    }

    if (result.count("query") == 0) {
        try {
            const fts::fs::path temp_dir = fts::fs::temp_directory_path();
            const fts::fs::path hist_path = temp_dir / "replxx.hist";
            replxx::Replxx repl;
            repl.history_save(hist_path);

            while (true) {
                const std::string query = repl.input("> ");
                repl.clear_screen();
                if (query.empty()) {
                    break;
                }
                const fts::Json config
                        = fts::Json::parse(std::ifstream("ConfigParser.json"));
                const fts::IndexAccessor index_acc(index_name, config);
                const fts::Result result = fts::search(query, index_acc);
                fts::print_result(query, index_acc, result);
                repl.history_add(query);
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
            return EXIT_FAILURE;
        }
    } else {
        std::cout << "\033[2J\033[1;1H";
        const std::string query = result["query"].as<std::string>();
        if (query.empty()) {
            std::cout << "Invalid name for query" << std::endl;
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
    }

    return EXIT_SUCCESS;
}