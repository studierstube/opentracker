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
/** header file for SpaceMouseSource Node.
  *
  * @author Michael Wögerbauer
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/SpaceMouseSource.h,v 1.1 2002/10/31 19:53:06 splechtna Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section spacemousesource SpaceMouseSource
 * The SpaceMouseSource node is a simple EventGenerator that outputs the
 * current position and button state of the SpaceMouse. It is driven by
 * the @ref spacemousemodule. 
 * [Hints to be filled in...]
 *
 * An example element looks like this :
 * @verbatim
<SpaceMouseSource/>@endverbatim
 */

#ifndef _SPACEMOUSESOURCE_H
#define _SPACEMOUSESOURCE_H

#include "../OpenTracker.h"

#ifdef USE_SPACEMOUSE

/**
 * This class implements a simple source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Michael Woegerbauer
 * @ingroup input
 */
class OPENTRACKER_API SpaceMouseSource : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    State state;

// Methods
public:
    /** simple constructor, sets members to initial values */
    SpaceMouseSource() : Node()
    {};
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    /** pushes event down the line. */
    void push()
    {
        state.timeStamp();
        updateObservers( state );
    }

protected:
    /// the state that is updated by the @see JostickModule polling thread.
    State tmpState;
    /// a flag to indicate whether tmpState was changed during processing
    int changed;

	friend class SpaceMouseModule;
};

#endif

#endif
