#include "debianlinuxupdatecontroller.h"
#include "loggingcategories.h"

#include <QProcess>
#include <QTimer>

DebianLinuxUpdateController::DebianLinuxUpdateController(QObject *parent):
    PlatformUpdateController(parent)
{
    checkForUpdates();
    QTimer *refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &DebianLinuxUpdateController::checkForUpdates);
    refreshTimer->start(1000 * 60 * 60);
}

bool DebianLinuxUpdateController::updateManagementAvailable()
{
    return true;
}

QString DebianLinuxUpdateController::currentVersion() const
{
    return m_installedVersion;
}

QString DebianLinuxUpdateController::candidateVersion() const
{
    return m_candidateVersion;
}

void DebianLinuxUpdateController::checkForUpdates()
{
    enqueueJob("apt-get update", SLOT(aptUpdateFinished(int)));
    enqueueJob("apt-cache policy nymea", SLOT(aptCachePolicyFinished(int)));
}

bool DebianLinuxUpdateController::updateAvailable() const
{
    return m_installedVersion != m_candidateVersion;
}

bool DebianLinuxUpdateController::startUpdate()
{
    enqueueJob("apt-get --yes dist-upgrade", SLOT(aptUpgradeFinished(int)));
    checkForUpdates();
}

bool DebianLinuxUpdateController::updateInProgress() const
{
    return m_apt != nullptr;
}

QStringList DebianLinuxUpdateController::availableChannels() const
{
    return {"stable", "candidate"};
}

QString DebianLinuxUpdateController::currentChannel() const
{
    return m_currentChannel;
}

bool DebianLinuxUpdateController::selectChannel(const QString &channel)
{
    if (channel == m_currentChannel) {
        return true;
    }
    // Obtain a list of installed plugins:
    enqueueJob("apt-cache policy nymea-plugin*", SLOT(fetchPluginsFinished(int)));

    if (channel == "stable") {
        enqueueJob({"apt-add-repository", "--remove", "deb http://ci-repo.nymea.io/landing-silo bionic main"}, SLOT(aptAddRepoFinished(int)));
    } else if(channel == "candidate") {
        enqueueJob({"apt-add-repository", "deb http://ci-repo.nymea.io/landing-silo bionic main"}, SLOT(aptAddRepoFinished(int)));
    }
    checkForUpdates();
    enqueueJob("apt-get remove --yes libnymea1", SLOT(aptRemoveFinished(int)));
    enqueueJob("apt-get install --yes nymea", SLOT(aptInstallFinished(int)));
    checkForUpdates();
    return true;
}

void DebianLinuxUpdateController::enqueueJob(const QString &command, const QString &finishedSlot)
{
    m_jobQueue.append(Job(command.split(" "), finishedSlot));
    processQueue();
}

void DebianLinuxUpdateController::enqueueJob(const QStringList &command, const QString &finishedSlot)
{
    m_jobQueue.append(Job(command, finishedSlot));
    processQueue();
}

void DebianLinuxUpdateController::processQueue()
{
    if (m_apt) {
        // busy...
//        qCDebug(dcPlatform) << "Job queue busy.";
        return;
    }
    if (m_jobQueue.isEmpty()) {
//        qCDebug(dcPlatform) << "Job queue empty.";
        return;
    }

    Job job = m_jobQueue.first();

    m_apt = new QProcess(this);

    emit updateStatusChanged();

    // Workaround redundant signal definitions in QProcess with Qt 5.
    typedef void (QProcess:: *finishedSignal)(int exitCode);

    connect(m_apt, SIGNAL(finished(int)), this, job.finishedSlot.toUtf8().data());

    connect(m_apt, static_cast<finishedSignal>(&QProcess::finished), this, [this](int exitCode){
        QByteArray stdErr = m_apt->readAllStandardError();
        m_apt->deleteLater();
        m_apt = nullptr;

        switch (exitCode) {
        case 0:
//            qCDebug(dcPlatform) << "Apt finished with success.";
            m_jobQueue.removeFirst();
            if (m_jobQueue.isEmpty()) {
                emit updateStatusChanged();
            } else {
                processQueue();
            }
            break;
        case 100:
            qCWarning(dcPlatform) << "Failed to obtain dpkg lock. Waiting 5 seconds...";
            qCWarning(dcPlatform) << qUtf8Printable(stdErr);
            QTimer::singleShot(5000, this, &DebianLinuxUpdateController::processQueue);
            break;
        default:
            qCWarning(dcPlatform) << "Unhandled apt error:" << exitCode;
            qCWarning(dcPlatform) << qUtf8Printable(stdErr);
        }
    });

    qCDebug(dcPlatform) << "Starting:" << job.command.join(" ");
    QStringList commandWithArgs = job.command;
    m_apt->start(commandWithArgs.takeFirst(), commandWithArgs);
}

