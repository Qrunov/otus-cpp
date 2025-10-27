#include <gtest/gtest.h>
#include "parser.h"
#include "opencollector.h"
#include "async_test.h"
using namespace std;

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

TEST(Test, TestAsync1)
{
    async::handle_t h1 = async::connect_t<openCollector>(5);
    async::disconnect(h1);
    EXPECT_GT((uint64_t)h1, 0);
}

TEST(Test, TestAsync2)
{
    async::handle_t h1 = async::connect_t<openCollector>(5);
    auto a = static_pointer_cast<openCollector>(async::getOpenCollector(h1));
    string s = "cmd1";
    async::receive(h1, s.c_str(), s.size());
    async::receiveEof(h1);

    EXPECT_EQ(a->beginBlockWasCalled, true);
    EXPECT_EQ(a->endBlockWasCalled, true);
    EXPECT_EQ(a->lastCmd, "cmd1");

    async::disconnect(h1);

}


TEST(Test, TestAsync3)
{
    auto h1 = async::connect_t<openCollector>(5);
    auto h2 = async::connect_t<openCollector>(3);

    auto a1 = static_pointer_cast<openCollector>(async::getOpenCollector(h1));
    auto a2 = static_pointer_cast<openCollector>(async::getOpenCollector(h2));

    string s = "cmd1 cmd2 cmd3 cmd4";
    async::receive(h1, s.c_str(), s.size());
    async::receive(h2, s.c_str(), s.size());

    EXPECT_EQ(a1->beginBlockWasCalled, true);
    EXPECT_EQ(a1->endBlockWasCalled, false);
    EXPECT_EQ(a1->lastCmd, "cmd4");


    EXPECT_EQ(a2->beginBlockWasCalled, true);
    EXPECT_EQ(a2->endBlockWasCalled, true);
    EXPECT_EQ(a2->lastCmd, "cmd4");

    async::disconnect(h1);
    async::disconnect(h2);
}


TEST(Test, TestAsync4)
{
    auto h1 = async::connect_t<openCollector>(1);
    auto h2 = async::connect_t<openCollector>(1);

    auto a1 = static_pointer_cast<openCollector>(async::getOpenCollector(h1));
    auto a2 = static_pointer_cast<openCollector>(async::getOpenCollector(h2));

    string s[]{"cmd1", "cmd2", "cmd3", "cmd4"};
    async::receive(h1, s[0].c_str(), s[0].size());
    async::receive(h2, s[1].c_str(), s[1].size());

    async::receive(h2, s[2].c_str(), s[2].size());
    async::receive(h1, s[3].c_str(), s[3].size());

    EXPECT_EQ(a1->beginBlockWasCalled, true);
    EXPECT_EQ(a1->endBlockWasCalled, true);
    EXPECT_EQ(a1->lastCmd, "cmd4");

    EXPECT_EQ(a2->beginBlockWasCalled, true);
    EXPECT_EQ(a2->endBlockWasCalled, true);
    EXPECT_EQ(a2->lastCmd, "cmd3");

    async::disconnect(h1);
    async::disconnect(h2);
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}