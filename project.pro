TEMPLATE = subdirs
CONFIG += ordered debug_and_release

include(project.pri)

SUBDIRS += src/interface
SUBDIRS += src
SUBDIRS += src/plugins
#SUBDIRS += test