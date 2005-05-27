/* ========================================================================
 * Copyright (C) 2000,2001,2002  Vienna University of Technology
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: sman2
 * ======================================================================== */
/**
 * implementation of the SessionManager class
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SessionManager.cxx 3427 2004-07-14 12:20:49Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4284)
#endif

#include "SessionManager.h"
#include <stbapi/workspace/distrManager/StartApplicationMessage.h>
#include <stbapi/workspace/distrManager/SetApplicationModeMessage.h>
#include <stbapi/workspace/distrManager/StopApplicationMessage.h>
#include <stbapi/workspace/distrManager/AddUserMessage.h>
#include <stbapi/workspace/distrManager/RemoveUserMessage.h>
#include <stbapi/workspace/distrManager/JoinLocaleMessage.h>
#include <stbapi/workspace/distrManager/LeaveLocaleMessage.h>
#include <stbapi/workspace/distrManager/SetDIVParamMessage.h>
#include <stbapi/workspace/distrManager/UserKitMessage.h>
#include <stbapi/workspace/distrManager/ChangeFocusMessage.h>
#include <stbapi/workspace/distrManager/SetFocusMessage.h>
#include <stbapi/workspace/distrManager/SheetBarMessage.h>
#include <stbapi/workspace/distrManager/SystemBarMessage.h>
#include <stbapi/workspace/distrManager/TaskBarMessage.h>
#include <stbapi/workspace/distrManager/UseResourcesMessage.h>
#include <stbapi/workspace/distrManager/WindowBarMessage.h>
#include <stbapi/workspace/distrManager/UserKitMessageVisitor.h>
#include "SessionVisitorAdaptor.h"

#include <algorithm>

using namespace std;

SessionManager * SessionManager::instance = NULL;

SessionManager::SessionManager()
{
    instance = this;
    factory.addMessageFactory( SessionVisitor::START_APPLICATION,   StartApplicationMessage::create );
    factory.addMessageFactory( SessionVisitor::STOP_APPLICATION,    StopApplicationMessage::create );
    factory.addMessageFactory( SessionVisitor::SET_APPLICATION_MODE,SetApplicationModeMessage::create );
    factory.addMessageFactory( SessionVisitor::ADD_USER,            AddUserMessage::create );
    factory.addMessageFactory( SessionVisitor::REMOVE_USER,         RemoveUserMessage::create );
    factory.addMessageFactory( SessionVisitor::JOIN_LOCALE,         JoinLocaleMessage::create );
    factory.addMessageFactory( SessionVisitor::LEAVE_LOCALE,        LeaveLocaleMessage::create );
    factory.addMessageFactory( SessionVisitor::SET_DIV_PARAM,       SetDIVParamMessage::create );
    // now the sub types of UserKitMessage
    //factory.addMessageFactory( SessionVisitor::USERKIT_PARAM,       UserKitMessage::create );
    factory.addMessageFactory( UserKitMessageVisitor::FOCUSCHANGE,  ChangeFocusMessage::create );
    factory.addMessageFactory( UserKitMessageVisitor::FOCUSSET,     SetFocusMessage::create );
    factory.addMessageFactory( UserKitMessageVisitor::SHEETBAR,     SheetBarMessage::create );
    factory.addMessageFactory( UserKitMessageVisitor::SYSTEMBAR,    SystemBarMessage::create );
    factory.addMessageFactory( UserKitMessageVisitor::TASKBAR,      TaskBarMessage::create );
    factory.addMessageFactory( UserKitMessageVisitor::USE_RESOURCES,UseResourcesMessage::create );
    factory.addMessageFactory( UserKitMessageVisitor::WINDOWBAR,    WindowBarMessage::create );
}

SessionManager::~SessionManager()
{

}

int SessionManager::open(const ACE_INET_Addr & session, const ACE_INET_Addr & monitor)
{
    if( sessionSocket.open( session ) != 0 )
    {
        ACE_DEBUG((LM_DEBUG,"sman2: could not open session socket !\n"));
        return -1;
    }

    if( monitorSocket.open( monitor ) != 0 )
    {
        ACE_DEBUG((LM_DEBUG,"sman2: could not open monitor socket !\n"));
        return -1;
    }

	msg_queue()->high_water_mark( HIGH_WATER_MARK );
	msg_queue()->low_water_mark( LOW_WATER_MARK );

    return 0;
}

void SessionManager::run(void)
{
	activate();
    ACE_Reactor::run_event_loop();
}

int SessionManager::svc(void)
{
	for( ACE_Message_Block * blk; getq( blk ) != -1; )
	{
		SessionVisitorAdaptor::PipeMessage * pipe = (SessionVisitorAdaptor::PipeMessage *) blk->rd_ptr();
		switch( blk->msg_type())
		{
		case ACE_Message_Block::MB_START :
			addHost( pipe->adapter->host );
			break;
		case ACE_Message_Block::MB_STOP :
			removeHost( pipe->adapter->host );
			delete pipe->adapter->host;
			delete pipe->adapter;
			break;
		default :
			pipe->message->execute( *pipe->adapter );
			break;
		}
		delete pipe->message;
		blk->release();
	}
	return 0;
}

SessionManager & SessionManager::getInstance()
{
    return *instance;
}

MessageFactory & SessionManager::getFactoryInstance()
{
    return getInstance().getFactory();
}

void SessionManager::execute( StartApplicationMessage & message, Host & sender )
{
    if( message.attach )  // attach to an already running application ...
    {
        Application * app = findAppByName( message.name );
        if( app == NULL )
        {  // application does not exist ...
            ACE_DEBUG((LM_ERROR,"StartApplication: application %s does not exist !\n", message.name.c_str()));           
            return;
        }
        if( locales.find( message.locale ) == locales.end())
        {
            ACE_DEBUG((LM_ERROR,"StartApplication: locale %s for app %s does not exist !\n",
                message.locale.c_str(), app->appId.c_str()));
            return;
        }

        Locale * locale = locales[message.locale];
        if( locale->applications.find( app ) != locale->applications.end())
        {
            ACE_DEBUG((LM_ERROR,"StartApplication: locale %s contains already app %s !\n",
                message.locale.c_str(), app->appId.c_str()));
            return;
        }

        locale->applications.insert( app );
        auto_ptr<set<Host *> > receivers = getHostsFromLocale( locale );
        if(!receivers->empty())
        {
            message.application = app->appId;            
            sendMessage( *receivers, message );
        }
        if( app->isShared )
        {
            SetDIVParamMessage divMessage;
            divMessage.active = true;
            divMessage.initFromMaster = true;
            divMessage.multicastAddress = app->divAddress;
            divMessage.port = app->divPort;
            divMessage.application = app->appId;
            divMessage.master = false;
            sendMessage( *receivers, divMessage );
        }
        else
        {
            setSharedStatus( app );
        } 
    }
    else // should be a new application
    {
        if( applications.find(message.application) != applications.end() )
        {  // application exists already
            ACE_DEBUG((LM_ERROR,"StartApplication: application %s exists already !\n", message.application.c_str()));           
            return;
        }
        // just print out all errors and stop doing what you do !!
        if( locales.find( message.locale ) == locales.end())
        {
            ACE_DEBUG((LM_ERROR,"StartApplication: locale %s for app %s does not exist !\n",
                message.locale.c_str(), message.application.c_str()));
            return;
        }
        
        Application * app = new Application;
        app->appId = message.application;
        app->name = message.name;
        applications[ message.application ] = app;
        Locale * locale = locales[ message.locale ];
        locale->applications.insert( app );
        sender.masterApps.insert( app );
        
        auto_ptr<set<Host *> > receivers = getHostsFromLocale( locale );
        receivers->erase( &sender );
        if(!receivers->empty())
        {
            sendMessage( *receivers, message );
            setSharedStatus( app );
        }                    
    }
    ACE_DEBUG((LM_INFO,"Started application %s(%s) in locale %s\n\n", 
		message.application.c_str(),message.name.c_str(), message.locale.c_str()));
}

void SessionManager::execute( SetApplicationModeMessage & message, Host & sender )
{
    if( applications.find( message.application ) == applications.end())
    {
        ACE_DEBUG((LM_ERROR,"SetApplicationMode: application %s does not exist !\n", message.application.c_str()));
        return;
    }
    Application * app = applications[ message.application ];
    if( message.master )
    {
        sender.masterApps.insert( app );
    }
    else
    {
        sender.masterApps.erase( app );
    }
    ACE_DEBUG((LM_INFO,"SetApplicationMode for app %s and host %s:%d to %d\n\n", app->appId.c_str(),
        sender.getAddress(), sender.getPort(), message.master ));
}

void SessionManager::execute( StopApplicationMessage & message, Host & sender )
{
    if( applications.find( message.application ) == applications.end())
    {
        ACE_DEBUG((LM_DEBUG,"StopApplication: application %s does not exist !\n", message.application.c_str()));
        return;
    }
    if( locales.find( message.locale ) == locales.end())
    {
        ACE_DEBUG((LM_DEBUG,"StopApplication: locale %s does not exist !\n", message.locale.c_str()));
        return;
    }
    Application * app = applications[ message.application ];
    Locale * locale = locales[ message.locale ];
    locale->applications.erase( app );

    auto_ptr<set<Host *> > hosts = getHostsFromLocale( locale );
    hosts->erase( &sender );
    sendMessage( *hosts, message );
    
    setSharedStatus( app );    

    ACE_DEBUG((LM_INFO,"StopApplication: stopped application %s in %s\n\n", 
        message.application.c_str(), message.locale.c_str()));
}

void SessionManager::execute( AddUserMessage & message, Host & sender )
{
    if( locales.find( message.locale ) == locales.end())
    {
        ACE_DEBUG((LM_ERROR,"AddUser requires non-existring locale %s !\n",
            message.locale.c_str()));
        return;
    }
    // store user in senders user map
    sender.users[message.userId].userId = message.userId;
    sender.users[message.userId].userKit = message.userKit;

    Locale * locale = locales[message.locale];
    // is such a user already stored in the locale ?
    if( locale->users.find( message.userId ) != locale->users.end())
    {
        ACE_DEBUG((LM_DEBUG,"AddUser has already a user %d in locale %s !\n",
            message.userId, locale->name.c_str()));
        return;
    } 
    else // no, send all other users an update
    {
        locale->users[message.userId] = sender.users[message.userId];
        
        // send to all hosts in the locale except sender
        auto_ptr<set<Host *> > receivers = getHostsFromLocale( locale );
        receivers->erase( &sender );
        sendMessage( *receivers, message );    
    }
    ACE_DEBUG((LM_INFO,"Added user %d to locale %s\n\n", message.userId, message.locale.c_str()));
}

void SessionManager::execute( RemoveUserMessage & message, Host & sender )
{
    if( sender.users.find( message.userId ) == sender.users.end())
    {
        ACE_DEBUG((LM_DEBUG,"RemoveUser user %d not used by host %s !\n",
            message.userId, sender.getAddress()));
        return;
    }

    if( locales.find( message.locale ) == locales.end())
    {
        ACE_DEBUG((LM_ERROR,"RemoveUser requires non-existring locale %s !\n",
            message.locale.c_str()));
        return;
    }
    Locale * locale = locales[message.locale];

    if( locale->users.find( message.userId ) == locale->users.end())
    {
        ACE_DEBUG((LM_ERROR,"RemoveUser requires non-existing user %d in locale %s !\n",
            message.userId, locale->name.c_str()));
        return;
    }   

    // remove user from the sender
    sender.users.erase( message.userId );

    auto_ptr<set<Host*> > hosts = getHostsFromLocale( locale );
    hosts->erase( &sender );

    set<Host *>::iterator it;
    bool found = false;
    for( it = hosts->begin(); it != hosts->end(); it++ )
    {
        if( (*it)->users.find( message.userId ) != (*it)->users.end())
            found = true;
    }

    // no other host uses this as well
    if( !found )
    {
        // remove user
        locale->users.erase( message.userId );    
        
        sendMessage( *hosts, message );
       ACE_DEBUG((LM_INFO,"Removed user %d from locale %s\n", message.userId, message.locale.c_str() ));
    }
    ACE_DEBUG((LM_INFO,"Removed user %d for host %s:%d\n\n", message.userId, sender.getAddress(), sender.getPort()));
}

void SessionManager::execute( JoinLocaleMessage & message, Host & sender )
{
    if( locales.find( message.locale ) == locales.end())
    {
        Locale * locale = new Locale;
        locale->name = message.locale;
        locales[locale->name] = locale;
        ACE_DEBUG((LM_INFO, "Created new locale %s\n", locale->name.c_str()));
    }
    Locale * locale = locales[message.locale];
    sender.locales.insert( locale );

    // check for any users in the locale and send them to the sender
    if(!locale->users.empty())
    {
        AddUserMessage addUser;
        addUser.locale = locale->name;        
        map<unsigned int, User>::iterator it;
        for( it = locale->users.begin(); it != locale->users.end(); it++ )
        {
            addUser.userId = (*it).second.userId;
            addUser.userKit = (*it).second.userKit;
            if( sender.sendMessage( addUser ) != 0 )
            {
                ACE_DEBUG((LM_ERROR,"Error sending to host %s:%d\n", sender.getAddress(), sender.getPort()));
                return;
            }
        }
    }
    // check for any applications in the locale, and send them to the sender    
    if( !locale->applications.empty())
    {
        // send any application to the sender
        StartApplicationMessage startApp;
        startApp.locale = locale->name;
        set<Application *>::iterator it;
        for( it = locale->applications.begin(); it != locale->applications.end(); it++ )
        {
            startApp.application = (*it)->appId;
            if( sender.sendMessage( startApp ) != 0 )
             {
                ACE_DEBUG((LM_ERROR,"Error sending to host %s:%d\n", sender.getAddress(), sender.getPort()));
                return;
            }
            // if it is shared, tell the sender the parameters to get it
            if((*it)->isShared)
            {
                SetDIVParamMessage divMessage;
                divMessage.active = true;
                divMessage.initFromMaster = true;
                divMessage.multicastAddress = (*it)->divAddress;
                divMessage.port = (*it)->divPort;
                divMessage.application = (*it)->appId;
                divMessage.master = false;
                sender.sendMessage( divMessage );
            }
            // if not, check whether it should be shared.
            else
            {
                setSharedStatus((*it));
            }
        }
    }
    ACE_DEBUG((LM_INFO,"Host %s:%d joined locale %s\n\n", sender.getAddress(), sender.getPort(), locale->name.c_str()));
}

void SessionManager::execute( LeaveLocaleMessage & message, Host & sender )
{
    if( locales.find( message.locale ) == locales.end())
    {
        ACE_DEBUG((LM_ERROR,"Host is trying to leave non-exising locale %s\n", 
            message.locale.c_str()));
    }
    else
    {
        Locale * locale = locales[message.locale];
        sender.locales.erase( locale );        
        // ToDo
        // remove applications and send div false to the host
        // 
        // perform check only on applications in the locale
        set<Application *>::iterator it;
        for( it = locale->applications.begin(); it != locale->applications.end(); )
        {
            setSharedStatus((*it++));
        }
        ACE_DEBUG((LM_INFO,"Host %s:%d left locale %s\n\n", 
            sender.getAddress(), sender.getPort(),message.locale.c_str()));
    }    
}

void SessionManager::execute( SetDIVParamMessage & message, Host & sender )
{
    ACE_DEBUG((LM_ERROR,"SetDIVParamMessage received. This should NOT happen !!!!\n\n"));
}

void SessionManager::execute( UserKitMessage & message, Host & sender )
{
    Locale * locale = getLocaleFromUser( message.userId );
    if( locale == NULL )
    {
        ACE_DEBUG((LM_DEBUG,"UserKitMessage with unknown userId %d!\n", message.userId ));
        return;
    }
    auto_ptr<set<Host *> > receivers = getHostsFromLocale( locale );
    sendMessage( *receivers, message );    
    ACE_DEBUG((LM_INFO,"UserKitMessage for %d distributed\n\n", message.userId ));
}

void SessionManager::sendMessage( set<Host *> & receivers, Message & message )
{
    set<Host *>::iterator it;
    for( it = receivers.begin(); it != receivers.end(); it++ )
    {
        (*it)->sendMessage( message );
    }
}

void SessionManager::addHost( Host * host )
{
    hosts.insert( host );
    ACE_DEBUG((LM_INFO,"New connection from %s:%d\n", host->getAddress(), host->getPort()));
}

void SessionManager::removeHost( Host * host )
{
    // first stop all master apps
    // leave all locales ( will recheck the slave apps )
    // finally clean up :)
    hosts.erase( host );    
    host->masterApps.clear();
    set<Locale *>::iterator locIt;
    while( !host->locales.empty() )
    {
        locIt = host->locales.begin();
        LeaveLocaleMessage leaveMsg;
        leaveMsg.locale = (*locIt)->name;
        execute( leaveMsg, *host );
    }
    set<Application *>::iterator it;
    for( it = host->masterApps.begin(); it != host->masterApps.end(); it ++ )
    {
        setSharedStatus((*it));
    }
    ACE_DEBUG((LM_INFO,"Connection closed from %s:%d\n", host->getAddress(), host->getPort()));  
}

auto_ptr<set<Host *> > SessionManager::getHostsFromLocale( Locale * locale )
{
    auto_ptr<set<Host *> > hostSet(new set<Host *>);
    set<Host *>::iterator it;
    for( it = hosts.begin(); it != hosts.end(); it++ )
    {
        if( (*it)->locales.find( locale ) != (*it)->locales.end())
            hostSet->insert((*it));
    }
    return hostSet;
}

Locale * SessionManager::getLocaleFromUser( unsigned int userId )
{
    map<string, Locale *>::iterator it;
    for( it = locales.begin(); it != locales.end(); it++ )
    {
        if( (*it).second->users.find( userId ) != (*it).second->users.end())
            return (*it).second; 
    }
    return NULL;
}

auto_ptr<set<Locale *> > SessionManager::getLocalesFromApplication( Application * app )
{
    auto_ptr<set<Locale *> > localeSet( new set<Locale *>);
    map<string, Locale *>::iterator it;
    for( it = locales.begin(); it != locales.end(); it++ )
    {
        if( (*it).second->applications.find( app ) != (*it).second->applications.end())
            localeSet->insert((*it).second);
    }
    return localeSet;
}

auto_ptr<set<Host *> > SessionManager::getHostsFromApplication( Application * app )
{
    // ToDo
    // could be simpler because the hosts have references to the apps as well ...
    auto_ptr<set<Host *> > hostSet( new set<Host *>);
    auto_ptr<set<Locale *> > locales = getLocalesFromApplication( app );
    set<Locale *>::iterator it;
    for( it = locales->begin(); it != locales->end(); it++ )
    {
        auto_ptr<set<Host *> > hosts = getHostsFromLocale((*it));
        // not nice, union of two sets should be simpler !
        set<Host *>::iterator it;
        for( it = hosts->begin(); it != hosts->end(); it++ )
            hostSet->insert( (*it));
        
    } 
    return hostSet;    
}

void SessionManager::setSharedStatus( Application * app )
{
    auto_ptr<set<Host *> > hosts = getHostsFromApplication( app );
    // if there is no host using this application we remove it
    if( hosts->empty())   
    {
        auto_ptr<set<Locale *> > locales = getLocalesFromApplication( app );
        set<Locale *>::iterator it;
        for( it = locales->begin(); it != locales->end(); it ++ )
            (*it)->applications.erase( app );
        applications.erase( app->appId );
        ACE_DEBUG((LM_INFO,"setSharedStatus: stopped application %s totally\n", app->appId.c_str()));
        delete app;        
    }
    else {  // otherwise check the number of hosts
        bool shouldBeShared = (hosts->size() > 1);

        if( shouldBeShared != app->isShared )
        {
            if( shouldBeShared )
            {
                // find master
                set<Host *>::iterator it;
                for( it = hosts->begin(); it != hosts->end(); it++ )
                {
                    if( (*it)->masterApps.find( app ) != (*it)->masterApps.end())
                        break;
                }
                if( it == hosts->end())
                {
                    ACE_DEBUG((LM_ERROR,"setSharedStatus has masterless app %s !!!!\n",app->appId.c_str()));
                    it = hosts->begin();
                    (*it)->masterApps.insert( app );
                }
                
                Host * master = (*it);
                hosts->erase( it );
                
                SetDIVParamMessage divMessage;
                divMessage.active = true;
                divMessage.initFromMaster = true;
                divMessage.multicastAddress = app->divAddress;
                divMessage.port = app->divPort;
                divMessage.application = app->appId;
                divMessage.master = false;
                sendMessage( *hosts, divMessage );

                hosts->insert( master );
                divMessage.initFromMaster = false;
                divMessage.master = true;
                master->sendMessage( divMessage );
            }
            else
            {
                SetDIVParamMessage message;
                message.active = false;
                message.application = app->appId;
                sendMessage( *hosts, message );
            }
            app->isShared = shouldBeShared;
            ACE_DEBUG((LM_INFO,"Application %s set to %s\n", app->appId.c_str(),app->isShared?"SHARED":"NOT SHARED"));
        }
        if( app->isShared )  // make sure there is a master
        {
            // find master
            set<Host *>::iterator it;
            for( it = hosts->begin(); it != hosts->end(); it++ )
            {
                if( (*it)->masterApps.find( app ) != (*it)->masterApps.end())
                    break;
            }
            if( it == hosts->end())
            {
                it = hosts->begin();
                (*it)->masterApps.insert( app );
                SetDIVParamMessage divMessage;
                divMessage.active = true;
                divMessage.initFromMaster = false;
                divMessage.multicastAddress = app->divAddress;
                divMessage.port = app->divPort;
                divMessage.application = app->appId;
                divMessage.master = true;
                (*it)->sendMessage( divMessage );
                ACE_DEBUG((LM_ERROR,"setSharedStatus selected new master for %s !!!!\n",app->appId.c_str()));             
            }
        }
    }
}

Application * SessionManager::findAppByName( string & name )
{
    map<string, Application *>::iterator it;
    for( it = applications.begin(); it != applications.end(); it++ )
    {
        if(name.compare((*it).second->name) == 0)
            return (*it).second;
    }
    return NULL;
}
