VERSION = 0.1.0
CONFIG += debug_and_release

BLACK_MILORD_BUILD_ROOT=$$PWD

CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS += -g3 -gdwarf-2 -O0
    DESTPREFIX = "debug"
}
else {
    QMAKE_LFLAGS += -s
    DEFINES += QT_NO_DEBUG_OUTPUT
    DESTPREFIX = "release"
}

QMAKE_CXXFLAGS += -Werror -Wextra

QMAKE_LFLAGS += -L$$BLACK_MILORD_BUILD_ROOT/dist/$$DESTPREFIX

INCLUDEPATH +=  $$BLACK_MILORD_BUILD_ROOT/src/ \
                $$BLACK_MILORD_BUILD_ROOT/src/interface/ \
                $$BLACK_MILORD_BUILD_ROOT/src/options/ \
                $$BLACK_MILORD_BUILD_ROOT/src/dialogs/ \
                $$BLACK_MILORD_BUILD_ROOT/src/utils/ \
                $$BLACK_MILORD_BUILD_ROOT/src/book/ \
                $$BLACK_MILORD_BUILD_ROOT/src/book/mobi/ \
                $$BLACK_MILORD_BUILD_ROOT/src/highlighter/ \
                $$BLACK_MILORD_BUILD_ROOT/src/gui/ \
                $$BLACK_MILORD_BUILD_ROOT/src/gui/data

DESTDIR = $$BLACK_MILORD_BUILD_ROOT/dist/$$DESTPREFIX
OBJECTS_DIR = $$BLACK_MILORD_BUILD_ROOT/build/$$DESTPREFIX
MOC_DIR = $$BLACK_MILORD_BUILD_ROOT/build/$$DESTPREFIX
