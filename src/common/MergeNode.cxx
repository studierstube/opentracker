 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for Merge Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/MergeNode.cxx,v 1.3 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "MergeNode.h"

/** definitions for flags defining whats happening for each
 * EventGenerator. These are binary flags that can be or'd
 * together.
 */
unsigned DEFAULT = 1, 
         POSITION = 2, 
         ORIENTATION = 4, 
         BUTTON = 8,
         CONFIDENCE = 16, 
         TIME = 32;

void MergeNode::onEventGenerated( State& event, Node & generator)
{
	unsigned flag = 0;
	if( generator.isWrapperNode() == 1 )  // this should always be the case
	{
		WrapperNode & wrap = (WrapperNode &)generator;
		if( wrap.getTagName().compare("MergeDefault") == 0 )
		{
			if( countWrappedChildren((string)"MergePosition") == 0 )
				flag |= POSITION;
			if( countWrappedChildren((string)"MergeOrientation") == 0 )
				flag |= ORIENTATION;
			if( countWrappedChildren((string)"MergeButton") == 0 )
				flag |= BUTTON;
			if( countWrappedChildren((string)"MergeConfidence") == 0 )
				flag |= CONFIDENCE;
			if( countWrappedChildren((string)"MergeTime") == 0 )
				flag |= TIME;
		} 
		else if( wrap.getTagName().compare("MergePosition") == 0 )
			flag |= POSITION;
		else if( wrap.getTagName().compare("MergeOrientation") == 0 )
			flag |= ORIENTATION;
		else if( wrap.getTagName().compare("MergeButton") == 0 )
			flag |= BUTTON;
		else if( wrap.getTagName().compare("MergeConfidence") == 0 )
			flag |= CONFIDENCE;
		else if( wrap.getTagName().compare("MergeTime") == 0 )
			flag |= TIME;			
		if( flag & POSITION )
		{
			localState.position[0] = event.position[0];
			localState.position[1] = event.position[1];
			localState.position[2] = event.position[2];
		}	
		if( flag & ORIENTATION )
		{
			localState.orientation[0] = event.orientation[0];
			localState.orientation[1] = event.orientation[1];
			localState.orientation[2] = event.orientation[2];
			localState.orientation[3] = event.orientation[3];
		}
		if( flag & BUTTON )
			localState.button = event.button;
		if( flag & CONFIDENCE )
			localState.confidence = event.confidence;
		if( flag & TIME )        
			localState.time = event.time;        
		updateObservers( localState );
	}
}                                  
