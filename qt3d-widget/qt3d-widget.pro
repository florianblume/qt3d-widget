TEMPLATE = lib

QT       += core gui widgets 3dcore 3drender 3dextras 3dinput 3dlogic opengl
QT_PRIVATE += 3drender-private opengl-private widgets-private core-private gui-private

CONFIG += c++11 shared_and_static build_all

DEFINES += QT3DWIDGETLIB_LIBRARY

public_headers.path = include
public_headers.files = include/*.h

INSTALLS += public_headers

DESTDIR += ../../../lib

SOURCES += \
    qt3dwidget.cpp

HEADERS += \
    qt3dwidget.h \
    qt3dwidget_p.h
