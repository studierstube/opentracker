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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/ParButtonModule.cxx,v 1.1 2001/08/04 18:07:31 reitmayr Exp $
  *
  * @file                                                                   */
 /* ======================================================================= */

#include "ParButtonSource.h"
#include "ParButtonModule.h"

#include <stropts.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 

using namespace std;

#ifdef WIN32
#include <iostream>    // VisualC++ uses STL based IOStream lib
#else
#ifdef _SGI_SOURCE
#include <sys/plp.h> 
#endif
#include <iostream.h>
#endif

// This method is called to construct a new Node

Node * ParButtonModule::createNode( const std::string& name,  StringTable& attributes)
{
    if( name.compare("ParButtonSource") == 0)
    {
#ifdef _SGI_SOURCE
        std::string dev = attributes.get("dev");
        if( nodes.find( dev ) !=  nodes.end() )
        {
            cout << "ParButtonSource on port " << dev << " allready defined !" << endl;
            return NULL;
        }
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
        ParButtonSource * source = new ParButtonSource( handle );
        nodes[dev] = source;
        cout << "Build ParButtonSource on " << dev << endl;
        return source;
#endif
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
    unsigned char data;
    for( map<string, Node *>::iterator it = nodes.begin(); it != nodes.end(); it++ )
    {
        ParButtonSource * source = (ParButtonSource*)(*it).second;
#ifdef _SGI_SOURCE
    cout << "Reading from parallel" << endl;
        if( read( source->handle, &data, 1 ) == 1 )
        {
            source->state.button = data;
            source->state.timeStamp();
            source->updateObservers( source->state );
        }
    cout << "Done" << endl;
#endif
    }  
}
