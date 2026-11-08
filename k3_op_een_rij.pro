# K3 Op Een Rij, a K3-themed connect-three game
#
# Desktop:     qmake6 && make
# WebAssembly: <Qt for WebAssembly>/bin/qmake && make
#              produces k3_op_een_rij.html, .js and .wasm

QT += core gui widgets
CONFIG += c++17

TARGET = k3_op_een_rij
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

# High warning levels, for GCC and Clang (including Emscripten)
!msvc {
  QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
}

include(k3_op_een_rij.pri)

SOURCES += qtmain.cpp

# Music is for the desktop version only: the web version is silent
!wasm {
  QT += multimedia
  RESOURCES += k3_op_een_rij_music.qrc
}

win32: RC_ICONS = K3OpEenRijIcon.ico
