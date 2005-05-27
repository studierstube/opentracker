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
 * implementation of class Host
 *
 * @author Gerhard Reitmayr
 *
 * $Id: Host.cxx 3427 2004-07-14 12:20:49Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <ace/Reactor.h>
#include "Host.h"

Host::Host(SessionHandler * handler_ )
: handler(handler_)
{   
}

Host::~Host()
{
	delete handler;
}


int Host::sendMessage( Message & message )
{
    if( handler->sendMessage( message ) != 0 )
    {
        handler->reactor()->remove_handler( handler, ACE_Event_Handler::READ_MASK);
        return -1;
    }
    return 0;
}


const char * Host::getAddress() const
{
    ACE_INET_Addr addr;
    handler->peer().get_remote_addr( addr );
    return addr.get_host_name();
}

int Host::getPort() const
{
    ACE_INET_Addr addr;
    handler->peer().get_remote_addr( addr );
    return addr.get_port_number();
}
