  /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for CyberMouseSource Node.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/CyberMouseSource.h,v 1.2 2001/03/27 05:35:18 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section cybermousesource CyberMouseSource
 * The CyberMouseSource node is a simple EventGenerator that outputs the
 * current position, orientation and button state of the CyberMouse. If
 * the mouse does not seem to work, press a button. This should initialize
 * it and it will work. The mouse buttons are mapped to button numbers in
 * the following way : middle button = 1, second button = 2, both = 3 ???
 *
 * @todo update cyber mouse button description !
 *
 * An example element looks like this :
 * @verbatim
<CyberMouseSource/>@endverbatim
 */

#ifndef _CYBERMOUSESOURCE_H
#define _CYBERMOUSESOURCE_H

#include "../OpenTracker.h"

#ifdef USE_CYBERMOUSE

/**
* This class implements a simple source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Ivan Viola, Matej Mlejnek
 */
class OPENTRACKER_API CyberMouseSource : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    State state;

// Methods
public:
    /** simple constructor, sets members to initial values */
    CyberMouseSource() : Node()
    {};
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }
};

#endif

#endif