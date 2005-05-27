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
 * implementation of Navigator node
 *
 * @author Gerhard Reitmayr
 *
 * $Id: Navigator.cxx 3801 2004-09-30 14:02:02Z reitmayr $
 * @file                                                                   */
/* ======================================================================= */

#include "Navigator.h"

#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <cassert>

// #define OCAR_DEBUG

#ifdef OCAR_DEBUG
#define DEBUG(a) a
#else
#define DEBUG(a) ;
#endif

using namespace std;
using namespace boost;

SO_NODE_SOURCE(Navigator);

void Navigator::initClass(void)
{
    SO_NODE_INIT_CLASS(Navigator, SoNode, "Node" );
}

Navigator::Navigator()
{
    SO_NODE_CONSTRUCTOR(Navigator);

    isBuiltIn = TRUE;
    
    SO_NODE_ADD_FIELD( currentWaypoint, (""));
    SO_NODE_ADD_FIELD( destination, (""));
    SO_NODE_ADD_FIELD( destinationPosition, (0,0,0));
    SO_NODE_ADD_FIELD( destinationReached, (FALSE));
    SO_NODE_ADD_FIELD( path, (""));
    SO_NODE_ADD_FIELD( waypoint, (""));
    SO_NODE_ADD_FIELD( waypointPosition, (0,0,0));
    SO_NODE_ADD_FIELD( numEdge, (0));
    SO_NODE_ADD_FIELD( edge, (0));
    SO_NODE_ADD_FIELD( userPosition, (0,0,0));
    SO_NODE_ADD_FIELD( userOrientation, (SbVec3f(0,1,0),0));
    SO_NODE_ADD_FIELD( currentIndex, (0));
    SO_NODE_ADD_FIELD( edgeIndex, (0));
    SO_NODE_ADD_FIELD( nodeIndex, (0));
    SO_NODE_ADD_FIELD( gotoIndex, (0));
    SO_NODE_ADD_FIELD( hysteresis, (1.1f));
    SO_NODE_ADD_FIELD( computedTarget, (""));
    SO_NODE_ADD_FIELD( distance, (0));
    SO_NODE_ADD_FIELD( direction, (SbVec3f(0,1,0),0));
    SO_NODE_ADD_FIELD( relativeDirection, (0));
    SO_NODE_ADD_FIELD( targetIndex, (-1));
    SO_NODE_ADD_FIELD( targetPosition, (0,0,0));
    
    destSensor.setData( this );
    destSensor.setFunction( Navigator::destinationChanged );
    positionSensor.setData( this );
    positionSensor.setFunction( Navigator::positionChanged );
    orientationSensor.setData( this );
    orientationSensor.setFunction( Navigator::orientationChanged );
    destPostSensor.setData( this );
    destPostSensor.setFunction( Navigator::destinationPositionChanged );

    destSensor.attach( &destination );
    positionSensor.attach( &userPosition );
    orientationSensor.attach( &userOrientation );
    destPostSensor.attach( &destinationPosition );

    waypointSensor.setData( this );
    waypointSensor.setFunction( Navigator::dataChanged );
    waypointPositionSensor.setData( this );
    waypointPositionSensor.setFunction( Navigator::dataChanged );
    edgeSensor.setData( this );
    edgeSensor.setFunction( Navigator::dataChanged );
    numEdgeSensor.setData( this );
    numEdgeSensor.setFunction( Navigator::dataChanged );

    waypointSensor.attach( &waypoint );
    waypointPositionSensor.attach( &waypointPosition );
    edgeSensor.attach( &edge );
    numEdgeSensor.attach( &numEdge );
}

Navigator::~Navigator()
{
}

