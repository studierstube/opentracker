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
 * distribution manager
 *
 * @author Gerd Hesina, Jan Prikryl, Gerhard Reitmayr
 *
 * $Id: StbDistrManager.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _STBDISTRMANAGER_H_
#define _STBDISTRMANAGER_H_

#include <string>
#include <map>
#include <stbapi/stbapi.h>
#include <stbapi/workspace/windef.h>
#include <stbapi/workspace/SoContextManagerKit.h>

#define CHECKFORMASTER \
   SoContextKit *ctx; \
   if(uk->currentApp == 0) \
	{ \
       ctx=NULL; \
	} \
   else \
   { \
       if(!(SoContextManagerKit::getInstance()->getApp(uk->currentApp))) return; \
       ctx=SoContextManagerKit::getInstance()->getApp(uk->currentApp)->getContext(); \
   } \
   if(ctx&&(ctx->getMasterMode()==FALSE)) return; 

enum MPROXY_MESSAGES
{
	MPROXY_CREATE_MULTICAST_GROUP,
	MPROXY_DELETE_MULTICAST_GROUP,
	MPROXY_MULTICAST_DATA,
	MPROXY_SAG_NEW_GROUP,
    MPROXY_SAG_DELETE_GROUP
};

class SoUserKit;
class DistributionStrategy;

/**
 * The class StbDistrManager class is a Studierstube manager class to handle
 * all tasks related to the distributed system. It is implemented as a singleton.
 * It provides an internal API to be used only by other manager classes. Therefore
 * typical application development need not be concerned with this class.
 *
 * Upon startup it tries to connect to a session manager 2 running on a given
 * address. If this fails it operates in standalone mode and will behave 
 * transparent to the system. If it can reach a session manager it will behave
 * as part of the distributed system and relate all information to the session
 * manager.
 *
 * Internally it delegates all work to an instance of the class DistributionStrategy.
 * There are two different derived classes LocalDistribution and RemoteDistribution,
 * that implement the two different behaviours.
 *
 * @author Gerhard Reitmayr
 * @ingroup workspace
 * @ingroup sman
 */
class STBAPI_API StbDistrManager
{    
public:   
    
    ~StbDistrManager();
    /// performs some initialization tasks
    void init(void);           

    /** 
    * notifies the session manager that an application is started.
    */
    void startApplication( SbAppID appId, const SbName & locale, int userId );
    /**
     * notifies the session manager that it wants to attach a running application
     * to a certain locale.
     */
	void attachApplication( const SbString & appName, const SbName & locale, int userId );
    /**
     * sets the master/slave state of the application.
     */
    void setApplicationMode( SbAppID appId, SbBool mode );
    /**
     * stops an application in the locale it is running. If no other locales contain
     * this application, it will be stopped and removed from the system completely.
     */
    void stopApplication( SbAppID appId );
    
    /**
     * notifies the session manager that it wants to join a shared locale. The locale
     * must already be exist on the local instance.
     */
    void joinLocale( const SbName  & locale );
    /**
     * notifies the session manager that it wants to leave a shared locale.
     */
    void leaveLocale( const SbName  & locale );

    /**
     * adds a certain user to the shared system. The user must already be exist
     * in the local instance.
     */
    void addUser( SoUserKit * userkit );
    /** 
     * removes a user from a shared locale.
     */
    void removeUser( SoUserKit * userKit );

    void windowBarChanged( int userId, int style, int state );
	void sheetBarChanged( int userId, SbAppID appId, int whichChild );
	void taskBarChanged( int userId, int buttonId );    
  	void systemBarChanged( int userId, int buttonId, int tabInfo );
    void focusChanged( int userId, SbAppID appId );
    void setFocus(int userId, const SbName & windowName, SbAppID appId);
	void useResources( int userId, const SbName & windowName, SbAppID appId );
    
    void shutDown(void);

    bool isActive(void) 
    {
        return active;
    }
    
    /// returns the singleton instance which is created if necessary
    static StbDistrManager* getInstance(void); 

protected:
    /// flag to note whether we are connected or not !
    bool active;
    /// the actual work is done by this object which is a subclass of
    /// DistributionStrategy.
    DistributionStrategy * impl;    
    /// protected constructor can only be called by getInstance to create the singleton
    StbDistrManager();
    /// a static pointer to the singleton instance
    static StbDistrManager* instance;       

    //int sagSocket;
};

#endif
