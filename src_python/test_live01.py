import sys

from omniORB import CORBA, any

import OTGraph
from OTGraph import *

# Import the stubs for the Naming service
import CosNaming

def initialiseORB():
    return CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

def getGraphReference(orb):
    # Obtain a reference to the root naming context
    obj         = orb.resolve_initial_references("NameService")
    rootContext = obj._narrow(CosNaming.NamingContext)

    if rootContext is None:
        print "Failed to narrow the root naming context"
        sys.exit(1)

    name = [CosNaming.NameComponent("Foo", "Test")]

    obj = rootContext.resolve(name)
    graph_ref = obj._narrow(OTGraph.DataFlowGraph)
    return graph_ref

def init():
    orb = initialiseORB()
    return orb, getGraphReference(orb)

if __name__ == '__main__':
    # Initialise the ORB
    orb, graph = init()
    n1=graph.create_node("CORBASink",[KeyValuePair("name","CORBA.Sink01")])
    n2=graph.create_node("TestSource", [KeyValuePair("frequency", "100"), KeyValuePair("noise", "0.1")])
    graph.add_node(n1, n2)
    graph.add_node_to_root(n1)
    #graph.add_node_to_root(n2)