void Navigator::adaptToState( void )
{
    int i;

    // test if setup properly
    if( num_edges(graph) == 0 )
    {
        DEBUG(printf("Navigator::adaptToState graph not setup! returning...\n"));
        return;
    }

    // compute destination index
    vector<int> destIndex;
    for( i = 0; i < destination.getNum(); i ++ )
    {
        int index = waypoint.find( destination[i] );
        if( index != -1)
            destIndex.push_back(index);
    }
    if( destIndex.size() == 0 )
    {
        destination.setValue("");
        nodeIndex.deleteValues(0);
        edgeIndex.deleteValues(0);
        path.deleteValues(0);
        computedTarget.setValue("");
        targetIndex.setValue(-1);
        targetPosition.setValue(destinationPosition.getValue());
        distance.setValue(-1);
        destinationReached.setValue(FALSE);
        return;
    }

    // compute currentWaypoint and index
    int current = currentIndex.getValue();
    /*
    float dist = 10e+10;
    for( i = 0; i < waypointPosition.getNum(); i ++ )
    {
        if((userPosition.getValue() - waypointPosition[i]).length() < dist )
        {
            current = i;
            dist = (userPosition.getValue() - waypointPosition[i]).length();
        }
    }
    assert( current != -1 && "Navigation could not compute current waypoint !");
    */
    
    // compute path -> this is where we use boost !!
    // it computes the minimal spanning tree with the current waypoint as root node
    // then we simply follow the tree back from all destination points to the
    // root to find the closest destination and the shortest path.
    vector<Vertex> predecessors(num_vertices(graph));
    vector<float> distances(num_vertices(graph));
        
    Vertex s = indices[current];
#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
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

    // compute the length for all paths
    float minimum = distances[destIndex[0]];
    int dest = destIndex[0];
    for( i = 1; (unsigned int)i < destIndex.size(); i++ )
    {
        if( distances[destIndex[i]]  < minimum )
        {
            dest = destIndex[i];
            minimum = distances[destIndex[i]];
        }
    }

    Vertex d = indices[dest];
    Vertex pre;
    vector<int> pathNodes;
    vector<int> pathEdges;
    property_map<NavGraph, vertex_name_t>::type vertexIndices = boost::get(vertex_name, graph);
    property_map<NavGraph, edge_name_t>::type edgeIndices = boost::get(edge_name, graph);
    graph_traits<NavGraph>::edge_descriptor e;    
    bool resultOk;    
        
    // this way of inserting is not optimal, but it allows us to use the vector
    // data directly in the subsequent Open Inventor calls. And I trust the STL
    // more than Open Inventor with efficiency :)
    while( d != s )
    {
        pre = predecessors[d];

        // there is no path to the start node
        if( pre == d )
            break;

        pathNodes.insert( pathNodes.begin(), vertexIndices[d] );
        tie(e,resultOk) = boost::edge(pre, d, graph);
        if( resultOk != true )
        {
            DEBUG(printf("Navigator::adaptToState cannot find edge. dest=%s\n", 
                          waypoint[dest].getString()));
            break;
        }
        pathEdges.insert( pathEdges.begin(),edgeIndices[e] );
        d = pre;
    }
    // test for an actual path
    if( d == s )
    {
        pathNodes.insert( pathNodes.begin(), vertexIndices[d] );
        
        nodeIndex.setValues(0, pathNodes.size(), &pathNodes[0] );
        nodeIndex.setNum( pathNodes.size());
        edgeIndex.setValues(0, pathEdges.size(), &pathEdges[0] );
        edgeIndex.setNum( pathEdges.size());
        
        path.setNum( pathNodes.size() );
        vector<int>::iterator it;
        for( it = pathNodes.begin(); it != pathNodes.end(); it++ )
            path.set1Value( it - pathNodes.begin(), waypoint[(*it)] );
        
        distance.setValue( minimum );
        targetIndex.setValue(dest);
        computedTarget.setValue( waypoint[dest] );
        targetPosition.setValue( waypointPosition[dest] );
    }
    else // no path found set to something reasonable
    {
        path.deleteValues(0, -1);
        computedTarget.setValue("");
        distance.setValue(-1);
        nodeIndex.deleteValues(0,-1);
        edgeIndex.deleteValues(0,-1);
        targetIndex.setValue( -1 );
        targetPosition.setValue(0,0,0);
    }
    // make sure orientation is updated as well
    // userOrientation.touch();
}

void Navigator::setupInternalData( void )
{    
    // clear everything
    indices.clear();
    graph.clear();

    // some simple tests if the data is somewhat plausible
    if( (waypointPosition.getNum() != waypoint.getNum()) || ( waypoint.getNum() != numEdge.getNum()))
    {
        DEBUG(printf("Navigator::setupInternalData: data sizes do not match -> no setup\n"));
        return;
    }
            
    property_map<NavGraph, vertex_name_t>::type waypointIndex = boost::get(vertex_name, graph );
    property_map<NavGraph, edge_name_t>::type edgeIndex = boost::get(edge_name, graph );
    property_map<NavGraph, edge_weight_t>::type edgeLength = boost::get( edge_weight, graph );
        
    // input all the graph data into the graph
    Vertex v, u;
    graph_traits<NavGraph>::edge_descriptor e;    
    int i, j;
    bool inserted;
    for( i = 0; i < waypoint.getNum(); i++ )
    {
        u = add_vertex(graph);
        indices[i] = u;
        waypointIndex[u] = i;
    }
    vector<int> offset(numEdge.getNum()+1);
    offset[0] = 0;
    for( i = 0; i < numEdge.getNum(); i++ )
    {
        offset[i+1] = offset[i] + numEdge[i];
    }
    for( i = 0; i < waypoint.getNum(); i++ )
    {
        u = indices[i];
        for( j = offset[i]; j < offset[i+1]; j ++)
        {
            assert(((int)indices.size() > edge[j]) && "Navigator::setupInternalData wrong index in edge !");
            v = indices[edge[j]];
            tie(e,inserted) = add_edge(u, v, graph);
            assert( inserted && "Navigator::setupInternalData could not insert new edge !");
            edgeIndex[e] = j;
            edgeLength[e] = (waypointPosition[i] - waypointPosition[edge[j]]).length();
        }
    }
    DEBUG(printf("Navigator::setupInternalData finished !\n"));
}

