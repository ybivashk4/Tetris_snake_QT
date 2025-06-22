#ifndef TETRIS_CONTROLLER_H
#define TETRIS_CONTROLLER_H
#include "../brick_game.h"
#include "s21_TetrisModel.h"

namespace s21 {
class TetrisController : public Controller {
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

  TetrisController() { data = nullptr; }
  ~TetrisController() {
    if (data) {
      delete data;
    }
  }
  int **get_next() override { return data->get_next(); }

 private:
  TetrisModel *data;
};
}  // namespace s21
#endif