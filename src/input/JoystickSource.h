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
/** header file for JoystickSource Node.
  *
  * @author Rainer Splechtna
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/JoystickSource.h,v 1.2 2003/01/09 04:14:13 tamer Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section joysticksource JoystickSource 
 *
 * The JoystickSource node is a simple EventGenerator that inserts events generated from
 * the joystick input data into the tracker tree. The JoystcikSource element has the 
 * following attributes :
 * @li @c id the ID of the joystick the source is associated with
 *
 * An example element looks like this :
 * @verbatim
<JoystickSource id="1"/>@endverbatim
 */
 
#ifndef _JOYSTICKSOURCE_H
#define _JOYSTICKSOURCE_H

#include "../OpenTracker.h"

#ifdef USE_JOYSTICK

#include "../dllinclude.h"

/**
 * This class implements a simple EventGenerator. It is updated by the
 * JoystickModule.
 * @ingroup input
 * @author Rainer Splechtna
 */
class OPENTRACKER_API JoystickSource : public Node
{
// Members
public:
    /// stores joystick ID
    int id;
    /// the state that is posted to the EventObservers
    State state;

// Methods
protected:
    /** simple constructor, sets members to initial values
     * @param id_ joystick ID */
    JoystickSource( int id_ ) : 
        Node(),
	    id( id_ )
    {}

    /// the state that is updated by the @see JostickModule polling thread.
    State tmpState;
    /// a flag to indicate whether tmpState was changed during processing
    int changed;

public:            
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    int isEventGenerator()
    {
        return 1;
    }
    /** pushes event down the line. */
    void push()
    {
        state.timeStamp();
        updateObservers( state );
    }

    friend class JoystickModule;
};

#endif
#endif
