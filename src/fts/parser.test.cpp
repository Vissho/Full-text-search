#include <fts/parser.hpp>

#include <gtest/gtest.h>

TEST(TestFtsSum, Subtest1) // NOLINT
{
    const double test_a = 4;
    const double test_b = 3;
    const double res = test_a + test_b;
    ASSERT_EQ(res, 7);
}
