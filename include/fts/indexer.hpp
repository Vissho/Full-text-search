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
                std::cerr << "\x1B[31mJson:\033[0m " << e.what() << '\n';
                return;
            }

            if (config_["ngram_min_length"] > config_["ngram_max_length"]
                || config_["ngram_min_length"] < 1) {
                throw std::domain_error("Invalid range");
            }
        }
        void add_document(const size_t& document_id, Words& text);
        Index get_index();
        void print_index();
    };

    class IndexWriter {
    private:
        Index index_;
        std::string path_;

    public:
        explicit IndexWriter(Index index) : index_(std::move(index))
        {
            if (index_.documents_.empty() || index_.entries_.empty()) {
                throw std::domain_error("Invalid index");
            }
        }
        explicit IndexWriter(std::string path) : path_(std::move(path))
        {
            if (path_.empty()) {
                throw std::domain_error("Invalid path");
            }
        }
        IndexWriter(Index index, std::string path)
            : index_(std::move(index)), path_(std::move(path))
        {
            if (index_.documents_.empty() || index_.entries_.empty()) {
                throw std::domain_error("Invalid index");
            }
            if (path_.empty()) {
                throw std::domain_error("Invalid path");
            }
        }

        void set_index(const Index& index);
        void set_path(const std::string& path);

        void write_text();
        // void write_binary();
    };

    // class TextIndexWriter : public IndexWriter {
    // public:
    //     void write();
    // };

} // namespace fts
