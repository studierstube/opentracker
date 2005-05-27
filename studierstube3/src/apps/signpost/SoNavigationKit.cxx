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
  * $Id: SoNavigationKit.cxx 3801 2004-09-30 14:02:02Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <assert.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#include <stbapi/workspace/SoApplicationKit.h>
#include <stbapi/workspace/SoContextManagerKit.h>
#include <stbapi/resource/SoTrackedArtifactKit.h>
#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/widgets/SoListBox.h>
#include <stbapi/util/ivio.h>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <vector>

#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoCoordinate3.h>

#include "SoNavigationKit.h"
#include "SoBAUBuilding.h"

using namespace std;
using namespace boost;

SO_KIT_SOURCE(SoNavigationKit);

//----------------------------------------------------------------------------

void
SoNavigationKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoNavigationKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoNavigationKit::SoNavigationKit()
{
    SO_KIT_CONSTRUCTOR(SoNavigationKit);

    SO_KIT_ADD_FIELD(building, (NULL));
	SO_KIT_ADD_FIELD(position, (0, 0, 0));
	SO_KIT_ADD_FIELD(orientation, (1, 0, 0, 0));
	SO_KIT_ADD_FIELD(destinationRoom, (""));
	SO_KIT_ADD_FIELD(currentRoom, (""));
	SO_KIT_ADD_FIELD(nextRoom, (""));
	SO_KIT_ADD_FIELD(linePoints, (0, 0, 0));
	SO_KIT_ADD_FIELD(path, (""));
	SO_KIT_ADD_FIELD(arrowDirection, (1, 0, 0, 0));
	SO_KIT_ADD_FIELD(destinationReached, (FALSE));
    SO_KIT_ADD_FIELD(portalPosition, (0,0,0));
    SO_KIT_ADD_FIELD(portalIndex, (-1));

    SO_KIT_INIT_INSTANCE();

    setupSensor = new SoFieldSensor();
    setupSensor->setFunction(SoNavigationKit::setupSensorCB);
    setupSensor->setData(this);

    positionSensor = new SoFieldSensor();
    positionSensor->setFunction(SoNavigationKit::positionSensorCB);
    positionSensor->setData(this);

    currentRoomSensor = new SoFieldSensor();
    currentRoomSensor->setFunction(SoNavigationKit::currentRoomSensorCB);
    currentRoomSensor->setData(this);

    destinationRoomSensor = new SoFieldSensor();
    destinationRoomSensor->setFunction(SoNavigationKit::destinationRoomSensorCB);
    destinationRoomSensor->setData(this);

    pathSensor = new SoFieldSensor();
    pathSensor->setFunction(SoNavigationKit::pathSensorCB);
    pathSensor->setData(this);

    setUpConnections( TRUE, TRUE );
}


//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoNavigationKit::~SoNavigationKit()
{
    delete setupSensor;
    delete positionSensor;
    delete currentRoomSensor;
    delete destinationRoomSensor;
    delete pathSensor;
}

//----------------------------------------------------------------------------

SbBool 
SoNavigationKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
    if ((doitalways == FALSE) && (connectionsSetUp == onoff)) return onoff;
    if (onoff) 
    {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onoff, doitalways);

        // do your own special setup here !
        setupSensor->attach(&building);
        positionSensor->attach(&position);
        currentRoomSensor->attach(&currentRoom);
        destinationRoomSensor->attach(&destinationRoom);
        pathSensor->attach(&path);

    }
    else 
    {
        // We disconnect BEFORE base class.
        // do your own special setup here !
        positionSensor->detach();
        currentRoomSensor->detach();
        destinationRoomSensor->detach();
        pathSensor->detach();

        SoBaseKit::setUpConnections(onoff, doitalways);
    }
    return !(connectionsSetUp = onoff);
}


//----------------------------------------------------------------------------

void 
SoNavigationKit::setup()
{
    printf("SoNavigationKit::setup\n");

    if(building.getValue() != NULL )
    {
        if(building.getValue()->isOfType(SoBAUBuilding::getClassTypeId()))
        {
            SoBAUBuilding * bau = (SoBAUBuilding *)building.getValue();
            SoListBox *lb = (SoListBox *)getByName("ROOMLISTBOX");
            lb->values.connectFrom(&(bau->roomNames));
        }
    }

}

