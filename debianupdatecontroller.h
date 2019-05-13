/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Copyright (C) 2019 Michael Zanetti <michael.zanetti@nymea.io>          *
 *                                                                         *
 *  This file is part of nymea.                                            *
 *                                                                         *
 *  nymea is free software: you can redistribute it and/or modify          *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, version 2 of the License.                *
 *                                                                         *
 *                                                                         *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with nymea. If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef DEBIANUPDATECONTROLLER_H
#define DEBIANUPDATECONTROLLER_H

#include <QObject>
#include <QProcess>
#include <QNetworkAccessManager>

#include "platform/platformupdatecontroller.h"

class DebianUpdateController : public PlatformUpdateController
{
    Q_OBJECT
public:
    explicit DebianUpdateController(QObject *parent = nullptr);

    bool updateManagementAvailable() override;
    virtual QString currentVersion() const override;
    virtual QString candidateVersion() const override;

//    virtual QMap<QString, QString> changelog() const = 0;

    virtual void checkForUpdates() override;
    virtual bool updateAvailable() const override;
    virtual bool startUpdate() override;

    bool rollbackAvailable() const override { return false; }
    bool startRollback() override {}

    virtual bool updateInProgress() const override;

    virtual QStringList availableChannels() const override;
    QString currentChannel() const override;
    virtual bool selectChannel(const QString &channel) override;

private:
    class Job {
    public:
        Job(const QStringList &command, const QString &finishedSlot): command(command), finishedSlot(finishedSlot) {}
        QStringList command;
        QString finishedSlot;
    };

private slots:
    void enqueueJob(const QString &command, const QString &finishedSlot);
    void enqueueJob(const QStringList &command, const QString &finishedSlot);
    void processQueue();

    void aptUpdateFinished(int exitCode);
    void aptCachePolicyFinished(int exitCode);
    void aptUpgradeFinished(int exitCode);
    void aptAddRepoFinished(int exitCode);
    void aptRemoveFinished(int exitCode);
    void aptInstallFinished(int exitCode);
    void fetchPluginsFinished(int exitCode);
    void installPluginFinished(int exitCode);
private:
    QProcess *m_apt = nullptr;

    QList<Job> m_jobQueue;

    QNetworkAccessManager m_nam;

    QString m_installedVersion;
    QString m_candidateVersion;

    QStringList m_availableChannels;
    QString m_currentChannel = "stable";
};

#endif // DEBIANLINUXUPDATECONTROLLER_H
