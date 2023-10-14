#pragma once

#include <fts/parser.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

namespace fts {

    using Docs = std::map<size_t, std::string>;
    using Posintext = std::map<size_t, std::vector<size_t>>;
    using Entries = std::map<std::string, Posintext>;

    struct Index {
        Docs documents_;
        Entries entries_;
    };

    class IndexBuilder {
    private:
        Index index_;
        Json config_;

    public:
        IndexBuilder()
        {
            std::ifstream filename("ConfigParser.json");
            try {
                config_ = Json::parse(filename);
            } catch (const std::exception& e) {
                throw std::domain_error("\x1B[31mInvalid config parser\033[0m");
            }

            if (config_["ngram_min_length"] > config_["ngram_max_length"]
                || config_["ngram_min_length"] < 1) {
                throw std::domain_error("\x1B[31mInvalid range\033[0m");
            }
        }
        explicit IndexBuilder(Json config) : config_(std::move(config))
        {
            if (config_.empty() || config_.is_null()) {
                throw std::domain_error("\x1B[31mInvalid config\033[0m");
            }
            if (config_["ngram_min_length"] > config_["ngram_max_length"]
                || config_["ngram_min_length"] < 1) {
                throw std::domain_error("\x1B[31mInvalid range\033[0m");
            }
        }

        void add_document(size_t document_id, const Words& text);
        Index get_index() const;
        void print_index() const;
    };

    class IndexWriter {
    private:
        Index index_;
        std::string path_;

    public:
        IndexWriter(Index index, std::string path)
            : index_(std::move(index)), path_(std::move(path))
        {
            if (index_.documents_.empty() || index_.entries_.empty()) {
                throw std::domain_error("\x1B[31mInvalid index\033[0m");
            }
            if (path_.empty()) {
                throw std::domain_error("\x1B[31mInvalid path\033[0m");
            }
        }

        void write_text();
        // void write_binary();
    };

    // class TextIndexWriter : public IndexWriter {
    // public:
    //     void write();
    // };

} // namespace fts
