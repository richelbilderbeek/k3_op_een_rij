#ifndef QTK3OPEENRIJSELECTPLAYERWIDGET_H
#define QTK3OPEENRIJSELECTPLAYERWIDGET_H

#include <bitset>

#include <QWidget>

#include "k3opeenrijnames.h"

namespace ribi {
namespace koer {

///Lets the user pick, per hair color, which girl plays
///and whether she is played by a human or by the computer.
///Clicking a colored girl again cycles to the next girl with that hair color.
class QtK3OpEenRijSelectPlayerWidget : public QWidget
{
  Q_OBJECT
public:
  explicit QtK3OpEenRijSelectPlayerWidget(QWidget *parent = nullptr);
  const std::bitset<3>& GetIsPlayerHuman() const noexcept { return m_is_player_human; }
  BlackHairedGirl GetBlack() const noexcept { return m_black; }
  BlondeGirl GetBlond() const noexcept { return m_blond; }
  RedHairedGirl GetRed() const noexcept { return m_red; }

protected:
  void mousePressEvent(QMouseEvent * e) override;
  void paintEvent(QPaintEvent *) override;

private:
  BlackHairedGirl m_black;
  BlondeGirl m_blond;
  RedHairedGirl m_red;
  std::bitset<3> m_is_player_human;

  static constexpr int m_sprite_height{50};
  static constexpr int m_sprite_width{50};
};

} //~namespace koer
} //~namespace ribi

#endif // QTK3OPEENRIJSELECTPLAYERWIDGET_H
