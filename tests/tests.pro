TEMPLATE = app

QT       += core gui widgets 3dcore 3drender 3dextras

lessThan(QT_MAJOR_VERSION, 6): QT += opengl
greaterThan(QT_MAJOR_VERSION, 5): QT += openglwidgets


CONFIG += c++11


DEPENDPATH += . ../qt3dwidget/include

INCLUDEPATH += ../qt3dwidget/include


lessThan(QT_MAJOR_VERSION, 6): LIBS += "$${PWD}/../lib/libqt3dwidget.a"
greaterThan(QT_MAJOR_VERSION, 5): LIBS += "$${PWD}/../lib-qt6/libqt3dwidget.a"


SOURCES += \
    main.cpp \
    mainwindow.cpp


HEADERS += \
    mainwindow.h \


FORMS += \
    mainwindow.ui
