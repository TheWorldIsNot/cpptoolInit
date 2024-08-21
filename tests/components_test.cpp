#include "Components/EndEffector.h"
#include "Components/Joint.h"
#include "Components/RobotArm.h"
#include "Components/RobotComponent.h"
#include <gtest/gtest.h>
#include <iostream>
#include <memory> // For smart pointers

class RobotComponentTest : public ::testing::Test {
protected:
  std::unique_ptr<RobotComponent> component;

  void SetUp() override { component = std::make_unique<RobotComponent>(1); }
};

TEST_F(RobotComponentTest, TestConstruction) {
  EXPECT_EQ(component->getComponentID(), 1);
  EXPECT_EQ(component->getStatus(), "Initialized");
}

TEST_F(RobotComponentTest, TestUpdate) {
  component->update();
  EXPECT_NE(component->getStatus(), "Initialized");
}

class RobotArmTest : public ::testing::Test {
protected:
  std::unique_ptr<RobotArm> arm;

  void SetUp() override { arm = std::make_unique<RobotArm>(1, 3); }
};

TEST_F(RobotArmTest, TestMoveArm) {
  for (auto &joint : arm->getJoints()) {
    joint.setAngle(0);
  }

  arm->moveArm();

  for (const auto &joint : arm->getJoints()) {
    EXPECT_NE(joint.getAngle(), 0);
  }
}

class JointTest : public ::testing::Test {
protected:
  std::unique_ptr<Joint> joint;

  void SetUp() override { joint = std::make_unique<Joint>(1); }
};

TEST_F(JointTest, TestSetAngle) {
  joint->setAngle(90.0);
  EXPECT_NEAR(joint->getAngle(), 90.0, 0.01);
}

class EndEffectorTest : public ::testing::Test {
protected:
  std::unique_ptr<EndEffector> effector;

  void SetUp() override { effector = std::make_unique<EndEffector>(1); }
};

TEST_F(EndEffectorTest, TestSetToolType) {
  effector->setToolType("GrippingTool");
  EXPECT_EQ(effector->getToolType(), "GrippingTool");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout << "Starting tests..." << std::endl;
  int result = RUN_ALL_TESTS();
  if (result == 0) {
    std::cout << "All tests passed." << std::endl;
  } else {
    std::cout << "Some tests failed." << std::endl;
  }
  return result;
}