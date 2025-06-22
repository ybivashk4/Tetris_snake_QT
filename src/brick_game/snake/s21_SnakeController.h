#ifndef SNAKE_CONTROLLER_H
#define SNAKE_CONTROLLER_H
#include "../brick_game.h"
#include "s21_SnakeModel.h"

namespace s21 {
class SnakeController : public Controller {
 public:
  void userInput(UserAction_t action, bool hold) override;
  GameInfo_t updateCurrentState() override {
    if (data) {
      return data->updateCurrentState();
    }
    GameInfo_t none = {0, 0, 0, 0, 0, 0, 0};
    return none;
  };
  GameInfo_t get_current_state() override {
    if (data) {
      return data->get_current_state();
    }
    GameInfo_t none = {0, 0, 0, 0, 0, 0, 0};
    return none;
  };
  SnakeController() { data = nullptr; }
  ~SnakeController() {
    if (data) delete data;
  }
  int **get_next() override { return nullptr; }

 private:
  SnakeModel *data;
};
}  // namespace s21
#endif