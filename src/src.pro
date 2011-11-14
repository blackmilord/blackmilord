TEMPLATE = app
VERSION = 0.1.0
CONFIG += qt debug_and_release
QT = core gui
TARGET = blackmilord
RESOURCES = resource.qrc

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


win32 {
    RC_FILE = blackmilord_win.rc
    CONFIG(debug, debug|release) {
        CONFIG += console
    }
}

INCLUDEPATH +=  . \
                options/ \
                dialogs/ \
                utils/ \
                book/ \
                book/mobi/ \
                highlighters/ \
                device \
                gui

SOURCES += main.cpp
SOURCES += gui/Gui.cpp
SOURCES += gui/MainWindow.cpp
SOURCES += gui/StatusBar.cpp
SOURCES += gui/PlainTextEditor.cpp
SOURCES += gui/PlainTextEditorUndoStack.cpp
SOURCES += book/Book.cpp
SOURCES += book/BookPicture.cpp
SOURCES += book/AbstractBook.cpp
SOURCES += book/BackupManager.cpp
SOURCES += book/mobi/MobiFileObject.cpp
SOURCES += book/mobi/DatabaseRecordInfoEntry.cpp
SOURCES += book/mobi/DatabaseHeader.cpp
SOURCES += book/mobi/PalmDOCHeader.cpp
SOURCES += book/mobi/MOBIHeader.cpp
SOURCES += book/mobi/EXTHHeader.cpp
SOURCES += book/mobi/EXTHHeaderEntry.cpp
SOURCES += book/mobi/MobiCodec.cpp
SOURCES += utils/AspellWrapper.cpp
SOURCES += utils/Dictionary.cpp
SOURCES += utils/Formatting.cpp
SOURCES += dialogs/HowToUseAspellWindow.cpp
SOURCES += dialogs/SpellCheckingWindow.cpp
SOURCES += dialogs/FindReplaceWindow.cpp
SOURCES += dialogs/MetaDataWindow.cpp
SOURCES += dialogs/AboutWindow.cpp
SOURCES += dialogs/PictureViewerWindow.cpp
SOURCES += highlighters/HighlighterManager.cpp
SOURCES += highlighters/AbstractHighlighter.cpp
SOURCES += highlighters/HighlighterHTMLTags.cpp
SOURCES += highlighters/HighlighterSpellcheck.cpp
SOURCES += highlighters/HighlighterThread.cpp
SOURCES += highlighters/event/HighlightersApplySettingsEvent.cpp
SOURCES += highlighters/event/HighlightBlockEvent.cpp
SOURCES += highlighters/event/HighlightBlockEventResponse.cpp
SOURCES += options/OptionsWindow.cpp
SOURCES += options/EditorPage.cpp
SOURCES += options/MainPage.cpp
SOURCES += options/Preferences.cpp
SOURCES += device/DeviceConfiguration.cpp

HEADERS += Version.h
HEADERS += gui/Gui.h
HEADERS += gui/MainWindow.h
HEADERS += gui/StatusBar.h
HEADERS += gui/PlainTextEditor.h
HEADERS += gui/PlainTextEditorUndoStack.h
HEADERS += book/Book.h
HEADERS += book/BookPicture.h
HEADERS += book/AbstractBook.h
HEADERS += book/BackupManager.h
HEADERS += book/mobi/MobiFileObject.h
HEADERS += book/mobi/DatabaseRecordInfoEntry.h
HEADERS += book/mobi/DatabaseHeader.h
HEADERS += book/mobi/PalmDOCHeader.h
HEADERS += book/mobi/MOBIHeader.h
HEADERS += book/mobi/EXTHHeader.h
HEADERS += book/mobi/EXTHHeaderEntry.h
HEADERS += book/mobi/MobiCodec.h
HEADERS += utils/AspellWrapper.h
HEADERS += utils/Dictionary.h
HEADERS += utils/Formatting.h
HEADERS += dialogs/HowToUseAspellWindow.h
HEADERS += dialogs/SpellCheckingWindow.h
HEADERS += dialogs/FindReplaceWindow.h
HEADERS += dialogs/MetaDataWindow.h
HEADERS += dialogs/AboutWindow.h
HEADERS += dialogs/PictureViewerWindow.h
HEADERS += highlighters/HighlighterManager.h
HEADERS += highlighters/AbstractHighlighter.h
HEADERS += highlighters/HighlighterHTMLTags.h
HEADERS += highlighters/HighlighterSpellcheck.h
HEADERS += highlighters/HighlighterThread.h
HEADERS += highlighters/event/HighlightersApplySettingsEvent.h
HEADERS += highlighters/event/HighlightBlockEvent.h
HEADERS += highlighters/event/HighlightBlockEventResponse.h
HEADERS += options/OptionsWindow.h
HEADERS += options/EditorPage.h
HEADERS += options/MainPage.h
HEADERS += options/IPageWidget.h
HEADERS += options/Preferences.h
HEADERS += device/DeviceConfiguration.h

DESTDIR = ../dist/$$DESTPREFIX
OBJECTS_DIR = ../build/$$DESTPREFIX
MOC_DIR = ../build/$$DESTPREFIX
