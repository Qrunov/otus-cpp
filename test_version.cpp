#include <gtest/gtest.h>
#include "ip_filter.h"
using namespace std;

TEST(Test1, TestsIpClass) {
    ip _ip;
	_ip.set_field(1, 10);
	_ip.set_field(2, 20);
	_ip.set_field(3, 30);
	_ip.set_field(4, 40);
	EXPECT_STREQ(_ip.get_string().c_str(),"10.20.30.40");
}


TEST(Test2, TestsParser) {
    stringstream ss;
	ss << "192.168.3.4\t5\t66\n192.168.5.5\t555\t6767\n";
	EXPECT_EQ(get<0>(ipParser::parseStream(ss)),ipParser::Error_code::no_error);
}

TEST(Test3, TestsParser) {
    stringstream ss;
	ss << "192.168.333.4\t5\t66\n192.168.5.5\t555\t6767\n";
	EXPECT_EQ(get<0>(ipParser::parseStream(ss)),ipParser::Error_code::range_error);
}

TEST(Test4, TestsParser) {
    stringstream ss;
	ss << "192.168.3.7.8.4\t5\t66\n192.168.5.5\t555\t6767\n";
	EXPECT_EQ(get<0>(ipParser::parseStream(ss)),ipParser::Error_code::input_error);
}

TEST(Test5, TestsParser) {
    stringstream ss;
	ss << "192.168.3.7\t66\n192.168.5.5\t555\t6767\n";
	EXPECT_EQ(get<0>(ipParser::parseStream(ss)),ipParser::Error_code::input_error);
}

TEST(Test6, TestsParser) {
    stringstream ss;
	ss << "192.168.3.7";
	EXPECT_EQ(get<0>(ipParser::parseStream(ss)),ipParser::Error_code::input_error);
}

TEST(Test7, TestsParser) {
    stringstream ss;
	ss << "192.168:3.7\t5\t66\n192.168.5.5\t555\t6767\n";
	EXPECT_EQ(get<0>(ipParser::parseStream(ss)),ipParser::Error_code::input_error);
}

TEST(Test8, TestsParser) {
    stringstream ss;
	ss << "0.0.0.0\t5\t66\n255.255.255.255\t555\t6767\n";
	EXPECT_EQ(get<0>(ipParser::parseStream(ss)),ipParser::Error_code::no_error);
}

TEST(Test9, TestsParser) {
    stringstream ss;
	ss << "192.168.1.1\t5\t66\n192.168.2.1\t555\t6767\n";
	auto v = get<1>(ipParser::parseStream(ss));
	EXPECT_TRUE(v[0] <= v[1]);
}

TEST(Test10, TestsParser) {
    stringstream ss;
	ss << "192.168.2.2\t5\t66\n192.168.2.2\t555\t6767\n";
	auto v = get<1>(ipParser::parseStream(ss));
	EXPECT_TRUE(v[0] <= v[1]);
}

TEST(Test11, TestsParser) {
    stringstream ss;
	ss << "254.168.2.2\t5\t66\n255.168.2.2\t555\t6767\n";
	auto v = get<1>(ipParser::parseStream(ss));
	EXPECT_TRUE(v[0] <= v[1]);
}


TEST(Test12, TestsParser) {
    stringstream ss;
	ss << "254.168.2.1\t5\t66\n250.168.2.2\t555\t6767\n";
	auto v = get<1>(ipParser::parseStream(ss));
	EXPECT_FALSE(v[0] <= v[1]);
}





int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}