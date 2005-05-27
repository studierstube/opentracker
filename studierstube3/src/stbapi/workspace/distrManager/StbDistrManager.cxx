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
 * distribution manager implementation
 *
 * @author Gerd Hesina, Jan Prikryl, Gerhard Reitmayr
 *
 * $Id: StbDistrManager.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <ace/SOCK_Connector.h>
#include <ace/INET_Addr.h>
#include <ace/Reactor.h>

#include <stbapi/workspace/distrManager/StbDistrManager.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/workspace/SoLocaleManagerKit.h>
#include <stbapi/workspace/SoLocaleKit.h>
#include <stbapi/workspace/distrManager/ConnectionHandler.h>
#include <stbapi/workspace/distrManager/LocalDistribution.h>
#include <stbapi/workspace/distrManager/RemoteDistribution.h>
#include <stbapi/util/ivio.h>

using namespace std;

StbDistrManager* StbDistrManager::instance = NULL;

StbDistrManager::StbDistrManager()
// :
//sagSocket(-1)
{
 	impl = NULL;

}

StbDistrManager::~StbDistrManager()
{
    if (impl != NULL) delete impl;
}

StbDistrManager * StbDistrManager::getInstance(void)
{
    if(instance == NULL) {
        instance = new StbDistrManager;
        instance->init();
    }

    return instance;
}

void StbDistrManager::init(void)
{
   // open connection
	const char * sessionManagerAddress = StbWorkspace::getInstance()->getSessionManagerAddress().getString();
	int sessionManagerPort = StbWorkspace::getInstance()->getSessionManagerPort();
	
    ACE_DEBUG((LM_INFO,"SETUP: contacting sessionmanager at %s:%i\n", sessionManagerAddress, sessionManagerPort ));

    ConnectionHandler * handler = new ConnectionHandler();
    if( handler->open( sessionManagerAddress, sessionManagerPort ) != 0 )
    {
        ACE_DEBUG((LM_ERROR,"INFO: StbDistrManager could not connect to SessionManager\n"));
        impl = new LocalDistribution();
        delete handler;
        active = false;
    }
    else
    {
        ACE_DEBUG((LM_INFO,"INFO: StbDistrManager connect to SessionManager at %s:%d\n", 
            sessionManagerAddress, sessionManagerPort ));
        RemoteDistribution * remote = new RemoteDistribution( handler );
        handler->setVisitor( remote );
        impl = remote;
    }

    /*
    sagSocket = createMulticastGroup("224.100.200.220", "12843");
    if(sagSocket<0)
	{
        fprintf(stderr,"cannot create multicastgroup at address: %s, port: "
                  "%s\n","224.100.200.220", "12843");
        exit(-1);
	}
    fprintf(stderr,"create new multicastgroup at address: %s, port: "
                  "%s\n","224.100.200.220", "12843");
	*/
}
        
void StbDistrManager::startApplication( SbAppID appId, const SbName  & locale, int userId )
{
    impl->startApplication( appId, locale, userId );
}

void StbDistrManager::attachApplication( const SbString & appName, const SbName & locale, int userId )
{
	impl->attachApplication( appName, locale, userId );
}

void StbDistrManager::setApplicationMode( SbAppID appId, SbBool mode )
{
    impl->setApplicationMode( appId, mode );
}

void StbDistrManager::stopApplication( SbAppID appId )
{
   impl->stopApplication( appId );
}
    
void StbDistrManager::joinLocale(const SbName  & locale )
{
    impl->joinLocale( locale );
}

void StbDistrManager::leaveLocale(const SbName  & locale )
{
    impl->leaveLocale( locale );
}

void StbDistrManager::addUser( SoUserKit * userKit )
{   
    impl->addUser( userKit );
}

void StbDistrManager::removeUser( SoUserKit * userKit )
{
    impl->removeUser( userKit );    
}

void StbDistrManager::windowBarChanged( int userId, int style, int state )
{
    impl->windowBarChanged( userId, style, state );
}
	
void StbDistrManager::sheetBarChanged(int userId, SbAppID appId, int whichChild)
{    
    impl->sheetBarChanged(userId, appId, whichChild );
}

void StbDistrManager::taskBarChanged( int userId, int buttonId )
{
    impl->taskBarChanged( userId, buttonId );
}

void StbDistrManager::systemBarChanged( int userId, int buttonId, int tabInfo )
{
    impl->systemBarChanged( userId, buttonId, tabInfo );
}

void StbDistrManager::focusChanged( int userId, SbAppID appId )
{
    impl->focusChanged( userId, appId );
}

void StbDistrManager::setFocus(int userId, const SbName & windowName, SbAppID appId)
{
    impl->setFocus( userId, windowName, appId );
}

void StbDistrManager::useResources( int userId, const SbName & windowName, SbAppID appId )
{
    impl->useResources( userId, windowName, appId );
}

void StbDistrManager::shutDown(void)
{
    SoNodeKitListPart * list = SO_GET_PART(SoLocaleManagerKit::getInstance(), "visibleLocales", SoNodeKitListPart);
    assert(list);
    ACE_DEBUG((LM_INFO,"INFO: StbDistrManager::shutDown - leaving all locales\n"));
    for( int i = 0; i < list->getNumChildren(); i++ )
    {
        SoLocaleKit * locale = (SoLocaleKit *) list->getChild(i);
        if( locale->shared.getValue())
        {
            leaveLocale( locale->name.getValue());
        }
    }

}

/* ===========================================================================
    End of StbDistrManager.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */
