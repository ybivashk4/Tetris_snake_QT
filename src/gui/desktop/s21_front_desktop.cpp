#include "s21_front_desktop.h"

#define CELL_SIZE 30  // Размер клетки
#define WIDTH 10
#define HEIGHT 20

namespace s21 {

// Конструктор
QtFront::QtFront(char game, QWidget *parent) : QWidget(parent), data(nullptr) {
  if (game == 't' || game == 'T') {
    data = new TetrisController();
  } else if (game == 's' || game == 'S') {
    data = new SnakeController();
  } else {
    throw std::runtime_error("Unknown game, t(T) for Tetris, s(S) for Snake");
  }
  data->userInput(Start, false);

  scoreLabel = new QLabel(this);
  highScoreLabel = new QLabel(this);
  levelLabel = new QLabel(this);
  speedLabel = new QLabel(this);

  scoreLabel->setGeometry(10 * cellSize + 20, 50, infoWidth - 40, 30);
  highScoreLabel->setGeometry(10 * cellSize + 20, 90, infoWidth - 40, 30);
  levelLabel->setGeometry(10 * cellSize + 20, 130, infoWidth - 40, 30);
  speedLabel->setGeometry(10 * cellSize + 20, 170, infoWidth - 40, 30);

  scoreLabel->setStyleSheet("color: pink; font-size: 18px;");
  highScoreLabel->setStyleSheet("color: pink; font-size: 18px;");
  levelLabel->setStyleSheet("color: pink; font-size: 18px;");
  speedLabel->setStyleSheet("color: pink; font-size: 18px;");

  setFixedSize(WIDTH * CELL_SIZE + infoWidth,
               HEIGHT * CELL_SIZE);  // Устанавливаем размер окна
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &QtFront::updateGame);
  timer->start(data->get_current_state().speed /
               2);  // Интервал обновления, например, 200 мс
}

void QtFront::updateGame() {
  int newSpeed =
      data->get_current_state().speed / 2;  // Получаем текущую скорость
  if (timer->interval() != newSpeed) {      // Изменяем только если отличается
    timer->setInterval(newSpeed);
  }

  data->updateCurrentState();  // Двигаем змейку или тетрис
  update();                    // Перерисовываем окно
}

// Деструктор
QtFront::~QtFront() { delete data; }

// Отрисовка окна
void QtFront::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  drawGrid(painter);
  updateLabels();
  drawGame(painter);
  drawNext(painter);
}

void QtFront::updateLabels() {
  GameInfo_t game_info = data->get_current_state();

  scoreLabel->setText("Score: " + QString::number(game_info.score));
  highScoreLabel->setText("High Score: " +
                          QString::number(game_info.high_score > game_info.score
                                              ? game_info.high_score
                                              : game_info.score));
  levelLabel->setText("Level: " + QString::number(game_info.level));
  speedLabel->setText("Speed: " + QString::number(game_info.speed / 2));
}

// Рисуем сетку
void QtFront::drawGrid(QPainter &painter) {
  painter.setPen(Qt::gray);
  for (int x = 0; x <= WIDTH; x++)
    painter.drawLine(x * CELL_SIZE, 0, x * CELL_SIZE, HEIGHT * CELL_SIZE);
  for (int y = 0; y <= HEIGHT; y++)
    painter.drawLine(0, y * CELL_SIZE, WIDTH * CELL_SIZE, y * CELL_SIZE);
}

// Рисуем элементы игры
void QtFront::drawGame(QPainter &painter) {
  if (!data) return;
  GameInfo_t game_info = data->get_current_state();
  if (!game_info.next) {
    return;
  };

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      // qDebug() << "game_info.next[" << y << "][" << x << "] = " <<
      // game_info.next[y][x];
      if (game_info.next[y][x]) {
        painter.setBrush(
            QColor::fromHsv((game_info.next[y][x] * 30) % 360, 255, 255));
        painter.drawRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
      }
    }
  }
}
void QtFront::drawNext(QPainter &painter) {
  GameInfo_t game_info = data->get_current_state();
  if (!game_info.next) {
    return;
  };
  if (data->get_next()) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        if (data->get_next()[i][j]) {
          painter.setBrush(
              QColor::fromHsv((data->get_next()[i][j] * 30) % 360, 255, 255));
          painter.drawRect(10 * cellSize + 20 + j * CELL_SIZE,
                           210 + i * CELL_SIZE, CELL_SIZE, CELL_SIZE);
        }
      }
    }
  }
}

// Обработка нажатий клавиш
void QtFront::keyPressEvent(QKeyEvent *event) {
  if (!data) return;
  switch (event->key()) {
    case Qt::Key_Left:
      data->userInput(Left, false);
      update();
      break;
    case Qt::Key_Right:
      data->userInput(Right, false);
      update();
      break;
    case Qt::Key_Down:
      data->userInput(Down, false);
      update();
      break;
    case Qt::Key_Up:
      data->userInput(Up, false);
      update();
      break;
    case Qt::Key_R:
      data->userInput(Action, false);
      update();
      break;
    case Qt::Key_P:
      data->userInput(Pause, false);
      update();
      break;
    case Qt::Key_Q:
      data->userInput(Terminate, false);
      delete data;
      data = nullptr;
      close();
      break;
  }
  // Обновляем экран после нажатия
}

}  // namespace s21
