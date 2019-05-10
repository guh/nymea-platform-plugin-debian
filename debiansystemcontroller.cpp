#include <QDBusInterface>
#include <QDBusPendingReply>

#include "debiansystemcontroller.h"

#include "loggingcategories.h"

DebianSystemController::DebianSystemController(QObject *parent):
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

bool DebianSystemController::powerManagementAvailable() const
{
    return m_canControlPower;
}

bool DebianSystemController::reboot()
{
    qCDebug(dcPlatform) << "Rebooting";
    QDBusInterface logind("org.freedesktop.login1", "/org/freedesktop/login1", "org.freedesktop.login1.Manager", QDBusConnection::systemBus());
    QDBusPendingReply<> powerOff = logind.callWithArgumentList(QDBus::Block, "Reboot", {true, });
    powerOff.waitForFinished();
    if (powerOff.isError()) {
        const auto error = powerOff.error();
        qCWarning(dcPlatform) << "Error calling reboot on logind.";
        return false;
    }
    return true;
}

bool DebianSystemController::shutdown()
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
