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
#ifndef _STREAMHANDLER_H_
#define _STREAMHANDLER_H_

#include <ace/ACE.h>
#include <ace/Reactor.h>
#include <ace/Svc_Handler.h>
#include <ace/Acceptor.h>
#include <ace/Synch.h>
#include <ace/SOCK_Acceptor.h>

class StreamHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> // ACE_Event_Handler
{
public:
    StreamHandler();
    virtual ~StreamHandler();
    
    int open(void*);
    
    // new method which returns the handle to the reactor when it
    //asks for it.
    ACE_HANDLE get_handle(void) const;

    //new method which returns a reference to the peer stream
    ACE_SOCK_Stream & peer(void) const
    {
        return (ACE_SOCK_Stream &) this->peer_;
    }
        
    int close (u_long flags=0);
protected:
    ACE_SOCK_Stream peer_;    
};


#endif // _STREAMHANDLER_H_
