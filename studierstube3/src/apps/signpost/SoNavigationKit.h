/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
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
  * PROJECT: Studierstube
  * ======================================================================== */ 

 /* ======================================================================= */
/** SignPost Navigation
  *
  * @author Michael Knapp <knapp@cg.tuwien.ac.at>
  *
  * $Id: SoNavigationKit.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SoNavigationKit_H_
#define _SoNavigationKit_H_

#include <stbapi/workspace/SoContextKit.h>
#include <boost/graph/adjacency_list.hpp>

#include "signpost.h"

class SoBAUBuilding;

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * The Navigation Application (NAV)
 * @ingroup signpost
 */
class SIGNPOST_API SoNavigationKit : public SoContextKit
{
    SO_KIT_HEADER(SoNavigationKit);

//	SO_KIT_CATALOG_ENTRY_HEADER(augmentation);

//    enum contentEnum { ROOM, PATH, ALL };
//    enum renderStyleEnum { INVISIBLE, WIREFRAME, TRANSLUCENT, WIREFRAME_TRANSLUCENT, HIDDENLINE };

public:
    static void initClass();
    SoNavigationKit();
    ~SoNavigationKit();

    // Configuration fields
    
    /// contains reference to the SoBAUBuilding node to use
    SoSFNode building;

    // Input fields

    /// the destination room, this is an input field used to set the room, the user wants to go to. 
    SoSFString destinationRoom;
    /// user position, from an SoTrakEngine 
    SoSFVec3f position;
    /// user orientation, from same SoTrakEngine as above 
    SoSFRotation orientation;

    // Output fields

    /// the current room, I think this is an output field. If merged with startRoom it will also be an input field 
    SoSFString currentRoom;
    /// output field with next room. 
    SoSFString nextRoom;
    /// output field with next room in path of rooms to follow. 
    SoMFString path;
    /// indicated wheter destination reached
    SoSFBool destinationReached;
    /// 2 values to draw the stroked arrow geometry between user and portal 
    SoMFVec3f linePoints;
    /// rotation output for HUD arrow 
    SoSFRotation arrowDirection;
    /// next portal orientation for VRLU group
    SoSFVec3f portalPosition;    
    /// index of portal to next room in path 
    SoSFString portalIndex;

/*
    ///  switch output for HUD arrow 
    SoSFInt32 arrowState;

    /// output field with path of rooms to follow. 
    SoSFBool showPortals;
    /// show red line through the model, this could also be done with fixed arrows at turning points (where the red line has to bend a lot) !! 
    SoSFBool showLine;
    /// show the directional arrow, a 2D icon 
    SoSFBool showArrow;
    /// show heads up display with textual information on: current room, destination room and rooms left to cross 
    SoSFBool showHUD;
*/

protected:
    virtual SbBool setUpConnections(SbBool onoff, SbBool doitalways);

    SoFieldSensor *setupSensor;
    SoFieldSensor *positionSensor;
    SoFieldSensor *currentRoomSensor;
    SoFieldSensor *destinationRoomSensor;
    SoFieldSensor *pathSensor;

    // SoBAUBuilding *BAUBuilding;

    void setup();

    void setupGraph();
    void calculatePath();
    void updateState();

    static void setupSensorCB(void *data, SoSensor *sensor);
    static void positionSensorCB(void *data, SoSensor *sensor);
    static void currentRoomSensorCB(void *data, SoSensor *sensor);
    static void destinationRoomSensorCB(void *data, SoSensor *sensor);
    static void pathSensorCB(void *data, SoSensor *sensor);

    // boost stuff
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, 
        boost::property<boost::vertex_name_t, int>, 
        boost::property<boost::edge_name_t, int, boost::property<boost::edge_weight_t, float> >
    > NavGraph;
    typedef boost::graph_traits<NavGraph>::vertex_descriptor Vertex;
    typedef std::map<int, Vertex> IndexVertexMap;
    
    NavGraph graph;    
    IndexVertexMap indices;    
}; 

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif

