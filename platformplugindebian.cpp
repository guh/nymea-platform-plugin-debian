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
