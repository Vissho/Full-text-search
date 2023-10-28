#include <fts/indexer.hpp>

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

TEST(TestIndexer, NormalText)
{
    fts::IndexBuilder builder(getConfig());
    const std::string text1 = "The Matrix matrix";
    const size_t document_id1 = 199903;
    builder.add_document(document_id1, text1);

    const std::string text2 = "The Matrix Reloaded";
    const size_t document_id2 = 200305;
    builder.add_document(document_id2, text2);

    ASSERT_STREQ(
            builder.get_index().documents_.find(199903)->second.c_str(),
            "The Matrix matrix");
    ASSERT_STREQ(
            builder.get_index().documents_.find(200305)->second.c_str(),
            "The Matrix Reloaded");
    ASSERT_EQ(
            builder.get_index()
                    .entries_.find("mat")
                    ->second.find(199903)
                    ->second[0],
            0);
    ASSERT_EQ(
            builder.get_index()
                    .entries_.find("rel")
                    ->second.find(200305)
                    ->second[0],
            1);
}

TEST(TestIndexer, CheckCriticalSituation)
{
    fts::IndexBuilder builder(getConfig());
    const std::string text1 = "11:111 11_11 111111";
    const size_t document_id1 = 199903;
    builder.add_document(document_id1, text1);

    ASSERT_STREQ(
            builder.get_index().documents_.find(199903)->second.c_str(),
            "11:111 11_11 111111");
    ASSERT_EQ(
            builder.get_index()
                    .entries_.find("111")
                    ->second.find(199903)
                    ->second[0],
            0);
}

TEST(TestIndexer, CheckThrowException1)
{
    const fts::Json config;

    EXPECT_THROW(
            const fts::IndexBuilder builder(config), std::invalid_argument);
}

TEST(TestIndexer, CheckThrowException2)
{
    const fts::Json config = {{"ngram_min_length", 8}, {"ngram_max_length", 6}};

    EXPECT_THROW(const fts::IndexBuilder builder(config), std::range_error);
}

TEST(TestIndexer, CheckThrowException3)
{
    fts::IndexBuilder builder(getConfig());
    const std::string text;
    const size_t document_id = 199903;
    builder.add_document(document_id, text);

    EXPECT_THROW(
            const fts::IndexWriter writer(builder.get_index(), "test3"),
            std::invalid_argument);
}

TEST(TestIndexer, CheckThrowException4)
{
    fts::IndexBuilder builder(getConfig());
    const std::string text = "The Matrix matrix";
    const size_t document_id = 199903;
    builder.add_document(document_id, text);

    EXPECT_THROW(
            const fts::IndexWriter writer(builder.get_index(), ""),
            std::invalid_argument);
}
