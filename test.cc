#include <gtest/gtest.h>
#include "fileRepresentation.h"
// #include "mocRecursiveDirectoryIterator.h"
using namespace std;

class TestFiles : public ::testing::Test
{
protected:
    void SetUp()
    {
    }
    void TearDown()
    {
    }
};

using fm = class fileRepresentation<MockMapping, MockRegion>;

TEST_F(TestFiles, Test1)
{
    fm file1("/fake1", 100);
    EXPECT_EQ(file1.size(), 100);
}

TEST_F(TestFiles, Test2)
{
    fm file1("/fake1", 100);
    EXPECT_EQ(file1.fullPath(), string("/fake1"));
}

TEST_F(TestFiles, TestSameFiles)
{
    fm file1("/aaaa1", 100);
    fm file2("/bbbb1", 100);
    EXPECT_EQ(file1 == file2, true);
}

TEST_F(TestFiles, TestNotSameFiles)
{
    fm file1("/aaaa1", 100);
    fm file2("/bbbb2", 100);
    EXPECT_EQ(file1 == file2, false);
}

TEST_F(TestFiles, TestNotSameFiles2)
{
    fm file1("/aaaa1", 100);
    fm file2("/bbbb22131", 100);
    EXPECT_EQ(file1 == file2, false);
}

TEST_F(TestFiles, TestDifferentSizes)
{
    fm file1("/aaaa1", 100);
    fm file2("/bbbb1", 101);
    ASSERT_EQ(file1 == file2, false);
}

TEST_F(TestFiles, TestMinimalBlock)
{
    fm file1("/aaaa1", 100, 1);
    fm file2("/bbbb1", 100, 1);
    EXPECT_EQ(file1 == file2, true);
}

TEST_F(TestFiles, TestMinimalBlockWithDiffirents)
{
    fm file1("/aaaa13", 100, 1);
    fm file2("/bbbb12", 100, 1);
    EXPECT_EQ(file1 == file2, false);
}

TEST_F(TestFiles, TestMediumBlock)
{
    fm file1("/aaaa11", 100, 10);
    fm file2("/bbbb11", 100, 10);
    EXPECT_EQ(file1 == file2, true);
}

TEST_F(TestFiles, TestMediumBlockWithDifferents)
{
    fm file1("/aaaa12", 100, 10);
    fm file2("/bbbb11", 100, 10);
    EXPECT_EQ(file1 == file2, false);
}

TEST_F(TestFiles, TestNonFirstBlocksDifferent)
{
    fm file1("/aaaa111111112111111111", 100, 1);
    fm file2("/bbbb111111113111111111", 100, 1);
    EXPECT_EQ(file1 == file2, false);
}
TEST_F(TestFiles, TestMD5Hash)
{
    auto h = hasherFactory::getInstance()->create("md5");
    fm file1("/aaaa1", 100, 1, h);
    fm file2("/bbbb1", 100, 1, h);
    EXPECT_EQ(file1 == file2, true);
}

TEST_F(TestFiles, TestDifferentHash)
{
    auto h = hasherFactory::getInstance()->create("md5");
    fm file1("/aaaa1", 100, 1);
    fm file2("/bbbb1", 100, 1, h);
    EXPECT_EQ(file1 == file2, false);
}

TEST_F(TestFiles, TestDifferentHash2)
{
    auto h = hasherFactory::getInstance()->create("md5");
    fm file1("/aaaa1", 100, 512);
    fm file2("/bbbb1", 100, 512, h);
    EXPECT_EQ(file1 == file2, false);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
