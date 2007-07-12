#ifndef _LIVECONTEXT_H
#define _LIVECONTEXT_H

#ifdef USE_LIVE

#include "../dllinclude.h"

#include <omniORB4/poa.h>

#include <OpenTracker/common/CommonNodeFactory.h>
#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Context.h>
#include <OpenTracker/core/Module.h>
#include <OpenTracker/core/NodeFactoryContainer.h>

#include <OpenTracker/skeletons/OTGraph.hh>

#include <OpenTracker/core/OtLogger.h>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#if defined(XERCES_NEW_IOSTREAMS)
#include <iostream>
#else
#include <iostream.h>
#endif

XERCES_CPP_NAMESPACE_USE
    
using namespace std;

namespace ot {

  typedef std::pair<Node *, Node *> Edge;
  typedef std::vector<Edge> EdgeVector;
  
 class OPENTRACKER_API LiveContext : public Context, 
    public PortableServer::RefCountServantBase {
  private:
    CORBAModule* corba_module;
    int no_nodes;
    EdgeVector edges;

  public:
    LiveContext();
    virtual ~LiveContext() {
      cerr << "LiveContext:: Destructor" << endl;
    };
    
    ModuleMap getModules() {
      return modules;
    };

    OTGraph::Node_ptr get_node(const char* id);

    OTGraph::Node_var create_node(const char* _name, const OTGraph::StringTable& _attributes);

    OTGraph::Edge* connect_nodes(const OTGraph::Node_var& upstreamNode, const OTGraph::Node_var& downstreamNode);
    
    void disconnect_nodes(const OTGraph::Node_var& upstreamNode, const OTGraph::Node_var& downstreamNode); 

    OTGraph::NodeVector* get_nodes();

    OTGraph::EdgeVector* get_edges();

    void remove_node(const OTGraph::Node_var& target_ref);
    
    void run();

    char* getXMLString();

  protected:
    ConfigNode* configNode;
  };
}

#endif //USE_LIVE
#endif //_LIVECONTEXT_H
