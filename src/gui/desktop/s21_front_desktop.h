#ifndef QTFRONT_H
#define QTFRONT_H

#ifdef timeout
#undef timeout
#endif

#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/snake/s21_SnakeController.h"
#include "../../brick_game/tetris/s21_TetrisController.h"

namespace s21 {
class QtFront : public QWidget {
  Q_OBJECT

 public:
  explicit QtFront(char game, QWidget *parent = nullptr);
  ~QtFront();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

 private slots:
  void updateGame();

 private:
  Controller *data;
  QTimer *timer;
  const int cellSize = 30;
  const int infoWidth = 150;  // Ширина правой панели
  QLabel *scoreLabel;
  QLabel *highScoreLabel;
  QLabel *levelLabel;
  QLabel *speedLabel;

  void drawGame(QPainter &painter);
  void drawGrid(QPainter &painter);
  void drawNext(QPainter &painter);
  void updateLabels();
};
}  // namespace s21

#endif  // QTFRONT_H

/**
 *
 * Ещё label для next_figure
 * Изменение скорости
 *
 */