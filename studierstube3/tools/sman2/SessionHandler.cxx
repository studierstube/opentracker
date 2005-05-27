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
 * implementation of class SessionHandler
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SessionHandler.cxx 3427 2004-07-14 12:20:49Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4284)
#endif

#define ACE_NLOGGING

#include "SessionHandler.h"
#include <stbapi/workspace/distrManager/MessageFactory.h>
#include <stbapi/workspace/distrManager/SessionMessage.h>
#include "Host.h"
#include "SessionManager.h"
#include "SessionVisitorAdaptor.h"
#include <stbapi/util/buffer.h>
#include <memory>

using namespace std;

SessionHandler::SessionHandler() 
: host( NULL )
, adaptor( NULL )
, closed( true )
{

}

SessionHandler::~SessionHandler()
{
}

int SessionHandler::open(void* data)
{
    StreamHandler::open( data );
    host = new Host( this );
    adaptor = new SessionVisitorAdaptor( host );
	closed = false;
    return 0;
}

int SessionHandler::handle_input(ACE_HANDLE)
{
   	ACE_DEBUG((LM_DEBUG, "SessionHandler::handle_input\n"));

	long messageLength;	
	long len = peer().recv_n( &messageLength, sizeof( messageLength ));
	if( len > 0 && len < 1000000)
	{
		messageLength = ntohl( messageLength );
		auto_ptr<char> msg( new char[messageLength+1]);
		len = peer().recv_n( msg.get(), messageLength );
		if( len == messageLength )
        {
            SessionMessage * message = (SessionMessage *) SessionManager::getFactoryInstance().createMessage( msg.get(), messageLength );
            if( message != NULL )
            {				
				adaptor->put( message );
            }
            else
                ACE_DEBUG((LM_DEBUG, "SessionHandler: unknown message received\n"));
            return 0;
        }
		else
			ACE_DEBUG((LM_DEBUG, "SessionHandler: error receiving message with len %d\n", messageLength));
    }
	else
		ACE_DEBUG((LM_DEBUG, "SessionHandler: error receiving message with len %d\n", messageLength));
    return -1;
}

int SessionHandler::sendMessage( Message & message )
{
	if( closed == true )
	{
		ACE_DEBUG((LM_DEBUG, "SessionHandler: tried to send on closed handler\n"));
		return -1;
	}

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
		ACE_DEBUG((LM_DEBUG, "SessionHandler: error sending message with len %d\n", size));		
		return -1;
	}
	return 0;
}

int SessionHandler::handle_close( ACE_HANDLE handle, ACE_Reactor_Mask close_mask )
{
    ACE_DEBUG((LM_DEBUG,"SessionHandler::handle_close removing host !\n"));
	closed = true;
	adaptor->removeHost();
	peer().close();
    return 0;
}
