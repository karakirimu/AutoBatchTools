# This folder contains shared constants and
# shared functions used in multiple places
INCLUDEPATH += $$PWD

include($$PWD/process/process.pri)
include($$PWD/qss/qss.pri))

SOURCES += \
    $$PWD/variantconverter.cpp \
    $$PWD/xmlresolver.cpp

HEADERS += \
    $$PWD/variantconverter.h \
    $$PWD/xmlresolver.h
