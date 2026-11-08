#include "qtk3opeenrijmainwindow.h"

#include <cassert>

#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>

#include "qtk3opeenrijboardwidget.h"
#include "qtk3opeenrijresources.h"
#include "qtk3opeenrijselectplayerwidget.h"

namespace {

const char * const style_sheet = R"(
#main {
  background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,
    stop: 0 #bbf, stop: 0.2 #bbf, stop: 0.5 #bfb, stop: 0.8 #fbb, stop: 1 #fbb);
}
QScrollArea, QScrollArea > QWidget > QWidget { background: transparent; }
QLabel { color: #000; font-size: 20px; }
QLabel#title { font-size: 32px; font-weight: bold; color: #ED76AC; }
QPushButton {
  font-size: 24px;
  color: #000;
  padding: 2px 8px;
  background-color: qradialgradient(cx: 0.5, cy: 0.5, fx: 0.5, fy: 0.5, radius: 0.5,
    stop: 0 #fff, stop: 1 #aaa);
  border: 1px solid black;
}
QPushButton:pressed { background-color: #ddd; }
)";

QLabel * CreateImageLabel(const QString& filename)
{
  QLabel * const label = new QLabel;
  label->setPixmap(QPixmap(filename));
  label->setAlignment(Qt::AlignCenter);
  return label;
}

QLabel * CreateTextLabel(const QString& text, const Qt::Alignment alignment = Qt::AlignLeft)
{
  QLabel * const label = new QLabel(text);
  label->setAlignment(alignment);
  label->setWordWrap(true);
  return label;
}

///Put a page in the center, so it looks good in a big browser window too
QWidget * Centered(QWidget * const content)
{
  QWidget * const page = new QWidget;
  QGridLayout * const layout = new QGridLayout(page);
  layout->addWidget(content, 0, 0, Qt::AlignCenter);
  return page;
}

///A scroll area that wants to be as big as its content,
///so the desktop window fits the page, yet the page can be scrolled
///when the screen (e.g. the web page) is too small
struct FittingScrollArea : public QScrollArea
{
  explicit FittingScrollArea(QWidget * const content)
  {
    setWidget(Centered(content));
    setWidgetResizable(true);
    setFrameShape(QFrame::NoFrame);
  }
  QSize sizeHint() const override
  {
    //Ask for a bit more, to prevent scroll bars when everything fits
    return widget()->sizeHint() + QSize(24, 24);
  }
};

} //~namespace

ribi::koer::QtK3OpEenRijMainWindow::QtK3OpEenRijMainWindow(QWidget *parent)
  : QWidget(parent),
    m_stack{new QStackedWidget},
    m_menu_page{nullptr},
    m_instructions_page{nullptr},
    m_about_page{nullptr},
    m_game_page{nullptr},
    m_winner_page{nullptr},
    m_select{nullptr},
    m_board_holder{nullptr},
    m_board{nullptr},
    m_player_labels{},
    m_player_filenames{},
    m_player_grey_filenames{},
    m_winner_image{nullptr},
    m_winner_text{nullptr}
{
  setObjectName("main");
  setAttribute(Qt::WA_StyledBackground);
  setStyleSheet(style_sheet);
  setWindowTitle("K3 Op Een Rij");

  m_menu_page = CreateMenuPage();
  m_instructions_page = CreateInstructionsPage();
  m_about_page = CreateAboutPage();
  m_game_page = CreateGamePage();
  m_winner_page = CreateWinnerPage();
  for (QWidget * const page: { m_menu_page, m_instructions_page, m_about_page, m_game_page, m_winner_page })
  {
    m_stack->addWidget(page);
  }

  QVBoxLayout * const layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_stack);

  ShowPage(m_menu_page);
}

QWidget * ribi::koer::QtK3OpEenRijMainWindow::CreateMenuPage()
{
  QWidget * const content = new QWidget;
  QHBoxLayout * const layout = new QHBoxLayout(content);

  QVBoxLayout * const buttons = new QVBoxLayout;
  QPushButton * const button_start = new QPushButton("&Begin spel");
  QPushButton * const button_instructions = new QPushButton("&Handleiding");
  QPushButton * const button_about = new QPushButton("&Over");
  button_start->setObjectName("button_start");
  buttons->addWidget(button_start);
  buttons->addWidget(button_instructions);
  buttons->addWidget(button_about);
  #ifndef Q_OS_WASM
  //A web page cannot be quit
  QPushButton * const button_quit = new QPushButton("&Stoppen");
  buttons->addWidget(button_quit);
  connect(button_quit, &QPushButton::clicked, this, &QWidget::close);
  #endif
  layout->addLayout(buttons);

  m_select = new QtK3OpEenRijSelectPlayerWidget;
  layout->addWidget(m_select, 0, Qt::AlignTop);

  connect(button_start, &QPushButton::clicked, this, &QtK3OpEenRijMainWindow::StartGame);
  connect(button_instructions, &QPushButton::clicked, this, [this]() { ShowPage(m_instructions_page); });
  connect(button_about, &QPushButton::clicked, this, [this]() { ShowPage(m_about_page); });
  return Centered(content);
}

