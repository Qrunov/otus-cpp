#include <gtest/gtest.h>
#include "parser.h"
using namespace std;

class openCollector : public collectorInterface
{
public:
    void beginBlock() override;
    void endBlock() override;
    void addCmd(const std::string &cmd) override;

    bool beginBlockWasCalled;
    bool endBlockWasCalled;
    std::string lastCmd;
};

void openCollector::beginBlock()
{
    beginBlockWasCalled = true;
}

void openCollector::endBlock()
{
    endBlockWasCalled = true;
}

void openCollector::addCmd(const string &cmd)
{
    lastCmd = cmd;
}

class strstreamSource : public sourceInterface
{
public:
    std::string getData() override;
    bool wasFinished() override;
    strstreamSource &operator<<(const std::string &str);
    void clear();

private:
    std::stringstream data;
};

void strstreamSource::clear()
{
    data.str("");
    data.clear();
}

string strstreamSource::getData()
{
    string str;
    data >> str;
    return str;
}

bool strstreamSource::wasFinished()
{
    return data.eof();
}

strstreamSource &strstreamSource::operator<<(const string &str)
{
    data << str;
    return *this;
}

class TestParser : public ::testing::Test
{
protected:
    void SetUp()
    {

        a = make_shared<openCollector>();
        source = make_shared<strstreamSource>();
    }
    void TearDown()
    {
    }
    shared_ptr<openCollector> a;
    shared_ptr<strstreamSource> source;
};

TEST_F(TestParser, TestsN1)
{
    strstreamSource &src = *source;
    src << "cmd1";

    cmdParser parser(a, 1);
    parser.parseIt(source);

    EXPECT_EQ(a->beginBlockWasCalled, true);
    EXPECT_EQ(a->endBlockWasCalled, true);
    EXPECT_EQ(a->lastCmd, "cmd1");
}

TEST_F(TestParser, TestsN2)
{
    strstreamSource &src = *source;
    src << "cmd1";

    cmdParser parser(a, 2);
    parser.parseIt(source, false);

    EXPECT_EQ(a->beginBlockWasCalled, true);
    EXPECT_EQ(a->endBlockWasCalled, false);
    EXPECT_EQ(a->lastCmd, "cmd1");

    a->beginBlockWasCalled = false;
    src.clear();

    src << "cmd2";
    parser.parseIt(source);

    EXPECT_EQ(a->beginBlockWasCalled, false);
    EXPECT_EQ(a->endBlockWasCalled, true);
    EXPECT_EQ(a->lastCmd, "cmd2");
}

TEST_F(TestParser, TestsN3)
{
    strstreamSource &src = *source;
    src << "cmd1";

    cmdParser parser(a, 3);
    parser.parseIt(source, false);

    EXPECT_EQ(a->beginBlockWasCalled, true);
    EXPECT_EQ(a->endBlockWasCalled, false);
    EXPECT_EQ(a->lastCmd, "cmd1");

    a->beginBlockWasCalled = false;
    src.clear();

    src << "cmd2";
    parser.parseIt(source, false);

    EXPECT_EQ(a->beginBlockWasCalled, false);
    EXPECT_EQ(a->endBlockWasCalled, false);
    EXPECT_EQ(a->lastCmd, "cmd2");

    src.clear();
    src << "cmd3";
    parser.parseIt(source, false);

    EXPECT_EQ(a->beginBlockWasCalled, false);
    EXPECT_EQ(a->endBlockWasCalled, true);
    EXPECT_EQ(a->lastCmd, "cmd3");
    a->endBlockWasCalled = false;

    src.clear();
    src << "cmd4";
    parser.parseIt(source, false);

    EXPECT_EQ(a->beginBlockWasCalled, true);
    EXPECT_EQ(a->endBlockWasCalled, false);
    EXPECT_EQ(a->lastCmd, "cmd4");
}

TEST_F(TestParser, TestsBlockNoFilled)
{
    strstreamSource &src = *source;
    src << "cmd1";

    cmdParser parser(a, 3);
    parser.parseIt(source);

    EXPECT_EQ(a->beginBlockWasCalled, true);
    EXPECT_EQ(a->endBlockWasCalled, true);
    EXPECT_EQ(a->lastCmd, "cmd1");
}

TEST_F(TestParser, TestsDynamicBlock1)
{
    strstreamSource &src = *source;
    src << "{ cmd1 } cmd2";

    cmdParser parser(a, 3);
    parser.parseIt(source, false);

    EXPECT_EQ(a->beginBlockWasCalled, true);
    EXPECT_EQ(a->endBlockWasCalled, true);
    EXPECT_EQ(a->lastCmd, "cmd2");
}

TEST_F(TestParser, TestsDynamicBlockFollowsStatic)
{
    strstreamSource &src = *source;
    src << "cmd1 {";

    cmdParser parser(a, 3);
    parser.parseIt(source, false);

    EXPECT_EQ(a->beginBlockWasCalled, true);
    EXPECT_EQ(a->endBlockWasCalled, true);
    EXPECT_EQ(a->lastCmd, "cmd1");
}

TEST_F(TestParser, TestsInnerDynamicBlock)
{
    strstreamSource &src = *source;
    src << "{ cmd1 { cmd2 } }"; //}} - interpretated as cmd

    cmdParser parser(a, 3);
    parser.parseIt(source);

    EXPECT_EQ(a->beginBlockWasCalled, true);
    EXPECT_EQ(a->endBlockWasCalled, true);
    EXPECT_EQ(a->lastCmd, "cmd2");
}

TEST_F(TestParser, TestsDynamicBlockEndsOpened)
{
    strstreamSource &src = *source;
    src << "{ cmd1"; //}} - interpretated as cmd

    cmdParser parser(a, 3);
    parser.parseIt(source);

    EXPECT_EQ(a->beginBlockWasCalled, true);
    EXPECT_EQ(a->endBlockWasCalled, false);
    EXPECT_EQ(a->lastCmd, "cmd1");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}