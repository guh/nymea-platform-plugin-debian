#include "platformplugindebianlinux.h"
#include "debianlinuxsystemcontroller.h"
#include "debianlinuxupdatecontroller.h"

PlatformPluginDebianLinux::PlatformPluginDebianLinux(QObject *parent): PlatformPlugin(parent)
{
    m_systemController = new DebianLinuxSystemController(this);
    m_updateController = new DebianLinuxUpdateController(this);
}

PlatformSystemController *PlatformPluginDebianLinux::systemController() const
{
    return m_systemController;
}

PlatformUpdateController *PlatformPluginDebianLinux::updateController() const
{
    return m_updateController;
}
