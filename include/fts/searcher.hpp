#pragma once

#include <fts/indexer.hpp>

namespace fts {

    namespace fs = std::filesystem;

    using Score = std::unordered_map<size_t, double>;
    using Result = std::vector<std::pair<size_t, double>>;

    struct TermInfos {
        std::unordered_map<size_t, double> term_frequencies_;
        double document_frequency_ = 0;
    };

    class IndexAccessor {
    private:
        fs::path path_;
        Json config_;

    public:
        IndexAccessor(fs::path path, Json config)
            : path_(std::move(path)), config_(std::move(config))
        {
            if (path_.empty()) {
                throw std::invalid_argument("\x1B[31mInvalid path\033[0m");
            }
            if (config_.empty() || config_.is_null()) {
                throw std::invalid_argument("\x1B[31mInvalid config\033[0m");
            }
            if (config_["ngram_min_length"] > config_["ngram_max_length"]
                || config_["ngram_min_length"] < 1) {
                throw std::range_error("\x1B[31mInvalid range\033[0m");
            }
        }

        TermInfos get_term_infos(const std::string& term) const;
        std::string load_document(const size_t& document_id) const;
        double total_docs() const;

        Json get_config() const;
    };

    Result search(const std::string& query, const IndexAccessor& index);
    void print_result(
            const std::string& query,
            const IndexAccessor& index,
            const Result& result);

} // namespace fts
