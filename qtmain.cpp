#include <cstdlib>
#include <ctime>

#include <QApplication>
#include <QIcon>

#include "qtk3opeenrijmainwindow.h"
#include "qtk3opeenrijresources.h"

int main(int argc, char *argv[])
{
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  QApplication a(argc, argv);
  a.setWindowIcon(QIcon(ribi::koer::QtK3OpEenRijResources::GetIconFilename()));

  ribi::koer::QtK3OpEenRijMainWindow w;
#ifdef Q_OS_WASM
  w.showFullScreen();
#else
  w.show();
#endif
  return a.exec();
}