QWidget * ribi::koer::QtK3OpEenRijMainWindow::CreateInstructionsPage()
{
  QWidget * const content = new QWidget;
  QGridLayout * const layout = new QGridLayout(content);
  layout->addWidget(CreateTextLabel("Bij K3 Op Een Rij moet je drie op een rij krijgen, op een rechte lijn:"), 0, 0, 1, 3);
  layout->addWidget(CreateTextLabel("Goed", Qt::AlignCenter), 1, 0);
  layout->addWidget(CreateTextLabel("Goed", Qt::AlignCenter), 1, 1);
  layout->addWidget(CreateTextLabel("Fout", Qt::AlignCenter), 1, 2);
  layout->addWidget(CreateImageLabel(":/images/K3OpEenRijInstructions1.png"), 2, 0);
  layout->addWidget(CreateImageLabel(":/images/K3OpEenRijInstructions2.png"), 2, 1);
  layout->addWidget(CreateImageLabel(":/images/K3OpEenRijInstructions3.png"), 2, 2);
  layout->addWidget(CreateTextLabel(
    "Klik in het menu op een meisje om een ander meisje met dezelfde haarkleur te kiezen. "
    "Klik op de computer om haar door de computer te laten spelen. "
    "Om bijvoorbeeld met Kathleen te spelen, klik op Josje:"), 3, 0, 1, 3);
  {
    QHBoxLayout * const choose = new QHBoxLayout;
    choose->addStretch();
    choose->addWidget(CreateImageLabel(":/images/K3OpEenRijKiesJosje.png"));
    choose->addWidget(CreateTextLabel("→", Qt::AlignCenter));
    choose->addWidget(CreateImageLabel(":/images/K3OpEenRijKiesKathleen.png"));
    choose->addStretch();
    layout->addLayout(choose, 4, 0, 1, 3);
  }

  QPushButton * const button_back = new QPushButton("&Terug");
  connect(button_back, &QPushButton::clicked, this, [this]() { ShowPage(m_menu_page); });
  layout->addWidget(button_back, 5, 0, 1, 3, Qt::AlignCenter);

  content->setFixedWidth(800);
  //The instructions are higher than the web version's screen
  return new FittingScrollArea(content);
}

QWidget * ribi::koer::QtK3OpEenRijMainWindow::CreateAboutPage()
{
  QWidget * const content = new QWidget;
  QVBoxLayout * const layout = new QVBoxLayout(content);
  QLabel * const title = CreateTextLabel("K3 Op Een Rij " + GetVersion(), Qt::AlignCenter);
  title->setObjectName("title");
  layout->addWidget(title);
  QLabel * const about = CreateTextLabel(
    "K3-thema drie-op-een-rij spel<br/>"
    "door Richèl Bilderbeek, 2007-2026<br/>"
    "<a href=\"http://richelbilderbeek.github.io/k3_op_een_rij/\">"
    "http://richelbilderbeek.github.io/k3_op_een_rij/</a><br/>"
    "Met dank aan Mark Wiering voor zijn uitstekende bug reports",
    Qt::AlignCenter
  );
  about->setOpenExternalLinks(true);
  layout->addWidget(about);
  QLabel * const history = CreateTextLabel(GetVersionHistory().join("<br/>"));
  history->setStyleSheet("font-size: 12px;");
  layout->addWidget(history);
  QPushButton * const button_back = new QPushButton("&Terug");
  connect(button_back, &QPushButton::clicked, this, [this]() { ShowPage(m_menu_page); });
  layout->addWidget(button_back, 0, Qt::AlignCenter);
  content->setFixedWidth(700);

  return new FittingScrollArea(content);
}

QWidget * ribi::koer::QtK3OpEenRijMainWindow::CreateGamePage()
{
  QWidget * const content = new QWidget;
  QVBoxLayout * const layout = new QVBoxLayout(content);
  m_board_holder = new QVBoxLayout;
  layout->addLayout(m_board_holder, 1); //The board gets all space left

  QHBoxLayout * const bottom = new QHBoxLayout;
  for (QLabel *& label: m_player_labels)
  {
    label = new QLabel;
    label->setFixedSize(QtK3OpEenRijBoardWidget::sprite_size, QtK3OpEenRijBoardWidget::sprite_size);
    bottom->addWidget(label);
  }
  bottom->addStretch();
  QPushButton * const button_quit = new QPushButton("&Stoppen");
  connect(button_quit, &QPushButton::clicked, this, &QtK3OpEenRijMainWindow::StopGame);
  bottom->addWidget(button_quit);
  layout->addLayout(bottom);
  //Not centered: the board grows to fill the screen
  return content;
}

