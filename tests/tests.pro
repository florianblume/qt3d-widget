TEMPLATE = app

QT       += core gui widgets 3dcore 3drender 3dextras opengl

CONFIG += c++11

DEPENDPATH += . ../include
INCLUDEPATH += ../include
# Three times up because we are in the build folder
LIBS += ../../../lib/libqt3d-widget.a

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h \

FORMS += \
    mainwindow.ui
