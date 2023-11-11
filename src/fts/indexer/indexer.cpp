#include <fts/indexer.hpp>
#include <fts/parser.hpp>

#include <picosha2.h>

namespace fts {

    void IndexBuilder::add_document(size_t document_id, const std::string& text)
    {
        index_.documents_[document_id] = text;
        index_.count_docs_++;
        const Ngrams main_ngrams = ngram_parser(text, config_);

        size_t position = 0;
        for (const auto& ngrams : main_ngrams) {
            for (const auto& ngram : ngrams) {
                Posintext posintext;
                auto iter = index_.entries_.find(ngram);
                if (iter != index_.entries_.end()) {
                    posintext.insert(iter->second.begin(), iter->second.end());
                    index_.entries_.erase(iter);
                }
                posintext[document_id].push_back(position);
                index_.entries_.insert({ngram, posintext});
            }
            position++;
        }
    }

    Index IndexBuilder::get_index() const
    {
        return index_;
    }

    void IndexBuilder::print_index() const
    {
        std::cout << "\nid\ttext\n\n";
        for (const auto& document : index_.documents_) {
            std::cout << document.first << "\t" << document.second << '\n';
        }

        std::cout << "\nterm\tentries\n";
        for (const auto& entry : index_.entries_) {
            if (entry.first.size() == config_["ngram_min_length"]) {
                std::cout << '\n';
            }

            std::cout << entry.first << ":\t{";

            for (const auto& subentry : entry.second) {
                std::cout << subentry.first << ": [";
                for (const auto& position : subentry.second) {
                    std::cout << position;
                    if (position != subentry.second.back()) {
                        std::cout << ", ";
                    }
                }

                std::cout << "]";
                if (subentry.first != (--entry.second.end())->first) {
                    std::cout << ", ";
                }
            }
            std::cout << "}\n";
        }
        std::cout << std::endl;
    }

    std::string term_to_hash(const std::string& term)
    {
        return picosha2::hash256_hex_string(term).substr(0, 6);
    }

    void create_directory(const std::string& path)
    {
        if (!fs::exists(path)) {
            if (!fs::create_directories(path)) {
                throw std::invalid_argument(
                        "Invalid create a directory: " + path);
            }
        }
    }

    void write_docs(const fs::path& docs_path, const Docs& docs)
    {
        for (const auto& [document_id, text] : docs) {
            std::ofstream file_docs(docs_path / std::to_string(document_id));

            if (!file_docs.is_open()) {
                throw std::invalid_argument(
                        "Invalid open file for docs: " + text);
            }

            file_docs << text;
        }
    }

    void write_entries(const fs::path& entries_path, const Entries& entries)
    {
        for (const auto& [term, pos_in_text] : entries) {
            const std::string term_hash = term_to_hash(term);

            std::ofstream file_ent(entries_path / term_hash, std::ios::app);

            if (!file_ent.is_open()) {
                throw std::invalid_argument(
                        "Invalid open file for docs: " + term);
            }

            file_ent << term << ' ' << pos_in_text.size();
            for (const auto& [document_id, positions] : pos_in_text) {
                file_ent << ' ' << document_id << ' ' << positions.size();
                for (const auto& position : positions) {
                    file_ent << ' ' << position;
                }
            }
            file_ent << '\n';
        }
    }

    void write_count_docs(const fs::path& main_path, const size_t& count_docs)
    {
        std::ofstream file_count(main_path / "count_docs");

        if (!file_count.is_open()) {
            throw std::invalid_argument(
                    "Invalid open file for count documents");
        }

        file_count << count_docs;
    }

    void IndexWriter::write_text() const
    {
        const fs::path main_path = path_ / "index";
        const fs::path docs_path = main_path / "docs";
        const fs::path entries_path = main_path / "entries";

        create_directory(path_);
        create_directory(main_path);
        create_directory(docs_path);
        create_directory(entries_path);

        write_docs(docs_path, index_.documents_);
        write_entries(entries_path, index_.entries_);
        write_count_docs(main_path, index_.count_docs_);
    }

} // namespace fts
