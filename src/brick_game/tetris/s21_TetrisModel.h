#ifndef TETRIS_MODEL_H
#define TETRIS_MODEL_H
#include "../brick_game.h"

#define ALLOC_MEM(field_)                                                      \
  game_info.field_ = (int **)malloc(sizeof(int *) * get_height());             \
  if (game_info.field_) {                                                      \
    for (int i = 0; i < get_height(); i++) {                                   \
      if (!(game_info.field_[i] = (int *)malloc(sizeof(int) * get_width()))) { \
        exit(1);                                                               \
      }                                                                        \
    }                                                                          \
  } else {                                                                     \
    exit(1);                                                                   \
  }

#define FREE_MEM(field_)                   \
  for (int i = 0; i < get_height(); i++) { \
    free(game_info.field_[i]);             \
    game_info.field_[i] = nullptr;         \
  }                                        \
  free(game_info.field_);                  \
  game_info.field_ = nullptr;

namespace s21 {

class TetrisModel : public Brick_game {
 private:
  struct Figure_t {
    bool figure[4][4];
    int color;
    Figure_t &operator=(Figure_t &other) {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          figure[i][j] = other.figure[i][j];
        }
      }
      color = other.color;
      return *this;
    }
    Figure_t() {
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          figure[i][j] = 0;
        }
      }
      color = 0;
    }
  };

  struct Block_t {
    int x, y;  // Позиция на поле
    Figure_t figure;
    int rotate;  // 0 - no rotate, 1 - 90, 2 - 180, 3 - 270
  };
  /**
   * @brief Вспомогательная функция, для заполнения начальных значений фигур -
   * положения и цвета
   */
  void init_figures();
  /**
   * @brief Фукнция, для инициализации нового блока, после того как старый блок
   * упал
   */
  void init_block();
  /**
   * @brief Вспомогательная функция для проверки фигур на выход за пределы поля
   * или попадания в точку, в которой уже лежит фигура
   * @return Возвращается ошибка (0 - нет ошибки, 1 - ошибка), где ошибка -
   * выход за пределы поля или попадания в точку, в которой уже лежит фигура
   */
  int check_collision();
  /**
   * @brief Вспомогательная функция для для того, чтобы упавший блок "прилип" к
   * полю
   * @return Возвращается ошибка (0 - нет ошибки, 1 - ошибка), где ошибка -
   * выход за пределы поля
   */
  int merge_block();
  /**
   * @brief Вспомогательная функция для отчистки линии(линий), которая(ые)
   * заполена(ы)
   */
  void clear_lines();

  /**
   * @brief Основная функция, для ввода
   * @param action - Действие, которое хочет совершить пользователь
   * @param bool - зажата ли кнопка, которой пользователь совершает действие
   */
  void ping();
  /**
   * @brief Вспомогательная функция, для получения высоты фигуры
   * @return Высота фигуры
   */
  int get_height_figure();
  /**
   * @brief Вспомогательная функция, которая позволяет получить рекорд из файла
   * @return рекорд
   */
  int get_record();
  /**
   * @brief Вспомогательная функция, которая позволяет изменить рекорд в файле,
   * если текущий рекорд выше максимального
   */
  void update_record();

  /**
   * @brief геттер для информации об игре
   */

  void clear();  // новая функция, где стоит *!, там надо её вставить
  Block_t current;
  Figure_t figures[7];
  int min_speed = 1100;
  int next_figure_number = -1;
  int **next_figure;

 public:
  GameInfo_t updateCurrentState() override;

  void move(bool is_left);
  /**
   * @brief Функция, которая моментально опускает фигуру вниз
   */
  void blink_down();
  /**
   * @brief Функция, которая поворачивает фигуру
   */
  void rotate_block();
  int **get_next();
  TetrisModel();
  ~TetrisModel();
};
};  // namespace s21
#endif