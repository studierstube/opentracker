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
/** source file for DGPSMirror_Handler
*
* @author Gerhard Reitmayr
* 
* $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/DGPSMirror_Handler.cxx,v 1.3 2003/07/18 17:27:58 tamer Exp $
*
* @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "GPSDriver.h"
#include "DGPSMirror_Handler.h"

DGPSMirror_Handler::DGPSMirror_Handler()
{
    driver = NULL;
}

DGPSMirror_Handler::~DGPSMirror_Handler()
{
    if( driver != NULL )
        driver->removeClient( this );
}

int DGPSMirror_Handler::open( void * factory )
{
    int result = ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::open( factory );
    if( result == 0)
    {
        DGPSMirror_Acceptor * acceptor = (DGPSMirror_Acceptor *)factory;
        acceptor->getDriver()->addClient( this );
    }
    return result;
}

int DGPSMirror_Handler::handle_input(ACE_HANDLE fd)
{
    // handle rtcm input and send to receiver
    char buf[4*1024];
    int cnt;	
    cnt = peer().recv( buf, sizeof(buf));
    if( driver->getDebug())
        printf("DGPSMirror_Handler received %i bytes\n", cnt);
    return 0;
}
