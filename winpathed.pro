TARGET = winpathed
QT += core \
    gui
HEADERS += src/win_api.h \
    src/path_editor.h
SOURCES += src/win_api.cpp \
    src/main.cpp \
    src/path_editor.cpp
RESOURCES += resources/path_editor.qrc

# win32:debug:CONFIG += console
RC_FILE += resources/path_editor.rc
