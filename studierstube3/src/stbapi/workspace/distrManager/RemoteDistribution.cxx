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
 * implementation of the remote distribution strategy
 *
 * @author Gerhard Reitmayr
 *
 * $Id: RemoteDistribution.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <string>
#include <ace/Log_Msg.h>
#include <stbapi/workspace/distrManager/ConnectionHandler.h>
#include <stbapi/workspace/distrManager/RemoteDistribution.h>
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

#include <stbapi/workspace/SoLocaleManagerKit.h>
#include <stbapi/workspace/SoLocaleKit.h>
#include <stbapi/resource/SoUserKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
// #include <stbapi/workspace/StbWinManager.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/workspace/SoApplicationKit.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/util/ivio.h>

using namespace std;

RemoteDistribution::RemoteDistribution(ConnectionHandler * handler_)
: handler( handler_ )
{
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
    // init the handler 
    handler->setVisitor( this );
    handler->setFactory( &factory );
    // add the default system application id
    map.add(0, "0" );
}

RemoteDistribution::~RemoteDistribution()
{
    handler->close();
}

void RemoteDistribution::startApplication( SbAppID appId, const SbName & locale, int userId )
{
    SoLocaleKit * localeKit = SoLocaleManagerKit::getInstance()->getLocale( locale );
    if( localeKit == NULL )
    {
        ACE_DEBUG((LM_ERROR,"RemoteDistribution::startApplication locale %s does not exist!\n", locale.getString()));
        return;
    }
    if( localeKit->shared.getValue() == FALSE )
        return;

    SoApplicationKit * appKit = SoContextManagerKit::getInstance()->getApp( appId );
    if( appKit == NULL )
    {
        ACE_DEBUG((LM_ERROR,"RemoteDistribution::startApplication app %ld does not exist!\n", appId));
        return;
    }

    string netId;
    createNetId( appId, netId );
    map.add( appId, netId );
    StartApplicationMessage startMsg;
    startMsg.application = netId;
    startMsg.locale = locale.getString();
    startMsg.name = appKit->getName().getString();
    startMsg.userID = userId;
	startMsg.attach = false;

    handler->sendMessage( startMsg );
}

void RemoteDistribution::attachApplication( const SbString & appName, const SbName & locale, int userId )
{
    SoLocaleKit * localeKit = SoLocaleManagerKit::getInstance()->getLocale( locale );
	if( localeKit == NULL )
	{
		ACE_DEBUG((LM_ERROR,"RemoteDistribution::attachApplication locale %s does not exist!\n", locale.getString()));
        return;
    }
    if( localeKit->shared.getValue() == FALSE )
        return;

	StartApplicationMessage startMsg;
	startMsg.attach = true;
	startMsg.name = appName.getString();
	startMsg.locale = locale.getString();
	startMsg.userID = userId;

	handler->sendMessage( startMsg );
}

void RemoteDistribution::setApplicationMode( SbAppID appId, SbBool mode )
{
    // if the application is not shared, there shouldn't exist an entry in the IdMap !!!! :)
    if( map.getNetId( appId ) == "" )
        return;
    SetApplicationModeMessage modeMsg;
    modeMsg.application = map.getNetId( appId );
    modeMsg.master = (mode == TRUE);

    handler->sendMessage( modeMsg );
}

void RemoteDistribution::stopApplication( SbAppID appId )
{
     // if the application is not shared, there shouldn't exist an entry in the IdMap !!!! :)    
    ACE_DEBUG((LM_DEBUG,"%T:RemoteDistribution sending stop application for %d\n", appId ));

    if( map.getNetId( appId ) == "" )
        return;

    SoGroup * appGroup = SoContextManagerKit::getInstance()->getAppGroup( appId );
    SoLocaleKit * locale = SoLocaleManagerKit::getInstance()->getLocaleFromNode( appGroup );
	//only send message if the locale is still shared
    if( locale == NULL || !locale->shared.getValue())
        return;
    StopApplicationMessage stopMsg;
    stopMsg.application = map.getNetId( appId );    
    stopMsg.locale = locale->name.getValue().getString();
    map.remove( appId );

    handler->sendMessage( stopMsg );
}

