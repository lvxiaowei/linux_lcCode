#-------------------------------------------------
#
# Project created by QtCreator 2018-07-30T09:50:24
#
#-------------------------------------------------

QT       += core gui xml sql serialport
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LcdApp_01
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

target.files = LcdApp_01
target.path = /home
INSTALLS += target

SOURCES += \
        main.cpp \
    schedule.cpp \
    baseClassWgt.cpp \
    mainWindow.cpp \
    frm/Frm_chainProcessingClass.cpp \
    serialComm.cpp \
    xddpComm.cpp \
    api/myMessageBox.cpp \
    api/comConfigClass.cpp \
    api/DBProcessingClass.cpp \
    frm/Frm_settingMenu.cpp \
    frm/Frm_testingMenu.cpp \
    frm/Frm_patternManage.cpp \
    frm/Frm_timingTable.cpp \
    frm/Frm_parameterSettings.cpp \
    api/myhelper.cpp \
    ttkmarqueewidget.cpp

HEADERS += \
    schedule.h \
    baseClassWgt.h \
    mainWindow.h \
    api/myhelper.h \
    frm/Frm_chainProcessingClass.h \
    serialComm.h \
    xddpComm.h \
    api/myMessageBox.h \
    api/comConfigClass.h \
    api/publicData.h \
    api/DBProcessingClass.h \
    frm/Frm_settingMenu.h \
    frm/Frm_testingMenu.h \
    frm/Frm_patternManage.h \
    frm/Frm_timingTable.h \
    frm/Frm_parameterSettings.h \
    ttkmarqueewidget.h

FORMS += \
    mainWindow.ui \
    frm/Frm_chainProcessingClass.ui \
    api/myMessageBox.ui \
    frm/Frm_settingMenu.ui \
    frm/Frm_testingMenu.ui \
    frm/Frm_patternManage.ui \
    frm/Frm_timingTable.ui \
    mainWindowPop.ui \
    frm/Frm_parameterSettings.ui \
    frm/Frm_parameterSettingsPop.ui


RESOURCES += \
    style.qrc

DISTFILES +=

