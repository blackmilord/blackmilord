TEMPLATE = lib
CONFIG += plugin
TARGET = spellcheck_aspell

SOURCES += Aspell.cpp
HEADERS += Aspell.h

LIBS += -laspell

include(../../../../project.pri)