void Navigator::adaptToPosition( void )
{
//    if( computedTarget.getValue() == "")
//        return;
    // compute currentWaypoint and index
    int i;
    int closest = -1;
    float distClosest = 10e+10f;
    for( i = 0; i < waypointPosition.getNum(); i ++ )
    {
        if((userPosition.getValue() - waypointPosition[i]).length() < distClosest )
        {
            closest = i;
            distClosest = (userPosition.getValue() - waypointPosition[i]).length();
        }
    }
    if( closest != currentIndex.getValue() ) // test for new current
    {
		float distCurrent = (userPosition.getValue() - waypointPosition[currentIndex.getValue()]).length();
        if( distCurrent > distClosest * hysteresis.getValue() ) // hysteresis not anymore -> change
        {
            // we got a new current !
		    DEBUG(printf("Navigator new closest %i dist %f vs. current %i, dist %f\n", closest, distClosest,
				currentIndex.getValue(), distCurrent));
            currentIndex.setValue( closest );
            currentWaypoint.setValue( waypoint[closest] );            
            adaptToState();
        }
    }
	if( nodeIndex.getNum() > 1)
	{
		SbVec3f cu = userPosition.getValue() - waypointPosition[currentIndex.getValue()];
		SbVec3f cn = waypointPosition[nodeIndex[1]] - waypointPosition[nodeIndex[0]];
        // this variable stores the first edge, in case we need to remove it for
        // display purposes. This is a little bit of a hack :)
        static long edge;
		if( cu.dot(cn) >= 0)
		{
			gotoIndex.setValue( nodeIndex[1] );
            if( edgeIndex.getNum() == nodeIndex.getNum() - 1)
            {
                edge = edgeIndex[0];
                edgeIndex.deleteValues(0,1);
            }            
		}        
		else
		{
			gotoIndex.setValue( nodeIndex[0] );
            if( edgeIndex.getNum() < nodeIndex.getNum() - 1)
            {
                edgeIndex.insertSpace(0,1);
                edgeIndex.set1Value(0,edge);
            }            
		}
	}
    destinationReached.setValue(currentWaypoint.getValue() == computedTarget.getValue());
    // make sure orientation is updated as well
    // userOrientation.touch();
}

void Navigator::destinationChanged( void * data, SoSensor *)
{
    assert( data != NULL );
    Navigator * self = (Navigator *)data;
    self->adaptToState();
    self->destPostSensor.detach();
    self->destinationPosition.setValue( self->targetPosition.getValue() );
    self->destPostSensor.attach( &self->destinationPosition );
}

void Navigator::positionChanged( void * data, SoSensor *)
{
    assert( data != NULL );
    ((Navigator *)data)->adaptToPosition();
}

void Navigator::orientationChanged( void * data, SoSensor *)
{
    assert( data != NULL );
    Navigator * self = (Navigator *)data;

    // project vector between user position and next waypoint onto ground plane
    SbVec3f cu = self->waypointPosition[self->gotoIndex.getValue()] - self->userPosition.getValue();
    cu[1] = 0;
    cu.normalize();
    SbVec3f viewDir( 0, 0, -1);
    SbRotation direction( viewDir, cu );
    self->direction.setValue( direction );
    DEBUG(printf("absolute direction "); direction.print(stdout); printf("\n"););

    SbVec3f userDir;
    SbRotation userOr = self->userOrientation.getValue();

    // project user viewing direction onto ground plane.
    userOr.multVec(viewDir, userDir);
    userDir[1] = 0;
    userDir.normalize();
    // right handed normal vector 
    SbVec3f userNormal( userDir[2], 0, -userDir[0]);

    // relative angle is between cu and userDir
    double angle = acos( cu.dot(userDir));
    if( cu.dot(userNormal) < 0 ) // negative angle
        angle *= -1.0;

    self->relativeDirection.setValue((float)angle);
    DEBUG(printf("relative direction %lf\n", angle););
}

void Navigator::destinationPositionChanged( void * data, SoSensor *)
{
    assert( data != NULL );
    Navigator * self = (Navigator *)data;

    int i;
    int closest = -1;
    float distClosest = 10e+10f;
    for( i = 0; i < self->waypointPosition.getNum(); i ++ )
    {
        float len = (self->destinationPosition.getValue() - self->waypointPosition[i]).length();
        if( len < distClosest )
        {
            closest = i;
            distClosest = len;
        }
    }
    assert((closest != -1) && "Could not find the closest point ??");
    if( self->computedTarget.getValue() != self->waypoint[closest] )
    {
        self->destSensor.detach();
        self->destination.setValue(self->waypoint[closest]);
        self->adaptToState();
        self->destSensor.attach( &self->destination );
    }
}

void Navigator::dataChanged( void * data, SoSensor *)
{
    assert( data != NULL );
    ((Navigator *)data)->setupInternalData();
}

SbBool Navigator::readInstance (SoInput *in, unsigned short flags)
{
    SbBool result = SoNode::readInstance( in, flags );
    if( result )
    {
        setupInternalData();
    }
    return result;
}
