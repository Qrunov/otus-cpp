#include <gtest/gtest.h>
#include "table.h"

using namespace std;

TEST(TestTable, TestSaveLoad)
{
    Table t;
    t.insert(10, "string1");

    auto r = t.select();
    EXPECT_EQ(r.size(), 1);
    if (r.size())
    {
        EXPECT_EQ(r[0].first, 10);
        EXPECT_EQ(r[0].second, "string1");
    }
}

TEST(TestTable, TestTruncate)
{
    Table t;
    t.insert(10, "string1");
    t.truncate();

    auto r = t.select();
    EXPECT_EQ(r.size(), 0);
}

TEST(TestTable, TestIntersect1)
{
    Table t1, t2;
    t1.insert(10, "string1");
    t2.insert(20, "string2");

    auto r = t1.intersect(t2);

    EXPECT_EQ(r.size(), 0);
}

TEST(TestTable, TestIntersect2)
{
    Table t1, t2;
    t1.insert(10, "string1");
    t2.insert(10, "string2");

    auto r = t1.intersect(t2);

    EXPECT_EQ(r.size(), 1);
    if (1 == r.size())
    {
        EXPECT_EQ(get<0>(r[0]), 10);
        EXPECT_EQ(get<1>(r[0]), "string1");
        EXPECT_EQ(get<2>(r[0]), "string2");
    }
}

TEST(TestTable, TestIntersect3)
{
    Table t1, t2;
    t1.insert(10, "string1");
    t2.insert(10, "string2");

    t1.insert(21, "string1");
    t2.insert(2, "string2");

    auto r = t1.intersect(t2);

    EXPECT_EQ(r.size(), 1);
    if (1 == r.size())
    {
        EXPECT_EQ(get<0>(r[0]), 10);
        EXPECT_EQ(get<1>(r[0]), "string1");
        EXPECT_EQ(get<2>(r[0]), "string2");
    }
}

TEST(TestTable, TestIntersect4)
{
    Table t1, t2;
    t1.insert(10, "string1");
    t2.insert(10, "string2");

    t1.insert(2, "string3");
    t2.insert(2, "string4");

    auto r = t1.intersect(t2);

    EXPECT_EQ(r.size(), 2);
    if (2 == r.size())
    {
        EXPECT_EQ(get<0>(r[0]), 2);
        EXPECT_EQ(get<1>(r[0]), "string3");
        EXPECT_EQ(get<2>(r[0]), "string4");

        EXPECT_EQ(get<0>(r[1]), 10);
        EXPECT_EQ(get<1>(r[1]), "string1");
        EXPECT_EQ(get<2>(r[1]), "string2");
    }
}

TEST(TestTable, TestSymmetricDifference1)
{
    Table t1, t2;
    t1.insert(10, "string1");
    t2.insert(10, "string2");

    auto r = t1.symmetric_difference(t2);
    EXPECT_EQ(r.size(), 0);
}

TEST(TestTable, TestSymmetricDifference2)
{
    Table t1, t2;
    t1.insert(10, "string1");
    t2.insert(11, "string2");

    auto r = t1.symmetric_difference(t2);

    EXPECT_EQ(r.size(), 2);
    if (2 == r.size())
    {
        EXPECT_EQ(get<0>(r[0]), 10);
        EXPECT_EQ(get<1>(r[0]), "string1");
        EXPECT_EQ(get<2>(r[0]), "");

        EXPECT_EQ(get<0>(r[1]), 11);
        EXPECT_EQ(get<1>(r[1]), "");
        EXPECT_EQ(get<2>(r[1]), "string2");
    }
}

TEST(TestTable, TestSymmetricDifference3)
{
    Table t1, t2;
    t1.insert(10, "string1");
    t2.insert(10, "string2");

    t1.insert(11, "string11");
    t2.insert(12, "string12");

    auto r = t1.symmetric_difference(t2);

    EXPECT_EQ(r.size(), 2);
    if (2 == r.size())
    {
        EXPECT_EQ(get<0>(r[0]), 11);
        EXPECT_EQ(get<1>(r[0]), "string11");
        EXPECT_EQ(get<2>(r[0]), "");

        EXPECT_EQ(get<0>(r[1]), 12);
        EXPECT_EQ(get<1>(r[1]), "");
        EXPECT_EQ(get<2>(r[1]), "string12");
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}