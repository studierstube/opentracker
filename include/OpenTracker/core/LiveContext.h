#ifndef _LIVECONTEXT_H
#define _LIVECONTEXT_H

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
  typedef std::map<std::string, Node *> NodeMap;
  
 class OPENTRACKER_API LiveContext : public Context, 
    public PortableServer::RefCountServantBase {
  private:
    CORBAModule* corba_module;
    int no_nodes;
    NodeVector nodes;
    EdgeVector edges;

    string generateNewId() {
      char node_string[5];
      sprintf(node_string,"%d", no_nodes++);
      return string("Node") + string(node_string);
    };

  public:
  LiveContext() : Context(1), no_nodes(0) {
      cerr << "LiveContext:: Constructor" << endl;
      //initializeContext( this , NULL);
      corba_module = (CORBAModule*) modules["CORBAConfig"];
      StringTable st;
      //      st.put("endPoint", "giop:tcp:scumble.lce.cl.cam.ac.uk:9999");
      st.put("endPoint", "giop:tcp:localhost:9999");
      corba_module->init(st, NULL);
      parseConfigurationString("<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE OpenTracker SYSTEM \"opentracker.dtd\"><OpenTracker> <configuration/></OpenTracker>");
    };

    virtual ~LiveContext() {
      cerr << "LiveContext:: Destructor" << endl;
    };
    
    ModuleMap getModules() {
      return modules;
    };

    OTGraph::Node_ptr get_node(const char* id) {
      PortableServer::POA_var poa = corba_module->getPOA();
      PortableServer::ObjectId_var node_id = PortableServer::string_to_ObjectId(CORBA::string_dup(id));
      return OTGraph::Node::_duplicate(OTGraph::Node::_narrow(poa->id_to_reference(node_id)));
    };

    Node* getNodeFromRef(const OTGraph::Node_var& node_ref) {
      PortableServer::POA_var poa = corba_module->getPOA();
      PortableServer::ServantBase* node_servant = poa->reference_to_servant(OTGraph::Node::_duplicate(node_ref));
      Node* node = dynamic_cast<Node*>(node_servant);
      return node;
    }

    OTGraph::Node_var getRefFromNode(Node* node) {
      string id(node->get("ID"));
      PortableServer::ObjectId_var node_id = PortableServer::string_to_ObjectId(id.c_str());
      OTGraph::Node_var node_ref = OTGraph::Node::_narrow((corba_module->getPOA())->id_to_reference(node_id));
      return node_ref;
    }

    OTGraph::Node_var activateNode(Node* node) {
      PortableServer::POA_var poa = corba_module->getPOA();
      CORBA::String_var string_id = CORBA::string_dup(node->get("ID").c_str());
      PortableServer::ObjectId_var corba_id = PortableServer::string_to_ObjectId(string_id);
      poa->activate_object_with_id(corba_id, node);
      OTGraph::Node_var node_ref = OTGraph::Node::_narrow(poa->id_to_reference(corba_id));
      return node_ref;
    }

    void deactivateNode(Node* node) {
      string id(node->get("ID"));
      PortableServer::ObjectId_var node_id = PortableServer::string_to_ObjectId(id.c_str());
      (corba_module->getPOA())->deactivate_object(node_id);
    }

    OTGraph::Node_var create_node(const char* _name, const OTGraph::StringTable& _attributes) {
      string name(_name);
      StringTable attributes(_attributes);
      if (!attributes.containsKey("ID")) {
	attributes.put("ID", generateNewId());
	cerr << "generated ID = " << attributes.get("ID") << endl;
      } else {
	cerr << "ID in attributes = " << attributes.get("ID") << endl;
      }
      lock();
      Node* value = createNode(name, attributes);
      value->type = name;
      value->name = value->get("ID");

      // get Module (loading it if necessary)
      Module * mod = NULL;
      mod  = getModuleFromNode(value);
      if (mod != NULL){
	mod->addNode(value);
      }

      // By default add to root Node
      rootNode->addChild(*value);

      // Add node to local list of nodes
      nodes.push_back(value);
      OTGraph::Node_var node_ref = activateNode(value); 
      unlock();
      return node_ref;
    }

    void connect_nodes(const OTGraph::Node_var& upstreamNode, const OTGraph::Node_var& downstreamNode) {
      lock();
      Node* sender   = getNodeFromRef(upstreamNode);
      Node* receiver = getNodeFromRef(downstreamNode);
      // Test to see whether parent of sender is rootNode
      if (sender->getParent() == rootNode) {
	// The sender node has no functional Node as parent
	// so remove from rootNode and add to the receivingNode
	rootNode->removeChild(*sender);
      }
      receiver->addChild(*sender);
      // add new edge to list of edges
      edges.push_back(Edge(sender, receiver));
      unlock();
    }

    
    void disconnect_nodes(const OTGraph::Node_var& sendingNode, const OTGraph::Node_var& receivingNode) {
      lock();
      Node* sender   = getNodeFromRef(sendingNode);
      Node* receiver = getNodeFromRef(receivingNode);
      if (receiver == sender->getParent()) {
	// The sender node is the child of the receiver node
	receiver->removeChild(*sender);
	rootNode->addChild(*sender);
      } else {
	// The nodes must be connected via a RefNode
	Node* child = receiver->getChild(0);
	if ((child->getType()).compare("Ref") == 0) {
	  // remove reference to child in referencing node
	  sender->removeReference(child); 
	  // remove as Child from receiving node
	  receiver->removeChild(*child);
	  // delete the RefNode
	  delete child;
	}
      }
      // removing edge from local list of edges
      Edge edge(sender, receiver);
      EdgeVector::iterator result = std::find( edges.begin(), edges.end(), edge );
      if( result != edges.end())
        {
	  edges.erase( result );
        }
      unlock();
    }


    OTGraph::NodeVector* get_nodes() {
      OTGraph::NodeVector_var node_refs = new OTGraph::NodeVector;
      int len = nodes.size();
      node_refs->length(len);
      for (int i=0; i < nodes.size(); i++) {
	node_refs[i] = getRefFromNode(nodes[i]);
      }
      return node_refs._retn();
    };

    OTGraph::EdgeVector* get_edges() {
      OTGraph::EdgeVector_var _edges = new OTGraph::EdgeVector;
      int len = edges.size();
      _edges->length(len);
      for (int i=0; i < edges.size(); i++) {
	OTGraph::Edge _edge;
	_edge.sender   = getRefFromNode(edges[i].first);
	_edge.receiver = getRefFromNode(edges[i].second);
	_edges[i] = _edge;
      }
      return _edges._retn();
    };

    void remove_node(const OTGraph::Node_var& target_ref) {
      cerr << "remove_node" << endl;
      lock();
      Node* target  = getNodeFromRef(target_ref);

      // Handle child nodes (upstream nodes)
      NodeVector::iterator child_it = target->children.begin();
      while ( child_it != target->children.end())
        {
	  Node* child = child_it->item();
	  target->removeChild(*child);
	  //  and add to rootNode if child node has been orphaned
	  if (child->parents.size() == 0) {
	    rootNode->addChild(*child);
	  }
	  child_it++;
        }
      // Handle parents
      NodeVector::iterator parent_it = target->parents.begin();
      while ( parent_it != target->parents.end())
        {
	  parent_it->item()->removeChild(*target);
	  parent_it++;
        }

      // deactivate the object before passing the node to the module to be deleted
      deactivateNode(target);

      // removing node from local list
      NodeVector::iterator result = std::find( nodes.begin(), nodes.end(), target );
      if( result != nodes.end())
        {
	  nodes.erase( result );
        }
      
      // identify module, and instruct module to delete node
      Module * mod = getModuleFromNode(target);
      if (mod != NULL) {
	mod->removeNode(target);
      } else {
	// node doesn't have a known module so delete here
	delete target;
      }
      unlock();
    }

    void run()
    {
      //start();
      int stopflag = stop();
      while ( stopflag == 0 )
        {
	  stopflag=loopOnce();
        }
      //close(); 
    };
    

    char* getXMLString() {
      return CORBA::string_dup("<foo/>");
    }

  protected:
    ConfigNode* configNode;
  };
}
#endif //_LIVECONTEXT_H
