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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/ParButtonModule.cxx,v 1.9 2002/09/10 13:09:06 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include "ParButtonSource.h"
#include "ParButtonModule.h"

// enable this define, if you want to use an alternative implementation of
// the parallel port access. You will also have to make sure to include the
// right libraries etc.
// #define _DLPORTIO

using namespace std;

#ifdef WIN32
#include <windows.h>
#include <iostream>    // VisualC++ uses STL based IOStream lib
#ifndef _DLPORTIO
#include "../misc/portio.h"
#else
#include <Dlportio.h>
#endif
#else
#include <stropts.h> 
#include <iostream.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#ifdef _SGI_SOURCE
#include <sys/plp.h> 
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
        outportb(addr+2, 0x20); 
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
        ParButtonSource * source = new ParButtonSource((unsigned int)handle );
#else  // Linux
	ParButtonSource * source = NULL;
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
#ifdef _SGI_SOURCE
        ::close(((ParButtonSource*)(*it).second)->handle);
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
		data = (~DlPortReadPortUchar(source->handle )) & 0xff;
#endif
        if( data != source->state.button )
        {
            source->state.button = data;
            source->state.timeStamp();
            source->updateObservers( source->state );
        }
#endif
#ifdef _SGI_SOURCE
        if( read( source->handle, &data, 1 ) == 1 )
        {
            source->state.button = data;
            source->state.timeStamp();
            source->updateObservers( source->state );
        }
#endif
    }  
}
