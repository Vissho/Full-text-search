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
        std::string str_text = vecstr_to_str(text);
        index_.documents_[document_id] = str_text;
        Ngrams main_ngrams = ngram_parser(text, config_);

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

} // namespace fts
