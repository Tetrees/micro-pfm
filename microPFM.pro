#-------------------------------------------------
#
# Project created by QtCreator 2012-06-23T10:38:35
#
#-------------------------------------------------

QT       += core gui
QT       += xml

TARGET = microPFM
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fileinfoitem.cpp \
    projectitem.cpp \
    browsermodel.cpp \
    projectslist.cpp \
    browserproxymodel.cpp \
    filesetmodel.cpp \
    filesetproxymodel.cpp \
    newprojectdialog.cpp \
    calendardelegate.cpp \
    projectfolderedit.cpp

HEADERS  += mainwindow.h \
    fileinfoitem.h \
    projectitem.h \
    FileInfo.h \
    projectslist.h \
    browsermodel.h \
    browserproxymodel.h \
    filesetmodel.h \
    filesetproxymodel.h \
    newprojectdialog.h \
    calendardelegate.h \
    projectfolderedit.h \
    constDefinitions.h

FORMS    += mainwindow.ui \
    newprojectdialog.ui

RESOURCES += \
    Resourse.qrc

RC_FILE = microPFM.rc
