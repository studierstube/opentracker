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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/ParButtonModule.cxx,v 1.19 2003/10/16 07:52:44 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include "ParButtonSource.h"
#include "ParButtonModule.h"

// enable this define, if you want to use an alternative implementation of
// the parallel port access. You will also have to make sure to include the
// right libraries etc.
//#define _DLPORTIO

using namespace std;

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
#include <stropts.h> 
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
            cout << "ParButtonSource on port " << dev << " already defined !" << endl;
            return NULL;
        }

#ifdef WIN32
        UINT addr;
        if( sscanf( dev.c_str(), " %i", &addr ) != 1 )
        {
            cout << "ParButtonModule not an address " << dev << endl;
            return NULL;
        }
        // setting parallel port for input

#ifndef _DLPORTIO
 
			HANDLE hUserPort;

		  hUserPort = CreateFile("\\\\.\\UserPort", GENERIC_READ, 0, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
            cout << "ParButtonModule Error opening parallel port " << dev << endl;
            return NULL;
        }
        if(ioctl(handle, PLPIOMODE, PLP_BI) < 0) {
            ::close(handle);
            cout << "ParButtonModule Error setting centronics mode on" << dev << endl;
            return NULL;
        }
        if(ioctl(handle, PLPIOCREAD, 1) < 0) 
        {
            ::close(handle);
            cout << "ParButtonModule Error setting bidirectional mode on " << dev << endl;
            return NULL;
        }
        if(ioctl(handle, PLPIOCRTO, 1) < 0) 
        {
            ::close(handle);
            cout << "ParButtonModule Error timeout on " << dev << endl;
            return NULL;
        }
        ParButtonSource * source = new ParButtonSource((unsigned int) handle );
#else  // Linux
	int handle = open( dev.c_str(), O_RDWR | O_NDELAY );
	if( handle < 0 )
        {
            cout << "ParButtonModule Error opening parallel port " << dev << endl;
            return NULL;
        }
	
	int mode;
	if(ioctl(handle, PPCLAIM) < 0) 
	{
	    cout << "ParButtonModule Error claiming port" 
	         << dev << endl;
	    ::close(handle);
	    return NULL;
	}

	unsigned char datadir=0x01;
  
	if (ioctl(handle, PPDATADIR, &datadir) < 0)
	{
	    cout << "ParButtonModule Error setting datadir" << dev << endl;
	    ::close(handle);
	    return NULL;
	}
  
	ParButtonSource * source = new ParButtonSource((unsigned int) handle );
#endif
#endif
        nodes[dev] = source;
        cout << "Build ParButtonSource on " << dev << endl;
        return source;
    }
    return NULL;
}
	
// closes the devices    
    
void ParButtonModule::close()
{
    for( map<string, Node *>::iterator it = nodes.begin(); it != nodes.end(); it++ )
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

void ParButtonModule::pushState()
{
    unsigned short data;
    
    for( map<string, Node *>::iterator it = nodes.begin(); it != nodes.end(); it++ )
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
        if( data != source->state.button )
        {
            source->state.button = data;
            source->state.timeStamp();
            source->updateObservers( source->state );
        }
#else

#ifdef _SGI_SOURCE
        if( read( source->handle, &data, 1 ) == 1 )
        {
            source->state.button = data;
            source->state.timeStamp();
            source->updateObservers( source->state );
        }
#else  // LINUX
    int cstatus = ioctl(source->handle, PPRDATA, &data);	
	
	if( (unsigned int)(~data) != source->state.button )
        {
            source->state.button = 0x00ff&(~data);
            source->state.timeStamp();
            source->updateObservers( source->state );
        }
#endif

#endif
    }  
}
