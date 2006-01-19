/* ========================================================================
* Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * ========================================================================
  * PROJECT: OpenTracker
* ======================================================================== */
/** source file for DGPSMirror_Handler
*
* @author Gerhard Reitmayr
* 
* $Id$
*
* @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <stdlib.h>
#include "GPSDriver.h"
#include "DGPSMirror_Handler.h"

namespace ot {

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
        setDriver( acceptor->getDriver() );  
        driver->addClient( this );
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

} // namespace ot
