#ifndef _TRACKERSERVER_H
#define _TRACKERSERVER_H

// Forward extern class declarations
class DOM_Element;

//@START_USER1
#include <vector>
#include <map>
struct ltstr
{
    bool operator()(const char* s1, const char* s2) const
    {
        return strcmp(s1,s2) <0;
    }
}; 
//@END_USER1

// Defines needed for relations between templated classes

// Type declarations
//@START_DECLARATION_99 NodeVector
class Node;
typedef std::vector<Node*> NodeVector;
//@END_DECLARATION_99

//@START_DECLARATION_1131 NodeFactoryVector
class NodeFactory;
typedef std::vector<NodeFactory*> NodeFactoryVector;
//@END_DECLARATION_1131

//@START_DECLARATION_1615 MarkerVector
class Marker;
typedef std::vector<Marker *> MarkerVector;
//@END_DECLARATION_1615

//@START_DECLARATION_3227 StationVector
class Station;
typedef std::vector<Station *> StationVector;
//@END_DECLARATION_3227

//@START_DECLARATION_3497 StringMap
typedef std::map<const char*, const char*, ltstr> StringMap;
//@END_DECLARATION_3497

//@START_DECLARATION_3867 ModuleMap
class Module;
typedef std::map<const char *, Module *, ltstr> ModuleMap;
//@END_DECLARATION_3867

//@START_DECLARATION_4412 ModuleVector
class Module;
typedef std::vector<Module *> ModuleVector;
//@END_DECLARATION_4412

// Forward class declarations
class Node;
class State;
class TreeNode;
class Transformation;
class VirtualTransformation;
class MaxConfidenceFilter;
class Module;
class VideoSource;
class NodeFactory;
class CommonNodeFactory;
class NodeFactoryContainer;
class Marker;
class Station;
class NetworkDriver;
class ConfigurationParser;
class MathUtils;
class Context;

// Needed ClassBuilder include files

// Make sure the inline implementations are skipped
#ifdef CB_INLINES
#undef CB_INLINES
#endif

// Include classes, for declarations
#include "Node.h"
#include "State.h"
#include "TreeNode.h"
#include "Transformation.h"
#include "VirtualTransformation.h"
#include "MaxConfidenceFilter.h"
#include "Module.h"
#include "NodeFactory.h"
#include "VideoSource.h"
#include "CommonNodeFactory.h"
#include "NodeFactoryContainer.h"
#include "Marker.h"
#include "Station.h"
#include "NetworkDriver.h"
#include "ConfigurationParser.h"
#include "MathUtils.h"
#include "Context.h"


// Include classes again, for inline implementation
#define CB_INLINES
#include "Station.h"

//@START_USER2
//@END_USER2

#endif
