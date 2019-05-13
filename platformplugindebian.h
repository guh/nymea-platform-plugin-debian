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