void DebianLinuxUpdateController::aptUpdateFinished(int exitCode)
{
    if (exitCode != 0) {
        qCWarning(dcPlatform) << "apt-get update failed with status code:" << exitCode;
        return;
    }

    QByteArray stdOut = m_apt->readAllStandardOutput();
    qCDebug(dcPlatform) << "apt-get update finished with success";
    qCDebug(dcPlatform) << stdOut;
}

void DebianLinuxUpdateController::aptCachePolicyFinished(int exitCode)
{
    if (exitCode != 0) {
        return;
    }
    QByteArray data = m_apt->readAllStandardOutput();

    bool changed = false;
    QString channel = "stable";
    foreach (const QByteArray &line, data.split('\n')) {
//        qCDebug(dcPlatform) << "apt cache policy:" << line;
        if (line.contains("Installed:")) {
            QList<QByteArray> parts = line.split(':');
            if (parts.count() != 2) {
                qCWarning(dcPlatform) << "Error parsing apt-cache response:" << line;
            } else {
                QString installedVersion = parts.at(1).trimmed();
                if (m_installedVersion != installedVersion) {
                    m_installedVersion = installedVersion;
                    changed = true;
                }
            }
        }
        if (line.contains("Candidate:")) {
            QList<QByteArray> parts = line.split(':');
            if (parts.count() != 2) {
                qCWarning(dcPlatform) << "Error parsing apt-cache response:" << line;
            } else {
                QString candidateVersion = parts.at(1).trimmed();
                if (m_candidateVersion != candidateVersion) {
                    m_candidateVersion = candidateVersion;
                    changed = true;
                }
            }
        }
        if (line.contains("landing-silo")) {
            channel = "candidate";
        }
    }
    if (m_currentChannel != channel) {
        m_currentChannel = channel;
        changed = true;
    }

    qCDebug(dcPlatform) << "Installed version:" << m_installedVersion;
    qCDebug(dcPlatform) << "Candidate version:" << m_candidateVersion;
    qCDebug(dcPlatform) << "Available channels:" << availableChannels();
    qCDebug(dcPlatform) << "Current channel:" << m_currentChannel;

    if (changed) {
        emit updateStatusChanged();
    }
}

void DebianLinuxUpdateController::aptUpgradeFinished(int exitCode)
{
    if (exitCode != 0) {
        return;
    }
    qCDebug(dcPlatform) << "apt-get upgrade finished with success";
}

void DebianLinuxUpdateController::aptAddRepoFinished(int exitCode)
{
    qCDebug(dcPlatform) << "apt-add-repository finished" << m_apt->readAllStandardOutput();
}

void DebianLinuxUpdateController::aptRemoveFinished(int exitCode)
{
    qCDebug(dcPlatform) << "apt-get remove nymea finished";
}

void DebianLinuxUpdateController::aptInstallFinished(int exitCode)
{
    qCDebug(dcPlatform) << "apt-get install nymea finished";
}

void DebianLinuxUpdateController::fetchPluginsFinished(int exitCode)
{
    if (exitCode != 0) {
        qCWarning(dcPlatform) << "Error reading plugins. Aborting upgrade";
        m_jobQueue.clear();
        checkForUpdates();
        return;
    }    QString currentPlugin;
    while (m_apt->canReadLine()) {
        QString line = m_apt->readLine();
        if (line.startsWith("nymea-plugin")) {
            currentPlugin = line.split(":").first();
        } else if (line.startsWith(" ***")) {
            qCDebug(dcPlatform) << "Scheduling reinstall of plugin:" << currentPlugin;
            enqueueJob("apt-get install --yes " + currentPlugin, SLOT(installPluginFinished(int)));
        }
    }
}

void DebianLinuxUpdateController::installPluginFinished(int exitCode)
{
    qCDebug(dcPlatform) << "Plugin installation" << (exitCode == 0 ? "succeeded" : "failed");
}
