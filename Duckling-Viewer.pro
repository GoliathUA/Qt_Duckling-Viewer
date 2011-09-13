#-------------------------------------------------
#
# Project created by QtCreator 2011-09-08T23:49:15
#
#-------------------------------------------------

QT       += core gui webkit network

TARGET = Duckling-Viewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    stab.cpp \
    tabstreamwidget.cpp \
    settings.cpp \
    shortviewerdialog.cpp

HEADERS  += mainwindow.h \
    stab.h \
    tabstreamwidget.h \
    settings.h \
    shortviewerdialog.h

FORMS    += mainwindow.ui \
    stab.ui \
    tabstreamwidget.ui \
    shortviewerdialog.ui

RESOURCES += \
    icons.qrc

TRANSLATIONS += languages/en_EN.ts \
               languages/ru_RU.ts




















