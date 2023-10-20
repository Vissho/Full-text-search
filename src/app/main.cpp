#include <fts/indexer.hpp>
#include <fts/parser.hpp>

#include <cxxopts.hpp>
#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
    cxxopts::Options options("Text");
    options.add_options()("text", "text", cxxopts::value<fts::Words>())(
            "id", "id", cxxopts::value<size_t>());
    options.parse_positional({"id", "text"});

    try {
        auto result = options.parse(argc, argv);
        const fts::Words text = result["text"].as<fts::Words>();
        const size_t document_id = result["id"].as<size_t>();

        fts::IndexBuilder builder;
        builder.add_document(document_id, text);

        const fts::Words text1 = {"The", "Matrix", "Reloaded"};
        const size_t document_id1 = 200305;
        builder.add_document(document_id1, text1);

        const fts::Words text2 = {"The", "Matrix", "Revolutions"};
        const size_t document_id2 = 200311;
        builder.add_document(document_id2, text2);

        builder.print_index();

        const fts::IndexWriter writer(builder.get_index(), "index.txt");
        writer.write_text();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
