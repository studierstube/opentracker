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
/** header file for DGPSMirror_Handler
  *
  * @author Gerhard Reitmayr
  * 
  * $Id$
  *
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _DGPSMIRROR_HANDLER_H
#define _DGPSMIRROR_HANDLER_H

#include <ace/Svc_Handler.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>

class GPSDriver;

class DGPSMirror_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
	DGPSMirror_Handler();
	virtual ~DGPSMirror_Handler();

    void setDriver( GPSDriver * driver_ )
    {
        driver = driver_;
    }    

    virtual int open( void * factory );
    virtual int handle_input(ACE_HANDLE fd);    
    
protected:
    GPSDriver * driver;
};

class DGPSMirror_Acceptor : public ACE_Acceptor<DGPSMirror_Handler, ACE_SOCK_ACCEPTOR> 
{
public:
    DGPSMirror_Acceptor( GPSDriver * driver_ ):
    driver( driver_ ) 
    {};

    GPSDriver * getDriver( void )
    {
        return driver;
    }

protected:
    GPSDriver * driver;
};

#endif // !defined(_DGPSMIRROR_HANDLER_H)
