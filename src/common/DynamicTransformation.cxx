 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for DynamicTransformation Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/DynamicTransformation.cxx,v 1.1 2001/02/20 18:02:49 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "DynamicTransformation.h"

// Constructor

DynamicTransformation::DynamicTransformation() : StaticTransformation()
{    
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
    } else 
    {
        StaticTransformation::onEventGenerated( event, generator );
    }
}