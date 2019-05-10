#ifndef DEBIANSYSTEMCONTROLLER_H
#define DEBIANSYSTEMCONTROLLER_H

#include <QObject>

#include "platform/platformsystemcontroller.h"

class DebianSystemController : public PlatformSystemController
{
    Q_OBJECT
public:
    explicit DebianSystemController(QObject *parent = nullptr);

    bool powerManagementAvailable() const override;

    bool reboot() override;
    bool shutdown() override;

private:
    bool m_canControlPower = false;;
};

#endif // DEBIANLINUXSYSTEMCONTROLLER_H
