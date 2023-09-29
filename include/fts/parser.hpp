#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

using Words = std::vector<std::string>;
using Ngrams = std::vector<Words>;

Ngrams NgramParser(Words& text, json config);
