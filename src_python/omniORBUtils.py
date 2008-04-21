from omniORB import CORBA, _omnipy

import re, sys, socket
ipaddress_re = re.compile(r'IIOP\s\d.\d\s([\w]*.[\w]*.[\w]*.[\w]*)\s[\d]*', re.M)
from popen2 import popen4

import CosNaming
from naming import str2name
from copy import deepcopy

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

def initialiseORB():
    return CORBA.ORB_init(sys.argv, CORBA.ORB_ID)

def initialiseORBandPOA():
    orb = initialiseORB()
    poa = getRootPOA(orb)
    getActivePOAManager(poa)
    return orb, poa

def getRootContext(orb):
    # Obtain a reference to the root naming context
    obj         = orb.resolve_initial_references("NameService")
    rootContext = obj._narrow(CosNaming.NamingContext)

    if rootContext is None:
        print "Failed to narrow the root naming context"
        sys.exit(1)
    return rootContext

def getObjectReference(orb, name):
    rootContext = getRootContext(orb)
    obj = rootContext.resolve(name)
    return obj

def bindObjectReferenceToName(orb, obj, name):
    cos_name = str2name(name)
    root_context = getRootContext(orb)
    context = getContext(root_context, cos_name[:-1])
    print cos_name[-1], context
    try:
	root_context.bind(cos_name, obj)
	"Object bound"
    except CosNaming.NamingContext.AlreadyBound:
	print cos_name[-1]
	root_context.rebind(cos_name, obj)
	"Object rebound"

def getNarrowedObjectReference(orb, name, objectClass):
    obj = getObjectReference(orb, name)
    narrowed_ref = obj._narrow(objectClass)
    return narrowed_ref

def getRootPOA(orb):
    poa=orb.resolve_initial_references("RootPOA")
    poaManager=poa._get_the_POAManager()
    return poa

def getActivePOAManager(poa):
    poaManager=poa._get_the_POAManager()
    poaManager.activate()
    return poaManager

class injector:
    def __init__(self, obj, attr):
        self.obj = obj
        self.attr = attr
        
    def __call__(self, attr_obj):
        setattr(self.obj, self.attr, attr_obj)

def injectClassWithMethods(orb, klass):
    @injector(klass, "__str__")
    def stringify_obj(self):
        return orb.object_to_string(self)

    @injector(klass, "__hash__")
    def hash_obj(self):
        return _omnipy.hash(self, 10000)

    @injector(klass, "__cmp__")
    def cmp_obj(self, other):
        if _omnipy.isEquivalent(self, other):
            return 0
        else:
            return 1

    @injector(klass, "host")
    def ipaddress_obj(self):
        (output, input) = popen4('catior %s' % self.__str__())
        ipaddress = ipaddress_re.findall(output.read())[0]
        output.close()
        input.close()
        return ipaddress

    @injector(klass, "ipaddress")
    def ipaddress_obj(self):
        (output, input) = popen4('catior %s' % self.__str__())
        hostname = ipaddress_re.findall(output.read())[0]
        output.close()
        input.close()
        host, aliases, ips = socket.gethostbyname_ex(hostname)
        ip = ips[0]
        return ip


def getObjRefClasses(module):
    for d in dir(module):
        if 'objref' in d:
            yield module.__getattribute__(d)

def injectNewMethods(orb, modules):
    for module in modules:
        for klass in getObjRefClasses(module):
            injectClassWithMethods(orb, klass)


