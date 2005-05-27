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
/** header file for ConnectionHandler adaptor class. 
 *
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ConnectionHandler.h 3521 2004-07-21 08:42:40Z tomp $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _CONNECTIONHANDLER_H_
#define _CONNECTIONHANDLER_H_

#include <ace/Event_Handler.h>
#include <ace/SOCK_Stream.h>
#include <stbapi/stbapi.h>

class SessionVisitor;
class Message;
class MessageFactory;

/**
 * This class handles the network connection to the session manager. It 
 * provides the interface to the ACE framework and sends and reads messages
 * via a TCP connection. For each received message it calls a SessionVisitor
 * object to handle the message.
 *
 * @author Gerhard Reitmayr
 * @ingroup sman
 */
class STBAPI_API ConnectionHandler : public ACE_Event_Handler
{
public:
	ConnectionHandler();

	virtual ~ConnectionHandler();

    int open( const char * address, int port );

    void close();

	virtual int handle_input( ACE_HANDLE handle );

	virtual ACE_HANDLE get_handle(void) const;
    
    virtual int handle_close( ACE_HANDLE handle = ACE_INVALID_HANDLE, 
        ACE_Reactor_Mask mask = ACE_Event_Handler::READ_MASK);

    /// sends the content of the buffer member to the network connection
	int sendMessage( Message & message );
	
    //new method which returns a reference to the peer stream
    ACE_SOCK_Stream & peer(void) const
    {
        return (ACE_SOCK_Stream &) this->net;
    }

    void setVisitor( SessionVisitor * visitor_ )
    {
        visitor = visitor_;
    }

    SessionVisitor * getVisitor()
    {
        return visitor;
    }

    void setFactory( MessageFactory * factory_ )
    {
        factory = factory_;
    }

    MessageFactory * getFactory()
    {
        return factory;
    }

protected:
   	/// Stream object encapsulating the TCP connection to the session manager
	ACE_SOCK_Stream net;
    /// pointer the Visitor object that is called to handle the messages
	SessionVisitor * visitor;
    /** message factory object to create the message objects from the
     * data blocks read from the network. 
     */
    MessageFactory * factory;
};

#endif // _CONNECTIONHANDLER_H_
