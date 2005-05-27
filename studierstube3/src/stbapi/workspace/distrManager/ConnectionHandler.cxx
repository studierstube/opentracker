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
/** source file for the ConnectionHandler adaptor class
 *
 * @author Gerhard Reitmayr
 *
 * $Id: ConnectionHandler.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

// turn of ACE debug messages for this file
#define ACE_NLOGGING

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <assert.h>

#include <ace/Reactor.h>
#include <ace/SOCK_Connector.h>
#include <stbapi/workspace/distrManager/ConnectionHandler.h>
#include <stbapi/workspace/distrManager/SessionMessage.h>
#include <stbapi/workspace/distrManager/SessionVisitor.h>
#include <stbapi/workspace/distrManager/MessageFactory.h>
#include <stbapi/util/buffer.h>

ConnectionHandler::ConnectionHandler() 
: ACE_Event_Handler()
,visitor( NULL )
,factory( NULL ) 
{
}

ConnectionHandler::~ConnectionHandler()
{
	ACE_DEBUG((LM_DEBUG,"ConnectionHandler::Destructor\n"));
}

int ConnectionHandler::open( const char * address, int port )
{
    ACE_SOCK_Connector connector;
	ACE_INET_Addr serverAddr;

	if( serverAddr.set( port, address ) == -1 )
	{
		ACE_DEBUG((LM_DEBUG, "ConnectionHandler::open error parsing address %s:%d\n", 
            address, port));
		return -1;
	}

	if( connector.connect( net, serverAddr ) == -1 )
	{
		ACE_DEBUG((LM_DEBUG, "ConnectionHandler::open could not establish connection to %s:%d.\n", 
            serverAddr.get_host_addr(), serverAddr.get_port_number()));
        return -1;
	}
	ACE_Reactor::instance()->register_handler( this, ACE_Event_Handler::READ_MASK );
    return 0;
}

void ConnectionHandler::close()
{
	reactor()->remove_handler( this, ACE_Event_Handler::READ_MASK );
    peer().close();
}

int ConnectionHandler::handle_close( ACE_HANDLE handle, ACE_Reactor_Mask mask )
{
    delete this;
    return 0;
}

int ConnectionHandler::handle_input( ACE_HANDLE handle )
{
	long messageLength;	
	long len = peer().recv_n( &messageLength, sizeof( messageLength ));
	if( len > 0 )
	{
		messageLength = ntohl( messageLength );
		char * msg = new char[messageLength+1];
		len = peer().recv_n( msg, messageLength );
		if( len == messageLength )
        {
            assert( factory );
            SessionMessage * message = (SessionMessage *) factory->createMessage( msg, messageLength );
            if( message != NULL )
            {
            	ACE_DEBUG((LM_DEBUG,"ConnectionHandler received msg %d\n", message->getType()));
                assert( visitor );
                message->execute( *visitor );    
                delete message;
            }
            else
                ACE_DEBUG((LM_DEBUG, "ConnectionHandler: unknown message received\n"));
        }
		else
			ACE_DEBUG((LM_DEBUG, "ConnectionHandler: error receiving message with len %d\n", messageLength));
		delete[] msg;
    }
	else
		ACE_DEBUG((LM_DEBUG, "ConnectionHandler: error receiving message with len %d\n", messageLength));
    return 0;
}

ACE_HANDLE ConnectionHandler::get_handle(void) const
{
	ACE_DEBUG((LM_DEBUG,"ConnectionHandler::get_handle\n"));
	return peer().get_handle();
}

// sends the content of the buffer member to the network connection
int ConnectionHandler::sendMessage( Message & message )
{
    CBuffer out;
    message.writeMessage( &out );

    long size = out.getSize();
    long messageSize;

	if( size <= 0 ){
		size = 1;
	}	
    messageSize=htonl(size);

	struct iovec iov[2];
	iov[0].iov_len = sizeof( messageSize );
	iov[0].iov_base = (char *) & messageSize;
	iov[1].iov_len = size;
	iov[1].iov_base = (char *) out.getBuffer();
    
	if( peer().sendv_n( iov, 2) < 0 )
    {
		ACE_DEBUG((LM_DEBUG, "ConnectionHandler::sendMessage error sending message with len %d\n", size));		
		return -1;
	}
	return 0;
}
