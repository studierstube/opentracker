 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for DynamicTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/DynamicTransformation.cxx,v 1.3 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "DynamicTransformation.h"

#ifdef WIN32
#include <iostream>    // VisualC++ uses the STL based iostream lib
#else
#include <iostream.h>
#endif

// Constructor

DynamicTransformation::DynamicTransformation() : StaticTransformation()
{
}

// this method is called by the EventGenerator to update it's observers.

void DynamicTransformation::onEventGenerated( State& event, Node& generator)
{
    if( generator.isWrapperNode() == 1 )  // if the event is from the wrapper 
    {									  // node, its a change to the base.
        for( int i = 0; i < 3; i ++ )
        {
            translation[i] = event.position[i];
            rotation[i] = event.orientation[i];
        }
        this->rotation[3] = event.orientation[3];
        StaticTransformation::onEventGenerated( store, generator );
    } else 
    {
        store = event;
        StaticTransformation::onEventGenerated( event, generator );
    }
}