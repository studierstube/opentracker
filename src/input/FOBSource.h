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
/** header file for FOBSource Node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/FOBSource.h,v 1.3 2002/01/25 15:17:55 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section fobsource FOBSource 
 *
 * An example element looks like this :
 * @verbatim
<FOBSource number="1"/>@endverbatim
 */

#ifndef _FOBSOURCE_H
#define _FOBSOURCE_H

#include "../OpenTracker.h"

/**
 * This class implements a simple EventGenerator. It is updated by the
 * FOBModule.
 * @author Gerhard Reitmayr
 * @ingroup input
 */
class OPENTRACKER_API FOBSource : public Node
{
// Members
public: 
    /// the state that is posted to the EventObservers
    State state;

// Methods
protected:
    /** simple constructor, sets members to initial values */
    FOBSource() : Node()
    {}

public:            
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }

    friend class FOBModule;
};

#endif

