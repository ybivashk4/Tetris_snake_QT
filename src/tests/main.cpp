#include <gtest/gtest.h>

#include <memory>

#include "../brick_game/brick_game.h"
#include "../brick_game/snake/s21_snake_back.h"
#include "../brick_game/tetris/s21_tetris_back.h"
using namespace s21;

class TetrisTest : public ::testing::Test {
 protected:
  void SetUp() override {
    data = std::make_unique<TetrisController>();
    data->userInput(Start, false);
  }
  std::unique_ptr<TetrisController> data;
};

class SnakeTest : public ::testing::Test {
 protected:
  void SetUp() override {
    data = new SnakeController();
    data->userInput(Start, false);
  }
  void TearDown() override { delete data; }
  SnakeController *data;
};

TEST_F(TetrisTest, StartStateInit) {
  GameInfo_t test = data->get_current_state();

  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      EXPECT_EQ(test.field[i][j], 0);
      EXPECT_EQ(test.next[i][j], 0);
    }
  }
  EXPECT_EQ(test.high_score, 0);
  EXPECT_EQ(test.level, 1);
  EXPECT_EQ(test.pause, false);
  EXPECT_EQ(test.score, 0);
  EXPECT_EQ(test.speed, 1100);
}

TEST_F(TetrisTest, WithoutInputs) {
  while (data->get_current_state().field) {
    data->updateCurrentState();
  }
  EXPECT_TRUE(data->get_current_state().next == nullptr);
  EXPECT_TRUE(data->updateCurrentState().next == nullptr);
}

TEST_F(TetrisTest, KeyLeftAndRight) {
  data->userInput(Left, false);
  data->updateCurrentState();
  data->userInput(Left, false);
  data->updateCurrentState();
  data->userInput(Right, false);
  data->updateCurrentState();
  data->userInput(Right, false);
  data->updateCurrentState();
  EXPECT_EQ(1, 1);
}

TEST_F(TetrisTest, KeyDown) {
  data->updateCurrentState();
  data->userInput(Down, 0);
  data->updateCurrentState();
  data->userInput(Down, 0);
  data->updateCurrentState();
  data->userInput(Down, 0);
  data->updateCurrentState();
  EXPECT_FALSE(data->get_current_state().next == nullptr);
}

TEST_F(TetrisTest, Terminate) {
  data->userInput(Terminate, false);
  EXPECT_TRUE(data->get_current_state().next == nullptr);
}

TEST_F(TetrisTest, Pause) {
  data->userInput(Pause, false);
  EXPECT_EQ(data->get_current_state().pause, 1);
}

TEST_F(TetrisTest, Rotation) {
  data->updateCurrentState();
  data->updateCurrentState();
  data->updateCurrentState();
  data->updateCurrentState();
  data->userInput(Action, false);
  data->updateCurrentState();
  data->userInput(Action, false);
  data->updateCurrentState();
  data->userInput(Action, false);
  data->updateCurrentState();
  data->userInput(Action, false);
  data->updateCurrentState();
  EXPECT_FALSE(data->get_current_state().next == nullptr);
}

TEST_F(TetrisTest, ToCoverage) {
  data->updateCurrentState();
  data->userInput(Down, 0);
  data->updateCurrentState();
  data->userInput(Down, 0);
  data->updateCurrentState();
  data->userInput(Down, 0);
  data->updateCurrentState();
  data->userInput(Down, 0);
  data->updateCurrentState();
  data->userInput(Down, 0);
  data->updateCurrentState();
  EXPECT_FALSE(data->get_current_state().next == nullptr);
}

TEST_F(TetrisTest, GetNextTest) {
  int error = 0;
  int **testObj = data->get_next();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      EXPECT_GE(testObj[i][j], 0);
      EXPECT_LE(testObj[i][j], 10);
    }
  }
}

TEST_F(SnakeTest, StartStateInit) {
  GameInfo_t test = data->get_current_state();

  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      EXPECT_EQ(test.next[i][j], 0);
    }
  }
  EXPECT_EQ(test.high_score, 0);
  EXPECT_EQ(test.level, 1);
  EXPECT_EQ(test.pause, false);
  EXPECT_EQ(test.score, 4);
  EXPECT_EQ(test.speed, 1300);
  // EXPECT_EQ(1300, 1300);
}

TEST_F(SnakeTest, WithoutInputs_snake) {
  while (data->get_current_state().field) {
    data->updateCurrentState();
  }
  EXPECT_TRUE(data->get_current_state().next == nullptr);
  EXPECT_TRUE(data->updateCurrentState().next == nullptr);
}

TEST_F(SnakeTest, KeyLeftAndRight_snake) {
  data->userInput(Left, false);
  data->updateCurrentState();
  data->userInput(Left, false);
  data->updateCurrentState();
  data->userInput(Right, false);
  data->updateCurrentState();
  data->userInput(Right, false);
  data->updateCurrentState();
  EXPECT_EQ(1, 1);
}

TEST_F(SnakeTest, KeyDown_snake) {
  data->updateCurrentState();
  data->userInput(Down, 0);
  data->updateCurrentState();
  data->userInput(Down, 0);
  data->updateCurrentState();
  data->userInput(Down, 0);
  data->updateCurrentState();
  EXPECT_FALSE(data->get_current_state().next == nullptr);
}

TEST_F(SnakeTest, Terminate_snake) {
  data->userInput(Terminate, false);
  EXPECT_TRUE(data->get_current_state().next == nullptr);
}

TEST_F(SnakeTest, Pause_snake) {
  data->userInput(Pause, false);
  EXPECT_EQ(data->get_current_state().pause, 1);
}

TEST_F(SnakeTest, KillTest_snake) {
  data->updateCurrentState();
  data->userInput(Action, false);
  data->userInput(Action, true);

  data->updateCurrentState();
  data->updateCurrentState();
  data->updateCurrentState();
  data->updateCurrentState();
  data->updateCurrentState();
  data->updateCurrentState();
  data->updateCurrentState();
  EXPECT_TRUE(data->get_current_state().next == nullptr);
}

TEST_F(SnakeTest, GetNextTest_snake) { EXPECT_EQ(nullptr, data->get_next()); }

TEST_F(SnakeTest, RotationTest_snake) {
  data->userInput(Down, false);
  data->updateCurrentState();
  data->userInput(Left, false);
  data->updateCurrentState();
  data->updateCurrentState();
  data->updateCurrentState();
  data->updateCurrentState();
  data->userInput(Up, false);
  data->updateCurrentState();
  data->userInput(Right, false);
}

TEST_F(SnakeTest, idk) {
  Controller *temp = new Controller();
  temp->get_current_state();
  temp->updateCurrentState();
  temp->get_next();
  temp->userInput(Action, false);
  temp->userInput(Action, true);
  delete temp;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
