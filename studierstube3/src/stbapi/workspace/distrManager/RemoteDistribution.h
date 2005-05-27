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
 * implements a distribution strategy by communicating with a session manager.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: RemoteDistribution.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _REMOTEDISTRIBUTION_H_
#define _REMOTEDISTRIBUTION_H_

#include <string>
#include <map>
#include <stbapi/workspace/distrManager/DistributionStrategy.h>
#include <stbapi/workspace/distrManager/SessionVisitor.h>
#include <stbapi/workspace/distrManager/UserKitMessageVisitor.h>
#include <stbapi/workspace/distrManager/MessageFactory.h>

class ConnectionHandler;

class IdMap {
public:
    IdMap(): nullstring("") {};
    ~IdMap(){};

    long getAppId( const std::string & netId )
    {
        std::map<std::string, long>::iterator it = net2app.find( netId );
        if( it == net2app.end())
            return -1;
        return (*it).second;
    };

    const std::string & getNetId( long appId )
    {
        std::map<long, std::string>::iterator it = app2net.find( appId );
        if( it == app2net.end())
            return nullstring;
        return (*it).second;
    };

    void add( long appId, const std::string & netId )
    {
        app2net[appId] = netId;
        net2app[netId] = appId;
    };

    void remove( long appId )
    {
        std::map<long, std::string>::iterator it = app2net.find( appId );
        if( it != app2net.end())
        {
            net2app.erase((*it).second);
            app2net.erase( it );
        }
    };

    void remove( const std::string & netId )
    {
        std::map<std::string, long>::iterator it = net2app.find( netId );
        if( it != net2app.end())
        {
            app2net.erase((*it).second);
            net2app.erase( it );
        }
    };

private:
    std::map<long, std::string> app2net;
    std::map<std::string, long> net2app;
    std::string nullstring;
};

/**
 * This class implements the DistributionStrategy interface for a connected
 * Studierstube instance. It handles input from two sources, the distribution
 * needs of the Studierstube instance and the messages from the remote SessionManager.
 *
 * In the first case, it creates the necessary distribution messages and
 * deals with translations of application ids to a network unique form.
 *
 * In the second case, it calls the APIs of various manager objects to carry out
 * the actions requested by the central SessionManager.
 *
 * @ingroup sman
 * @author Gerhard Reitmayr
 */
class RemoteDistribution : public DistributionStrategy, public SessionVisitor, public UserKitMessageVisitor 
{
public:
	RemoteDistribution(ConnectionHandler * handler_);
	virtual ~RemoteDistribution();

    /** @name DistributionStrategy interface
     * The following methods implement the DistributionStrategy. They are called by
     * the facade methods of the StbDistrManager.
     */
    //@{
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
    //@}

    /** @name SessionVisitor interface
     * These methods implement the SessionVisitor interface. They are called 
     * by the received messages to make the class correspond to the message.
     */
    //@{
    virtual void execute( StartApplicationMessage & message );
    virtual void execute( SetApplicationModeMessage & message );
    virtual void execute( StopApplicationMessage & message );
    virtual void execute( AddUserMessage & message );
    virtual void execute( RemoveUserMessage & message );
    virtual void execute( JoinLocaleMessage & message );
    virtual void execute( LeaveLocaleMessage & message );
    virtual void execute( SetDIVParamMessage & message );
    virtual void execute( UserKitMessage & message );
    //@}

    /** @name UserKitMessageVisitor interface
     * These methods implement the UserKitMessageVisitor interface. They are
     * called by subtypes of the UserKitMessage to refine the behaviour
     * regarding this message type.
     */
    //@{
    virtual void execute( WindowBarMessage & message );
    virtual void execute( SheetBarMessage & message );
    virtual void execute( SystemBarMessage & message );
    virtual void execute( TaskBarMessage & message );
    virtual void execute( UseResourcesMessage & message );
    virtual void execute( ChangeFocusMessage & message );
    virtual void execute( SetFocusMessage & message );
    //@}

protected:    
    /// factory class for messages 
    MessageFactory factory;
    /// map between local appIds and networked application names
    IdMap map;
    /// map for userkit strings
    std::map<int, std::string> userKits;
    /// the connection we are working with
    ConnectionHandler * handler;
    /** helper method to create the uique net id for any given
     * local application id.
     */
    void createNetId( SbAppID appId, std::string & out );

};

#endif // !defined(_REMOTEDISTRIBUTION_H_)
