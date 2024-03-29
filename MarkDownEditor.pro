#-------------------------------------------------
#
# Project created by QtCreator 2023-01-05T18:26:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MarkDownEditor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    editor.cpp \
    finddialog.cpp \
    highlighter.cpp \
    main.cpp \
    mainwindow.cpp \
    tabeditor.cpp \
    linenumberarea.cpp \
    utils.cpp

HEADERS += \
    editor.h \
    finddialog.h \
    highlighter.h \
    mainwindow.h \
    markdowneditor.h \
    markdownrender.h \
    markdownviewer.h \
    searchhistory.h \
    tabeditor.h \
    linenumberarea.h \
    utils.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resources.qrc \
    resources.qrc

DISTFILES += \
    README.md

