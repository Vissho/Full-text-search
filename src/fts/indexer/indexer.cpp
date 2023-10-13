#include <fts/indexer.hpp>
#include <fts/parser.hpp>

namespace fts {

    std::string vecstr_to_str(const Words& text)
    {
        std::string str_text;
        for (const auto& word : text) {
            str_text += word + ' ';
        }
        str_text[str_text.size() - 1] = '\0';
        return str_text;
    }

    void IndexBuilder::add_document(const size_t& document_id, Words& text)
    {
        index_.documents_[document_id] = vecstr_to_str(text);
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

    Index IndexBuilder::get_index()
    {
        return index_;
    }

    void IndexBuilder::print_index()
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

    void IndexWriter::set_index(const Index& index)
    {
        index_ = index;
    }

    void IndexWriter::set_path(const std::string& path)
    {
        path_ = path;
    }

    void IndexWriter::write_text()
    {
        std::ofstream file(path_);
        if (!(file.is_open())) {
            throw std::domain_error("Invalid open file");
        }

        for (const auto& entry : index_.entries_) {
            file << entry.first << ' ' << entry.second.end()->first << ' ';

            for (const auto& subentry : entry.second) {
                file << subentry.first << ' ' << subentry.second.size() << ' ';
                for (const auto& position : subentry.second) {
                    file << position;
                    if (position != subentry.second.back()) {
                        file << ' ';
                    }
                }

                if (subentry.first != (--entry.second.end())->first) {
                    file << ' ';
                }
            }
            file << '\n';
        }
    }

} // namespace fts
