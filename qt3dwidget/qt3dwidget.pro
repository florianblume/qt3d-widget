TEMPLATE = lib

VERSION = 2.5


QT       += core gui widgets 3dcore 3drender 3dextras 3dinput 3dlogic

lessThan(QT_MAJOR_VERSION, 6): QT += opengl
greaterThan(QT_MAJOR_VERSION, 5): QT += openglwidgets


CONFIG += c++11 shared_and_static build_all


DEFINES += QT3DWIDGETLIB_LIBRARY


lessThan(QT_MAJOR_VERSION, 6): DESTDIR += "$${PWD}/../lib"
greaterThan(QT_MAJOR_VERSION, 5): DESTDIR += "$${PWD}/../lib-qt6"


SOURCES += \
    qt3dwidget.cpp


HEADERS += \
    qt3dwidget.h \
    qt3dwidget_p.h
