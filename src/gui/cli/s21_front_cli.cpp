#include "s21_front_cli.h"

#include <ncurses.h>
#ifdef scroll
#undef scroll
#endif
#include <cstdlib>
#include <ctime>
#include <iostream>
#define HEIGHT 20
#define WIDTH 10
namespace s21 {
void CLIFront::draw_border() {
  // Рисуем рамку вокруг игрового поля
  for (int y = 1; y < HEIGHT + 1; y++) {
    mvprintw(y, WIDTH * 2 + 1, "|");  // Правая граница
    mvprintw(y, 0, "|");              // Левая граница
  }
  for (int x = 1; x < WIDTH * 2 + 1; x++) {
    mvprintw(0, x, "-");           // Верхняя граница
    mvprintw(HEIGHT + 1, x, "-");  // Нижняя граница
  }
  // Углы рамки
  mvprintw(0, 0, "+");
  mvprintw(0, WIDTH * 2 + 1, "+");
  mvprintw(HEIGHT + 1, 0, "+");
  mvprintw(HEIGHT + 1, WIDTH * 2 + 1, "+");
}

int CLIFront::draw() {
  int error = 0;
  clear();  // Очищаем экран
  // Рисуем поле
  GameInfo_t game_info = data->updateCurrentState();
  if (game_info.next == NULL) {
    error = 1;
  }
  if (!error) {
    draw_border();
    for (int y = 0; y < HEIGHT && !error; y++) {
      for (int x = 0; x < WIDTH * 2 && !error; x += 2) {
        if (game_info.next[y][x / 2]) {
          attron(COLOR_PAIR(game_info.next[y][x / 2]));
          mvprintw(y + 1, x + 1, "[");
          mvprintw(y + 1, x + 2, "]");
          attroff(COLOR_PAIR(game_info.next[y][x / 2]));
        } else {
          attron(COLOR_PAIR(8));
          mvprintw(y + 1, x + 1, ".");
          mvprintw(y + 1, x + 2, " ");
          attroff(COLOR_PAIR(8));
        }
      }
    }
    // Вывод счета
    // добавить паузу, скорость
    mvprintw(HEIGHT + 2, 0, "Score: %d", game_info.score);
    mvprintw(HEIGHT + 3, 0, "Record is %d",
             (game_info.high_score > game_info.score ? game_info.high_score
                                                     : game_info.score));
    mvprintw(HEIGHT + 4, 0, "level is %d", game_info.level);
    mvprintw(HEIGHT + 5, 0, "pause is %d", game_info.pause);
    mvprintw(HEIGHT + 6, 0, "speed is %d", game_info.speed);
    mvprintw(HEIGHT + 7, 0, "controllers: arrows, r to rotate, Q to exit");
    int color = -1;
    int** next = data->get_next();
    if (next) {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          if ((next)[i][j]) {
            color = (next)[i][j];
          }
        }
      }
      attron(COLOR_PAIR(color));
      for (int i = HEIGHT + 8; i < HEIGHT + 12; i++) {
        for (int j = 0; j < 4; j++) {
          if (!(next)[i - (HEIGHT + 8)][j])
            mvprintw(i, j, ".");
          else
            mvprintw(i, j, "#");
        }
      }
      attroff(COLOR_PAIR(color));
    }

    refresh();
  }

  return error;
}

void CLIFront::init_ncurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);  // Скрыть курсор

  // Инициализация цветов
  start_color();
  init_color(100, 1000, 500, 0);    // Оранжевый
  init_color(101, 1000, 700, 800);  // Розовый
  init_pair(1, 100, COLOR_BLACK);
  init_pair(2, 101, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_GREEN, COLOR_BLACK);
  init_pair(6, COLOR_YELLOW, COLOR_BLACK);
  init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(8, COLOR_BLACK, COLOR_BLACK);
  srand(time(0));
  data->userInput(Start, 0);
  printw("click any button");
}

}  // namespace s21
