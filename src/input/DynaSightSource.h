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
/** header file for DynaSightSource Node. Version 1.02
  *
  * @author Alexander Schaelss
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section DynaSightSource DynaSightSource
 * The DynaSightSource node is a simple EventGenerator that inserts events from
 * the DynaSightSource into the tracker tree. The DynaSightSource element has the 
 * following attribute:
 * @li @c number the target number, between 0 and 8
 *  
 * An example element looks like this :
 * @verbatim
<DynaSightSource target="0" />@endverbatim
 */

#ifndef _DYNASIGHTSOURCE_H
#define _DYNASIGHTSOURCE_H

#include "../OpenTracker.h"

/**
 * This class implements a simple node that inserts new events from the DynaSightSource
 * into the tracker tree.
 * @author Alexander Schaelss
 * @ingroup input
 */
class OPENTRACKER_API DynaSightSource : public Node
{
// Members
public:
    /// the new state
    State state;

// Methods
protected:
    /** constructor */
    DynaSightSource() : Node()
    {}

public:                
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }  
    
    friend class DynaSightModule;
};

#endif
