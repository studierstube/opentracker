/* ========================================================================
 * Copyright (C) 2000,2001,2002  Vienna University of Technology
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
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: ocar
 * ======================================================================== */
/**
 * header for navigation application context
 *
 * @author Gerhard Reitmayr
 *
 * $Id: Navigator.h 3801 2004-09-30 14:02:02Z reitmayr $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _NAVIGATOR_H_
#define _NAVIGATOR_H_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoSubNode.h>

#include "ocar.h"

#include <boost/graph/adjacency_list.hpp>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**	
 * This class handles navigation computations for the navigation application. The implementation depends
 * on the Boost library version 1.30.0.
 * @author Gerhard Reitmayr
 * @ingroup ocar
 */
class OCAR_API Navigator : public SoNode  
{
    SO_NODE_HEADER(Navigator);

public:
    static void initClass(void);

	Navigator();

	virtual ~Navigator();

public:

    /** @name Configuration data fields
     *  hold configuration data that is not expected to change at runtime. 
     * In a future distributed model with database servers, these could be 
     * the interfaces to update the clients.
     */
    /*@{*/

    /// list of names for all waypoints
    SoMFString waypoint;
    
    /// list of positions for all waypoints
    SoMFVec3f waypointPosition;
    
    /// list of number of outgoing edges for all waypoints
    SoMFInt32 numEdge;
    
    /// list of all end points of any edges, grouped by vertex 
    SoMFInt32 edge;

    /// hysteresis distance used in determining the closest waypoint to the users position
    SoSFFloat hysteresis;
    /*@}*/
        
    /** @name Input data fields
     * These will only be read by the
     * navigation application and trigger updates in the state.
     */
    /*@{*/
    /// list of possible destinations, the closest one along the graph will be computed as target
    SoMFString destination;

    /// current user position
    SoSFVec3f userPosition;

    /// current user orientation
    SoSFRotation userOrientation;

    /** alternative input for route computation as a destination position in space.
     * The last field set will be used for the computation 
     */
    SoSFVec3f destinationPosition;
    /*@}*/
        
    /** @name Output data fields
     * These will only be written to by the
     * navigation application. Writing to these fields does not do anything.
     */
    /*@{*/

    /// name of the waypoint closest to the current user position
    SoSFString currentWaypoint;

    /// index of waypoint closest to the current user position
    SoSFInt32 currentIndex;

    /// index of the next waypoint in the route
    SoSFInt32 gotoIndex;

    /// total distance to target
    SoSFFloat distance;
    
    /// absolute direction of the vector from current user position to current waypoint projected into the x-z plane
    SoSFRotation direction;

    /// angle between users current rotation and the direction
    SoSFFloat relativeDirection;

    /// list of waypoints that form the current route
    SoMFString path;
    
    /// list of edges that form the current route
    SoMFInt32 edgeIndex;

    /// list of waypoint indices that form the current route
    SoMFInt32 nodeIndex;
    
    /// flag true if currentWaypoint == computedTarget
    SoSFBool destinationReached;

    /// name of computed target waypoint
    SoSFString computedTarget;

    /// index of computed target waypoint
    SoSFInt32 targetIndex;

    /// position of computed target waypoint
    SoSFVec3f targetPosition;
    /*@}*/
    
protected:
        
    /** This method caculates new outputs from changed inputs.
    */
    void adaptToState( void );
    
    /** This method sets up internal graph data structures for the graph 
    * algorithms involved. 
    */
    void setupInternalData( void );

    /** This method computes the gotoIndex based on the users position
     * to provide correct guidance. 
     */
    void adaptToPosition( void );

    /** setup data after reading in from file !
     */
    virtual SbBool readInstance (SoInput *in, unsigned short flags);
    
    static void destinationChanged( void * data, SoSensor * sensor );

    static void positionChanged( void * data, SoSensor * sensor );
    
    static void orientationChanged( void * data, SoSensor * sensor );

    static void destinationPositionChanged( void * data, SoSensor * sensor );


    SoFieldSensor destSensor;
    SoFieldSensor positionSensor;
    SoFieldSensor orientationSensor;
    SoFieldSensor destPostSensor;

    static void dataChanged( void * data, SoSensor * sensor );

    SoFieldSensor waypointSensor;    
    SoFieldSensor waypointPositionSensor;    
    SoFieldSensor numEdgeSensor;    
    SoFieldSensor edgeSensor;
    
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, 
        boost::property<boost::vertex_name_t, int>, 
        boost::property<boost::edge_name_t, int, boost::property<boost::edge_weight_t, float> >
    > NavGraph;
    
    typedef boost::graph_traits < NavGraph >::vertex_descriptor Vertex;
    typedef std::map < int, Vertex > IndexVertexMap;
    
    NavGraph graph;    

    IndexVertexMap indices;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // !defined(_NAVIGATOR_H_)
