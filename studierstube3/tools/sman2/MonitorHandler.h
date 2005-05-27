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
 * header file for class MonitorHandler
 *
 * @author Gerhard Reitmayr
 *
 * $Id: MonitorHandler.h 3427 2004-07-14 12:20:49Z tamer $
 * @todo implement a remote monitor here
 * @file                                                                   */
/* ======================================================================= */

#ifndef _MONITORHANDLER_H_
#define _MONITORHANDLER_H_

#include <stbapi/workspace/distrManager/StreamHandler.h>

class MonitorHandler; //forward declaration

typedef ACE_Acceptor<MonitorHandler,ACE_SOCK_ACCEPTOR> MonitorAcceptor;

/**
 * 
 * @author Gerhard Reitmayr
 * @ingroup sman2
 */
class MonitorHandler : public StreamHandler
{
public:
	MonitorHandler();
	virtual ~MonitorHandler();

    int handle_input(ACE_HANDLE);

};

#endif // _MONITORHANDLER_H_
