#include <fts/indexer.hpp>
#include <fts/parser.hpp>

#include <cxxopts.hpp>
#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>

int main(/*int argc, char** argv*/)
{
    try {
        fts::IndexBuilder builder;

        const std::string text = "The Matrix";
        const size_t document_id = 199903;
        builder.add_document(document_id, text);

        const std::string text1 = "The Matrix Reloaded";
        const size_t document_id1 = 200305;
        builder.add_document(document_id1, text1);

        const std::string text2 = "The Matrix Revolutions";
        const size_t document_id2 = 200311;
        builder.add_document(document_id2, text2);

        builder.print_index();

        const fts::IndexWriter writer(builder.get_index(), "docs");
        writer.write_text();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
