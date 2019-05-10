#include <QDBusInterface>
#include <QDBusPendingReply>

#include "debianlinuxsystemcontroller.h"

#include "loggingcategories.h"

DebianLinuxSystemController::DebianLinuxSystemController(QObject *parent):
    PlatformSystemController(parent)
{

    QDBusInterface logind("org.freedesktop.login1", "/org/freedesktop/login1", "org.freedesktop.login1.Manager", QDBusConnection::systemBus());

    const auto message = logind.callWithArgumentList(QDBus::Block, "CanPowerOff", {});
    QDBusPendingReply<QString> canPowerOff = message;
    canPowerOff.waitForFinished();

    if (canPowerOff.isError()) {
        qCWarning(dcPlatform) << "DBus call to logind failed:" << canPowerOff.error().name() <<  canPowerOff.error().message();
    } else if (canPowerOff.value() == "yes") {
        m_canControlPower = true;
    }
}

bool DebianLinuxSystemController::powerManagementAvailable() const
{
    return m_canControlPower;
}

bool DebianLinuxSystemController::reboot()
{
    qCDebug(dcPlatform) << "Rebooting";
    return true;
}

bool DebianLinuxSystemController::shutdown()
{
    qCDebug(dcPlatform()) << "Shutting down...";
    QDBusInterface logind("org.freedesktop.login1", "/org/freedesktop/login1", "org.freedesktop.login1.Manager", QDBusConnection::systemBus());
    QDBusPendingReply<> powerOff = logind.callWithArgumentList(QDBus::Block, "PowerOff", {true, });
    powerOff.waitForFinished();
    if (powerOff.isError()) {
        const auto error = powerOff.error();
        qCWarning(dcPlatform) << "Error calling poweroff on logind.";
        return false;
    }
    return true;
}
