#include "platformplugindebian.h"
#include "debiansystemcontroller.h"
#include "debianupdatecontroller.h"

PlatformPluginDebian::PlatformPluginDebian(QObject *parent): PlatformPlugin(parent)
{
    m_systemController = new DebianSystemController(this);
    m_updateController = new DebianUpdateController(this);
}

PlatformSystemController *PlatformPluginDebian::systemController() const
{
    return m_systemController;
}

PlatformUpdateController *PlatformPluginDebian::updateController() const
{
    return m_updateController;
}
