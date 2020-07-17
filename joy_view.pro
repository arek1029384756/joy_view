######################################################################
# joy_view.pro
######################################################################

QT += widgets

QMAKE_PROJECT_NAME = joy_view

INCLUDEPATH += .
INCLUDEPATH += ./task ./gui_qt ./comm
QMAKE_CXXFLAGS += -Wextra -pedantic -std=c++17

# Input
HEADERS += gui_qt/screen.h \
gui_qt/screen_view.hpp \
gui_qt/screen_params.hpp \
gui_qt/joystick.hpp \
comm/comm_thread.hpp \
comm/com.hpp \
task/base_thread.hpp \

SOURCES += main.cpp \
gui_qt/screen.cpp
