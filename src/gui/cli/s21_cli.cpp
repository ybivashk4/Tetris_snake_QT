#include <ncurses.h>
#ifdef scroll
#undef scroll
#endif
#include <stdio.h>

#include <cstdlib>
#include <iostream>

#include "s21_front_cli.h"

namespace s21 {

void main_loop_cli(CLIFront* program) {
  int ch = getch();
  int game_over = 0;
  while (!game_over) {
    timeout(program->get_current_state().speed);
    // Обработка ввода
    ch = getch();
    switch (ch) {
      case KEY_LEFT:
        program->userInput(Left, 0);
        break;
      case KEY_RIGHT:
        program->userInput(Right, 0);
        break;
      case KEY_DOWN:
        program->userInput(Down, 0);
        break;
      case KEY_UP:
        program->userInput(Up, 0);
        break;
      case 'q':
      case 'Q':
        program->userInput(Terminate, false);
        game_over = 1;
        break;
      case 'r':
      case 'R':
        program->userInput(Action, false);
        break;
      case 'p':
      case 'P':
        program->userInput(Pause, false);
        break;
      default:
        break;
    }
    if (!program->get_current_state().pause) {
      if (!game_over && program->draw()) {
        game_over = 1;
      };
    }
  }
}
}  // namespace s21

// int main() {
//   // Инициализация ncurses
//   CLIFront program('s');
//   program.init_ncurses();
//   main_loop_snake_cli(&program);
//   endwin();  // Завершение работы с ncurses
//   printf("Game Over!, score: %d", program.get_high_score());
//   return 0;
// }