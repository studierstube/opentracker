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
 * abstract base class for different implementations of the StbDistrManager.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: DistributionStrategy.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _DISTRIBUTIONSTRATEGY_H_
#define _DISTRIBUTIONSTRATEGY_H_

#include <Inventor/SbBasic.h>
#include <stbapi/workspace/SoContextManagerKit.h>

class SbName;
class SbString;
class SoUserKit;

/**
 * This abstract class defines the interface for any implementation of the
 * DistributionManager functionality. The StbDistrManager instantiates any
 * of the implementation subclasses and forwards any messages to them to 
 * achieve a certain functionality.
 * @ingroup sman
 */
class DistributionStrategy  
{
public:
    DistributionStrategy()
    {};

    virtual ~DistributionStrategy()
    {};
    
    /** 
    * sends start application to the sman2 
    */
    virtual void startApplication( SbAppID appId, const SbName & locale, int userId ) = 0;    
	virtual void attachApplication( const SbString & appName, const SbName & locale, int userId ) = 0;
    
    virtual void setApplicationMode( SbAppID appId, SbBool mode ) = 0;
    virtual void stopApplication( SbAppID appId ) = 0;
    
    virtual void joinLocale( const SbName  & locale ) = 0;
    virtual void leaveLocale( const SbName  & locale ) = 0;

    virtual void addUser( SoUserKit * userkit ) = 0;
    virtual void removeUser( SoUserKit * userKit ) = 0;

    virtual void sheetBarChanged( int userId, SbAppID appId, int whichChild ) = 0;
    virtual void windowBarChanged( int userId, int style, int state ) = 0;
	virtual void taskBarChanged( int userId, int buttonId ) = 0;    
  	virtual void systemBarChanged( int userId, int buttonId, int tabInfo ) = 0;
    virtual void focusChanged( int userId, SbAppID appId ) = 0;
    virtual void setFocus(int userId, const SbName & windowName, SbAppID appId) = 0;
	virtual void useResources( int userId, const SbName & windowName, SbAppID appId ) = 0;
};

#endif // !defined(_DISTRIBUTIONSTRATEGY_H_)
