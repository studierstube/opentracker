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
/** header file for MagicYSource Node.
  * @author Christoph Traxler
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/MagicYSource.h,v 1.1 2003/06/30 12:30:27 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section MagicYSource MagicYSource
 * The MagicYSource node is a simple EventGenerator that inserts events from
 * the MagicYSource into the tracker tree. The MagicYSource element has the 
 * following attribute:
 * @li @c number the number of the wand, between 0 and 99
 * @li @c average, when set to 1 it ignores the number and calculates the
 * average of all MagicY points.
 *
 * An example element looks like this :
 * @verbatim <MagicYSource number="0" average="false"/>@endverbatim
 */

#ifndef _MagicYSource_H
#define _MagicYSource_H

#include "../OpenTracker.h"

/**
 * This class implements a simple node that inserts new events from the MagicY
 * into the tracker tree.
 * @author Christoph Traxler
 * @ingroup input
 */
class OPENTRACKER_API MagicYSource : public Node
{
// Members
public:
    /// the new state
    State state;

// Methods
protected:
    /** constructor */
    MagicYSource() : Node()
    {}

public:                
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    }  

    friend class MagicYModule;
};

#endif