//----------------------------------------------------------------------------

void 
SoNavigationKit::setupSensorCB(void *data, SoSensor *sensor)
{
    SoNavigationKit *nk = (SoNavigationKit *)data;
    nk->setup();
    if( nk->building.getValue() != NULL )
        nk->setupGraph();
}

void 
SoNavigationKit::positionSensorCB(void *data, SoSensor *sensor)
{
    SoNavigationKit *nk = (SoNavigationKit *)data;
    if( nk->building.getValue() != NULL )
    {
        SoBAUBuilding * bau = (SoBAUBuilding *)nk->building.getValue();
        assert( bau->isOfType(SoBAUBuilding::getClassTypeId()));
        const SbString *roomname = bau->inRoom(nk->position.getValue());
        if (roomname != NULL) 
            if (*roomname != nk->currentRoom.getValue()) 
                nk->currentRoom.setValue(*roomname); 
        nk->updateState();
    }
}

void 
SoNavigationKit::currentRoomSensorCB(void *data, SoSensor *sensor)
{
    SoNavigationKit *nk = (SoNavigationKit *)data;
    nk->calculatePath();
}

void 
SoNavigationKit::destinationRoomSensorCB(void *data, SoSensor *sensor)
{
    SoNavigationKit *nk = (SoNavigationKit *)data;
    nk->calculatePath();
}

void 
SoNavigationKit::pathSensorCB(void *data, SoSensor *sensor)
{
    SoNavigationKit *nk = (SoNavigationKit *)data;

    printf("***  Found Path: ");
    for (int j=0; j<nk->path.getNum(); j++) printf("%s ", nk->path[j].getString());
    printf("\n");

    nk->updateState();
}

//----------------------------------------------------------------------------

void 
SoNavigationKit::updateState()
{
    if (path.getNum() > 0)
    {
        if( destinationReached.getValue() != (currentRoom == destinationRoom ))
            destinationReached.setValue((currentRoom == destinationRoom) ? TRUE : FALSE);
    }
    if (path.getNum() >= 2)
    {
        linePoints.set1Value(0, position.getValue());
        if( nextRoom.getValue() != path[1] )
        {
            SoBAUBuilding * bau = (SoBAUBuilding *)building.getValue();
            assert( bau->isOfType(SoBAUBuilding::getClassTypeId()));

            SoBAURoom *room = bau->getRoom(currentRoom.getValue());
            int pi = room->neighbors.find(path[1]);
            portalPosition.setValue( room->portalPositions[pi]);
            linePoints.set1Value(1, room->portalPositions[pi]);
            nextRoom.setValue(path[1]);
            portalIndex.setValue( bau->getPortalIndex( currentRoom.getValue(), nextRoom.getValue()));
        }

        SbVec3f dir = linePoints[1] - linePoints[0];
        SbVec3f xdir(1, 0, 0); 
        SbRotation rot(xdir, dir);
        SbRotation invusrrot = orientation.getValue().inverse();
        arrowDirection.setValue(rot * invusrrot);


/*
printf("%i %i\n", room->portalPositions.getNum(), room->portalNodes.getNum());
        // highlight portal
        if (highlightedPortalSep != NULL)
        {
            highlightedPortalSep->removeAllChildren();
SoFaceSet *fs = (SoFaceSet *)(room->portalNodes[pi]);
int xx = ((SoCoordinate3 *)(fs->vertexProperty.getValue()))->point.getNum();
SoNode *nfs = fs->copy(FALSE);

printf("A%i",xx);
            highlightedPortalSep->addChild(nfs);
printf("B");
        }

printf("C");
*/
    }
}

//----------------------------------------------------------------------------

