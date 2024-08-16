// #include "sample_head.h"
// #include "gtest/gtest.h"

// // 测试 add 函数
// TEST(AddTest, HandlesPositiveNumbers) {
//     EXPECT_EQ(5, add(2, 3));
// }

// TEST(AddTest, HandlesNegativeNumbers) {
//     EXPECT_EQ(-5, add(-2, -3));
// }

// TEST(AddTest, ZeroTest) {
//     EXPECT_EQ(0, add(0, 0));
// }

// // 测试 processVector 函数
// TEST(ProcessVectorTest, HandlesEmptyVector) {
//     std::vector<int> empty;
//     EXPECT_EQ(0, processVector(empty));
// }

// TEST(ProcessVectorTest, HandlesNonEmptyVector) {
//     std::vector<int> vec = {1, 2, 3, 4};
//     EXPECT_EQ(10, processVector(vec));
// }

// // 可以添加更多的测试用例来检查不同的输入情况
// TEST(ProcessVectorTest, HandlesNegativeNumbers) {
//     std::vector<int> vec = {-1, -2, -3};
//     EXPECT_EQ(-6, processVector(vec));
// }

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }