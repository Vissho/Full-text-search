#pragma once
#include <fts/parser.hpp>
#include <iostream>
#include <map>
#include <vector>

using docs = std::map<size_t, std::string>;
using posintext = std::map<size_t, std::vector<size_t>>;
using entries = std::map<std::string, posintext>;

namespace fts {

    struct Index {
        docs Documents;
        entries Entries;
    };

    struct IndexBuilder {
        void add_document(const int& document_id, const std::string& text);
    };

    // class IndexWriter {};

    // class TextIndexWriter {};

} // namespace fts
