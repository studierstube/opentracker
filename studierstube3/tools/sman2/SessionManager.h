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
 * header file for SessionManager, the central class of sman2
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SessionManager.h 3427 2004-07-14 12:20:49Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SESSIONMANAGER_H_
#define _SESSIONMANAGER_H_

/**
 * @defgroup sman2 Session Manager 2
 * This group contains the implementation classes of the Session Manager 2. This
 * is a central server that organises distributed instances of Studierstube into
 * locales. See the @ref sman2manual page for a description of how to use it. 
 * Details on the design can be found in the documentation of the following 
 * classes:
 * @see SessionManager
 * @see SessionVisitor
 *
 * @ingroup tools
 */

/**
@page sman2manual Session Manager 2 User Manual

The Session Manager 2 (sman2) is a helper service process to organise and 
coordinate several Studierstube processes into locales. It operates without 
user intervention after it is started. All Studierstube processes need to 
be configured to know the address of the sman2 process they should connect to.
This is achieved by setting the address of the sman2 process using commandline
parameters of the workspace executable (see @ref commandline).

The sman2 executable itself takes the following parameters:
@li @b -i hostname or ip address. The interface it should listen for incoming 
          connections from a Studierstube process.
@li @b -p port number of the port it should use.
@li @b -m hostname or ip address. The interface it should listen to for monitoring
          connections. This interface would allow some manual control.
@li @b -n port number of the monitoring interface.

By default it listens on port 1234 for incoming connections and on port 1235 for
monitoring connections. The monitoring interface is not implemented yet. 
If this will be finished, its functionality will be documented here.
*/

#include <ace/Synch.h>
#include <ace/Task.h>

#include "SessionHandler.h"
#include "MonitorHandler.h"
#include "Locale.h"
#include "Application.h"
#include "Host.h"
#include <stbapi/workspace/distrManager/MessageFactory.h>
#include <stbapi/workspace/distrManager/SessionVisitor.h>
#include <set>
#include <string>
#include <map>

/**
 * This is the central class of the Session Manager 2. It holds all the necessary
 * data and reacts to any incoming messages. Several assumptions are made in its design:
 *
 * @li locale names are unique
 * @li application names are unique
 * @li user ids are unique
 * @li clients do the right thing. While all parameters are checked for reasonable
 *     values before acting, error resulting from sending wrong messages and crashing
 *     clients are likely to be not caught !
 *
 * Design :
 * The Session Manager 2 is designed as a single threaded server processing a number of
 * TCP connections made by clients. There are two ports it processes. The session port
 * for Studierstube clients to work within the distributed system and the monitor port
 * for telnet connections to administer the session manager (this is not implemented yet).
 *
 * It receives and sends message objects from and to the Studierstube clients. They are
 * organized in a class hierarchy deriving from the base class Message. The message objects
 * are dealt with by passing them to an instance of a SessionVisitor which delegates the
 * call to one of its methods depending on the type of message. 
 *
 * The ACE framework is used to listen to all connected clients and demultiplex incoming
 * data to corresponding SessionHandlers. These demarshal incoming messages and put them in
 * a message queue owned by the SessionManager. The SessionManager is an active object
 * and processes the messages in its own thread. As all processing is done in there, no
 * concurrency issues concerning the internal datastructures should appear. Among other
 * things, it then in turn sends messages out to the clients using the Host objects.
 * This way we can avoid locks, if both sessionmanager and studierstube client block 
 * on large writes to the same TCP pipe (No joke, this happened - probably need a 
 * more sophisticated network protocol).
 *
 * The SessionManager itself only deals with Host, Application and Locale objects. It
 * is implemented as a singleton and features static methods to retrieve the singleton
 * object. It implements a number of methods to search through the stored data and compute 
 * relations. These are used to act on different messages. Currently it is basically
 * a refactoring of common functionality that is needed to react to different messages.
 * A more general framework would be nice.
 *
 * @author Gerhard Reitmayr
 * @ingroup sman2
 */
class SessionManager : public ACE_Task<ACE_MT_SYNCH>
{

public:
	
	enum { 
		HIGH_WATER_MARK = 1024*1024, 
		LOW_WATER_MARK = 1024*1024 
	};

    /** constructor sets up the global MessageFactory with the right factory functions.
     */
	SessionManager();
    /// destructor
	virtual ~SessionManager();
    
    /**
     * starts the SessionManager by opening listening sockets or rather ACE
     * SOCK_Acceptors on the addresses given for the session service and the 
     * monitor service.
     * @param session the address of the session service
     * @param monitor the address of the monitor service
     */
    int open(const ACE_INET_Addr & session, const ACE_INET_Addr & monitor);

    /**
     * start to process incoming messages
     */
    void run(void);

	/**
	 * thread worker method that processes incoming messages.
	 */
	int svc (void);

    /**
     * returns the global MessageFactory
     */
    MessageFactory & getFactory()
    {
        return factory;
    }

    /**
     * returns the singleton SessionManager object 
     */
    static SessionManager & getInstance();
    /**
     * returns the MessageFactory of the singleton SessionManager object
     */
    static MessageFactory & getFactoryInstance();

    /**
     * adds a new Host object to the distributed system
     * @param host pointer to the new Host object.
     */
    void addHost( Host * host );
    /**
     * removes a Host object from the distributed system and cleans up behind it.
     * First it stops all master apps associated with the host. Then it 
     * notifies all locales that it left and finally cleans up the data structures.
     * @param host pointer to the Host object
     */
    void removeHost( Host * host );

    void execute( StartApplicationMessage & message, Host & sender );
    void execute( SetApplicationModeMessage & message, Host & sender );
    void execute( StopApplicationMessage & message, Host & sender );
    void execute( AddUserMessage & message, Host & sender );
    void execute( RemoveUserMessage & message, Host & sender );
    void execute( JoinLocaleMessage & message, Host & sender );
    void execute( LeaveLocaleMessage & message, Host & sender );
    void execute( SetDIVParamMessage & message, Host & sender );
    void execute( UserKitMessage & message, Host & sender );

protected:
    /**
     * sends a message to a set of Hosts. This is often needed, if changes
     * happen to a locale. 
     * @param receivers set of pointers to all receiving hosts
     * @param message message to send to the receivers
     */
    void sendMessage( std::set<Host *> & receivers, Message & message );
  
    std::auto_ptr<std::set<Host *> > getHostsFromLocale( Locale * locale );
    Locale * getLocaleFromUser( unsigned int userId );
    std::auto_ptr<std::set<Locale *> > getLocalesFromApplication( Application * app );
    std::auto_ptr<std::set<Host *> > getHostsFromApplication( Application * app );
    Application * findAppByName( std::string & name );
    void setSharedStatus( Application * app );

    /// socket listener object for the session service
    SessionAcceptor sessionSocket;
    /// socket listener object for the monitor service
    MonitorAcceptor monitorSocket;
    /// the global MessageFactory object
    MessageFactory factory;

    /// stores the locales indexed by name
    std::map<std::string, Locale *> locales;
    /// stores the locales indexed by their unique application id
    std::map<std::string, Application *> applications;
    /// stores the Hosts as a set
    std::set<Host *> hosts;

    /// pointer to the singleton SessionManager object
    static SessionManager * instance;
};

#endif // _SESSIONMANAGER_H_
