#include <fts/sum.h>

#include <gtest/gtest.h>

TEST(TestFtsSum, Subtest1) // NOLINT
{
    double test_a = 4;
    double test_b = 3;
    double res = sum(test_a, test_b);
    ASSERT_EQ(res, 7);
}
