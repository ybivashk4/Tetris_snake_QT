#ifndef TETRIS_BASE_H
#define TETRIS_BASE_H

namespace s21 {
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

enum class colors {
  COLOR_RED_ = 3,
  COLOR_GREEN_ = 5,
  COLOR_YELLOW_ = 6,
  COLOR_BLUE_ = 4,
  COLOR_MAGENTA_ = 7,
  COLOR_ORANGE_ = 1,
  COLOR_PINK_ = 2
};

class Controller {
 public:
  virtual GameInfo_t updateCurrentState() {
    GameInfo_t none = {0, 0, 0, 0, 0, 0, 0};
    return none;
  };

  virtual void userInput(UserAction_t action, bool hold) {
    int temp = static_cast<int>(action) - 1;
    if (hold) {
      temp -= 1;
      return;
    }
  };

  virtual GameInfo_t get_current_state() {
    GameInfo_t none = {0, 0, 0, 0, 0, 0, 0};
    return none;
  };

  virtual int **get_next() { return nullptr; };
  virtual ~Controller() = default;
};

class Brick_game {
 public:
  virtual GameInfo_t updateCurrentState() {
    GameInfo_t none = {0, 0, 0, 0, 0, 0, 0};
    return none;
  };

  GameInfo_t get_current_state() { return game_info; };

  int get_width() { return width; };
  int get_height() { return height; };
  Brick_game() : width(10), height(20) { ; }
  void switch_pause() { game_info.pause = !game_info.pause; };
  virtual ~Brick_game() = default;
  GameInfo_t game_info;

 private:
  int width;
  int height;
};
}  // namespace s21
#endif