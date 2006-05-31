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
/** source file for ParButton module.
  *
  * @author Gerhard Reitmayr 
  *
  *****************************************************************
  *
  * ALF 9.Dec.2002:
  * some of the newer mainboards seem to lack pull-up resistors
  * which results in showing always a value of "0xFF" as data
  *
  * I corrected this by using external pull-ups of about 2kOhm
  * from the input pins to pin 14 (output Autofeed), which gets
  * initialized as HIGH. (This is a hack. Better would be using 
  * +5V from e.g. the PS/2 connector, but this means another 
  * connector, which is too much trouble)
  *
  * Try this hack, if you've got the same problems.
  *
  *****************************************************************
  *
  * $Id$
  *
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "ParButtonSource.h"
#include "ParButtonModule.h"
#include "../tool/OT_ACE_Log.h"

// enable this define, if you want to use an alternative implementation of
// the parallel port access. You will also have to make sure to include the
// right libraries etc.
//#define _DLPORTIO

//using namespace std;

#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>

#ifdef WIN32
#ifndef _DLPORTIO
#include "../misc/portio.h"
#else
#include "Dlportio.h"
#endif
#else
#include <sys/ioctl.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#ifdef _SGI_SOURCE
#include <sys/plp.h> 
#else
#include <linux/ioctl.h>
#include <linux/parport.h>
#include <linux/ppdev.h>
#endif
#endif

namespace ot {

#ifdef WIN32
 #ifndef _DLPORTIO
	static BOOL bPrivException = FALSE;

	LONG WINAPI HandlerExceptionFilter ( EXCEPTION_POINTERS *pExPtrs )
	{

		if (pExPtrs->ExceptionRecord->ExceptionCode == EXCEPTION_PRIV_INSTRUCTION)
		{
			pExPtrs->ContextRecord->Eip ++; // Skip the OUT or IN instruction that caused the exception
			bPrivException = TRUE;
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		else
			return EXCEPTION_CONTINUE_SEARCH;
	}
 #endif
#endif

// This method is called to construct a new Node

Node * ParButtonModule::createNode( const std::string& name,  StringTable& attributes)
{
    if( name.compare("ParButtonSource") == 0)
    {
        std::string dev = attributes.get("dev");
        if( nodes.find( dev ) !=  nodes.end() )
        {
            LOG_ACE_INFO("ot:ParButtonSource on port %s already defined!\n", dev.c_str() );
            return NULL;
        }

#ifdef WIN32
        UINT addr;
        if( sscanf( dev.c_str(), " %i", &addr ) != 1 )
        {
            LOG_ACE_INFO("ot:ParButtonModule not an address %s\n", dev.c_str());
            return NULL;
        }
        // setting parallel port for input

#ifndef _DLPORTIO
 
			HANDLE hUserPort;

		  hUserPort = CreateFileA("\\\\.\\UserPort", GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		  CloseHandle(hUserPort); // Activate the driver
			Sleep(100); // We must make a process switch

			SetUnhandledExceptionFilter(HandlerExceptionFilter);
			
			bPrivException = FALSE;
			inportb(addr);  // Try to access the given port address

			if (bPrivException)
			{
				{
    				printf("	Privileged instruction exception has occured!\r\n\r\n"\
														 "To use this program under Windows NT or Windows 2000\r\n"\
														 "you need to install the driver 'UserPort.SYS' and grant\r\n"\
														 "access to the ports used by this program.");
					exit(-1);
				}
			}
		
		
		
		
		
		outportb(addr, 0x00 );
        outportb(addr+2, 0x20); // set to byte output mode,
								// all control lines to HIGH
#else
	DlPortWritePortUchar(addr, 0x00);
	DlPortWritePortUchar(addr+2, 0x20); 
	// nothing to be done here
#endif

        ParButtonSource * source = new ParButtonSource( addr );

#else
#ifdef _SGI_SOURCE       
        int handle = open( dev.c_str(), O_RDWR | O_NDELAY  );
        if( handle < 0 )
        {
            LOG_ACE_INFO("ot:ParButtonModule Error opening parallel port %s\n", dev.c_str());
            return NULL;
        }
        if(ioctl(handle, PLPIOMODE, PLP_BI) < 0) {
            ::close(handle);
            LOG_ACE_INFO("ot:ParButtonModule Error setting centronics mode on %s\n", dev.c_str());
            return NULL;
        }
        if(ioctl(handle, PLPIOCREAD, 1) < 0) 
        {
            ::close(handle);
            LOG_ACE_INFO("ot:ParButtonModule Error setting bidirectional mode on %s\n", dev.c_str());
            return NULL;
        }
        if(ioctl(handle, PLPIOCRTO, 1) < 0) 
        {
            ::close(handle);
            LOG_ACE_INFO("ot:ParButtonModule Error timeout on %s\n", dev.c_str());
            return NULL;
        }
        ParButtonSource * source = new ParButtonSource((unsigned int) handle );
#else  // Linux
	int handle = open( dev.c_str(), O_RDWR | O_NDELAY );
	if( handle < 0 )
        {
            LOG_ACE_INFO("ot:ParButtonModule Error opening parallel port %s\n", dev.c_str());
            return NULL;
        }
	
	int mode;
	if(ioctl(handle, PPCLAIM) < 0) 
	{
        LOG_ACE_INFO("ot:ParButtonModule Error claiming port %s\n" , dev.c_str() );
	    ::close(handle);
	    return NULL;
	}

	unsigned char datadir=0x01;
  
	if (ioctl(handle, PPDATADIR, &datadir) < 0)
	{
        LOG_ACE_INFO("ot:ParButtonModule Error setting datadir %s\n", dev.c_str());
	    ::close(handle);
	    return NULL;
	}
  
	ParButtonSource * source = new ParButtonSource((unsigned int) handle );
#endif
#endif
        nodes[dev] = source;
        LOG_ACE_INFO("ot:Build ParButtonSource on %s\n", dev.c_str());
        return source;
    }
    return NULL;
}
	
// closes the devices    
    
void ParButtonModule::close()
{
    for( std::map<std::string, Node *>::iterator it = nodes.begin(); it != nodes.end(); it++ )
    {
#ifndef WIN32
#ifdef _SGI_SOURCE
        ::close(((ParButtonSource*)(*it).second)->handle);
#else  // LINUX
	ioctl(((ParButtonSource*)(*it).second)->handle, PPRELEASE);
        ::close(((ParButtonSource*)(*it).second)->handle);
#endif
#endif
    }
    nodes.clear();
}
  
// pushes events into the tracker tree

void ParButtonModule::pushEvent()
{
    unsigned short data;
    
    for( std::map<std::string, Node *>::iterator it = nodes.begin(); it != nodes.end(); it++ )
    {
        ParButtonSource * source = (ParButtonSource*)(*it).second;
#ifdef WIN32

#ifndef _DLPORTIO
        data = (~inportb( source->handle )) & 0xff;
#else
		// just check for two buttons if using hardware hack
		//data = (~DlPortReadPortUchar(source->handle )) & 0x03;
		data = (~DlPortReadPortUchar(source->handle )) & 0xFF;

#endif

#if 0
		printf("%d%d%d%d %d%d%d%d\n",
			(data>>7)&1,
			(data>>6)&1,
			(data>>5)&1,
			(data>>4)&1,
			(data>>3)&1,
			(data>>2)&1,
			(data>>1)&1,
			(data>>0)&1
			);
#endif
        if( data != source->event.getButton() )
        {
            source->event.getButton() = data;
            source->event.timeStamp();
            source->updateObservers( source->event );
        }
#else

#ifdef _SGI_SOURCE
        if( read( source->handle, &data, 1 ) == 1 )
        {
            source->event.getButton() = data;
            source->event.timeStamp();
            source->updateObservers( source->event );
        }
#else  // LINUX
    int cstatus = ioctl(source->handle, PPRDATA, &data);	
	
	if( (unsigned int)(~data) != source->event.getButton() )
        {
            source->event.getButton() = 0x00ff&(~data);
            source->event.timeStamp();
            source->updateObservers( source->event );
        }
#endif

#endif
    }  
}

} // namespace ot
