#-------------------------------------------------
#
# Project created by QtCreator 2017-09-12T11:50:21
#
#-------------------------------------------------

QT       += core gui xlsx sql printsupport axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AddressManagement
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


SOURCES += main.cpp\
        mainwindow.cpp\
        addressadddialog.cpp \
    groupadddialog.cpp \
    tableprinter.cpp \
    addressdetaildialog.cpp \
    addressdownloaddialog.cpp \
    duplicationdialog.cpp \
    groupselectdialog.cpp

HEADERS  += mainwindow.h \
    define.h\
    addressadddialog.h \
    groupadddialog.h \
    tableprinter.h \
    addressdetaildialog.h \
    addressdownloaddialog.h \
    duplicationdialog.h \
    groupselectdialog.h

FORMS    += mainwindow.ui \
    addressadddialog.ui \
    groupadddialog.ui \
    addressdetaildialog.ui \
    addressdownloaddialog.ui \
    duplicationdialog.ui \
    groupselectdialog.ui

RESOURCES += \
    addressmanagement.qrc

win32
{
   RC_FILE = AddressManagement.rc
   CONFIG += embed_manifest_exe
   QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:level=\'requireAdministrator\'
}

TRANSLATIONS += Lang_ko_KR.ts
