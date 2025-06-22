#include "s21_TetrisController.h"

namespace s21 {
void TetrisController::userInput(UserAction_t action, bool hold) {
  if (hold == true) return;
  switch (action) {
    case Start:
      data = new TetrisModel();
      break;
    case Pause:
      data->switch_pause();
      break;
    case Terminate:
      if (data) {
        delete data;
        data = nullptr;  // сосал?
      }
      // delete data;
      break;
    case Left:
      data->move(true);
      break;
    case Right:
      data->move(false);
      break;
    case Down:
      data->blink_down();
      break;
    case Action:
      data->rotate_block();
      break;
    default:
      break;
  }
}
}  // namespace s21