void RemoteDistribution::joinLocale( const SbName  & locale )
{
    SoLocaleKit * localeKit = SoLocaleManagerKit::getInstance()->getLocale( locale );
    if( localeKit == NULL || localeKit->shared.getValue() == TRUE)
        return;        

    JoinLocaleMessage joinMsg;
    joinMsg.locale = locale.getString();

    if( handler->sendMessage( joinMsg ))
        return;
    localeKit->shared.setValue( TRUE );

    // add local users 
    SoNodeKitListPart * list = SO_GET_PART(localeKit, "users", SoNodeKitListPart);
    
    int i = 0;
    
    for( i = 0; i < list->getNumChildren(); i++ )
    {
        SoUserKit * userKit = (SoUserKit *)list->getChild(i);
        if( userKits.find( userKit->userID.getValue()) != userKits.end())
        {
            AddUserMessage userMsg;
            userMsg.locale = locale.getString();
            userMsg.userId = userKit->userID.getValue();
            userMsg.userKit = userKits[userKit->userID.getValue()];
            handler->sendMessage( userMsg );
        }
    }

    // start applications that are already in the locale
    list = SO_GET_PART(localeKit, "windows", SoNodeKitListPart);
    for( i = 0; i < list->getNumChildren(); i++ )
    {
        SoGroup * appGroup = (SoGroup *)list->getChild(i);       
        if( appGroup->getChild(0) != NULL && appGroup->getChild(0)->isOfType( SoApplicationKit::getClassTypeId()))
        {
            SoApplicationKit * appKit = (SoApplicationKit *)appGroup->getChild(0);
            startApplication( appKit->getContext()->appID.getValue(), locale, 0 );
        }
    }
}

void RemoteDistribution::leaveLocale( const SbName  & locale )
{
    SoLocaleKit * localeKit = SoLocaleManagerKit::getInstance()->getLocale( locale );
    if( localeKit == NULL || localeKit->shared.getValue() == FALSE )
        return;

    localeKit->shared.setValue( FALSE );

    // remove all non locale users
    SoNodeKitListPart * list = SO_GET_PART(localeKit, "users", SoNodeKitListPart);
    int i = 0;

    for(i = 0; i < list->getNumChildren(); i++ )
    {
        SoUserKit * userKit = (SoUserKit *)list->getChild(i);
        if( userKits.find( userKit->userID.getValue()) != userKits.end())
        {
            RemoveUserMessage userMsg;
            userMsg.locale = locale.getString();
            userMsg.userId = userKit->userID.getValue();
            handler->sendMessage( userMsg );
        }
        else
        {
            SoUserManagerKit::getInstance()->checkOutUser( userKit );
        }
    }

    LeaveLocaleMessage leaveMsg;
    leaveMsg.locale = locale.getString();

    if( handler->sendMessage( leaveMsg ))
        return;
    
    // stop any slave applications and stop div on all masters
    list = SO_GET_PART(localeKit, "windows", SoNodeKitListPart);
    for( i = 0; i < list->getNumChildren(); )
    {
        SoDivGroup * appGroup = (SoDivGroup *)list->getChild(i);       
        if( appGroup->getChild(0) != NULL && appGroup->getChild(0)->isOfType( SoApplicationKit::getClassTypeId()))
        {
            SoApplicationKit * appKit = (SoApplicationKit *)appGroup->getChild(0);
            if( appGroup->isMaster.getValue() == TRUE )
            {
                appGroup->active.setValue( FALSE );
                i++;
            }
            else
            {
                SoContextManagerKit::getInstance()->stopApplication( appKit->getContext()->appID.getValue());
            }
        }
        else
        {
            appGroup->active.setValue( FALSE );
        }
    }
}

