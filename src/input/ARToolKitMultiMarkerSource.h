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
/** header file for ARToolKitPlus source node.
  *
  * @author Daniel Wagner
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section artoolkitmultimarkersource ARToolKitMultiMarkerSource
 * The ARToolKitMultiMarkerSource node is a simple EventGenerator that inserts events 
 * from tracking a visual multi-marker in a video image by using the ARToolKitPlus
 * library. See the @ref artoolkitplusmodule for information about how to
 * configure the module as a whole. It has the following elements :
 * @li @c cfg-file the file containing setup the marker patterns
 *
 * An example element looks like this :
 * @verbatim
<ARToolKitMultiMarkerSource cfg-file="test_setup.cfg" />@endverbatim
 */

#ifndef _ARTOOLKITMULTIMARKERSOURCE_H
#define _ARTOOLKITMULTIMARKERSOURCE_H

#include "../OpenTracker.h"


#if defined(USE_ARTOOLKIT) || defined(USE_ARTOOLKITPLUS)

namespace ot {

/** An ARToolKitSource represents a single marker that is tracked by the 
 * artoolkit library.
 * @ingroup input
 */
class OPENTRACKER_API ARToolKitMultiMarkerSource : public Node
{

public:
    /// the state pushed into the tree
    State state, 
    /// buffer variable for grabbing thread
        buffer;
	/// filename of the multimarker config file
	std::string cfgfile;
	/// pointer to the multi-marker configuration structure (loaded from cfgfile)
	void* mmConfig;
    /// is a new update ready in the buffer ?
    int modified;

protected:
    /** constructor method
     * @param markerId_ the id of the marker from artoolkit library
     * @param vertex_ vertex coordinates of the marker
     */
    ARToolKitMultiMarkerSource(std::string& cfgfile_, void* mmConfig_) : 
        Node(),
        cfgfile( cfgfile ),
		mmConfig(mmConfig_),
        modified(0)
    {
    }

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

#endif  // _ARTOOLKITMULTIMARKERSOURCE_H

