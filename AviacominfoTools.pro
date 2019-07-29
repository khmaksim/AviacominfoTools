#-------------------------------------------------
#
# Project created by QtCreator 2019-04-29T11:00:15
#
#-------------------------------------------------

QT       += core gui network sql quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AviacominfoTools
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
    obstraclesform.cpp \
    obstracleshandler.cpp \
    listitemdelegate.cpp \
    searchmodel.cpp \
    qgroupheaderview.cpp \
    filterpanel.cpp \
    sidebar.cpp \
    checkboxitemdelegate.cpp \
    flowlayout.cpp \
    databaseaccess.cpp \
    htmlparser.cpp \
    obstraclestyleditemdelegate.cpp \
    databaseupdate.cpp \
    mapview.cpp \
    mainwindow.cpp \
    sortsearchfilterobstraclemodel.cpp

HEADERS += \
    obstraclesform.h \
    obstracleshandler.h \
    listitemdelegate.h \
    searchmodel.h \
    qgroupheaderview.h \
    filterpanel.h \
    sidebar.h \
    checkboxitemdelegate.h \
    flowlayout.h \
    databaseaccess.h \
    htmlparser.h \
    obstraclestyleditemdelegate.h \
    databaseupdate.h \
    mapview.h \
    mainwindow.h \
    sortsearchfilterobstraclemodel.h

FORMS += \
    obstraclesform.ui \
    sidebar.ui \
    mainwindow.ui

RESOURCES += \
    aviacominfotools.qrc

TRANSLATIONS = aviacominfotools_ru.ts

RC_ICONS = ./res/img/appico.ico
RC_FILE = app.rc

DISTFILES += app.rc \
    app.rc

include(../QtWaitingSpinner-master/qtwaitingspinner.pri)