void RemoteDistribution::addUser( SoUserKit * userKit )
{
    if( userKits.find( userKit->userID.getValue()) != userKits.end())
    return;
    userKit->local.setValue( TRUE );
    size_t size;
    char * buffer = writeMemory( userKit, &size, 1024, FALSE );
    buffer[size] = 0; // make sure its a string :)
    userKits[userKit->userID.getValue()] = buffer;
    delete buffer;
}

void RemoteDistribution::removeUser( SoUserKit * userKit )
{
    userKits.erase( userKit->userID.getValue());
    userKit->local.setValue( FALSE );
}

void RemoteDistribution::execute( StartApplicationMessage & message )
{
    if( map.getAppId( message.application ) != -1 )
    {
        ACE_DEBUG((LM_DEBUG,"RemoteDistribution::StartApplication application %s already present\n", 
            message.application.c_str()));
        return;
    }
    SbAppID appId = SoContextManagerKit::getInstance()->prepareApplication(message.locale.c_str());
    map.add( appId, message.application );
    ACE_DEBUG((LM_DEBUG,"%T:RemoteDistribution::StartApplication %s\n", message.application.c_str()));
}

void RemoteDistribution::execute( SetApplicationModeMessage & message )
{
    ACE_DEBUG((LM_DEBUG,"RemoteDistribution::execute SetApplicationMode should NOT be called !!\n"));
}

void RemoteDistribution::execute( StopApplicationMessage & message )
{
    SbAppID appId = map.getAppId( message.application );
    if( appId  == -1 )
    {
        ACE_DEBUG((LM_DEBUG,"RemoteDistribution::StopApplication application %s does not exist\n", 
            message.application.c_str()));
        return;
    }
    SoApplicationKit * appKit = SoContextManagerKit::getInstance()->getApp( appId );
    SoLocaleKit * localeKit = SoLocaleManagerKit::getInstance()->getLocaleFromNode( appKit );
    if( localeKit == NULL )
        return;
    if( localeKit->name.getValue() != message.locale.c_str())
    {
        ACE_DEBUG((LM_DEBUG,"RemoteDistribution::StopApplication application %s is not in locale %s\n", 
            message.application.c_str(), message.locale.c_str()));
        return;
    }
    SoContextManagerKit::getInstance()->stopApplication( appId );
    map.remove( appId );
    ACE_DEBUG((LM_DEBUG,"%T:RemoteDistribution::StopApplication %s in %s\n", message.application.c_str(), 
        message.locale.c_str()));
}

void RemoteDistribution::execute( AddUserMessage & message )
{
    SoNode * node = readNodeFromMemory( message.userKit.c_str(), message.userKit.size());
    if( node->isOfType( SoUserKit::getClassTypeId()))
    {
        ((SoUserKit *)node)->local.setValue( FALSE );
        StbWorkspace::getInstance()->addUser( (SoUserKit *) node, message.locale.c_str() );
    }
    else
        ACE_DEBUG((LM_DEBUG,"RemoteDistribution::AddUser didn't get a user #%d\n", message.userId));
    ACE_DEBUG((LM_DEBUG,"RemoteDistribution::AddUser with id %d\n", message.userId));
}

void RemoteDistribution::execute( RemoveUserMessage & message )
{
    SoUserKit * userKit = SoUserManagerKit::getInstance()->getUserKit( message.userId);
    if( userKit != NULL )
    {
        if( userKit->local.getValue() == FALSE )
            StbWorkspace::getInstance()->removeUser( userKit );
    }
    ACE_DEBUG((LM_DEBUG,"RemoteDistribution::RemoveUser with id %d\n", message.userId ));
}

void RemoteDistribution::execute( JoinLocaleMessage & message )
{
    ACE_DEBUG((LM_DEBUG,"RemoteDistribution::execute JoinLocale should NOT be called !!\n"));
}

void RemoteDistribution::execute( LeaveLocaleMessage & message )
{
    ACE_DEBUG((LM_DEBUG,"RemoteDistribution::execute LeaveLocale should NOT be called !!\n"));
}

