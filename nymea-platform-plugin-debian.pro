QT -= gui
QT += network dbus

TARGET = nymea_platformplugindebian
TEMPLATE = lib

CONFIG += link_pkgconfig c++11
PKGCONFIG += nymea

DEFINES += DEBIANLINUX_LIBRARY

SOURCES += \
    platformplugindebian.cpp \
    debiansystemcontroller.cpp \
    debianupdatecontroller.cpp \


HEADERS += \
    platformplugindebian.h \
    debiansystemcontroller.h \
    debianupdatecontroller.h \


target.path = $$[QT_INSTALL_LIBS]/nymea/platforms/
INSTALLS += target
