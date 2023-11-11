#include <fts/searcher.hpp>

#include <gtest/gtest.h>

const fts::Json& get_config()
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

TEST(TestSearcher, NormalText)
{
    fts::IndexBuilder builder(get_config());
    const std::string text1 = "The Matrix matrix";
    const size_t document_id1 = 199903;
    builder.add_document(document_id1, text1);

    const std::string text2 = "Hello world";
    const size_t document_id2 = 200305;
    builder.add_document(document_id2, text2);

    const fts::IndexWriter writer(builder.get_index(), "docs");
    writer.write_text();

    const fts::IndexAccessor index_acc("docs", get_config());
    const fts::Result result = fts::search("The Matrix", index_acc);

    ASSERT_EQ(result[0].first, document_id1);
    ASSERT_DOUBLE_EQ(result[0].second, 5.5451774444795623);
}

TEST(TestSearcher, CheckCriticalSituation)
{
    fts::IndexBuilder builder(get_config());
    const std::string text1 = "11:111 11_11 111111";
    const size_t document_id1 = 199903;
    builder.add_document(document_id1, text1);

    const fts::IndexWriter writer(builder.get_index(), "docs");
    writer.write_text();

    const fts::IndexAccessor index_acc("docs", get_config());
    const fts::Result result = fts::search("1111", index_acc);

    ASSERT_EQ(result[0].first, document_id1);
    ASSERT_DOUBLE_EQ(result[0].second, 0);
}

TEST(TestSearcher, CheckThrowException1)
{
    const std::string path;

    EXPECT_THROW(
            const fts::IndexAccessor index_acc(path, get_config()),
            std::invalid_argument);
}

TEST(TestSearcher, CheckThrowException2)
{
    const fts::Json config;

    EXPECT_THROW(
            const fts::IndexAccessor index_acc("docs", config),
            std::invalid_argument);
}

TEST(TestSearcher, CheckThrowException3)
{
    const fts::Json config = {{"ngram_min_length", 8}, {"ngram_max_length", 6}};

    EXPECT_THROW(
            const fts::IndexAccessor index_acc("docs", config),
            std::range_error);
}

TEST(TestSearcher, CheckThrowException4)
{
    const std::string query;
    const fts::IndexAccessor index_acc("docs", get_config());

    EXPECT_THROW(
            const fts::Result result = fts::search(query, index_acc),
            std::invalid_argument);
}
