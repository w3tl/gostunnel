#-------------------------------------------------
#
# Project created by QtCreator 2014-07-22T09:03:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gostunnel-gui
TEMPLATE = app

win32 {
    WINSDK_DIR = C:\\Program Files (x86)\\Microsoft SDKs\\Windows\\v7.0A\\Bin
    WIN_PWD = $$replace(PWD, /, \\)
    OUT_PWD_WIN = $$replace(OUT_PWD, /, \\)
    QMAKE_POST_LINK = \"$$WINSDK_DIR\\mt.exe\" -manifest \"$$WIN_PWD\\$$basename(TARGET).manifest\" -outputresource:\"$$OUT_PWD_WIN\\${DESTDIR_TARGET}\"\;1
}

RC_FILE = gostunnel-gui.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    qmainsettings.cpp \
    aboutwidget.cpp

HEADERS  += mainwindow.h \
    qmainsettings.h \
    aboutwidget.h

FORMS    += mainwindow.ui \
    about.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    gostunnel-gui.rc
