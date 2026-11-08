#ifndef QTK3OPEENRIJMAINWINDOW_H
#define QTK3OPEENRIJMAINWINDOW_H

#include <array>

#include <QWidget>

#include "connectthreewinner.h"

class QLabel;
class QPushButton;
class QStackedWidget;
class QVBoxLayout;

namespace ribi {
namespace koer {

class QtK3OpEenRijBoardWidget;
class QtK3OpEenRijSelectPlayerWidget;

///The only window of the game.
///All screens (menu, instructions, about, game, winner) are pages
///of a QStackedWidget, instead of separate modal dialogs,
///because nested event loops (QDialog::exec) do not work
///in the WebAssembly version.
class QtK3OpEenRijMainWindow : public QWidget
{
  Q_OBJECT
public:
  explicit QtK3OpEenRijMainWindow(QWidget *parent = nullptr);

  static QString GetVersion();
  static QStringList GetVersionHistory();

  ///Number of columns and rows of the board
  static constexpr int n_cols{15};
  static constexpr int n_rows{8};

private:
  QStackedWidget * const m_stack;
  QWidget * m_menu_page;
  QWidget * m_instructions_page;
  QWidget * m_about_page;
  QWidget * m_game_page;
  QWidget * m_winner_page;
  QtK3OpEenRijSelectPlayerWidget * m_select;

  //Game page
  QVBoxLayout * m_board_holder;
  QtK3OpEenRijBoardWidget * m_board;
  std::array<QLabel *, 3> m_player_labels;
  std::array<QString, 3> m_player_filenames;
  std::array<QString, 3> m_player_grey_filenames;

  //Winner page
  QLabel * m_winner_image;
  QLabel * m_winner_text;

  QWidget * CreateMenuPage();
  QWidget * CreateInstructionsPage();
  QWidget * CreateAboutPage();
  QWidget * CreateGamePage();
  QWidget * CreateWinnerPage();

  void OnMoveDone();
  void ShowPage(QWidget * const page);
  void ShowWinner(const con3::Winner winner);
  void StartGame();
  void StopGame();
};

} //~namespace koer
} //~namespace ribi

#endif // QTK3OPEENRIJMAINWINDOW_H
