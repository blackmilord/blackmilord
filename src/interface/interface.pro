TEMPLATE = lib
CONFIG += qt
QT = core gui
TARGET = blackmilord

include(../../project.pri)

SOURCES += ../options/IPreferences.cpp
SOURCES += ../options/Preferences.cpp

SOURCES += ../spellcheck/Aspell.cpp
SOURCES += ../spellcheck/ISpellcheck.cpp

SOURCES += ../device/IDeviceConfiguration.cpp
SOURCES += ../device/DeviceConfiguration.cpp

SOURCES += ../utils/Dictionary.cpp

HEADERS += IPreferences.h
HEADERS += ../options/Preferences.h

HEADERS += ISpellcheck.h
HEADERS += ../spellcheck/Aspell.h

HEADERS += IDeviceConfiguration.h
HEADERS += ../device/DeviceConfiguration.h

HEADERS += Dictionary.h