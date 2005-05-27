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
 * header file for class SessionHandler
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SessionHandler.h 3427 2004-07-14 12:20:49Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SESSIONHANDLER_H_
#define _SESSIONHANDLER_H_

#include <stbapi/workspace/distrManager/StreamHandler.h>

//forward declaration
class Host;
class Message;
class SessionHandler;
class SessionVisitorAdaptor;

typedef ACE_Acceptor<SessionHandler,ACE_SOCK_ACCEPTOR> SessionAcceptor;

/**
 * The class SessionHandler serves a single connected Studierstube client.
 * It reads in data packages from the network stream and parses them
 * into Message objects to be further processed. It also marshals
 * messages send to the client and writes them to the network stream.
 * It relies on a MessageFactory to actually parse the messages and
 * on a SessionVisitorAdaptor to process the messages further.
 *
 * @author Gerhard Reitmayr
 * @ingroup sman2
 */
class SessionHandler : public StreamHandler
{
public:
    /// constructor
	SessionHandler();
    /// destructor
	virtual ~SessionHandler();

    /**
     * called by the ACE framework when a new client connects. It
     * creates and registers a new Host object for the client with
     * the system.
     */
    int open(void*);
    /**
     * called by the ACE framework whenever new data is received from
     * the client and ready to be processed. Here the data is
     * demarshalled and the resulting message object passed to
     * the adaptor object for further processing.
     */
    virtual int handle_input(ACE_HANDLE);
    /**
     * called by the ACE framework when the connection is closed to
     * perform cleanup tasks. It removes the associated Host object
     * from the system.
     */
    virtual int handle_close( ACE_HANDLE handle, ACE_Reactor_Mask close_mask );

    /**
     * sends a message to the client. It marshals it and writes it
     * the network stream. 
     * @param message Message object to be sent 
     */
    int sendMessage( Message & message );

protected:
    /// pointer to the associated Host object
    Host * host;
    /// pointer to the SessionVisitorAdaptor to use to handle the messages.
    SessionVisitorAdaptor * adaptor;
	/// flag to store whether connection is open or not
	bool closed;
};

#endif // _SESSIONHANDLER_H_
