#-------------------------------------------------
#
# Project created by QtCreator 2012-07-02T04:11:37
#
#-------------------------------------------------

QT       += core gui

TARGET = Qt_libusb
TEMPLATE = app


SOURCES += main.cpp\
        qt_libusb.cpp

HEADERS  += qt_libusb.h

FORMS    += qt_libusb.ui

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../../usr/lib/i386-linux-gnu/ -lusb-1.0

INCLUDEPATH += $$PWD/../../../../../usr/include/libusb-1.0
DEPENDPATH += $$PWD/../../../../../usr/include/libusb-1.0

unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/i386-linux-gnu/libusb.a

QMAKE_LFLAGS += -Wl,-O1 -Wl,-rpath,./lib
