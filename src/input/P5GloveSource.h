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
/** header file for P5GloveSource Node.
  *
  * @author Hannes Kaufmann, Istvan Barakonyi
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/P5GloveSource.h,v 1.1 2003/03/21 14:52:51 kaufmann Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section p5glovesource P5GloveSource
 * The P5GloveSource node is an EventGenerator that outputs the
 * current position and button state of the P5Glove. It is driven by
 * the @ref p5glovemodule. 
 */

#ifndef _P5GLOVESOURCE_H
#define _P5GLOVESOURCE_H

#include "../OpenTracker.h"

#ifdef USE_P5GLOVE

/**
 * This class implements a simple source that sets its valid flag in
 * regular intervals and updates any EventObservers. 
 * @author Hannes Kaufmann, Istvan Barakonyi
 * @ingroup input
 */
class OPENTRACKER_API P5GloveSource : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    State state;
	/// finger to be tracked
	int finger;

// Methods
protected:
    /** simple constructor, sets members to initial values */
    P5GloveSource( int finger_) : Node(), finger(finger_)
    {}

public:
    
        
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    friend class P5GloveModule;
};

#endif

#endif
