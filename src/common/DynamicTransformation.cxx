 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for DynamicTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/DynamicTransformation.cxx,v 1.2 2001/03/06 18:06:52 reitmayr Exp $
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
    baseChild = NULL;
}

// adds a child to the Node.

void DynamicTransformation::addChild( Node& node)
{
    WrapperNode * wrap = node.isWrapperNode();
    if( wrap != NULL && baseChild == NULL )
    {
        if( wrap->getTagName().compare("TransformBase") == 0 )
        {
            NodeVector &nodes = wrap->getChildren();
            if( nodes.size() > 0 )
            {
                baseChild = nodes[0]->isEventGenerator();
                if( baseChild != NULL )
                {
                    baseChild->addEventObserver( *this );
                }
            }
        } 
    } else 
    {
        StaticTransformation::addChild( node );
    }
}

// this method is called by the EventGenerator to update it's observers.

void DynamicTransformation::onEventGenerated( State& event,
                                       EventGenerator& generator)
{
    if( &generator == baseChild )
    {
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