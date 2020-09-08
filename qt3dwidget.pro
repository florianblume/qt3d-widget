TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = qt3dwidget tests

tests.depends = qt3dwidget
