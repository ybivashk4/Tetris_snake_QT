#ifndef SNAKE_MODEL_H
#define SNAKE_MODEL_H

#include <deque>
#include <utility>

#include "../brick_game.h"
namespace s21 {
class SnakeModel : public Brick_game {
 private:
  enum class type_direction { Left = 3, Right = 4, Down = 6, Up = 5 };

  int min_speed;
  type_direction head_direction;
  std::pair<int, int> head_coord;
  std::deque<std::pair<int, int>> snake;

  std::pair<int, int> get_new_pos_apple();
  int get_record();
  void init_snake();
  bool move();
  void clear();
  void update_record();
  void ping();
  int check_colision(int x, int y);

 public:
  GameInfo_t updateCurrentState() override;
  SnakeModel();
  ~SnakeModel();
  void rotate(UserAction_t action);
  void set_max_speed();
};
}  // namespace s21
#endif
/*
Спавн по центру ([height/2][width/2-2])
4 клавиши - стрелки, регает только лево/право относительно текущего положения
яблоки генерируются случайно, на незанятых позициях
начальная длина - 4 пикселя (длина = счёту, счёт = 4)


data->move(action);
data->set_max_speed();
*/