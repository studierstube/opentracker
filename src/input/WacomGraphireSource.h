 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for WacomGraphireSource Node.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/WacomGraphireSource.h,v 1.1 2001/03/05 17:53:05 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section wacomgraphiresource WacomGraphireSource
 * The WacomGraphireSource node is a simple EventGenerator that fires in fixed intervalls
 * standard events. it can be configured with the following attributes. It has the 
 * following elements :
 * @li @c device, type of pointing device (pen, eraser, puck)
 *
 * An example element looks like this :
 * @verbatim
<WacomGraphireSource device="1" />@endverbatim
 */

#ifndef _WACOMGRAPHIRESOURCE_H
#define _WACOMGRAPHIRESOURCE_H

#include "../OpenTracker.h"

#ifdef USE_WACOMGRAPHIRE

/**
 * This class implements a simple source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Ivan Viola, Matej Mlejnek
 */
class WacomGraphireSource : public Node, public EventGenerator
{
// Members
public: 
    /// the state that is posted to the EventObservers
    State state;
    /// the number of the device to use
	int device;
// Methods
public:
    /** simple constructor, sets members to initial values */
    WacomGraphireSource(int device_) : 
        Node(),
	    EventGenerator(),
	    device(device_)
    {}
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return this always.
     * @return always this */
    EventGenerator * isEventGenerator()
    {
        return this;
    }
    /** pushes event down the line. Needed to access protected
     * updateObservers method in EventGenerator */
    void push()
    {
        updateObservers( state );
    }
};

#endif

#endif