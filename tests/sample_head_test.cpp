#include "sample_head.h"
#include "gtest/gtest.h"

// 测试 add 函数
TEST(SampleHeadTest, AddFunction) {
  EXPECT_EQ(add(1, 2), 3);
  EXPECT_EQ(add(-1, 1), 0);
  EXPECT_EQ(add(-1, -1), -2);
}

// 测试 processVector 函数
TEST(SampleHeadTest, ProcessVectorFunction) {
  std::vector<int> testVector1 = {1, 2, 3, 4, 5};
  EXPECT_EQ(processVector(testVector1), 15);

  std::vector<int> testVector2 = {0, 0, 0, 0};
  EXPECT_EQ(processVector(testVector2), 0);

  std::vector<int> testVector3 = {-1, -2, -3};
  EXPECT_EQ(processVector(testVector3), -6);
}

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }