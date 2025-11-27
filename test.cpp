#include <gtest/gtest.h>
#include "disp.h"
#include <vector>
using namespace std;

TEST(Test, TestDisp1)
{
    vector<uint32_t>	v = {2,4,5,7,9,3,6,8,4,7};
    double res = disp(v);

    EXPECT_NEAR(res, 5.167, 0.001);
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}