 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for InterTraxModule module.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/Attic/InterTraxModule.cxx,v 1.1 2001/03/05 17:53:05 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "InterTraxSource.h"
#include "InterTraxModule.h"
#include "../core/MathUtils.h"

#ifdef USE_INTERTRAX

#include <stdio.h>
#ifdef WIN32
#include <iostream>    /// VisualC++ uses STL based IOStream lib
#else
#include <iostream.h>
#endif

ISD_TRACKER_HANDLE handle;
ISD_DATA_TYPE data;

// Destructor method
InterTraxModule::~InterTraxModule()
{
    nodes.clear();
}

// This method is called to construct a new Node.
Node * InterTraxModule::createNode( string& name, StringMap& attributes)
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
            source->push();
        }
    }
}

#endif