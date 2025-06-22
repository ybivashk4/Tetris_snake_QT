#include "s21_TetrisModel.h"

#include <fstream>
#include <iostream>

namespace s21 {
void s21::TetrisModel::rotate_block() {
  if (game_info.field != nullptr) {
    if (current.x >= get_width() - 1) {
      current.x--;
    }
    if (current.x == 0) {
      current.x++;
    }
    current.rotate = (current.rotate + 1) % 4;
    int tmp[4][4] = {0};
    // поворот на 90
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        tmp[i][j] = current.figure.figure[i][j];
      }
    }
    for (int i = 0; i < 4; i++) {
      for (int j = 3; j >= 0; j--) {
        current.figure.figure[j][i] = tmp[i][3 - j];
      }
    }

    if (current.rotate == 0) current.x -= 2;
    if (current.rotate == 2) current.x += 2;

    if (current.rotate == 1) current.y += 2;
    if (current.rotate == 3) current.y -= 2;

    if (check_collision()) {
      current.y--;
      merge_block();
    }
  }
}

GameInfo_t TetrisModel::updateCurrentState() {
  if (game_info.field != nullptr && !game_info.pause) {
    current.y++;
    int error = 0;
    if (check_collision()) {
      current.y--;
      if (current.y - get_height_figure() < 0) {
        current.y += get_height_figure();
        if (current.y >= get_height()) {
          current.y -= get_height_figure();
          error = merge_block();
        }
      } else {
        error = merge_block();
      }
    }
    if (!error) ping();
    game_info.high_score =
        (game_info.score > game_info.high_score ? game_info.score
                                                : game_info.high_score);
  }

  return game_info;
}

void TetrisModel::update_record() {
  int old_record = get_record();
  if (old_record < game_info.high_score) {
    FILE *file = fopen("record.txt", "w");
    fprintf(file, "%d", game_info.high_score);
    fclose(file);
  }
}

int TetrisModel::get_record() {
  int res = 0;
  FILE *file = NULL;
  file = fopen("record.txt", "r");
  if (file) {
    fscanf(file, "%d", &res);
    fclose(file);
  } else {
    printf(
        "can't open file with records, \
                please add \"record.txt\" file with \"0\" value inside\n");
    res = -1;
  }
  return res;
}
int TetrisModel::get_height_figure() {
  if (!game_info.field) return 0;

  int res = 0;
  bool stop = false;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4 && !stop; j++) {
      if (current.figure.figure[i][j]) {
        res = i + 1;
        stop = true;
      }
    }
    stop = false;
  }
  return res;
}

void TetrisModel::ping() {
  if (!game_info.field) return;

  for (int i = 0; i < get_height(); i++) {
    for (int j = 0; j < get_width(); j++) {
      if (current.x - j <= 3 && current.x - j >= 0 && current.y - i <= 3 &&
          current.y - i >= 0 && game_info.field[i][j] == 0 &&
          current.figure.figure[current.y - i][current.x - j]) {
        game_info.next[i][j] = current.figure.color;
      } else {
        game_info.next[i][j] = game_info.field[i][j];
      }
    }
  }
}
void TetrisModel::blink_down() {
  if (!game_info.field) return;
  while (!check_collision()) {
    current.y++;
  }
  current.y--;
  if (current.y < get_height()) {
    if (merge_block()) {
      // userInput(Terminate, 0); *!
      clear();
    } else {
      ping();
    }
  }
}

void TetrisModel::clear_lines() {
  if (!game_info.field) return;

  int count_lines = 0;
  for (int row = get_height() - 1; row >= 0; row--) {
    int full = 1;
    // Проверяем заполнена ли строка
    for (int col = 0; col < get_width(); col++)
      if (!game_info.field[row][col]) full = 0;

    if (full) {
      // Сдвигаем все строки выше вниз
      for (int r = row; r > 0; r--)
        for (int c = 0; c < get_width(); c++)
          game_info.field[r][c] = game_info.field[r - 1][c];

      row++;  // Повторно проверяем текущую строку
      count_lines++;
    }
  }
  int old_score = game_info.score % 600;
  switch (count_lines) {
    case 0:
      break;
    case 1:
      game_info.score += 100;
      old_score += 100;
      break;
    case 2:
      game_info.score += 300;
      old_score += 300;
      break;
    case 3:
      game_info.score += 700;
      old_score += 700;
      break;
    default:
      game_info.score += 1500;
      old_score += 1500;
      break;
  }
  game_info.level += (old_score / 600 % 10);
  game_info.speed = min_speed - 100 * (game_info.level - 1);
}
int TetrisModel::merge_block() {
  if (!game_info.field) return 1;
  int error = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (current.figure.figure[i][j]) {
        int y = current.y - i;
        int x = current.x - j;
        if (y >= 0 && y < get_height() && x >= 0 && x < get_width() &&
            !game_info.field[y][x])
          game_info.field[y][x] = current.figure.color;
        else {
          error = 1;
        }
      } else if (current.figure.figure[i][j] && game_info.field[i][j]) {
        error = 1;
      }
    }
  }
  if (!error) {
    clear_lines();
    init_block();
  } else {
    // userInput(Terminate, false); *!
    clear();
  }
  return error;
}

