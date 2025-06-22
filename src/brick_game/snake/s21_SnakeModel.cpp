#include "s21_SnakeModel.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

namespace s21 {
GameInfo_t SnakeModel::updateCurrentState() {
  bool error = 0;
  if (game_info.field != nullptr) {
    error = move();
    if (!error) {
      ping();
    } else {
      clear();
    }
  }
  return game_info;
}

// return error
bool SnakeModel::move() {
  int error = 0;

  switch (head_direction) {
    case type_direction::Left:
      error = check_colision(head_coord.first, head_coord.second - 1);
      if (error != 1) {
        game_info.field[head_coord.first][--head_coord.second] =
            static_cast<int>(colors::COLOR_MAGENTA_);
      }
      break;
    case type_direction::Right:
      error = check_colision(head_coord.first, head_coord.second + 1);
      if (error != 1) {
        game_info.field[head_coord.first][++head_coord.second] =
            static_cast<int>(colors::COLOR_MAGENTA_);
      }
      break;
    case type_direction::Up:
      error = check_colision(head_coord.first - 1, head_coord.second);
      if (error != 1) {
        game_info.field[--head_coord.first][head_coord.second] =
            static_cast<int>(colors::COLOR_MAGENTA_);
      }
      break;
    case type_direction::Down:
      error = check_colision(head_coord.first + 1, head_coord.second);
      if (error != 1) {
        game_info.field[++head_coord.first][head_coord.second] =
            static_cast<int>(colors::COLOR_MAGENTA_);
      }
      break;
    default:
      break;
  }
  if (error == -1) {
    snake.push_back(std::pair<int, int>(head_coord.first, head_coord.second));
    error = false;
    game_info.score++;
    if (game_info.level < 10) {
      game_info.level = (game_info.score - 4) / 5 + 1;
      if (game_info.speed != 300)
        game_info.speed = 1300 - (game_info.level - 1) * 100;
    }
    std::pair<int, int> apple = get_new_pos_apple();
    game_info.field[apple.first][apple.second] =
        static_cast<int>(colors::COLOR_RED_);
  } else {
    snake.push_back(std::pair<int, int>(head_coord.first, head_coord.second));
    game_info.field[snake.front().first][snake.front().second] = 0;
    snake.pop_front();
  }
  return error;
}

// 0 - no error, -1 apple, 1 - error
int SnakeModel::check_colision(int y, int x) {
  int error = 0;
  if (x < 0 || x >= get_width() || y < 0 || y >= get_height()) {
    error = 1;
  } else if (game_info.field[y][x] == static_cast<int>(colors::COLOR_RED_)) {
    error = -1;
  } else {
    error = game_info.field[y][x] == static_cast<int>(colors::COLOR_MAGENTA_);
  }
  return error;
}

void SnakeModel::clear() {
  if (game_info.field) {
    for (unsigned int i = 0; i < 20; i++) {
      delete[] game_info.field[i];
      game_info.field[i] = nullptr;

      delete[] game_info.next[i];
      game_info.next[i] = nullptr;
    }
    delete[] game_info.field;
    game_info.field = nullptr;
    delete[] game_info.next;
    game_info.next = nullptr;
  }
  update_record();
}

void SnakeModel::update_record() {
  int old_record = get_record();
  if (old_record < game_info.score) {
    std::ofstream file;
    file.open("record.txt");
    if (file.is_open()) {
      int temp = game_info.score;
      file << temp;
      file.close();
    } else {
      std::cout << "no such file for record";
      std::cout.flush();
      throw std::exception();
    }
  }
}

int SnakeModel::get_record() {
  std::ifstream file;
  int record = -1;
  file.open("record.txt");
  if (file.is_open()) {
    file >> record;
    file.close();
  } else {
    std::cout << "no such file for record";
    std::cout.flush();
    throw std::exception();
  }
  return record;
}

void SnakeModel::ping() {
  if (game_info.score != 200) {
    for (int i = 0; i < get_height(); i++) {
      for (int j = 0; j < get_width(); j++) {
        game_info.next[i][j] = game_info.field[i][j];
      }
    }
  }
}

std::pair<int, int> SnakeModel::get_new_pos_apple() {
  std::vector<std::pair<int, int>> avaliable_coord;
  for (int i = 0; i < get_height(); i++) {
    for (int j = 0; j < get_width(); j++) {
      if (!game_info.field[i][j]) {
        avaliable_coord.push_back(std::pair<int, int>(i, j));
      }
    }
  }
  return avaliable_coord[rand() % avaliable_coord.size()];
}

void SnakeModel::init_snake() {
  for (int i = 0; i < 4; i++) {
    game_info.field[get_height() / 2][get_width() / 2 - 2 + i] =
        static_cast<int>(colors::COLOR_MAGENTA_);
    snake.push_back(
        std::pair<int, int>(get_height() / 2, get_width() / 2 - 2 + i));
  }
  head_coord = {get_height() / 2, get_width() / 2 + 1};
}

void SnakeModel::rotate(UserAction_t action) {
  switch (head_direction) {
    case type_direction::Left:
      if (action != Right) {
        head_direction = static_cast<type_direction>(action);
      }
      break;
    case type_direction::Right:
      if (action != Left) {
        head_direction = static_cast<type_direction>(action);
      }
      break;
    case type_direction::Up:
      if (action != Down) {
        head_direction = static_cast<type_direction>(action);
      }
      break;
    case type_direction::Down:
      if (action != Up) {
        head_direction = static_cast<type_direction>(action);
      }
      break;
    default:
      break;
  }
}
void SnakeModel::set_max_speed() {
  if (game_info.speed == 300) {
    game_info.speed = 1300 - (game_info.level - 1) * 100;
  } else {
    game_info.speed = 300;
  }
}

SnakeModel::SnakeModel() {
  game_info.field = new int*[20];
  game_info.next = new int*[20];
  if (game_info.field && game_info.next) {
    for (unsigned int i = 0; i < 20; i++) {
      game_info.field[i] = new int[10];
      game_info.next[i] = new int[10];
      if (!game_info.field || !game_info.next) {
        std::cout << "no memory error";
        std::cout.flush();
        throw std::exception();
      }
    }
  } else {
    std::cout << "no memory error";
    std::cout.flush();
    throw std::exception();
  }

  for (int i = 0; i < get_height(); i++) {
    for (int j = 0; j < get_width(); j++) {
      game_info.field[i][j] = 0;
      game_info.next[i][j] = 0;
    }
  }
  init_snake();
  srand(time(NULL));
  std::pair<int, int> apple = get_new_pos_apple();
  game_info.field[apple.first][apple.second] =
      static_cast<int>(colors::COLOR_RED_);
  min_speed = 1300;
  game_info.high_score = get_record();
  game_info.score = 4;
  game_info.speed = min_speed;
  game_info.level = 1;
  head_direction = type_direction::Right;
  game_info.pause = false;
};

SnakeModel::~SnakeModel() { clear(); };

}  // namespace s21
/*
typedef struct {
    int **field;
    int **next;
    int score;
    int high_score;
    int level;
    int speed;
    int pause;
} GameInfo_t;
 */
/*

Не учитывается хвост для очистки
Не учитывается попадание на яблоко
Не учитывается удар об стенку
Не учитывается удар об себя
*/