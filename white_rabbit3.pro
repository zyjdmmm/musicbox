#-------------------------------------------------
#
# Project created by QtCreator 2021-05-20T19:50:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = white_rabbit3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    central_w.cpp \
    music_point.cpp \
    music_window.cpp \
    key_board.cpp \
    export_image.cpp \
    read_midi.cpp \
    export_check_btn.cpp \
    note_stretch.cpp

HEADERS += \
        mainwindow.h \
    central_w.h \
    music_point.h \
    music_window.h \
    key_board.h \
    export_image.h \
    read_midi.h \
    export_check_btn.h \
    note_stretch.h

FORMS += \
        mainwindow.ui \
    central_w.ui \
    music_point.ui \
    music_window.ui \
    key_board.ui \
    export_check_btn.ui

RESOURCES += \
    res.qrc
