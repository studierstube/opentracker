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
/** header file for ARToolKit source node.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section artoolkitsource ARToolKitSource
 * The ARToolKitSource node is a simple EventGenerator that inserts events 
 * from tracking a visual marker in a video image by using the ARToolKit
 * library. See the @ref artoolkitmodule for information about how to
 * configure the module as a whole. It has the following elements :
 * @li @c tag-file the file containing the marker pattern
 * @li @c center the center within the marker as two coordinates in millimeters
 * @li @c size the size of the quadratic marker in millimeters
 *
 * An example element looks like this :
 * @verbatim
<ARToolKitSource tag-file="pip.tag" center="0,0" size="40" />@endverbatim
 */

#ifndef _ARTOOLKITSOURCE_H
#define _ARTOOLKITSOURCE_H

#include "../OpenTracker.h"


#if defined(USE_ARTOOLKIT) || defined(USE_ARTOOLKITPLUS)

namespace ot {

/** An ARToolKitSource represents a single marker that is tracked by the 
 * artoolkit library.
 * @ingroup input
 */
class OPENTRACKER_API ARToolKitSource : public Node
{

public:
    /// the state pushed into the tree
    State state, 
    /// buffer variable for grabbing thread
        buffer;
    /// marker id from the artoolkit library
    int markerId;
    /// center of the marker
    double center[2];
    /// size of the marker
    double size;
    /// is a new update ready in the buffer ?
    int modified;

protected:
    /** constructor method
     * @param markerId_ the id of the marker from artoolkit library
     * @param vertex_ vertex coordinates of the marker
     */
    ARToolKitSource(int markerId_, double center_[2], double size_) : 
        Node(),        
        markerId( markerId_ ),
        size( size_ ),
        modified(0)
    {
        center[0] = center_[0];
        center[1] = center_[1];
    };

public:
    /** tests for EventGenerator interface being present. Is overriden to
     * return 1 always.
     * @return always 1 */
    virtual int isEventGenerator()
    {
        return 1;
    };  

    friend class ARToolKitModule;
    friend class ARToolKitPlusModule;
};

} // namespace ot

#endif	// defined(USE_ARTOOLKIT) || defined(USE_ARTOOLKITPPC)

#endif
