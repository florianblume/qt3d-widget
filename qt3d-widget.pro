TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = qt3d-widget tests

tests.depends = qt3d-widget