void 
SoNavigationKit::setupGraph()
{
    property_map<NavGraph, vertex_name_t>::type roomIndex = boost::get(vertex_name, graph);
    property_map<NavGraph, edge_name_t>::type portalIndex = boost::get(edge_name, graph);
    property_map<NavGraph, edge_weight_t>::type portalDist = boost::get(edge_weight, graph);
    
    SoBAUBuilding * bau = (SoBAUBuilding *)building.getValue();
    assert( bau->isOfType(SoBAUBuilding::getClassTypeId()));
        
    // Remove all the vertices. This is OK. (from boost doc)
    indices.clear();
    graph_traits<NavGraph>::vertex_iterator vi, vi_end, next;
    tie(vi, vi_end) = vertices(graph);
    for (next = vi; vi != vi_end; vi = next) 
    {
        ++next;
        clear_vertex(*vi, graph);
        remove_vertex(*vi, graph);
    }

    // input all the graph data into the graph
    Vertex v, u;
    graph_traits<NavGraph>::edge_descriptor e;    
    int i, j, k;
    bool inserted;

    for(i=0; i<bau->roomNodes.getNum(); i++)
    {
        u = add_vertex(graph);
        indices[i] = u;
        roomIndex[u] = i;
    }

    k = 0;
    for(i=0; i<bau->roomNodes.getNum(); i++)
    {
        u = indices[i];
        SoBAURoom *room = ((SoBAURoom *)(bau->roomNodes[i]));
        for (j=0; j<room->neighbors.getNum(); j++)
        {
            v = indices[bau->roomNames.find(room->neighbors[j])];
            tie(e, inserted) = add_edge(u, v, graph);
            portalIndex[e] = k;
            portalDist[e] = 1.0f; // (waypointPosition[i] - waypointPosition[edge[j]]).length();
            k++;
        }
    }
}

void 
SoNavigationKit::calculatePath()
{
/*
    printf("Current Room ....... %s\n", currentRoom.getValue().getString());
    printf("Destination Room ... %s\n", destinationRoom.getValue().getString());
*/
    
    if( building.getValue() == NULL )
        return;

    SoBAUBuilding * bau = (SoBAUBuilding *)building.getValue();
    assert( bau->isOfType(SoBAUBuilding::getClassTypeId()));    

    // compute destination index
    int destIndex = bau->roomNames.find(destinationRoom.getValue());
    int currentIndex = bau->roomNames.find(currentRoom.getValue());

    if(destIndex == -1) return;
    if(currentIndex == -1) return;
    
    // compute path -> this is where we use boost !!
    vector<Vertex> predecessors(num_vertices(graph));
    vector<float> distances(num_vertices(graph));
        
    Vertex s = indices[currentIndex];
    Vertex d = indices[destIndex];

#if defined(BOOST_MSVC) && (BOOST_MSVC <= 1300)
    // VC++ has trouble with the named parameters mechanism
    property_map<NavGraph, vertex_index_t>::type indexmap = boost::get(vertex_index, graph);
    property_map<NavGraph, edge_weight_t>::type weightmap = boost::get(edge_weight, graph);
    dijkstra_shortest_paths(graph, s, &predecessors[0], &distances[0], weightmap, indexmap, 
        std::less<float>(), closed_plus<float>(), 
        std::numeric_limits<float>::max(), 0,
        default_dijkstra_visitor());
#else
    dijkstra_shortest_paths(graph, s, predecessor_map(&predecessors[0]).distance_map(&distances[0]));
#endif

    // set outputs
    vector<int> pathNodes;
    vector<int> pathEdges;
    property_map<NavGraph, vertex_name_t>::type vertexIndices = boost::get(vertex_name, graph);
    graph_traits<NavGraph>::edge_descriptor e;    
        
    // this way of inserting is not optimal, but it allows us to use the vector
    // data directly in the subsequent Open Inventor calls. And I trust the STL
    // more than Open Inventor with efficiency :)
    Vertex pre;
    bool resultOk;
    while (d != s)
    {
        pre = predecessors[d];
        // there is no path to the start node
        if( pre == d )
            break;

        pathNodes.insert(pathNodes.begin(), vertexIndices[d]);
        tie(e, resultOk) = boost::edge(pre, d, graph);
        d = pre;
    }
    // test for an actual path
    if ( d == s ) {
        pathNodes.insert(pathNodes.begin(), vertexIndices[d]);
        
        path.setNum(pathNodes.size());
        vector<int>::iterator it;
        for(it=pathNodes.begin(); it!=pathNodes.end(); it++) path.set1Value(it-pathNodes.begin(), bau->roomNames[(*it)]);
    }
    else
    {
        path.deleteValues(0, -1);
        destinationRoom.setValue("");
        nextRoom.setValue("");
    }
}

//----------------------------------------------------------------------------
//eof
//----------------------------------------------------------------------------

