#include <fts/sum.h>

#include <gtest/gtest.h>

TEST(TestFtsSum, Subtest1) // NOLINT
{
    const double test_a = 4;
    const double test_b = 3;
    const double res = sum(test_a, test_b);
    ASSERT_EQ(res, 7);
}