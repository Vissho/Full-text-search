#include <gtest/gtest.h>
#include <fstream>
#include <fts/parser.hpp>
#include <string>

TEST(TestParser, NormalText)
{
    Words text = {"Dr.", "Jekyll", "and", "Mr.", "Hyde"};
    std::ifstream filename("ConfigParser.json");
    json config = json::parse(filename);

    Ngrams MainNgrams = NgramParser(text, config);
    ASSERT_STREQ(MainNgrams[0][0].c_str(), "jek");
    ASSERT_STREQ(MainNgrams[0][1].c_str(), "jeky");
    ASSERT_STREQ(MainNgrams[0][2].c_str(), "jekyl");
    ASSERT_STREQ(MainNgrams[0][3].c_str(), "jekyll");
    ASSERT_STREQ(MainNgrams[1][0].c_str(), "hyd");
    ASSERT_STREQ(MainNgrams[1][1].c_str(), "hyde");
}

TEST(TestParser, CheckPunctuationCharacter)
{
    Words text = {" ", "!", ".", ",", ":", "s.s.s.s.s.s.s.s.s.s"};
    std::ifstream filename("ConfigParser.json");
    json config = json::parse(filename);

    Ngrams MainNgrams = NgramParser(text, config);
    ASSERT_STREQ(MainNgrams[0][0].c_str(), "sss");
    ASSERT_STREQ(MainNgrams[0][1].c_str(), "ssss");
    ASSERT_STREQ(MainNgrams[0][2].c_str(), "sssss");
    ASSERT_STREQ(MainNgrams[0][3].c_str(), "ssssss");
}

TEST(TestParser, CheckCriticalSituation1)
{
    Words text = {"......................................................"};
    std::ifstream filename("ConfigParser.json");
    json config = json::parse(filename);

    Ngrams MainNgrams = NgramParser(text, config);
    if (MainNgrams.empty()) {
        SUCCEED();
    } else {
        FAIL();
    }
}

TEST(TestParser, CheckCriticalSituation2)
{
    Words text = {"                                                       "};
    std::ifstream filename("ConfigParser.json");
    json config = json::parse(filename);

    Ngrams MainNgrams = NgramParser(text, config);
    if (MainNgrams.empty()) {
        SUCCEED();
    } else {
        FAIL();
    }
}
