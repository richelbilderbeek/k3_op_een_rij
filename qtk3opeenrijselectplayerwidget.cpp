#include "qtk3opeenrijselectplayerwidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

#include "qtk3opeenrijresources.h"

ribi::koer::QtK3OpEenRijSelectPlayerWidget::QtK3OpEenRijSelectPlayerWidget(
  QWidget *parent)
  : QWidget(parent),
    m_black{BlackHairedGirl::marthe},
    m_blond{BlondeGirl::kathleen},
    m_red{RedHairedGirl::hanne},
    m_is_player_human{0b111}
{
  setFixedSize(2 * m_sprite_width, 3 * m_sprite_height);
  setCursor(Qt::PointingHandCursor);
}

void ribi::koer::QtK3OpEenRijSelectPlayerWidget::mousePressEvent(QMouseEvent * e)
{
  const QPoint pos = e->position().toPoint();
  const int col_index = pos.x() / m_sprite_width;
  const int row_index = pos.y() / m_sprite_height;
  if (col_index < 0 || col_index > 1 || row_index < 0 || row_index > 2) return;

  const bool was_human = m_is_player_human[row_index];
  const bool is_human = col_index == 0;
  m_is_player_human[row_index] = is_human;
  if (row_index == 0 && is_human && was_human) m_red = GetNext(m_red);
  if (row_index == 1 && is_human && was_human) m_black = GetNext(m_black);
  if (row_index == 2 && is_human && was_human) m_blond = GetNext(m_blond);
  update();
}

void ribi::koer::QtK3OpEenRijSelectPlayerWidget::paintEvent(QPaintEvent *)
{
  using R = QtK3OpEenRijResources;
  QPainter painter(this);

  painter.drawPixmap(0, 0 * m_sprite_height, QPixmap(R::GetPlayerFilename(m_is_player_human[0], m_red)));
  painter.drawPixmap(0, 1 * m_sprite_height, QPixmap(R::GetPlayerFilename(m_is_player_human[1], m_black)));
  painter.drawPixmap(0, 2 * m_sprite_height, QPixmap(R::GetPlayerFilename(m_is_player_human[2], m_blond)));

  for (int i = 0; i != 3; ++i)
  {
    painter.drawPixmap(
      m_sprite_width,
      i * m_sprite_height,
      QPixmap(m_is_player_human[i] ? R::GetComputerGreyFilename() : R::GetComputerFilename(i))
    );
  }
}
