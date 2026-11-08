#include <cstdlib>
#include <ctime>

#include <QApplication>
#include <QIcon>

#include "qtk3opeenrijmainwindow.h"
#include "qtk3opeenrijresources.h"

#ifndef Q_OS_WASM
#include <QAudioOutput>
#include <QMediaPlayer>
#include <QUrl>
#endif

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

  //Play the song in a loop
  QAudioOutput audio_output;
  QMediaPlayer music_player;
  music_player.setAudioOutput(&audio_output);
  music_player.setSource(QUrl("qrc:/bubblegum-pop.m4a"));
  music_player.setLoops(QMediaPlayer::Infinite);
  music_player.play();
#endif
  return a.exec();
}
