TULIPCONFIG_PATH = tulip-config

TEMPLATE = app

QMAKE_CXXFLAGS += `$$TULIPCONFIG_PATH --cxxflags`
QMAKE_LFLAGS += `$$TULIPCONFIG_PATH --libs`

HEADERS += ViewProfile.h
SOURCES += NodeLinkDiagramComponentProfile.cpp
CONFIG  += qt
