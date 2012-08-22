TEMPLATE = app
CONFIG += qt
QT = core gui
TARGET = blackmilord
RESOURCES = resource.qrc

win32 {
    QMAKE_LFLAGS += -static-libstdc++
    RC_FILE = ../blackmilord_win.rc
    CONFIG(debug, debug|release) {
        CONFIG += console
    }
}

LIBS += -lblackmilord

include(../project.pri)

SOURCES += main.cpp
SOURCES += gui/Gui.cpp
SOURCES += gui/MainWindow.cpp
SOURCES += gui/StatusBar.cpp
SOURCES += gui/PlainTextEditor.cpp
SOURCES += gui/data/BlockData.cpp
SOURCES += gui/data/XMLElement.cpp
SOURCES += book/Book.cpp
SOURCES += book/BookPicture.cpp
SOURCES += book/AbstractBook.cpp
SOURCES += book/BackupManager.cpp
SOURCES += book/mobi/MobiFile.cpp
SOURCES += book/mobi/DatabaseRecordInfoEntry.cpp
SOURCES += book/mobi/DatabaseHeader.cpp
SOURCES += book/mobi/PalmDOCHeader.cpp
SOURCES += book/mobi/MOBIHeader.cpp
SOURCES += book/mobi/EXTHHeader.cpp
SOURCES += book/mobi/EXTHHeaderEntry.cpp
SOURCES += book/mobi/MobiCodec.cpp
SOURCES += utils/Formatting.cpp
SOURCES += dialogs/HowToUseAspellWindow.cpp
SOURCES += dialogs/SpellCheckingWindow.cpp
SOURCES += dialogs/FindReplaceWindow.cpp
SOURCES += dialogs/MetaDataWindow.cpp
SOURCES += dialogs/AboutWindow.cpp
SOURCES += dialogs/PictureViewerWindow.cpp
SOURCES += options/OptionsWindow.cpp
SOURCES += options/EditorPage.cpp
SOURCES += options/MainPage.cpp
SOURCES += options/HighlighterPage.cpp
SOURCES += highlighter/HighlighterManager.cpp
SOURCES += highlighter/HighlighterThread.cpp
SOURCES += highlighter/HighlightersApplySettingsEvent.cpp
SOURCES += highlighter/HighlightBlockEvent.cpp
SOURCES += highlighter/HighlightBlockEventResponse.cpp

#HEADERS += interface/Version.h
#HEADERS += interface/Plugin.h
#HEADERS += interface/PluginHighlighter.h
#HEADERS += interface/IPreferences.h
HEADERS += gui/Gui.h
HEADERS += gui/MainWindow.h
HEADERS += gui/StatusBar.h
HEADERS += gui/PlainTextEditor.h
HEADERS += gui/data/BlockData.h
HEADERS += gui/data/XMLElement.h
HEADERS += book/Book.h
HEADERS += book/BookPicture.h
HEADERS += book/AbstractBook.h
HEADERS += book/BackupManager.h
HEADERS += book/MetadataEnum.h
HEADERS += book/mobi/MobiFile.h
HEADERS += book/mobi/DatabaseRecordInfoEntry.h
HEADERS += book/mobi/DatabaseHeader.h
HEADERS += book/mobi/PalmDOCHeader.h
HEADERS += book/mobi/MOBIHeader.h
HEADERS += book/mobi/EXTHHeader.h
HEADERS += book/mobi/EXTHHeaderEntry.h
HEADERS += book/mobi/MobiCodec.h
HEADERS += utils/Formatting.h
HEADERS += dialogs/HowToUseAspellWindow.h
HEADERS += dialogs/SpellCheckingWindow.h
HEADERS += dialogs/FindReplaceWindow.h
HEADERS += dialogs/MetaDataWindow.h
HEADERS += dialogs/AboutWindow.h
HEADERS += dialogs/PictureViewerWindow.h
HEADERS += options/OptionsWindow.h
HEADERS += options/EditorPage.h
HEADERS += options/MainPage.h
HEADERS += options/HighlighterPage.h
HEADERS += options/IPageWidget.h
HEADERS += highlighter/HighlighterManager.h
HEADERS += highlighter/HighlighterThread.h
HEADERS += highlighter/HighlightersApplySettingsEvent.h
HEADERS += highlighter/HighlightBlockEvent.h
HEADERS += highlighter/HighlightBlockEventResponse.h
