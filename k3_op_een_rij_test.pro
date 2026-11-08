# Tests of K3 Op Een Rij
QT += core gui widgets testlib
CONFIG += c++17

TARGET = k3_op_een_rij_test
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

SOURCES += k3_op_een_rij_test.cpp
