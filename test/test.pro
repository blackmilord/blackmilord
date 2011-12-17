TEMPLATE = app
VERSION = 0.1.0
CONFIG += qt debug_and_release qtestlib
QT = core gui testlib
TARGET = test_blackmilord
RESOURCES = ../src/resource.qrc

QMAKE_CXXFLAGS += -Werror -Wextra

win32 {
    QMAKE_LFLAGS += -static-libstdc++
}

CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -g3 -gdwarf-2 -O0
    DESTPREFIX = "debug"
}
else {
    QMAKE_LFLAGS += -s
    DEFINES += QT_NO_DEBUG_OUTPUT
    DESTPREFIX = "release"
}

INCLUDEPATH +=  . \
                ../src/ \
                ../src/options/ \
                ../src/dialogs/ \
                ../src/utils/ \
                ../src/book/ \
                ../src/book/mobi/ \
                ../src/highlighters/ \
                ../src/device \
                ../src/gui

SOURCES += ./main_tests.cpp
SOURCES += ./Tests.cpp
SOURCES += ../src/gui/Gui.cpp
SOURCES += ../src/gui/MainWindow.cpp
SOURCES += ../src/gui/StatusBar.cpp
SOURCES += ../src/gui/PlainTextEditor.cpp
SOURCES += ../src/book/Book.cpp
SOURCES += ../src/book/BookPicture.cpp
SOURCES += ../src/book/AbstractBook.cpp
SOURCES += ../src/book/BackupManager.cpp
SOURCES += ../src/book/mobi/MobiFileObject.cpp
SOURCES += ../src/book/mobi/DatabaseRecordInfoEntry.cpp
SOURCES += ../src/book/mobi/DatabaseHeader.cpp
SOURCES += ../src/book/mobi/PalmDOCHeader.cpp
SOURCES += ../src/book/mobi/MOBIHeader.cpp
SOURCES += ../src/book/mobi/EXTHHeader.cpp
SOURCES += ../src/book/mobi/EXTHHeaderEntry.cpp
SOURCES += ../src/book/mobi/MobiCodec.cpp
SOURCES += ../src/utils/AspellWrapper.cpp
SOURCES += ../src/utils/Dictionary.cpp
SOURCES += ../src/utils/Formatting.cpp
SOURCES += ../src/dialogs/HowToUseAspellWindow.cpp
SOURCES += ../src/dialogs/SpellCheckingWindow.cpp
SOURCES += ../src/dialogs/FindReplaceWindow.cpp
SOURCES += ../src/dialogs/MetaDataWindow.cpp
SOURCES += ../src/dialogs/AboutWindow.cpp
SOURCES += ../src/dialogs/PictureViewerWindow.cpp
SOURCES += ../src/highlighters/HighlighterManager.cpp
SOURCES += ../src/highlighters/AbstractHighlighter.cpp
SOURCES += ../src/highlighters/HighlighterHTMLTags.cpp
SOURCES += ../src/highlighters/HighlighterSpellcheck.cpp
SOURCES += ../src/highlighters/HighlighterThread.cpp
SOURCES += ../src/highlighters/event/HighlightersApplySettingsEvent.cpp
SOURCES += ../src/highlighters/event/HighlightBlockEvent.cpp
SOURCES += ../src/highlighters/event/HighlightBlockEventResponse.cpp
SOURCES += ../src/options/OptionsWindow.cpp
SOURCES += ../src/options/EditorPage.cpp
SOURCES += ../src/options/MainPage.cpp
SOURCES += ../src/options/HighlighterPage.cpp
SOURCES += ../src/options/Preferences.cpp
SOURCES += ../src/device/DeviceConfiguration.cpp

HEADERS += ./Tests.h
HEADERS += ../src/Version.h
HEADERS += ../src/gui/Gui.h
HEADERS += ../src/gui/MainWindow.h
HEADERS += ../src/gui/StatusBar.h
HEADERS += ../src/gui/PlainTextEditor.h
HEADERS += ../src/book/Book.h
HEADERS += ../src/book/BookPicture.h
HEADERS += ../src/book/AbstractBook.h
HEADERS += ../src/book/BackupManager.h
HEADERS += ../src/book/mobi/MobiFileObject.h
HEADERS += ../src/book/mobi/DatabaseRecordInfoEntry.h
HEADERS += ../src/book/mobi/DatabaseHeader.h
HEADERS += ../src/book/mobi/PalmDOCHeader.h
HEADERS += ../src/book/mobi/MOBIHeader.h
HEADERS += ../src/book/mobi/EXTHHeader.h
HEADERS += ../src/book/mobi/EXTHHeaderEntry.h
HEADERS += ../src/book/mobi/MobiCodec.h
HEADERS += ../src/utils/AspellWrapper.h
HEADERS += ../src/utils/Dictionary.h
HEADERS += ../src/utils/Formatting.h
HEADERS += ../src/dialogs/HowToUseAspellWindow.h
HEADERS += ../src/dialogs/SpellCheckingWindow.h
HEADERS += ../src/dialogs/FindReplaceWindow.h
HEADERS += ../src/dialogs/MetaDataWindow.h
HEADERS += ../src/dialogs/AboutWindow.h
HEADERS += ../src/dialogs/PictureViewerWindow.h
HEADERS += ../src/highlighters/HighlighterManager.h
HEADERS += ../src/highlighters/AbstractHighlighter.h
HEADERS += ../src/highlighters/HighlighterHTMLTags.h
HEADERS += ../src/highlighters/HighlighterSpellcheck.h
HEADERS += ../src/highlighters/HighlighterThread.h
HEADERS += ../src/highlighters/event/HighlightersApplySettingsEvent.h
HEADERS += ../src/highlighters/event/HighlightBlockEvent.h
HEADERS += ../src/highlighters/event/HighlightBlockEventResponse.h
HEADERS += ../src/options/OptionsWindow.h
HEADERS += ../src/options/EditorPage.h
HEADERS += ../src/options/MainPage.h
HEADERS += ../src/options/HighlighterPage.h
HEADERS += ../src/options/IPageWidget.h
HEADERS += ../src/options/Preferences.h
HEADERS += ../src/device/DeviceConfiguration.h

DESTDIR = ../dist/$$DESTPREFIX
OBJECTS_DIR = ../build/$$DESTPREFIX
MOC_DIR = ../build/$$DESTPREFIX
