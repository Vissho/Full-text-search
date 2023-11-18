#include <fts/indexer.hpp>

#include <cxxopts.hpp>
#include <rapidcsv.h>

#include <iostream>

int main(int argc, char** argv)
{
    cxxopts::Options options(
            "IndexCommand", "The program indexes the csv file");

    options.add_options()(
            "csv", "Path to the csv file", cxxopts::value<std::string>())(
            "index", "Path to the index file", cxxopts::value<std::string>())(
            "h,help", "Print usage");

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

    if (result.count("csv") == 0 || result.count("index") == 0) {
        std::cout << options.help() << std::endl;
        return EXIT_FAILURE;
    }

    const std::string csv_name = result["csv"].as<std::string>();
    const std::string index_name = result["index"].as<std::string>();

    if (csv_name.empty() || index_name.empty()) {
        std::cout << "Invalid name for csv or index" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<size_t> document_ids;
    std::vector<std::string> titles;

    try {
        const rapidcsv::Document doc(csv_name);
        document_ids = doc.GetColumn<size_t>("bookID");
        titles = doc.GetColumn<std::string>("title");
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\nNo such name for csv\n";
        return EXIT_FAILURE;
    }

    if (document_ids.empty() || titles.empty()) {
        std::cout << "Document IDs or titles are empty" << std::endl;
        return EXIT_FAILURE;
    }

    if (document_ids.size() != titles.size()) {
        std::cout << "Document IDs and titles have different sizes"
                  << std::endl;
        return EXIT_FAILURE;
    }

    try {
        const fts::Json config
                = fts::Json::parse(std::ifstream("ConfigParser.json"));
        fts::IndexBuilder builder(config);
        for (size_t i = 0; i < document_ids.size(); i++) {
            builder.add_document(document_ids[i], titles[i]);
        }
        const fts::IndexWriter writer(builder.get_index(), index_name);
        writer.write_text();

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}