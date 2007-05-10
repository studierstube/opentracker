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
/** source file for MobilabDriver
 *
 * @author Alexander Bornik
 * 
 * $Id$
 *
 * @file                                                                   */
/* ======================================================================= */


#include <cstdlib>

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <cstdlib>

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/tool/FixWinCE.h>
#include <ace/Reactor.h>
#include <iostream>
#include <algorithm>
#include <memory>

#include <OpenTracker/input/MobilabDriver.h>
#include <OpenTracker/input/Mobilab_Handler.h>

#include <ace/Log_Msg.h>
#include <OpenTracker/tool/OT_ACE_Log.h>

namespace ot {

    MobilabDriver::MobilabDriver(ACE_Reactor * reactor_) :
        reactor( reactor_ ),
        receiver( NULL )
    {
	if( NULL == reactor )
	{   
            logPrintW("ot:MobilabDriver: reactor is null!\n");
            reactor = ACE_Reactor::instance();
	}
    }

    MobilabDriver::~MobilabDriver()
    {
	if( receiver != NULL )
            close();

	listeners.clear();

    }

    int MobilabDriver::open( const std::string & device)
    {
        if( getDebug())
            ACE_DEBUG((LM_INFO, ACE_TEXT("ot:MobilabDriver open\n")));

	int result;
	// open the serial port the the mobilab device
	receiver = new Mobilab_Handler( this );
	Mobilab_Connector mobilabconnect( reactor );
	result = mobilabconnect.connect( receiver, 
ACE_DEV_Addr(ACE_TEXT_CHAR_TO_TCHAR(device.c_str())));
	if( result == 0)
	{
            // set the appropriate parameters
            ACE_TTY_IO::Serial_Params params;		
            result = receiver->peer().control( ACE_TTY_IO::GETPARAMS, &params);
            params.baudrate = 57600;
            params.databits = 8;
            params.stopbits = 1;
            params.parityenb = false;

            params.dtrdisable = false;
            params.rtsenb = 1;
            params.ctsenb = false;
            params.dsrenb = false;

            params.rcvenb = true;

            result = receiver->peer().control(ACE_TTY_IO::SETPARAMS, &params );
            if( result != 0 )
            {
                receiver = NULL;
                ACE_DEBUG((LM_ERROR, 
                           ACE_TEXT("ot:MobilabDriver port open failed: %s\n"), 
                           device.c_str()));
            }
	}

        if( getDebug())
            ACE_DEBUG((LM_INFO, 
                       ACE_TEXT("ot:MobilabDriver opened serial port %s\n"), 
                       device.c_str()));
	
	return result;
    }

    void MobilabDriver::close()
    {
        if ( reactor != NULL)
        {       
            reactor->close();
        }

	if( receiver != NULL )
	{
		
		receiver->shutdown();
		//receiver->destroy();
		//receiver = NULL;
	}

    }

    void MobilabDriver::addListener( MobilabListener * listener, 
                                     void * userData )
    {
	listeners[listener] = userData;
    }

    void MobilabDriver::removeListener( MobilabListener * listener, 
                                        void * userData )
    {
	std::map<MobilabListener *, void *>::iterator it = 
            listeners.find( listener );

	if( it != listeners.end())
            listeners.erase( listener );
    }


    void MobilabDriver::new_line( const char * line )
    {       
        std::map<MobilabListener *, void *>::iterator it;
        for( it = listeners.begin(); it != listeners.end(); it++ )
        {
            //(*it).first->newData(line, (*it).second );
            (*it).first->newData((char*)it->second);
        }
    }

    void MobilabDriver::setDebug( bool debug )
    {
	debugOn = debug;
    }

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of MobilabDriver.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
