#include <fts/indexer.hpp>
#include <fts/parser.hpp>

namespace fts {

    std::string vecstr_to_str(const Words& text)
    {
        std::string str_text;
        for (const auto& word : text) {
            str_text += word;
            str_text += ' ';
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
                if (index_.entries_.find(ngram) != index_.entries_.end()) {
                    posintext.insert(
                            index_.entries_.at(ngram).begin(),
                            index_.entries_.at(ngram).end());
                    index_.entries_.erase(ngram);
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
        for (const auto& pair : index_.documents_) {
            std::cout << pair.first << "\t" << pair.second << '\n';
        }

        std::cout << "\nterm\tentries\n";
        for (const auto& fpair : index_.entries_) {
            if (fpair.first.size() == config_["ngram_min_length"]) {
                std::cout << '\n';
            }

            std::cout << fpair.first << ":\t{";

            for (const auto& spair : fpair.second) {
                std::cout << spair.first << ": [";
                for (const auto& position : spair.second) {
                    std::cout << position;
                    if (position != spair.second.back()) {
                        std::cout << ", ";
                    }
                }

                std::cout << "]";
                if (spair.first != (--fpair.second.end())->first) {
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
        std::ofstream filename(path_);
        if (!(filename.is_open())) {
            throw std::domain_error("Invalid open file");
        }

        for (const auto& fpair : index_.entries_) {
            filename << fpair.first << ' ' << fpair.second.end()->first << ' ';

            for (const auto& spair : fpair.second) {
                filename << spair.first << ' ' << spair.second.size() << ' ';
                for (const auto& position : spair.second) {
                    filename << position << ' ';
                }

                if (spair.first != (--fpair.second.end())->first) {
                    filename << ' ';
                }
            }
            filename << '\n';
        }
    }

} // namespace fts
