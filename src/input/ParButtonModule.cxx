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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/ParButtonModule.cxx,v 1.13 2002/12/09 16:02:16 splechtna Exp $
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

#if defined (WIN32) || defined (GCC3)
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

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
	
	if( (~data) != source->state.button )
        {
            source->state.button = 0x00ff&(~data);
            source->state.timeStamp();
            source->updateObservers( source->state );
        }
#endif

#endif
    }  
}
