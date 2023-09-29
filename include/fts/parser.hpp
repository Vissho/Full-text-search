#pragma once
#include <iostream>
#include <string>
#include <vector>

using Words = std::vector<std::string>;
using Ngrams = std::vector<Words>;

Ngrams NgramParser(
        Words& text,
        size_t ngram_min_length,
        const Words& stop_words,
        size_t ngram_max_length);
