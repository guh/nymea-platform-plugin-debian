#ifndef SYSTEMPLUGINDEBIANLINUX_H
#define SYSTEMPLUGINDEBIANLINUX_H

#include "debian-linux_global.h"

#include <QObject>
#include "platform/platformplugin.h"

class DEBIANLINUXSHARED_EXPORT PlatformPluginDebianLinux: public PlatformPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.nymea.PlatformPlugin")
    Q_INTERFACES(PlatformPlugin)

public:
    PlatformPluginDebianLinux(QObject *parent = nullptr);

    PlatformSystemController* systemController() const override;
    PlatformUpdateController* updateController() const override;

private:
    PlatformSystemController* m_systemController = nullptr;
    PlatformUpdateController* m_updateController = nullptr;
};

#endif // PLATFORMPLUGINDEBIANLINUX_H
