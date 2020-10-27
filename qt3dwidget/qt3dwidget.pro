TEMPLATE = lib

VERSION = 2.0

QT       += core gui widgets 3dcore 3drender 3dextras 3dinput 3dlogic opengl

CONFIG += c++11 shared_and_static build_all

DEFINES += QT3DWIDGETLIB_LIBRARY

DESTDIR += $$PWD/../lib

SOURCES += \
    qt3dwidget.cpp

HEADERS += \
    qt3dwidget.h \
    qt3dwidget_p.h
