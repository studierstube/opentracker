/* ========================================================================
* TODO Uni-blabla
* ========================================================================
* PROJECT: OpenTracker
* ======================================================================== */
/** header file for Merge Tracker Node.
*
* @author Gratl/Werlberger VRVU
*
* $Id:$
* @file                                                                   */
/* ======================================================================= */

/**
* @page Nodes Node Reference
* @section mergetracker MergeTracker
* TODO description
*
* inputs :
* 
* TODO
* 
* An example element using all wrapper elements looks like this :
* @verbatim
TODO example
*/

#ifndef _MERGETRACKERNODE_H
#define _MERGETRACKERNODE_H

#include "../OpenTracker.h"

/**
*
*/

namespace ot
{

	class OPENTRACKER_API MergeTrackerNode : public Node   
	{

	protected:
		//! constructor method
		MergeTrackerNode();

		//! states of top tracking device
	    Event topState;

		//! states of top tracking device
		Event bLeftState;

		//! states of top tracking device
		Event bRightState;

		//! merged state
		Event mergedState;

	public: 

		/** tests for EventGenerator interface being present and returns
		* 1, if present.
		* @return always 1 */
		virtual int isEventGenerator()
		{
			return 1;
		}

		/**
		* This method is called by any child node. It updates the local
		* state with the right part of the passed event and notifies
		* it's parent and references in turn.
		* @param event the event value passed
		* @param generator the node generating the event
		*/
		virtual void onEventGenerated( Event& event, Node & generator);                                

		friend class CommonNodeFactory;
	};

} // namespace ot

#endif
