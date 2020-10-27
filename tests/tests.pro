TEMPLATE = app

QT       += core gui widgets 3dcore 3drender 3dextras opengl

CONFIG += c++11

DEPENDPATH += . ../include
INCLUDEPATH += ../include
# Three times up because we are in the build folder
LIBS += $$PWD/../lib/libqt3dwidget.a

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h \

FORMS += \
    mainwindow.ui