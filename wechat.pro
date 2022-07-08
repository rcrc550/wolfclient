QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(./netapi/netapi.pri)
INCLUDEPATH += ./netapi

include(./MD5/md5.pri)
include(.\AudioApi/audioapi.pri)
include(.\VideoApi/videoapi.pri)
INCLUDEPATH += ./VideoApi
INCLUDEPATH += ./MD5
INCLUDEPATH += ./audioapi

SOURCES += \
    ckernel.cpp \
    halldialog.cpp \
    logindialog.cpp \
    main.cpp \
    roomdialog.cpp \
    threadworker.cpp \
    usershow.cpp \
    wechatdialog.cpp \
    wolfdialog.cpp \
    wolfplaydialog.cpp

HEADERS += \
    ckernel.h \
    halldialog.h \
    logindialog.h \
    roomdialog.h \
    threadworker.h \
    usershow.h \
    wechatdialog.h \
    wolfdialog.h \
    wolfplaydialog.h

FORMS += \
    halldialog.ui \
    logindialog.ui \
    roomdialog.ui \
    usershow.ui \
    wechatdialog.ui \
    wolfdialog.ui \
    wolfplaydialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    images/wolf.png
