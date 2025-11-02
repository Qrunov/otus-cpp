#include <gtest/gtest.h>
#include "parser.h"
#include "opencollector.h"
#include "asyncController.h"
#include <gmock/gmock.h>
using namespace std;

class mockCollector : public ICollector
{
public:
    MOCK_METHOD(void, beginBlock, (), (override));
    MOCK_METHOD(void, endBlock, (), (override));

    std::string lastCmd;

    void addCmd(const std::string &cmd) override
    {
        lastCmd = cmd;
        onAddCmd(cmd);
    }

    MOCK_METHOD(void, onAddCmd, (const std::string &cmd));
};

class strstreamSource : public ISource
{
public:
    std::string getData() override;
    bool wasFinished() const override;
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

bool strstreamSource::wasFinished() const
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

        m_mockCollector = make_shared<mockCollector>();
        m_source = make_shared<strstreamSource>();
    }
    shared_ptr<mockCollector> m_mockCollector;
    shared_ptr<strstreamSource> m_source;
};

TEST_F(TestParser, TestsN1)
{
    strstreamSource &src = *m_source;
    src << "cmd1";

    EXPECT_CALL(*m_mockCollector, beginBlock()).Times(1);
    EXPECT_CALL(*m_mockCollector, onAddCmd(::testing::_)).Times(::testing::AtLeast(1));
    EXPECT_CALL(*m_mockCollector, endBlock()).Times(1);

    cmdParser parser(m_mockCollector, 1);
    parser.parseIt(m_source);
}

TEST_F(TestParser, TestsN2)
{
    strstreamSource &src = *m_source;
    src << "cmd1";

    EXPECT_CALL(*m_mockCollector, beginBlock()).Times(1);
    EXPECT_CALL(*m_mockCollector, onAddCmd(::testing::_)).Times(::testing::AtLeast(2));
    EXPECT_CALL(*m_mockCollector, endBlock()).Times(1);

    cmdParser parser(m_mockCollector, 2);
    parser.parseIt(m_source, false);

    EXPECT_EQ(m_mockCollector->lastCmd, "cmd1");

    src.clear();
    src << "cmd2";
    parser.parseIt(m_source);
}

TEST_F(TestParser, TestsN3)
{
    EXPECT_CALL(*m_mockCollector, beginBlock()).Times(1);
    EXPECT_CALL(*m_mockCollector, onAddCmd("cmd1")).Times(1);
    EXPECT_CALL(*m_mockCollector, endBlock()).Times(0);

    strstreamSource &src = *m_source;
    src << "cmd1";
    cmdParser parser(m_mockCollector, 3);
    parser.parseIt(m_source, false);
    src.clear();
    ::testing::Mock::VerifyAndClearExpectations(m_mockCollector.get());

    EXPECT_CALL(*m_mockCollector, beginBlock()).Times(0);
    EXPECT_CALL(*m_mockCollector, onAddCmd("cmd2")).Times(1);
    EXPECT_CALL(*m_mockCollector, endBlock()).Times(0);

    src << "cmd2";
    parser.parseIt(m_source, false);
    ::testing::Mock::VerifyAndClearExpectations(m_mockCollector.get());
    src.clear();

    EXPECT_CALL(*m_mockCollector, beginBlock()).Times(0);
    EXPECT_CALL(*m_mockCollector, onAddCmd("cmd3")).Times(1);
    EXPECT_CALL(*m_mockCollector, endBlock()).Times(1);

    src << "cmd3";
    parser.parseIt(m_source, false);
    ::testing::Mock::VerifyAndClearExpectations(m_mockCollector.get());
    src.clear();

    EXPECT_CALL(*m_mockCollector, beginBlock()).Times(1);
    EXPECT_CALL(*m_mockCollector, onAddCmd("cmd4")).Times(1);
    EXPECT_CALL(*m_mockCollector, endBlock()).Times(1);
    src << "cmd4";
    parser.parseIt(m_source, true);
}

TEST_F(TestParser, TestsBlockNoFilled)
{
    EXPECT_CALL(*m_mockCollector, beginBlock()).Times(1);
    EXPECT_CALL(*m_mockCollector, onAddCmd("cmd1")).Times(1);
    EXPECT_CALL(*m_mockCollector, endBlock()).Times(1);

    strstreamSource &src = *m_source;
    src << "cmd1";
    cmdParser parser(m_mockCollector, 3);
    parser.parseIt(m_source);
}

TEST_F(TestParser, TestsDynamicBlock1)
{
    EXPECT_CALL(*m_mockCollector, beginBlock()).Times(2);
    EXPECT_CALL(*m_mockCollector, onAddCmd("cmd2")).Times(1);
    EXPECT_CALL(*m_mockCollector, onAddCmd("cmd1")).Times(1);
    EXPECT_CALL(*m_mockCollector, endBlock()).Times(1);

    strstreamSource &src = *m_source;
    src << "{ cmd1 } cmd2";

    cmdParser parser(m_mockCollector, 3);
    parser.parseIt(m_source, false);

    EXPECT_EQ(m_mockCollector->lastCmd, "cmd2");
}

