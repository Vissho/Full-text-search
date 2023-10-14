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
    const fts::Words text1 = {"The", "Matrix", "matrix"};
    const size_t document_id1 = 199903;
    builder.add_document(document_id1, text1);

    const fts::Words text2 = {"The", "Matrix", "Reloaded"};
    const size_t document_id2 = 200305;
    builder.add_document(document_id2, text2);

    const fts::IndexWriter writer(builder.get_index(), "test1.txt");
    writer.write_text();

    std::string line;
    fts::Words lines;
    std::ifstream file("test1.txt");
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    ASSERT_STREQ(lines[0].c_str(), "mat 2 199903 2 0 1 200305 1 0");
    ASSERT_STREQ(lines[1].c_str(), "matr 2 199903 2 0 1 200305 1 0");
    ASSERT_STREQ(lines[2].c_str(), "matri 2 199903 2 0 1 200305 1 0");
    ASSERT_STREQ(lines[3].c_str(), "matrix 2 199903 2 0 1 200305 1 0");
    ASSERT_STREQ(lines[4].c_str(), "rel 1 200305 1 1");
    ASSERT_STREQ(lines[5].c_str(), "relo 1 200305 1 1");
}

TEST(TestIndexer, CheckCriticalSituation)
{
    fts::IndexBuilder builder(getConfig());
    const fts::Words text1 = {"11:111", "11_11", "111111"};
    const size_t document_id1 = 199903;
    builder.add_document(document_id1, text1);

    const fts::IndexWriter writer(builder.get_index(), "test2.txt");
    writer.write_text();

    std::string line;
    fts::Words lines;
    std::ifstream file("test2.txt");
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    ASSERT_STREQ(lines[0].c_str(), "111 1 199903 3 0 1 2");
    ASSERT_STREQ(lines[1].c_str(), "1111 1 199903 3 0 1 2");
    ASSERT_STREQ(lines[2].c_str(), "11111 1 199903 2 0 2");
    ASSERT_STREQ(lines[3].c_str(), "111111 1 199903 1 2");
}

TEST(TestIndexer, CheckThrowException1)
{
    const fts::Json config;

    EXPECT_THROW(const fts::IndexBuilder builder(config), std::domain_error);
}

TEST(TestIndexer, CheckThrowException2)
{
    const fts::Json config = {{"ngram_min_length", 8}, {"ngram_max_length", 6}};

    EXPECT_THROW(const fts::IndexBuilder builder(config), std::domain_error);
}

TEST(TestIndexer, CheckThrowException3)
{
    fts::IndexBuilder builder(getConfig());
    const fts::Words text = {""};
    const size_t document_id = 199903;
    builder.add_document(document_id, text);

    EXPECT_THROW(
            const fts::IndexWriter writer(builder.get_index(), "test3.txt"),
            std::domain_error);
}

TEST(TestIndexer, CheckThrowException4)
{
    fts::IndexBuilder builder(getConfig());
    const fts::Words text = {"The", "Matrix", "matrix"};
    const size_t document_id = 199903;
    builder.add_document(document_id, text);

    EXPECT_THROW(
            const fts::IndexWriter writer(builder.get_index(), ""),
            std::domain_error);
}