int TetrisModel::check_collision() {
  if (!game_info.field) return 1;
  int error = 0;
  for (int i = 0; i < 4 && !error; i++) {
    for (int j = 0; j < 4 && !error; j++) {
      if (current.figure.figure[i][j]) {
        int new_x = current.x - j;
        int new_y = current.y - i;
        // Выход за пределы поля
        if (new_x < 0 || new_x >= get_width()) error = 1;
        if (new_y < 0 || new_y >= get_height()) error = 1;

        // попадание в заполненую фигуру поля
        if (!error && game_info.field[new_y][new_x]) error = 1;
      }
    }
  }
  return error;
}

void TetrisModel::init_block() {
  current.x = get_width() / 2 - 1;  // Стартовая позиция по центру
  current.y = 1;
  current.rotate = 0;
  int cur = next_figure_number;
  switch (cur) {
    case 0:
      current.figure = figures[0];
      break;
    case 1:
      current.figure = figures[1];
      break;
    case 2:
      current.figure = figures[2];
      break;
    case 3:
      current.figure = figures[3];
      break;
    case 4:
      current.figure = figures[4];
      break;
    case 5:
      current.figure = figures[5];
      break;
    case 6:
      current.figure = figures[6];
      break;
  }

  next_figure_number = rand() % 7;
}

void TetrisModel::init_figures() {
  figures[0].color = static_cast<int>(colors::COLOR_RED_);
  figures[0].figure[0][0] = 1;
  figures[0].figure[0][1] = 1;
  figures[0].figure[0][2] = 1;
  figures[0].figure[0][3] = 1;

  figures[1].color = static_cast<int>(colors::COLOR_ORANGE_);
  figures[1].figure[0][0] = 1;
  figures[1].figure[0][1] = 1;
  figures[1].figure[0][2] = 1;
  figures[1].figure[1][0] = 1;

  figures[2].color = static_cast<int>(colors::COLOR_YELLOW_);
  figures[2].figure[0][0] = 1;
  figures[2].figure[0][1] = 1;
  figures[2].figure[0][2] = 1;
  figures[2].figure[1][2] = 1;

  figures[3].color = static_cast<int>(colors::COLOR_PINK_);
  figures[3].figure[0][0] = 1;
  figures[3].figure[1][0] = 1;
  figures[3].figure[0][1] = 1;
  figures[3].figure[1][1] = 1;

  figures[4].color = static_cast<int>(colors::COLOR_GREEN_);
  figures[4].figure[0][0] = 1;
  figures[4].figure[0][1] = 1;
  figures[4].figure[1][1] = 1;
  figures[4].figure[1][2] = 1;

  figures[5].color = static_cast<int>(colors::COLOR_BLUE_);
  figures[5].figure[0][0] = 1;
  figures[5].figure[0][1] = 1;
  figures[5].figure[0][2] = 1;
  figures[5].figure[1][1] = 1;

  figures[6].color = static_cast<int>(colors::COLOR_MAGENTA_);
  figures[6].figure[1][0] = 1;
  figures[6].figure[1][1] = 1;
  figures[6].figure[0][1] = 1;
  figures[6].figure[0][2] = 1;
}
void TetrisModel::clear() {
  if (game_info.field) {
    FREE_MEM(field);
    FREE_MEM(next);
  }
  if (next_figure) {
    for (int i = 0; i < 4; ++i) delete[] next_figure[i];
    delete[] next_figure;
    next_figure = nullptr;
  }

  current.figure.color = -1;
  update_record();
}
TetrisModel::TetrisModel() {
  init_figures();
  current.rotate = 0;
  current.x = 0;
  current.y = 0;
  current.figure.color = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      current.figure.figure[i][j] = 0;
    }
  }

  ALLOC_MEM(field);
  ALLOC_MEM(next);
  for (int i = 0; i < get_height(); i++) {
    for (int j = 0; j < get_width(); j++) {
      game_info.field[i][j] = 0;
      game_info.next[i][j] = 0;
    }
  }
  srand(time(NULL));
  next_figure_number = rand() % 7;

  init_block();
  game_info.high_score = get_record();
  game_info.score = 0;
  game_info.speed = min_speed;
  game_info.level = 1;
  game_info.pause = 0;

  next_figure = new int *[4];
  for (int i = 0; i < 4; i++) next_figure[i] = new int[4];
};
TetrisModel::~TetrisModel() { clear(); };
void TetrisModel::move(bool is_left) {
  if (!game_info.field) return;
  is_left ? current.x-- : current.x++;
  if (check_collision()) is_left ? current.x++ : current.x--;
}

int **TetrisModel::get_next() {
  if (!game_info.field) return nullptr;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (figures[next_figure_number].figure[i][j]) {
        next_figure[i][j] = figures[next_figure_number].color;
      } else {
        next_figure[i][j] = 0;
      }
    }
  }

  return next_figure;
}
}  // namespace s21
