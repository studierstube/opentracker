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
/** source file for InterTraxModule module.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/Attic/InterTraxModule.cxx,v 1.6 2001/04/16 15:43:11 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "InterTraxSource.h"
#include "InterTraxModule.h"

#ifdef USE_INTERTRAX

#include <stdio.h>
#ifdef WIN32
#include <iostream>    /// VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

// Destructor method
InterTraxModule::~InterTraxModule()
{
    nodes.clear();
}

// This method is called to construct a new Node.
Node * InterTraxModule::createNode( string& name, StringTable& attributes)
{
    if( name.compare("InterTraxSource") == 0 )
    {       
        InterTraxSource * source = new InterTraxSource;
        nodes.push_back( source );
        cout << "Build InterTraxSource node " << endl;
        initialized = 1;
        return source;
    }
    return NULL;
}

// opens intertrax library
void InterTraxModule::start()
{
    if( isInitialized() == 1 )
    {
        handle = ISLIB_OpenTracker( NULL, 0, FALSE, FALSE );
        if( handle <= 0 )
        {
            cout << "Failed to open InterTrax library !" << endl;
            initialized = 0;
        }
    }
}

// closes intertrax library
void InterTraxModule::close()
{
    if( isInitialized() == 1 )
    {
        ISLIB_CloseTracker( handle );
    }
}

// pushes events into the tracker tree.
void InterTraxModule::pushState()
{
    if( isInitialized() == 1 )
    {
        float quat[4];
        ISLIB_GetTrackerData( handle, &data );
        if( data.Station[0].Orientation[0] == backup[0] &&
            data.Station[0].Orientation[1] == backup[1] &&
            data.Station[0].Orientation[2] == backup[2] )
        {            
            return;
        }
        backup[0] = data.Station[0].Orientation[0];
        backup[1] = data.Station[0].Orientation[1];
        backup[2] = data.Station[0].Orientation[2];
        MathUtils::eulerToQuaternion(data.Station[0].Orientation[2],
		                             data.Station[0].Orientation[1],
									 data.Station[0].Orientation[0],								
                                     quat);
        for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
        {	
            InterTraxSource * source = (InterTraxSource *)(*it);
            source->state.orientation[0] = quat[0];
            source->state.orientation[1] = quat[1];
            source->state.orientation[2] = quat[2];
            source->state.orientation[3] = quat[3];
            source->state.timeStamp();
            source->updateObservers( source->state );
        }
    }
}

#endif
