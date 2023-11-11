#include <fts/indexer.hpp>
#include <fts/parser.hpp>
#include <fts/searcher.hpp>

#include <cxxopts.hpp>
#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>

int main(/*int argc, char** argv*/)
{
    try {
        const fts::Json config
                = fts::Json::parse(std::ifstream("ConfigParser.json"));
        fts::IndexBuilder builder(config);

        const std::string text = "The Matrix matrix MATRIX";
        const size_t document_id = 199903;
        builder.add_document(document_id, text);

        const std::string text1 = "The Matrix Reloaded";
        const size_t document_id1 = 200305;
        builder.add_document(document_id1, text1);

        const std::string text2 = "The Matrix Revolutions";
        const size_t document_id2 = 200311;
        builder.add_document(document_id2, text2);

        const std::string text3 = "Hellow World";
        const size_t document_id3 = 100;
        builder.add_document(document_id3, text3);

        const std::string text4 = "Bye World";
        const size_t document_id4 = 101;
        builder.add_document(document_id4, text4);

        const std::string text5 = "Hello Earth";
        const size_t document_id5 = 102;
        builder.add_document(document_id5, text5);

        builder.print_index();

        const fts::IndexWriter writer(builder.get_index(), "docs");
        writer.write_text();

        const fts::IndexAccessor index_acc("docs", config);
        const fts::Result result = fts::search("The Matrix", index_acc);
        fts::print_result("The Matrix", index_acc, result);
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
