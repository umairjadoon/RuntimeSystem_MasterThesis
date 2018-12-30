#include "gtest/gtest.h"
#include "module.h"

TEST(testTest, number) {
  EXPECT_EQ(1, *PrintHello(1));
}
TEST(test2Test, number) {
  EXPECT_EQ(0, *PrintHello(0));
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
