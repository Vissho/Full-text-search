#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace fts {

    using Json = nlohmann::json;

    using Words = std::vector<std::string>;
    using Ngrams = std::vector<Words>;

    Words str_to_vecstr(const std::string& text);
    Ngrams ngram_parser(std::string text, const Json& config);

} // namespace fts
