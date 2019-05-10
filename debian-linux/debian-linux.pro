QT -= gui
QT += network dbus

TARGET = nymea_platformplugindebian-linux
TEMPLATE = lib

DEFINES += DEBIANLINUX_LIBRARY

SOURCES += \
    platformplugindebianlinux.cpp \
    debianlinuxsystemcontroller.cpp \
    debianlinuxupdatecontroller.cpp \


HEADERS += \
    platformplugindebianlinux.h \
    debianlinuxsystemcontroller.h \
    debianlinuxupdatecontroller.h \

unix {
    target.path = /usr/lib
    INSTALLS += target
}
