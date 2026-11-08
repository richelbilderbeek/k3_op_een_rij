#ifndef QTK3OPEENRIJBOARDWIDGET_H
#define QTK3OPEENRIJBOARDWIDGET_H

#include <array>
#include <bitset>

#include <QPixmap>
#include <QWidget>

#include "connectthreegame.h"

namespace ribi {
namespace koer {

///The playing field: draws the board and lets humans click to place a piece
class QtK3OpEenRijBoardWidget : public QWidget
{
  Q_OBJECT
public:
  QtK3OpEenRijBoardWidget(
    const std::array<QString, 3>& player_filenames,
    const std::bitset<3>& is_player_human,
    const int n_cols,
    const int n_rows,
    QWidget *parent = nullptr
  );

  ///Let the computer do its move. Only call when IsComputerTurn() is true
  void DoComputerMove();

  const con3::Game& GetGame() const noexcept { return m_game; }
  bool IsComputerTurn() const noexcept;

  static constexpr int sprite_size{50};

signals:
  ///Emitted after every valid move, by human or computer
  void moveDone();

protected:
  void mousePressEvent(QMouseEvent * e) override;
  void paintEvent(QPaintEvent *) override;

private:
  con3::Game m_game;
  const std::bitset<3> m_is_player_human;
  const std::array<QPixmap, 3> m_players;
  const QPixmap m_empty;

  const QPixmap& GetPixmap(const con3::Square square) const noexcept;
};

} //~namespace koer
} //~namespace ribi

#endif // QTK3OPEENRIJBOARDWIDGET_H
