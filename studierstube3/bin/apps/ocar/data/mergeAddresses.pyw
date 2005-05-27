# mergeAddresses takes an aml model file and looks for waypoints that are not connected.
# Then it will add new edges to the model to connect these waypoints to the rest of the
# network.
# Usage : mergeAddresses input.xml output.xml
# @author Gerhard Reitmayr
# $Id: mergeAddresses.pyw 3427 2004-07-14 12:20:49Z tamer $

from xml.dom import *
from xml.dom.ext.reader import Sax2
from xml.dom.ext import Print, PrettyPrint
from string import *
import sys
import time

SplitCounter = 0
MinDist = 100

fminus = lambda x, y: x - y
fmult = lambda x, y: x*y
fadd = lambda x, y: x+y
dot = lambda x,y : reduce(fadd, map( fmult, x, y))

def makeScalar(s) :
    return lambda x : x * s

def generateString( list ) :
    adder = lambda a,b : a + ' ' + str(b)
    result = reduce( adder, list, '' )
    return result[1:]

def setupWaypoint(waypoint) :
    """sets various data on the waypoint for simplified data handling"""
    waypoint.id = waypoint.getAttribute('id')
    transforms = waypoint.getElementsByTagName('Transformation')
    if len(transforms) != 1 :
        print 'Error in ' + waypoint.getAttribute('id') + ' no transform element !'
        waypoint.position = [0, 0, 0]
    else :
        position = transforms[0].getAttribute('translation')
        waypoint.position = map( float, split(position))
    return waypoint

def createMap( waypointList ) :
    """creates an id to waypoint map and replaces the neighbor lists with
       references to the waypoints"""

    waypointmap = {}
    for point in waypointList :
        waypointmap[point.id] = point
    func = lambda x: waypointmap[x]
    for point in waypointList :
        point.neighbors = map( func, split( point.getAttribute('neighbors')) )
    return waypointmap

def createEdges( waypointList ) :
    """creates a list of edges that have a begin and an end vertex. Note that we only
       deal with an undirected graph here ! To guarantee that we will order the points
       lexicographically by their id. """
    edges = []
    for point in waypointList :
        for neighbor in point.neighbors :
            if point.id <= neighbor.id :
                key = (point, neighbor)
            else:
                key = (neighbor, point)
            if key not in edges :
                edges.append( key )
    return edges

def classifyPoints( waypointList ) :
    """ separates points into disconnected and connected, this is done in a single loop
        to speed up processing"""    
    disconnected = []
    connected = []
    for point in waypointList :
        if len(point.neighbors) == 0 :
            disconnected.append( point )
        else :
            connected.append( point )
    return connected, disconnected

def closestPoint( point, pointList ) :
    """returns the closest point of a given list to a given waypoint"""
    dist = 1000000000000
    result = None
    for other in pointList :
        diff = map(fminus,point.position, other.position)
        test = dot( diff, diff )
        if test < dist  :
            dist = test
            result = other
    return result, dist

def closestEdge( point, edgeList ) :
    """returns the closest edge to a given waypoint"""
    dist = 1000000000000
    result = None
    pos = 0,0,0
    rt = 0
    for edge in edgeList :
        dir = map(fminus, edge[1].position, edge[0].position)
        dirlen = dot(dir,dir)
        p = map(fminus, point.position, edge[0].position)
        plen = dot(p,p)
        pdir = dot(p,dir)
        t = pdir / dirlen
        if 0 <= t and t <= 1 :
            test = plen + t*t*dirlen - 2*t*pdir
            if test < dist :
                dist = test
                result = edge
                ft = makeScalar(t)
                ftminus = makeScalar(1-t)
                pos = map(fadd, map(ft, edge[1].position), map(ftminus, edge[0].position))
                rt = t
    return result, pos, dist, rt

def compare( a, b ) :
    """a simple compare to compare two floats without rounding errors"""
    if a < b :
        return -1
    elif a == b :
        return 0
    else :
        return 1

