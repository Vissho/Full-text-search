#include <fts/searcher.hpp>

namespace fts {

    void
    score(const TermInfos& term_infos,
          const double& total_documents,
          Score& score)
    {
        for (const auto& [document_id, positions] :
             term_infos.term_frequencies_) {
            double idf_t = std::log(
                    total_documents / term_infos.document_frequency_);
            double tf_idf_t_d = positions * idf_t;
            score[document_id] += tf_idf_t_d;
        }
    }

    void search(const std::string& query, const IndexAccessor& index)
    {
        Score all_score;
        Ngrams main_ngrams = ngram_parser(query, index.get_config());

        for (const auto& ngram : main_ngrams) {
            for (const auto& term : ngram) {
                TermInfos term_infos = index.get_term_infos(term);
                if (term_infos.term_frequencies_.empty()) {
                    continue;
                }
                score(term_infos, index.total_docs(), all_score);
            }
        }

        Result result(all_score.begin(), all_score.end());

        std::sort(
                result.begin(),
                result.end(),
                [](const auto& score1, const auto& score2) {
                    return score1.second > score2.second;
                });

        std::cout << "Query: " << query << "\n\nid\tscore\ttext\n\n";
        for (const auto& [document_id, score] : result) {
            std::cout << document_id << '\t' << score << '\t'
                      << index.load_document(document_id) << '\n';
        }
        std::cout << std::endl;
    }

    Json IndexAccessor::get_config() const
    {
        return config_;
    }

    TermInfos IndexAccessor::get_term_infos(const std::string& term) const
    {
        TermInfos term_infos;

        const fs::path path = path_ / "index" / "entries" / term_to_hash(term);
        std::ifstream file_term(path);

        if (!file_term.is_open()) {
            return term_infos;
        }

        std::string line;
        if (!std::getline(file_term, line)) {
            return term_infos;
        }

        Words words = str_to_vecstr(line);
        term_infos.document_frequency_ = std::stoi(words[1]);

        size_t item = 2;
        while (item < words.size()) {
            size_t document_id = std::stoi(words[item]);
            size_t positions = std::stoi(words[item + 1]);
            term_infos.term_frequencies_[document_id]
                    = static_cast<double>(positions);
            item = item + 2 + positions;
        }

        return term_infos;
    }

    std::string IndexAccessor::load_document(const size_t& document_id) const
    {
        const fs::path path
                = path_ / "index" / "docs" / std::to_string(document_id);
        std::ifstream file_doc(path);

        if (!file_doc.is_open()) {
            throw std::invalid_argument(
                    "Invalid open file for documents: "
                    + std::to_string(document_id));
        }

        std::string line;
        if (!std::getline(file_doc, line)) {
            throw std::invalid_argument(
                    "Invalid read file for documents: "
                    + std::to_string(document_id));
        }

        return line;
    }

    double IndexAccessor::total_docs() const
    {
        double total_documents = 0;
        const fs::path path = path_;
        std::ifstream file_count(path / "index" / "count_docs");

        if (!file_count.is_open()) {
            throw std::invalid_argument(
                    "Invalid open file for count documents");
        }

        file_count >> total_documents;
        return total_documents;
    }

} // namespace fts
