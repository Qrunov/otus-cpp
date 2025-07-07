#include <gtest/gtest.h>
#include "ip_filter.h"

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


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}