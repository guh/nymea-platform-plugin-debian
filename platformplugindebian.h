#ifndef SYSTEMPLUGINDEBIAN_H
#define SYSTEMPLUGINDEBIAN_H

#include <QObject>
#include "platform/platformplugin.h"

class PlatformPluginDebian: public PlatformPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "io.nymea.PlatformPlugin")
    Q_INTERFACES(PlatformPlugin)

public:
    PlatformPluginDebian(QObject *parent = nullptr);

    PlatformSystemController* systemController() const override;
    PlatformUpdateController* updateController() const override;

private:
    PlatformSystemController* m_systemController = nullptr;
    PlatformUpdateController* m_updateController = nullptr;
};

#endif // PLATFORMPLUGINDEBIAN_H
