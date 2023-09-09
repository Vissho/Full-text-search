#include <gmock/gmock.h>
#include <gtest/gtest.h>
#define FMT_HEADER_ONLY
#include <core.h>
#include <fts/sum.h>

TEST(TestFtsSum, Subtest1) // NOLINT
{
    double res = sum(3, 4.4);
    ASSERT_EQ(res, 7.4);
}

TEST(TestFtsSum, Subtest2) // NOLINT
{
    double res = sum(0, 0);
    ASSERT_EQ(res, 0);
}

TEST(TestFtsSum, Subtest3) // NOLINT
{
    double res = sum(-3, -4.4);
    ASSERT_EQ(res, -7.4);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
