# This is the qmake file for the QScintilla plugin for Qt Designer.


TEMPLATE = lib
TARGET = qscintillaplugin

CONFIG += designer release plugin

HEADERS = qscintillaplugin.h
SOURCES = qscintillaplugin.cpp

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target

LIBS += -lqscintilla2
