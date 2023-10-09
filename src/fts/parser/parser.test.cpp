#include <fts/parser.hpp>

#include <gtest/gtest.h>

const fts::Json& getConfig()
{
    static const fts::Json config
            = {{"stop_words",
                {"a",     "an",   "and",  "are", "as",    "at",   "be",
                 "but",   "by",   "for",  "if",  "in",    "into", "is",
                 "it",    "no",   "not",  "of",  "on",    "or",   "s",
                 "such",  "t",    "that", "the", "their", "then", "there",
                 "these", "they", "this", "to",  "was",   "will", "with"}},
               {"ngram_min_length", 3},
               {"ngram_max_length", 6}};
    return config;
}

TEST(TestParser, NormalText)
{
    fts::Words text = {"Dr.", "Jekyll", "and", "Mr.", "Hyde"};
    const fts::Json& config = getConfig();

    const fts::Ngrams MainNgrams = fts::ngram_parser(text, config);
    ASSERT_STREQ(MainNgrams[0][0].c_str(), "jek");
    ASSERT_STREQ(MainNgrams[0][1].c_str(), "jeky");
    ASSERT_STREQ(MainNgrams[0][2].c_str(), "jekyl");
    ASSERT_STREQ(MainNgrams[0][3].c_str(), "jekyll");
    ASSERT_STREQ(MainNgrams[1][0].c_str(), "hyd");
    ASSERT_STREQ(MainNgrams[1][1].c_str(), "hyde");
}

TEST(TestParser, CheckPunctuationCharacter)
{
    fts::Words text = {" ", "!", ".", ",", ":", "s.s.s.s.s.s.s.s.s.s"};
    const fts::Json& config = getConfig();

    const fts::Ngrams MainNgrams = fts::ngram_parser(text, config);
    ASSERT_STREQ(MainNgrams[0][0].c_str(), "sss");
    ASSERT_STREQ(MainNgrams[0][1].c_str(), "ssss");
    ASSERT_STREQ(MainNgrams[0][2].c_str(), "sssss");
    ASSERT_STREQ(MainNgrams[0][3].c_str(), "ssssss");
}

TEST(TestParser, CheckCriticalSituation1)
{
    fts::Words text
            = {"......................................................"};
    const fts::Json& config = getConfig();

    const fts::Ngrams MainNgrams = fts::ngram_parser(text, config);
    ASSERT_TRUE(MainNgrams.empty());
}

TEST(TestParser, CheckCriticalSituation2)
{
    fts::Words text
            = {"                                                       "};
    const fts::Json& config = getConfig();

    const fts::Ngrams MainNgrams = fts::ngram_parser(text, config);
    ASSERT_TRUE(MainNgrams.empty());
}
