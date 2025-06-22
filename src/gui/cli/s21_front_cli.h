#ifndef FRONT_CLI_H
#define FRONT_CLI_H
#include <exception>
#include <iostream>

#include "../../brick_game/brick_game.h"
#include "../../brick_game/snake/s21_SnakeController.h"
#include "../../brick_game/tetris/s21_TetrisController.h"
namespace s21 {
class CLIFront;

void main_loop_cli(CLIFront *program);

class CLIFront {
 public:
  void draw_border();
  int draw();
  void init_ncurses();
  /**
   * @param game - Выбор между тетрисом и змейкой, если t(T) - Тетрис, если s(S)
   * - Змейка
   */
  CLIFront(char game) {
    if (game == 't' || game == 'T') {
      data = new TetrisController();
    } else if (game == 's' || game == 'S') {
      data = new SnakeController();
    } else {
      std::cout << "Unknown game, t(tetris) and s(snake) avaliable";
      std::cout.flush();
      throw std::exception();
    }
  };
  ~CLIFront() {
    if (data) {
      delete data;
      data = nullptr;
    }
  };
  int get_score() { return data->get_current_state().score; };
  GameInfo_t get_current_state() { return data->get_current_state(); };
  void userInput(UserAction_t action, bool hold) {
    if (data) data->userInput(action, hold);
  };

 private:
  Controller *data;
};
}  // namespace s21

#endif