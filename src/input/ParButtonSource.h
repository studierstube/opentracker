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
/** header file for ParButtonSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/ParButtonSource.h,v 1.2 2001/08/07 09:21:23 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section parbuttonsource ParButtonSource 
 * This element reads button values from the parallel port. It will only generate
 * events, when the status changes. It works only on some hardware that has
 * a bidirectional parallel port. This includes Indigo2, O2, and most
 * modern PCs. There may be only one source per parallel port. The port itself
 * is defined by the device name or the base port address, depending on the
 * operating system.
 *
 * An example element looks like this :
 * @verbatim
<ParButtonSource dev="/par/lpt1" port="0x365"/>@endverbatim
 */

#ifndef _PARBUTTONSOURCE_H
#define _PARBUTTONSOURCE_H

#include "../OpenTracker.h"

/**
 * This class implements a simple EventGenerator. It is updated by the
 * InterSenseModule.
 * @author Gerhard Reitmayr
 * @ingroup input
 */
class OPENTRACKER_API ParButtonSource : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    State state;
    /// device handle or base address
    unsigned int handle;

// Methods
protected:
    /** simple constructor, sets members to initial values */
    ParButtonSource( unsigned int handle_ ) : Node(), handle( handle_ )
    {}

public:            
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    friend class ParButtonModule;
};

#endif

