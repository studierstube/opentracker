/* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for ARToolKit source node.
  *
  * @author Gerhard Reitmayr
  *
  * $Header1$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page Nodes Node Reference
 * @section artoolkitsource ARToolKitSource
 * The ARToolKitSource node is a simple EventGenerator that inserts events 
 * from tracking a visual marker in a video image by using the ARToolKit
 * library. It has the following elements :
 * @li @c tag-file the file containing the marker pattern
 * @li @c vertex two dimensional array of the 4 vertices of the marker pattern
 *
 * An example element looks like this :
 * @verbatim
<ARToolKitSource tag-file="pip.tag" vertex="-20 -20 -20 20 20 20 20 -20" />@endverbatim
 */

#ifndef _ARTOOLKITSOURCE_H
#define _ARTOOLKITSOURCE_H

#include "../OpenTracker.h"

/** An ARToolKitSource represents a single marker that is tracked by the 
 * artoolkit library.
 */
class ARToolKitSource : public Node, EventGenerator
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
        EventGenerator(),
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
     * return this always.
     * @return always this */
    EventGenerator * isEventGenerator()
    {
        return this;
    }

    /** pushes event down the line. Needed to access protected
     * updateObservers method in EventGenerator */
    void push()
    {
        updateObservers( state );
    }
};

#endif
