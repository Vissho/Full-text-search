#include <fts/indexer.hpp>
#include <fts/parser.hpp>

#include <picosha2.h>

namespace fts {

    void IndexBuilder::add_document(size_t document_id, const std::string& text)
    {
        index_.documents_[document_id] = text;
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
            file_docs.close();
        }
    }

    void write_entries(const fs::path& entries_path, const Entries& entries)
    {
        for (const auto& [term, pos_in_text] : entries) {
            std::string term_to_hash
                    = picosha2::hash256_hex_string(term).substr(0, 6);

            std::ofstream file_ent(entries_path / term_to_hash);

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

            file_ent.close();
        }
    }

    void IndexWriter::write_text() const
    {
        fs::path path = path_;
        fs::path main_path = path / "index";
        fs::path docs_path = main_path / "docs";
        fs::path entries_path = main_path / "entries";

        create_directory(path);
        create_directory(main_path);
        create_directory(docs_path);
        create_directory(entries_path);

        write_docs(docs_path, index_.documents_);
        write_entries(entries_path, index_.entries_);
    }

} // namespace fts
