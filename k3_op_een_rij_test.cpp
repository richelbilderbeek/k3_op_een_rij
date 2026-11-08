#include <QPushButton>
#include <QStackedWidget>
#include <QtTest>

#include "connectthreegame.h"
#include "qtk3opeenrijboardwidget.h"
#include "qtk3opeenrijmainwindow.h"
#include "qtk3opeenrijresources.h"
#include "qtk3opeenrijselectplayerwidget.h"

using namespace ribi;

class K3OpEenRijTest : public QObject
{
  Q_OBJECT

private:
  ///Save a screenshot if K3_SCREENSHOT_DIR is set
  static void Screenshot(QWidget& w, const QString& name)
  {
    const QString folder = qEnvironmentVariable("K3_SCREENSHOT_DIR");
    if (folder.isEmpty()) return;
    QVERIFY(w.grab().save(folder + "/" + name + ".png"));
  }

  static QPushButton * FindButton(QWidget& w, const QString& text)
  {
    for (QPushButton * const b: w.findChildren<QPushButton *>())
    {
      if (b->text() == text && b->isVisible()) return b;
    }
    return nullptr;
  }

private slots:
  void computer_games_end()
  {
    for (int i = 0; i != 200; ++i)
    {
      con3::Game g(15, 8);
      int n_moves = 0;
      while (g.GetWinner() == con3::Winner::no_winner)
      {
        g.DoMove(g.SuggestMove(std::bitset<3>(i % 8)));
        ++n_moves;
        QVERIFY(n_moves <= 15 * 8);
      }
    }
  }

  void blondes_cycle_via_julia()
  {
    using koer::BlondeGirl;
    QCOMPARE(koer::GetNext(BlondeGirl::klaasje), BlondeGirl::julia);
    QCOMPARE(koer::GetNext(BlondeGirl::julia), BlondeGirl::matt);
    QCOMPARE(koer::ToStr(BlondeGirl::julia), std::string("Julia"));
    //All blondes have their pictures
    BlondeGirl girl = BlondeGirl::kathleen;
    for (int i = 0; i != 5; ++i)
    {
      QVERIFY(!QPixmap(koer::QtK3OpEenRijResources::GetPlayerFilename(true, girl)).isNull());
      QVERIFY(!QPixmap(koer::QtK3OpEenRijResources::GetPlayerFilename(false, girl)).isNull());
      girl = koer::GetNext(girl);
    }
    QCOMPARE(girl, BlondeGirl::kathleen);
  }

  void three_in_a_row_wins()
  {
    con3::Game g(5, 5);
    //Player 1 plays (0,0), (1,0), (2,0), the others play in row 4
    g.DoMove(0, 0); g.DoMove(0, 4); g.DoMove(4, 4);
    g.DoMove(1, 0); g.DoMove(1, 4); g.DoMove(3, 4);
    QCOMPARE(g.GetWinner(), con3::Winner::no_winner);
    g.DoMove(2, 0);
    QCOMPARE(g.GetWinner(), con3::Winner::player1);
  }

  void play_full_game_via_ui()
  {
    koer::QtK3OpEenRijMainWindow w;
    w.show();
    QVERIFY(QTest::qWaitForWindowExposed(&w));
    Screenshot(w, "menu");

    QStackedWidget * const stack = w.findChild<QStackedWidget *>();
    QVERIFY(stack);
    QWidget * const menu_page = stack->currentWidget();

    //Instructions and back
    QTest::mouseClick(FindButton(w, "&Handleiding"), Qt::LeftButton);
    QVERIFY(stack->currentWidget() != menu_page);
    Screenshot(w, "instructions");
    QTest::mouseClick(FindButton(w, "&Terug"), Qt::LeftButton);
    QCOMPARE(stack->currentWidget(), menu_page);

    //About and back
    QTest::mouseClick(FindButton(w, "&Over"), Qt::LeftButton);
    Screenshot(w, "about");
    QTest::mouseClick(FindButton(w, "&Terug"), Qt::LeftButton);
    QCOMPARE(stack->currentWidget(), menu_page);

    //Let one human play first, to test clicking the board
    koer::QtK3OpEenRijSelectPlayerWidget * const select
      = w.findChild<koer::QtK3OpEenRijSelectPlayerWidget *>();
    QVERIFY(select);
    //The current band is the initial choice
    QCOMPARE(select->GetRed(), koer::RedHairedGirl::hanne);
    QCOMPARE(select->GetBlack(), koer::BlackHairedGirl::marthe);
    QCOMPARE(select->GetBlond(), koer::BlondeGirl::julia);
    QTest::mouseClick(select, Qt::LeftButton, {}, QPoint(75, 75)); //Black: computer
    QTest::mouseClick(select, Qt::LeftButton, {}, QPoint(75, 125)); //Blond: computer
    QTest::mouseClick(select, Qt::LeftButton, {}, QPoint(25, 25)); //Red: next girl
    QCOMPARE(select->GetIsPlayerHuman(), std::bitset<3>(0b001));
    QCOMPARE(select->GetRed(), koer::RedHairedGirl::tyler);
    QCOMPARE(select->GetBlond(), koer::BlondeGirl::julia);
    Screenshot(w, "menu_selected");

    QTest::mouseClick(FindButton(w, "&Begin spel"), Qt::LeftButton);
    koer::QtK3OpEenRijBoardWidget * board = w.findChild<koer::QtK3OpEenRijBoardWidget *>();
    QVERIFY(board);
    QVERIFY(!board->IsComputerTurn());
    QTest::mouseClick(board, Qt::LeftButton, {}, board->GetSquareCenter(0, 0));
    QCOMPARE(board->GetGame().GetSquare(0, 0), con3::Square::player1);
    //The two computers move
    QTRY_VERIFY_WITH_TIMEOUT(!board->IsComputerTurn(), 2000);
    Screenshot(w, "game");

    //The board scales to the window size, e.g. a small web page
    for (const QSize& size: { QSize(360, 234), QSize(976, 635), QSize(800, 520) })
    {
      w.resize(size);
      QTest::qWait(50);
      QVERIFY(board->x() + board->GetSquareCenter(14, 7).x() < w.width());
      QVERIFY(board->y() + board->GetSquareCenter(14, 7).y() < w.height());
      Screenshot(w, QString("game_%1x%2").arg(size.width()).arg(size.height()));
    }
    QTest::mouseClick(board, Qt::LeftButton, {}, board->GetSquareCenter(14, 7));
    QCOMPARE(board->GetGame().GetSquare(14, 7), con3::Square::player1);
    QTRY_VERIFY_WITH_TIMEOUT(!board->IsComputerTurn(), 2000);

    //Stop, then let the computers play a whole game
    QTest::mouseClick(FindButton(w, "&Stoppen"), Qt::LeftButton);
    QCOMPARE(stack->currentWidget(), menu_page);
    QTest::mouseClick(select, Qt::LeftButton, {}, QPoint(75, 25)); //Red: computer
    QCOMPARE(select->GetIsPlayerHuman(), std::bitset<3>(0b000));
    QTest::mouseClick(FindButton(w, "&Begin spel"), Qt::LeftButton);
    QTRY_VERIFY_WITH_TIMEOUT(FindButton(w, "&OK") != nullptr, 30000);
    Screenshot(w, "winner");
    QTest::mouseClick(FindButton(w, "&OK"), Qt::LeftButton);
    QCOMPARE(stack->currentWidget(), menu_page);
  }
};

QTEST_MAIN(K3OpEenRijTest)
#include "k3_op_een_rij_test.moc"
