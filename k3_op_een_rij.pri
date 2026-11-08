# Files shared by the game and its tests

INCLUDEPATH += $$PWD

# Qt for WebAssembly has exceptions disabled: a throw aborts the program.
# Disable them everywhere, so a throw fails to compile instead
CONFIG += exceptions_off

SOURCES += \
  $$PWD/connectthreegame.cpp \
  $$PWD/connectthreeplayer.cpp \
  $$PWD/k3opeenrijnames.cpp \
  $$PWD/qtk3opeenrijboardwidget.cpp \
  $$PWD/qtk3opeenrijmainwindow.cpp \
  $$PWD/qtk3opeenrijresources.cpp \
  $$PWD/qtk3opeenrijselectplayerwidget.cpp

HEADERS += \
  $$PWD/connectthreegame.h \
  $$PWD/connectthreemove.h \
  $$PWD/connectthreeplayer.h \
  $$PWD/connectthreesquare.h \
  $$PWD/connectthreewinner.h \
  $$PWD/k3opeenrijnames.h \
  $$PWD/qtk3opeenrijboardwidget.h \
  $$PWD/qtk3opeenrijmainwindow.h \
  $$PWD/qtk3opeenrijresources.h \
  $$PWD/qtk3opeenrijselectplayerwidget.h

RESOURCES += $$PWD/K3OpEenRij.qrc

