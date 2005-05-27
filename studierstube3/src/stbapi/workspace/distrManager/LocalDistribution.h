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
 * an almost null implementation of a DistributionStrategy for operating
 * in standalone mode.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: LocalDistribution.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _LOCALDISTRIBUTION_H_
#define _LOCALDISTRIBUTION_H_

#include <stbapi/workspace/distrManager/DistributionStrategy.h>

/**
 * This class implements an almost null implementation of a DistributionStrategy.
 * It is used by the StbDistrManager class, if no connection to a session manager
 * could be established. It only forwards certain method calls and ignores the rest.
 * @ingroup sman
 */
class LocalDistribution : public DistributionStrategy  
{
public:
	LocalDistribution();
	virtual ~LocalDistribution();

    /** 
    * sends start application to the sman2 
    */
    virtual void startApplication( SbAppID appId, const SbName & locale, int userId );    
	virtual void attachApplication( const SbString & appName, const SbName & locale, int userId );
    
    virtual void setApplicationMode( SbAppID appId, SbBool mode );
    virtual void stopApplication( SbAppID appId );
    
    virtual void joinLocale( const SbName  & locale );
    virtual void leaveLocale( const SbName  & locale );

    virtual void addUser( SoUserKit * userkit );
    virtual void removeUser( SoUserKit * userKit );    

    virtual void sheetBarChanged( int userId, SbAppID appId, int whichChild );
    virtual void windowBarChanged( int userId, int style, int state );
    virtual void taskBarChanged( int userId, int buttonId );    
    virtual void systemBarChanged( int userId, int buttonId, int tabInfo );
    virtual void focusChanged( int userId, SbAppID appId );
    virtual void setFocus(int userId, const SbName & windowName, SbAppID appId);
	virtual void useResources( int userId, const SbName & windowName, SbAppID appId );  
};

#endif // !defined(_LOCALDISTRIBUTION_H_)
