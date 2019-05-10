#ifndef DEBIANLINUXSYSTEMCONTROLLER_H
#define DEBIANLINUXSYSTEMCONTROLLER_H

#include <QObject>

#include "platform/platformsystemcontroller.h"

class DebianLinuxSystemController : public PlatformSystemController
{
    Q_OBJECT
public:
    explicit DebianLinuxSystemController(QObject *parent = nullptr);

    bool powerManagementAvailable() const override;

    bool reboot() override;
    bool shutdown() override;

private:
    bool m_canControlPower = false;;
};

#endif // DEBIANLINUXSYSTEMCONTROLLER_H
