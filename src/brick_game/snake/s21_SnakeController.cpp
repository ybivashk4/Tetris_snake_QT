#include "s21_SnakeController.h"

namespace s21 {
void SnakeController::userInput(UserAction_t action, bool hold) {
  if (hold == true) return;
  switch (action) {
    case Start:
      data = new SnakeModel();
      break;
    case Pause:
      data->switch_pause();
      break;
    case Terminate:
      if (data) {
        delete data;
        data = nullptr;
      }
      break;
    case Left:
    case Right:
    case Up:
    case Down:
      data->rotate(action);
      break;
    case Action:
      data->set_max_speed();
      break;
    default:
      break;
  }
};
}  // namespace s21
