#include <gtest/gtest.h>
#include "lib.h"

TEST(VersionTest, Tests1) {
    EXPECT_TRUE(version() > 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}