TEST_F(TestParser, TestsDynamicBlockFollowsStatic)
{
    EXPECT_CALL(*m_mockCollector, beginBlock()).Times(2);
    EXPECT_CALL(*m_mockCollector, onAddCmd("cmd1")).Times(1);
    EXPECT_CALL(*m_mockCollector, endBlock()).Times(1);

    strstreamSource &src = *m_source;
    src << "cmd1 {";

    cmdParser parser(m_mockCollector, 3);
    parser.parseIt(m_source, false);

    EXPECT_EQ(m_mockCollector->lastCmd, "cmd1");
}

TEST_F(TestParser, TestsInnerDynamicBlock)
{
    EXPECT_CALL(*m_mockCollector, beginBlock()).Times(1);
    EXPECT_CALL(*m_mockCollector, onAddCmd("cmd2")).Times(1);
    EXPECT_CALL(*m_mockCollector, onAddCmd("cmd1")).Times(1);
    EXPECT_CALL(*m_mockCollector, endBlock()).Times(1);

    strstreamSource &src = *m_source;
    src << "{ cmd1 { cmd2 } }"; //}} - interpretated as cmd

    cmdParser parser(m_mockCollector, 3);
    parser.parseIt(m_source);

    EXPECT_EQ(m_mockCollector->lastCmd, "cmd2");
}

TEST_F(TestParser, TestsDynamicBlockEndsOpened)
{
    EXPECT_CALL(*m_mockCollector, beginBlock()).Times(1);
    EXPECT_CALL(*m_mockCollector, onAddCmd("cmd1")).Times(1);
    EXPECT_CALL(*m_mockCollector, endBlock()).Times(0);

    strstreamSource &src = *m_source;
    src << "{ cmd1"; //}} - interpretated as cmd

    cmdParser parser(m_mockCollector, 3);
    parser.parseIt(m_source);

    EXPECT_EQ(m_mockCollector->lastCmd, "cmd1");
}

TEST(Test, TestAsync1)
{
    async::handle_t h1 = asyncController::getInstance()->connect_t<mockCollector>(5);
    async::disconnect(h1);
    EXPECT_GT((uint64_t)h1, 0);
}

TEST(Test, TestAsync2)
{
    async::handle_t h1 = asyncController::getInstance()->connect_t<mockCollector>(5);
    auto m_mockCollector = static_pointer_cast<mockCollector>(asyncController::getInstance()->getCollector(h1));
    EXPECT_CALL(*m_mockCollector, beginBlock()).Times(1);
    EXPECT_CALL(*m_mockCollector, onAddCmd("cmd1")).Times(1);
    EXPECT_CALL(*m_mockCollector, endBlock()).Times(1);

    string s = "cmd1";
    async::receive(h1, s.c_str(), s.size());
    async::disconnect(h1);
    //    async::receiveEof(h1);

    EXPECT_EQ(m_mockCollector->lastCmd, "cmd1");
}

TEST(Test, TestAsync3)
{
    auto h1 = asyncController::getInstance()->connect_t<mockCollector>(5);
    auto h2 = asyncController::getInstance()->connect_t<mockCollector>(3);

    auto a1 = static_pointer_cast<mockCollector>(asyncController::getInstance()->getCollector(h1));
    auto a2 = static_pointer_cast<mockCollector>(asyncController::getInstance()->getCollector(h2));

    EXPECT_CALL(*a1, beginBlock()).Times(1);
    EXPECT_CALL(*a1, onAddCmd(::testing::_)).Times(::testing::AtLeast(4));
    EXPECT_CALL(*a1, endBlock()).Times(1);

    EXPECT_CALL(*a2, beginBlock()).Times(2);
    EXPECT_CALL(*a2, onAddCmd(::testing::_)).Times(::testing::AtLeast(4));
    EXPECT_CALL(*a2, endBlock()).Times(2);

    string s = "cmd1 cmd2 cmd3 cmd4";
    async::receive(h1, s.c_str(), s.size());
    async::receive(h2, s.c_str(), s.size());

    EXPECT_EQ(a1->lastCmd, "cmd4");
    EXPECT_EQ(a2->lastCmd, "cmd4");

    async::disconnect(h1);
    async::disconnect(h2);
}

TEST(Test, TestAsync4)
{
    auto h1 = asyncController::getInstance()->connect_t<mockCollector>(1);
    auto h2 = asyncController::getInstance()->connect_t<mockCollector>(1);

    auto a1 = static_pointer_cast<mockCollector>(asyncController::getInstance()->getCollector(h1));
    auto a2 = static_pointer_cast<mockCollector>(asyncController::getInstance()->getCollector(h2));

    EXPECT_CALL(*a1, beginBlock()).Times(2);
    EXPECT_CALL(*a1, onAddCmd(::testing::_)).Times(::testing::AtLeast(2));
    EXPECT_CALL(*a1, endBlock()).Times(2);

    EXPECT_CALL(*a2, beginBlock()).Times(2);
    EXPECT_CALL(*a2, onAddCmd(::testing::_)).Times(::testing::AtLeast(2));
    EXPECT_CALL(*a2, endBlock()).Times(2);

    string s[]{"cmd1", "cmd2", "cmd3", "cmd4"};
    async::receive(h1, s[0].c_str(), s[0].size());
    async::receive(h2, s[1].c_str(), s[1].size());
    async::receive(h2, s[2].c_str(), s[2].size());
    async::receive(h1, s[3].c_str(), s[3].size());

    EXPECT_EQ(a1->lastCmd, "cmd4");
    EXPECT_EQ(a2->lastCmd, "cmd3");

    async::disconnect(h1);
    async::disconnect(h2);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}