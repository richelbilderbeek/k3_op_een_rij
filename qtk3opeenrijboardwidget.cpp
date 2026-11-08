#include "qtk3opeenrijboardwidget.h"

#include <algorithm>
#include <cassert>

#include <QMouseEvent>
#include <QPainter>

#include "qtk3opeenrijresources.h"

ribi::koer::QtK3OpEenRijBoardWidget::QtK3OpEenRijBoardWidget(
  const std::array<QString, 3>& player_filenames,
  const std::bitset<3>& is_player_human,
  const int n_cols,
  const int n_rows,
  QWidget *parent)
  : QWidget(parent),
    m_game(n_cols, n_rows),
    m_is_player_human{is_player_human},
    m_players{
      QPixmap(player_filenames[0]),
      QPixmap(player_filenames[1]),
      QPixmap(player_filenames[2])
    },
    m_empty(QtK3OpEenRijResources::GetEmptyFilename())
{
  //The board scales to the space available
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setCursor(Qt::PointingHandCursor);
}

void ribi::koer::QtK3OpEenRijBoardWidget::DoComputerMove()
{
  assert(IsComputerTurn());
  if (m_game.GetWinner() != con3::Winner::no_winner) return;
  m_game.DoMove(m_game.SuggestMove(m_is_player_human));
  update();
  emit moveDone();
}

bool ribi::koer::QtK3OpEenRijBoardWidget::IsComputerTurn() const noexcept
{
  return !m_is_player_human[static_cast<int>(m_game.GetActivePlayer())];
}

void ribi::koer::QtK3OpEenRijBoardWidget::mousePressEvent(QMouseEvent * e)
{
  if (IsComputerTurn()) return;
  if (m_game.GetWinner() != con3::Winner::no_winner) return;

  const QPoint pos = e->position().toPoint() - GetTopLeft();
  if (pos.x() < 0 || pos.y() < 0) return;
  const int x = pos.x() / GetSquareSize();
  const int y = pos.y() / GetSquareSize();
  if (!m_game.CanDoMove(x, y)) return;

  m_game.DoMove(x, y);
  update();
  emit moveDone();
}

void ribi::koer::QtK3OpEenRijBoardWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::SmoothPixmapTransform);
  const int size = GetSquareSize();
  const QPoint top_left = GetTopLeft();
  const int n_rows = m_game.GetRows();
  const int n_cols = m_game.GetCols();
  for (int y = 0; y != n_rows; ++y)
  {
    for (int x = 0; x != n_cols; ++x)
    {
      painter.drawPixmap(
        QRect(top_left + QPoint(x * size, y * size), QSize(size, size)),
        GetPixmap(m_game.GetSquare(x, y))
      );
    }
  }
}

int ribi::koer::QtK3OpEenRijBoardWidget::GetSquareSize() const noexcept
{
  return std::max(1, std::min(width() / m_game.GetCols(), height() / m_game.GetRows()));
}

QPoint ribi::koer::QtK3OpEenRijBoardWidget::GetSquareCenter(const int x, const int y) const noexcept
{
  const int size = GetSquareSize();
  return GetTopLeft() + QPoint(x * size + size / 2, y * size + size / 2);
}

QPoint ribi::koer::QtK3OpEenRijBoardWidget::GetTopLeft() const noexcept
{
  const int size = GetSquareSize();
  return QPoint(
    (width() - size * m_game.GetCols()) / 2,
    (height() - size * m_game.GetRows()) / 2
  );
}

QSize ribi::koer::QtK3OpEenRijBoardWidget::minimumSizeHint() const
{
  return QSize(m_game.GetCols() * 10, m_game.GetRows() * 10);
}

QSize ribi::koer::QtK3OpEenRijBoardWidget::sizeHint() const
{
  return QSize(m_game.GetCols() * sprite_size, m_game.GetRows() * sprite_size);
}

const QPixmap& ribi::koer::QtK3OpEenRijBoardWidget::GetPixmap(
  const con3::Square square) const noexcept
{
  switch (square)
  {
    case con3::Square::empty  : return m_empty;
    case con3::Square::player1: return m_players[0];
    case con3::Square::player2: return m_players[1];
    case con3::Square::player3: return m_players[2];
  }
  assert(!"Should not get here");
  return m_empty;
}