QWidget * ribi::koer::QtK3OpEenRijMainWindow::CreateWinnerPage()
{
  QWidget * const content = new QWidget;
  QVBoxLayout * const layout = new QVBoxLayout(content);
  m_winner_text = CreateTextLabel("Winnaar", Qt::AlignCenter);
  m_winner_text->setObjectName("title");
  layout->addWidget(m_winner_text);
  m_winner_image = new QLabel;
  m_winner_image->setAlignment(Qt::AlignCenter);
  layout->addWidget(m_winner_image);
  QPushButton * const button_ok = new QPushButton("&OK");
  connect(button_ok, &QPushButton::clicked, this, [this]() { ShowPage(m_menu_page); });
  layout->addWidget(button_ok, 0, Qt::AlignCenter);
  return Centered(content);
}

QString ribi::koer::QtK3OpEenRijMainWindow::GetVersion()
{
  return "10.1";
}

QStringList ribi::koer::QtK3OpEenRijMainWindow::GetVersionHistory()
{
  return {
    "2007-01-03: version 3.0: initial C++ Builder verion",
    "2009-12-31: version 4.0: replaced Kathleen by Josje, game arena always visible, made screens smaller, removed useless JPEGs",
    "2013-08-06: version 5.0: port to Qt Creator",
    "2013-08-06: version 5.1: allow selection of both Kathleen and Josje",
    "2013-08-09: version 6.0: implemented suggestions and bugfixes from Mark Wiering's report",
    "2014-01-23: version 6.1: added command line demo",
    "2014-01-30: version 6.2: added extra command line option to display the artists",
    "2015-06-09: version 6.3: show how to choose Kathleen in the menu screen, clean up files when program closes",
    "2015-11-14: version 7.0: moved to own GitHub, added pictures of new K3",
    "2026-11-08: version 10.0: port to Qt6, added WebAssembly version, no external dependencies",
    "2026-11-08: version 10.1: added Julia"
  };
}

void ribi::koer::QtK3OpEenRijMainWindow::OnMoveDone()
{
  assert(m_board);
  const con3::Winner winner = m_board->GetGame().GetWinner();
  if (winner != con3::Winner::no_winner)
  {
    ShowWinner(winner);
    return;
  }
  const int active = static_cast<int>(m_board->GetGame().GetActivePlayer());
  for (int i = 0; i != 3; ++i)
  {
    m_player_labels[i]->setPixmap(QPixmap(
      i == active ? m_player_filenames[i] : m_player_grey_filenames[i]
    ));
  }
  if (m_board->IsComputerTurn())
  {
    //The board is the context: if the game is stopped, the timer is cancelled
    QtK3OpEenRijBoardWidget * const board = m_board;
    QTimer::singleShot(100, board, [board]() { board->DoComputerMove(); });
  }
}

void ribi::koer::QtK3OpEenRijMainWindow::ShowPage(QWidget * const page)
{
  //Only let the current page determine the window size
  for (int i = 0; i != m_stack->count(); ++i)
  {
    QWidget * const w = m_stack->widget(i);
    const auto policy = w == page ? QSizePolicy::Preferred : QSizePolicy::Ignored;
    w->setSizePolicy(policy, policy);
  }
  m_stack->setCurrentWidget(page);
  #ifndef Q_OS_WASM
  if (!isFullScreen() && !isMaximized()) adjustSize();
  #endif
}

void ribi::koer::QtK3OpEenRijMainWindow::ShowWinner(const con3::Winner winner)
{
  switch (winner)
  {
    case con3::Winner::player1:
    case con3::Winner::player2:
    case con3::Winner::player3:
      m_winner_text->setText("Winnaar");
      m_winner_image->setPixmap(QPixmap(m_player_filenames[static_cast<int>(winner) - 1]));
      break;
    case con3::Winner::draw:
    case con3::Winner::no_winner:
      m_winner_text->setText("Gelijkspel");
      m_winner_image->setPixmap(QPixmap(QtK3OpEenRijResources::GetEmptyFilename()));
      break;
  }
  StopGame();
  ShowPage(m_winner_page);
}

void ribi::koer::QtK3OpEenRijMainWindow::StartGame()
{
  StopGame();
  m_player_filenames = QtK3OpEenRijResources::GetPlayerFilenames(
    true, m_select->GetBlack(), m_select->GetBlond(), m_select->GetRed());
  m_player_grey_filenames = QtK3OpEenRijResources::GetPlayerFilenames(
    false, m_select->GetBlack(), m_select->GetBlond(), m_select->GetRed());
  m_board = new QtK3OpEenRijBoardWidget(
    m_player_filenames, m_select->GetIsPlayerHuman(), n_cols, n_rows);
  m_board_holder->addWidget(m_board, 1);
  connect(m_board, &QtK3OpEenRijBoardWidget::moveDone, this, &QtK3OpEenRijMainWindow::OnMoveDone);
  ShowPage(m_game_page);
  OnMoveDone(); //Show whose turn it is, start computer move if needed
}

void ribi::koer::QtK3OpEenRijMainWindow::StopGame()
{
  if (m_board)
  {
    //Not deleted immediately, as this may be called from the board's own signal
    m_board->hide();
    m_board->disconnect(this);
    m_board->deleteLater();
    m_board = nullptr;
  }
  if (m_stack->currentWidget() == m_game_page) ShowPage(m_menu_page);
}
