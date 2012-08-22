TEMPLATE = lib
CONFIG += qt
QT = core gui
TARGET = blackmilord
RESOURCES = resourcelib.qrc

include(../../project.pri)

SOURCES += Preferences.cpp
SOURCES += DeviceConfiguration.cpp
SOURCES += Dictionary.cpp
SOURCES += Spellcheck.cpp

HEADERS += Preferences.h
HEADERS += DeviceConfiguration.h
HEADERS += Dictionary.h
HEADERS += Spellcheck.h
