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
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/ARToolKitSource.h,v 1.9 2001/04/12 06:42:34 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section artoolkitsource ARToolKitSource
 * The ARToolKitSource node is a simple EventGenerator that inserts events 
 * from tracking a visual marker in a video image by using the ARToolKit
 * library. It has the following elements :
 * @li @c tag-file the file containing the marker pattern
 * @li @c vertex two dimensional array of the 4 vertices of the marker pattern. The 
 *        corners are given in millimeters with respect to the center of the pattern.
 *
 * An example element looks like this :
 * @verbatim
<ARToolKitSource tag-file="pip.tag" vertex="-20 -20 -20 20 20 20 20 -20" />@endverbatim
 */

#ifndef _ARTOOLKITSOURCE_H
#define _ARTOOLKITSOURCE_H

#include "../OpenTracker.h"

#ifdef USE_ARTOOLKIT

/** An ARToolKitSource represents a single marker that is tracked by the 
 * artoolkit library.
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
    /// coordinates of the marker
    double vertex[4][2];
    /// is a new update ready in the buffer ?
    int modified;

public:
    /** constructor method
     * @param markerId_ the id of the marker from artoolkit library
     * @param vertex_ vertex coordinates of the marker
     */
    ARToolKitSource(int markerId_, double vertex_[4][2]) : 
        Node(),        
        markerId( markerId_ ),
        modified(0)
    {
        for( int i = 0; i < 4; i ++ )
        {
            vertex[i][0] = vertex_[i][0];
            vertex[i][1] = vertex_[i][1];
        }
    };

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