void RemoteDistribution::execute( SetDIVParamMessage & message )
{
    SbAppID appId = map.getAppId( message.application );
    if( appId == -1 )
    {
        ACE_DEBUG((LM_ERROR,"RemoteDistribution::SetDIVParam for unknown app %s\n", message.application.c_str()));
        return;
    }
    SoContextManagerKit::getInstance()->setDivParams(
        appId, message.multicastAddress.c_str(), message.port, message.active, message.master, message.initFromMaster );
    ACE_DEBUG((LM_DEBUG,"%T:RemoteDistribution::SetDIVParam %s with master %d, active %d, init %d\n",
        message.application.c_str(), message.master, message.active, message.initFromMaster ));
}

void RemoteDistribution::execute( UserKitMessage & message )
{
    message.execute((UserKitMessageVisitor &) *this);
}

void RemoteDistribution::execute( WindowBarMessage & message )
{
    SoUserManagerKit::getInstance()->windowBarChanged( 
        message.userId, message.windowStyle, message.windowState );
}

void RemoteDistribution::execute( SheetBarMessage & message )
{
    SbAppID appId = map.getAppId( message.appId );
    if( appId == -1 )
    {
        ACE_DEBUG((LM_DEBUG,"RemoteDistribution::execute SheetBarMessage got non existing appId %s\n",
            message.appId.c_str()));
        return;
    }
    SoUserManagerKit::getInstance()->sheetBarChanged( message.userId, 
        (int)appId, message.whichChild );
}

void RemoteDistribution::execute( SystemBarMessage & message )
{
    SoUserManagerKit::getInstance()->systemBarChanged(
        message.userId, message.buttonId, message.tabInfo );
}

void RemoteDistribution::execute( TaskBarMessage & message )
{
    SoUserManagerKit::getInstance()->taskBarChanged(
        message.userId, message.buttonId );
}

void RemoteDistribution::execute( UseResourcesMessage & message )
{
    SbAppID appId = map.getAppId( message.appId );
    if( appId == -1 )
    {
        ACE_DEBUG((LM_DEBUG,"RemoteDistribution::execute UseResources got non existing appId %s\n",
            message.appId.c_str()));
        return;
    }
    SoUserManagerKit::getInstance()->getUserKit( message.userId )->useResources( appId );
    //StbWinManager::getInstance()->useResources( 
    //    message.userId, message.windowName.c_str(), appId );
}

void RemoteDistribution::execute( ChangeFocusMessage & message )
{
    SbAppID appId = map.getAppId( message.appId );
    if( appId == -1 )
    {
        ACE_DEBUG((LM_DEBUG,"RemoteDistribution::execute ChangeFocus got non existing appId %s\n",
            message.appId.c_str()));
        return;
    }
    SoUserManagerKit::getInstance()->focusChanged( message.userId, appId );
}

void RemoteDistribution::execute( SetFocusMessage & message )
{
    SbAppID appId = map.getAppId( message.appId );
    if( appId == -1 )
    {
        ACE_DEBUG((LM_DEBUG,"RemoteDistribution::execute SetFocus got non existing appId %s\n",
            message.appId.c_str()));
        return;
    }
    //StbWinManager::getInstance()->setDistributedFocus( 
    //    message.userId, message.windowName.c_str(), appId );
}

void RemoteDistribution::createNetId( SbAppID appId, string & out )
{
    ACE_INET_Addr addr;
    handler->peer().get_local_addr( addr );
    char buffer[200];
    sprintf( buffer, "%s.%d.%d", addr.get_host_addr(), addr.get_port_number(), appId );
    out = buffer;
}

