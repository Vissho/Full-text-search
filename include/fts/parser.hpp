#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace fts {

    using Json = nlohmann::json;

    using Words = std::vector<std::string>;
    using Ngrams = std::vector<Words>;

    Ngrams ngram_parser(Words& text, const Json& config);

} // namespace fts
