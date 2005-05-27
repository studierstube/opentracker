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
 * header file for data structure class Host
 *
 * @author Gerhard Reitmayr
 *
 * $Id: Host.h 3427 2004-07-14 12:20:49Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _HOST_H_
#define _HOST_H_

#ifdef WIN32
#pragma warning(disable:4284)
#endif

#include <set>
#include "SessionHandler.h"
#include "Application.h"
#include "Locale.h"

/**
 * The class Host stores information related to a Studierstube instance
 * connected to the SessionManager. It also provides a front end to the
 * communication stream for sending messages. Finally it also keeps
 * track of the SessionHandler object that is dealing with any messages
 * received from the Studierstube client.
 *
 * @author Gerhard Reitmayr
 * @ingroup sman2
 */
class Host  
{
public:
    /** Constructor.
     * @param handler_ reference to the SessionHandler serving
     *    the Studierstube client.
     */
    Host( SessionHandler * handler_ );
    /// destructor
	virtual ~Host();

    /// returns the SessionHandler 
    SessionHandler & getHandler()
    {
        return *handler;
    }

    /// stores pointers to all applications this host is master of
    std::set<Application *> masterApps;
    /// stores pointers to all locales this host has joined
    std::set<Locale *> locales;
    /// stores pointers to all users that exist locally at the host
    std::map<unsigned int,User> users;

    /**
     * send a message to the host. 
     * @param message the message object to send
     */
    int sendMessage( Message & message );

    /** returns the client IP address of the connected host 
     */
    const char * getAddress() const;
    /**
     * returns the client port number of the connected host.
     */
    int getPort() const;

protected:
    /// the handler serving the connected Studierstube client
	SessionHandler * handler;
};

#endif // _HOST_H_
