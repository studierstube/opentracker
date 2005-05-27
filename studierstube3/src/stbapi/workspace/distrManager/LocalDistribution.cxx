/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/**
 * implementation of the local distribution strategy.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: LocalDistribution.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <stbapi/workspace/distrManager/LocalDistribution.h>
#include <stbapi/resource/SoUserManagerKit.h>

LocalDistribution::LocalDistribution()
{

}

LocalDistribution::~LocalDistribution()
{

}

void LocalDistribution::startApplication( SbAppID appId, const SbName & locale, int userId )
{
}

void LocalDistribution::attachApplication( const SbString & appName, const SbName & locale, int userId )
{
}

void LocalDistribution::setApplicationMode( SbAppID appId, SbBool mode )
{
}

void LocalDistribution::stopApplication( SbAppID appId )
{
}

void LocalDistribution::joinLocale( const SbName  & locale )
{
}

void LocalDistribution::leaveLocale( const SbName  & locale )
{
}

void LocalDistribution::addUser( SoUserKit * userkit )
{
}

void LocalDistribution::removeUser( SoUserKit * userKit )
{
}

void LocalDistribution::sheetBarChanged( int userId, SbAppID appId, int whichChild )
{
    SoUserManagerKit::getInstance()->sheetBarChanged( userId, (int)appId, whichChild );
}

void LocalDistribution::windowBarChanged( int userId, int style, int state )
{
    SoUserManagerKit::getInstance()->windowBarChanged( userId, style, state );
}

void LocalDistribution::taskBarChanged( int userId, int buttonId )
{
    SoUserManagerKit::getInstance()->taskBarChanged( userId, buttonId );
}

void LocalDistribution::systemBarChanged( int userId, int buttonId, int tabInfo )
{
    SoUserManagerKit::getInstance()->systemBarChanged( userId, buttonId, tabInfo );
}

void LocalDistribution::focusChanged( int userId, SbAppID appId )
{
    SoUserManagerKit::getInstance()->focusChanged( userId, appId );
}

void LocalDistribution::setFocus(int userId, const SbName & windowName, SbAppID appId)
{
}

void LocalDistribution::useResources( int userId, const SbName & windowName, SbAppID appId )
{
    SoUserManagerKit::getInstance()->getUserKit(userId)->useResources(appId);
}
