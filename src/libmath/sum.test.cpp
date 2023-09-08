#include <gmock/gmock.h>
#include <gtest/gtest.h>
#define FMT_HEADER_ONLY
#include <core.h>
#include <fts/sum.h>

TEST(TestFtsSum, Subtest1)
{
    char** TEST1 = {"bin", "-f", "2"};
    ASSERT_EQ(sum(3, TEST1), -1);
}

// TEST(TestFtsSum, Subtest2)
// {
//     char** TEST2 = {"bin", "-f", "2", "-s", "3.3"};
//     ASSERT_EQ(sum(5, TEST2), 0);
// }

// TEST(TestFtsSum, Subtest3)
// {
//     char** TEST3 = {"bin", "-h"};
//     ASSERT_EQ(sum(2, TEST3), 0);
// }

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
