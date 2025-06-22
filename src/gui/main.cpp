#include <ncurses.h>

#include "cli/s21_front_cli.h"
#ifdef scroll
#undef scroll
#endif
#include <QApplication>
#include <iostream>

#include "desktop/s21_front_desktop.h"

using namespace s21;

int desktop(int argc, char *argv[], char type) {
  QApplication app(argc, argv);

  s21::QtFront window(type);  // Запускаем Тетрис
  window.show();
  return app.exec();
}

int cli(char type) {
  CLIFront program(type);
  program.init_ncurses();
  main_loop_cli(&program);
  endwin();  // Завершение работы с ncurses
  printf("Game Over!, score: %d", program.get_score());
  return 0;
}

void displayMenu() {
  std::cout << "=========" << std::endl;
  std::cout << " M E N U " << std::endl;
  std::cout << "=========" << std::endl;
  std::cout << "1. tetris CLI" << std::endl;
  std::cout << "2. snake CLI" << std::endl;
  std::cout << "3. tetris Desktop" << std::endl;
  std::cout << "4. snake Desktop" << std::endl;
  std::cout << "0. Quit" << std::endl << std::endl;
}

std::pair<int, char> menu() {
  displayMenu();
  int type = 0;
  std::cin >> type;
  std::pair<int, char> res(0, 0);
  switch (type) {
    case 1:
      res.first = 1;
      res.second = 't';
      break;
    case 2:
      res.first = 1;
      res.second = 's';
      break;
    case 3:
      res.first = 2;
      res.second = 't';
      break;
    case 4:
      res.first = 2;
      res.second = 's';
      break;
    default:
      break;
  }
  return res;
}

int main(int argc, char *argv[]) {
  std::pair<int, char> choose = menu();
  int res = 0;
  if (choose.first == 1) {
    res = cli(choose.second);
  } else if (choose.first == 2) {
    res = desktop(argc, argv, choose.second);
  } else {
    std::cout << "Bye, Bye!\n";
  }

  // Инициализация ncurses
  return res;
}