void RemoteDistribution::sheetBarChanged( int userId, SbAppID appId, int whichChild )
{
	SoUserKit * userkit = SoUserManagerKit::getInstance()->getUserKit( userId );
	assert( userkit != NULL );
	if( SoLocaleManagerKit::getInstance()->getLocaleFromNode( userkit )->shared.getValue())
	{
		SheetBarMessage msg;
		msg.userId = userId;
		msg.appId = map.getNetId( appId );
		msg.whichChild = whichChild;
		handler->sendMessage( msg );
	}
	else
	    SoUserManagerKit::getInstance()->sheetBarChanged( userId, (int)appId, whichChild );
}

void RemoteDistribution::windowBarChanged( int userId, int style, int state )
{
	SoUserKit * userkit = SoUserManagerKit::getInstance()->getUserKit( userId );
	assert( userkit != NULL );
	if( SoLocaleManagerKit::getInstance()->getLocaleFromNode( userkit )->shared.getValue())
	{
		WindowBarMessage msg;
		msg.userId = userId;
		msg.windowStyle = style;
		msg.windowState = state;
		msg.buttonId = 0;
		handler->sendMessage( msg );
	}
	else 
		SoUserManagerKit::getInstance()->windowBarChanged( userId, style, state );
}

void RemoteDistribution::taskBarChanged( int userId, int buttonId )
{
	SoUserKit * userkit = SoUserManagerKit::getInstance()->getUserKit( userId );
	assert( userkit != NULL );
	if( SoLocaleManagerKit::getInstance()->getLocaleFromNode( userkit )->shared.getValue())
	{
		TaskBarMessage msg;
		msg.userId = userId;
		msg.buttonId = buttonId;
		handler->sendMessage( msg );
	}
	else
		SoUserManagerKit::getInstance()->taskBarChanged( userId, buttonId );
}

void RemoteDistribution::systemBarChanged( int userId, int buttonId, int tabInfo )
{
	SoUserKit * userkit = SoUserManagerKit::getInstance()->getUserKit( userId );
	assert( userkit != NULL );
	if( SoLocaleManagerKit::getInstance()->getLocaleFromNode( userkit )->shared.getValue())
	{
		SystemBarMessage msg;
		msg.userId = userId;
		msg.appId = "";
		msg.buttonId = buttonId;
		msg.tabInfo = tabInfo;
		handler->sendMessage( msg );
	}
	else
		SoUserManagerKit::getInstance()->systemBarChanged( userId, buttonId, tabInfo );
}

void RemoteDistribution::focusChanged( int userId, SbAppID appId )
{
	SoUserKit * userkit = SoUserManagerKit::getInstance()->getUserKit( userId );
	assert( userkit != NULL );
	if( SoLocaleManagerKit::getInstance()->getLocaleFromNode( userkit )->shared.getValue())
	{
		ChangeFocusMessage msg;
		msg.userId = userId;
		msg.appId = map.getNetId( appId );
		handler->sendMessage( msg );
	}
	else
		SoUserManagerKit::getInstance()->focusChanged( userId, appId );
}

void RemoteDistribution::setFocus(int userId, const SbName & windowName, SbAppID appId)
{
	SoUserKit * userkit = SoUserManagerKit::getInstance()->getUserKit( userId );
	assert( userkit != NULL );
	if( SoLocaleManagerKit::getInstance()->getLocaleFromNode( userkit )->shared.getValue())
	{
		SetFocusMessage msg;
		msg.userId = userId;
		msg.windowName = windowName.getString();
		msg.appId = map.getNetId( appId );
		handler->sendMessage( msg );
	}
}

void RemoteDistribution::useResources( int userId, const SbName & windowName, SbAppID appId )
{
	SoUserKit * userkit = SoUserManagerKit::getInstance()->getUserKit( userId );
	assert( userkit != NULL );
	if( SoLocaleManagerKit::getInstance()->getLocaleFromNode( userkit )->shared.getValue())
	{
		UseResourcesMessage msg;
		msg.userId = userId;
		msg.windowName = windowName.getString();
		msg.appId = map.getNetId( appId );
		handler->sendMessage( msg );
	}
	else
		SoUserManagerKit::getInstance()->getUserKit(userId)->useResources(appId);
}