def splitEdge( edge, splits ) :
    """splits an edge at the list of splitpoints listed and sets the correct
    neighbors in the attributes"""
    global SplitCounter
    global MinDist
    
    document = edge[0].ownerDocument
    edge[0].neighbors.remove(edge[1])
    edge[1].neighbors.remove(edge[0])
    splits.sort( lambda a,b : compare(a[2], b[2]) )
    pred = edge[0]
    newpoints = []

    for cut in splits :
        dist = map(fminus, pred.position, cut[1] )
        if dot(dist, dist) <= MinDist :
            pred.neighbors.append( cut[0] )
            cut[0].neighbors.append( pred )
        else :
            waypoint = document.createElement('Waypoint')
            newpoints.append( waypoint )
            pose = document.createElement('pose')
            transform = document.createElement('Transformation')
            pose.appendChild( transform )
            waypoint.appendChild( pose )
            waypoint.id = 'Split_' + str(SplitCounter)
            SplitCounter += 1
            waypoint.setAttribute('id', waypoint.id)
            waypoint.position = cut[1]            
            transform.setAttribute('translation', generateString( cut[1] ))
            waypoint.neighbors = [ pred, cut[0] ]
            cut[0].neighbors.append( waypoint )
            pred.neighbors.append(waypoint)
            pred = waypoint
            
    pred.neighbors.append( edge[1] )
    edge[1].neighbors.append( pred )
    return newpoints
        
def main() :
    global MinDist
    filename = 'e:\\studierstube\\bin\\apps\\ocar\\data\\small2.xml'
    if len(sys.argv) > 1 :
        filename = sys.argv[1]
        
    timer = time.clock()        
    print 'Reading in data from '  + filename + ' ',
    reader = Sax2.Reader()
    model = reader.fromStream( file(filename) )
    print '... ' + str( time.clock() - timer )
    timer = time.clock()        
    print 'Setting up graph structures: ',
    waypoints = model.getElementsByTagName('Waypoint')
    waypoints = map( setupWaypoint, waypoints )
    pointmap = createMap( waypoints )
    edges = createEdges( waypoints )
    edgesplits = {}
    connected, disconnected = classifyPoints( waypoints )
    print 'Connected',len(connected),', Disconnected',len(disconnected),'...',str( time.clock() - timer )

    timer = time.clock()        
    print 'Calculating new edges - ',
                # there are four cases to the whole thing (withouth counting empty results )
    case1 = 0   # another point is closer than any edge -> construct a new edge to that point
    case2 = 0   # an edge is closer than any point and the closest point is not endpoint -> split edge for a new t crossing
    case3 = 0   # an edge is closer and the closest point is an endpoint and the split point is close to the endpoint -> a new edge to that endpoint
    case4 = 0   # an edge is closer and not the above holds -> split edge for a new t crossing
    for point in disconnected :
        closestpoint, pointdist = closestPoint( point, connected )
        edge, cutpos, edgedist, edgeparameter = closestEdge( point, edges )
        if pointdist <= edgedist :
            point.neighbors.append( closestpoint )
            closestpoint.neighbors.append( point )
            case1 = case1 + 1
        else :
            if closestpoint <> edge[0] and closestpoint <> edge[1] :
                if edge not in edgesplits :
                    edgesplits[edge] = []
                edgesplits[edge].append( (point, cutpos, edgeparameter) )
                case2 = case2 + 1
            else :
                if closestpoint == edge[0] :
                    vertex = edge[0]
                else :
                    vertex = edge[1]
                diff = map( fminus, vertex.position, cutpos )
                dist = dot( diff, diff )
                if dist <= MinDist :
                    point.neighbors.append( vertex )
                    vertex.neighbors.append( point )
                    case3 = case3 + 1
                else :
                    if edge not in edgesplits :
                        edgesplits[edge] = []
                    edgesplits[edge].append( (point, cutpos, edgeparameter) )
                    case4 = case4 + 1
    print 'number of cases 1:',case1,'2:',case2,'3:',case3,'4:',case4,
    print '... ' + str( time.clock() - timer )

    timer = time.clock()        
    print 'Splitting edges at new points ',
    for edge in edgesplits :
        newpoints = splitEdge( edge, edgesplits[edge] )
        map( lambda x : waypoints.append(x), newpoints )
        map( lambda x : waypoints[0].parentNode.appendChild(x), newpoints )
    print '... ' + str( time.clock() - timer )

    timer = time.clock()        
    print 'Updating neighbor fields of all waypoints ',
    for point in waypoints :
        ids = map( lambda x : getattr(x, 'id' ), point.neighbors )
        point.setAttribute('neighbors', generateString( ids ))
    print '... ' + str( time.clock() - timer )

    timer = time.clock()        
    print 'Writing out file ',
    if len(sys.argv) > 2 :
        PrettyPrint( model, file( sys.argv[2], 'w+' ))
    else :
        PrettyPrint( model, file('e:\\studierstube\\bin\\apps\\ocar\\data\\out.xml', 'w+'  ))
    print '... ' + str( time.clock() - timer )
                
    del model
    
if __name__ == "__main__":
    main()
