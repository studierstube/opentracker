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
/** header file for CyberMouseSource Node.
  *
  * @author Ivan Viola, Matej Mlejnek
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/CyberMouseSource.h,v 1.6 2001/06/11 22:02:22 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section cybermousesource CyberMouseSource
 * The CyberMouseSource node is a simple EventGenerator that outputs the
 * current position and button state of the CyberMouse. It is driven by
 * the @ref cybermousemodule. 
 * If the mouse does not seem to work, press a button. This should initialize
 * it and it will work. The mouse buttons are mapped to button numbers in
 * the following way : the middle button sets the LSB to 1 and the second 
 * button sets the second to 1.
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
 * @ingroup input
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
