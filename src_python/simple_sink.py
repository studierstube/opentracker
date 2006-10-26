#!/usr/bin/env python
import sys
sys.path.append('../idl/skeletons')
from copy import deepcopy
from omniORB import CORBA, PortableServer, PortableServer__POA

# Import the stubs for the Naming service
import CosNaming

# Import the stubs and skeletons for the Example module
import OT_CORBA, OT_CORBA__POA

# Define an implementation of the Echo interface
class SimpleSink_i (OT_CORBA__POA.Sink):
    def setEvent(self, event):
        print "pushEvent() called with event:", event

def getContextFromName(root_context, context_name):
    try:
	obj = root_context.resolve(context_name)
    except CosNaming.NamingContext.NotFound, ex:
	print "Name not found"
	return None
    sink_context = obj._narrow(obj)
    if sink_context is None:
	print "Object reference isn't a Context"
	return None
    return sink_context
    
def getContext(root_context, context_name):
    name = deepcopy(context_name)
    context_already_bound = None
    uncontextualised_names = []
    # terminate loop when `name' is non-existent or a bound context is found
    while name and not(context_already_bound):
	context_already_bound = getContextFromName(root_context, name)
	if context_already_bound is None:
	    uncontextualised_names.append(name.pop())
    uncontextualised_names.reverse() # reverse order of names
    print "name = ", name
    if not(name):
	context_already_bound = root_context
    previous_context = context_already_bound
    for n in uncontextualised_names:
	print "binding context with name ", n, " to context ", previous_context
	next_context = previous_context.bind_new_context([n])
	previous_context = next_context
    return previous_context

if __name__ == '__main__':
    # Initialise the ORB
    sys.argv.extend(["-ORBendPoint", "giop:tcp:localhost:9903"])
    print sys.argv
    orb = CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

    # Find the root POA
    poa = orb.resolve_initial_references("RootPOA")
    poaManager = poa._get_the_POAManager()
    
    # Create a child POA with the right policies 
    ps = [poa.create_id_assignment_policy(PortableServer.USER_ID),
	  poa.create_lifespan_policy(PortableServer.PERSISTENT)]
    persistentPOA = poa.create_POA("childPOA", poaManager, ps)
    childManager = persistentPOA._get_the_POAManager()

    # Create an instance of SimpleSink_i
    servant = SimpleSink_i()

    # don't implicitly activate the object
    #sinkref = servant._this()
    #oid = PortableServer.string_to_ObjectId("my object");
    #persistentPOA.activate_object(servant)
    persistentPOA.activate_object_with_id("test", servant);
    sinkref = servant._this()
    
    # Obtain a reference to the root naming context
    obj         = orb.resolve_initial_references("NameService")
    extContext  = obj._narrow(CosNaming.NamingContextExt)

    if extContext is None:
	print "Failed to narrow the root naming context"
	sys.exit(1)

    string_name = sys.argv[1]
    name = extContext.to_name(string_name)
    context_name = name[:-1]
    sink_name = name[-1:]
    sink_context = getContext(extContext, context_name)

    # Bind the SimpleSink `sink_name' object to the appropriate context
    try:
	sink_context.bind(sink_name, sinkref)
	print "New Sink object bound"
    except CosNaming.NamingContext.AlreadyBound:
	sink_context.rebind(sink_name, sinkref)
	print "Sink binding already existed -- rebound"

	# Note that is should be sufficient to just call rebind() without
	# calling bind() first. Some Naming service implementations
	# incorrectly raise NotFound if rebind() is called for an unknown
	# name, so we use the two-stage approach above

    # Activate the POA
    childManager.activate()
    poaManager.activate()

    # Everything is running now, but if this thread drops out of the end
    # of the file, the process will exit. orb.run() just blocks until the
    # ORB is shut down
    orb.run()
