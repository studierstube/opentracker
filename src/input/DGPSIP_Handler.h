 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
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
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for DGPSIP_Handler
  *
  * @author Gerhard Reitmayr
  * 
  * $Id$
  *
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _DGPSIP_HANDLER_H
#define _DGPSIP_HANDLER_H

#include <ace/Svc_Handler.h>
#include <ace/Connector.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>
#include "GPSDriver.h"

/**
 * This class retrieves RTCM correction data from a DGPSIP server and sends
 * the correction data to a connected GPS receiver. For debugging purposes, it 
 * can parse and output the RTCM data.
 * @ingroup input
 * @author Gerhard Reitmayr
 */
class DGPSIP_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>, GPSListener
{
public:
	/// default constructor for ace framework. Do not use !
	DGPSIP_Handler(){};

	DGPSIP_Handler( GPSDriver * parent_ );
	virtual ~DGPSIP_Handler();

	virtual int open( void * factory );
	virtual int handle_input(ACE_HANDLE fd);

	virtual void newData( const GPResult * point, const char * line, void * userData );

    virtual int handle_close( ACE_HANDLE fd = ACE_INVALID_HANDLE, ACE_Reactor_Mask mask = ACE_Event_Handler::ALL_EVENTS_MASK);

protected:
    bool isReconnecting;
	GPSDriver * parent;
	int counter;
    ACE_INET_Addr remoteAddr;
};

typedef ACE_Connector<DGPSIP_Handler, ACE_SOCK_CONNECTOR> DGPSIP_Connector;

#endif // !defined(_DGPSIP_HANDLER_H)
