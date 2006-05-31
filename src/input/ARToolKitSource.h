 /* ========================================================================
  * Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    /// the event pushed into the tree
    Event event, 
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
