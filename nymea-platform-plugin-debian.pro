QT -= gui
QT += network dbus

TARGET = nymea_platformplugindebian-linux
TEMPLATE = lib

CONFIG += link_pkgconfig
PKGCONFIG += nymea

DEFINES += DEBIANLINUX_LIBRARY

SOURCES += \
    platformplugindebianlinux.cpp \
    debianlinuxsystemcontroller.cpp \
    debianlinuxupdatecontroller.cpp \


HEADERS += \
    platformplugindebianlinux.h \
    debianlinuxsystemcontroller.h \
    debianlinuxupdatecontroller.h \


target.path = $$[QT_INSTALL_LIBS]/nymea/platforms/
INSTALLS += target
