#ifndef _TRACKERSERVER_H
#define _TRACKERSERVER_H

// Forward extern class declarations
class DOM_Element;

//@START_USER1
#include <vector>
#include <map>

using namespace std;

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
//@START_DECLARATION_99 TreeNodeVector
class TreeNode;
typedef std::vector<TreeNode*> TreeNodeVector;
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

//@START_DECLARATION_7216 CursesOutputVector
class CursesOutput;
typedef std::vector<CursesOutput *> CursesOutputVector;
//@END_DECLARATION_7216

//@START_DECLARATION_13363 StateVector
class State;
typedef std::vector<State *> StateVector;
//@END_DECLARATION_13363

//@START_DECLARATION_13371 NetworkSourceBuffer
class NetworkSource;
class State;

typedef struct {
    NetworkSource * source;
    State * buffer;
    int active;
} NetworkSourceBuffer;
//@END_DECLARATION_13371

//@START_DECLARATION_13372 NetworkSourceBufferVector
typedef std::vector<NetworkSourceBuffer *> NetworkSourceBufferVector;
//@END_DECLARATION_13372

// Forward class declarations
class State;
class TreeNode;
class Transformation;
class MathUtils;
class VirtualTransformation;
class MaxConfidenceFilter;
class Module;
class VideoModule;
class NodeFactory;
class CommonNodeFactory;
class NodeFactoryContainer;
class Marker;
class Station;
class NetworkModule;
class ConfigurationParser;
class Context;
class TestSource;
class TestModule;
class CursesModule;
class CursesOutput;
class NetworkSourceModule;
class NetworkSource;

// Needed ClassBuilder include files

// Make sure the inline implementations are skipped
#ifdef CB_INLINES
#undef CB_INLINES
#endif

// Include classes, for declarations
#include "State.h"
#include "TreeNode.h"
#include "Transformation.h"
#include "MathUtils.h"
#include "VirtualTransformation.h"
#include "MaxConfidenceFilter.h"
#include "Module.h"
#include "NodeFactory.h"
#include "VideoModule.h"
#include "CommonNodeFactory.h"
#include "NodeFactoryContainer.h"
#include "Marker.h"
#include "Station.h"
#include "NetworkModule.h"
#include "ConfigurationParser.h"
#include "Context.h"
#include "TestSource.h"
#include "TestModule.h"
#include "CursesModule.h"
#include "CursesOutput.h"
#include "NetworkSourceModule.h"
#include "NetworkSource.h"


// Include classes again, for inline implementation
#define CB_INLINES
#include "Module.h"
#include "Station.h"
#include "NetworkSource.h"

//@START_USER2
//@END_USER2

#endif
