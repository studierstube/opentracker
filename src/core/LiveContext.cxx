/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** source file for Context class
 *
 * @author Joseph Newman
 *
 * $Id: LiveContext.cxx 1790 2007-03-26 09:55:14Z jfn $
 * @file                                                                   */     
/* ======================================================================= */

#include <OpenTracker/OpenTracker.h>
#ifdef USE_LIVE
#include <OpenTracker/network/CORBAModule.h>
#include <OpenTracker/core/LiveContext.h>
#include <OpenTracker/core/Context.h>
#include <omniORB4/poa.h>
#include <OpenTracker/skeletons/OTGraph.hh>
#endif //USE_LIVE

using namespace std;

namespace ot {

#ifdef USE_LIVE
  LiveContext::LiveContext() : Context(1), no_nodes(0) {
      cerr << "LiveContext:: Constructor" << endl;
      //initializeContext( this , NULL);
      corba_module = (CORBAModule*) modules["CORBAConfig"].item();
      StringTable st;
      //      st.put("endPoint", "giop:tcp:scumble.lce.cl.cam.ac.uk:9999");
      st.put("endPoint", "giop:tcp:localhost:9999");
      corba_module->init(st, NULL);
      parseConfigurationString("<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE OpenTracker SYSTEM \"opentracker.dtd\"><OpenTracker> <configuration/></OpenTracker>");
    }

  OTGraph::Node_ptr LiveContext::get_node(const char* id) {
      PortableServer::POA_var poa = corba_module->getPOA();
      PortableServer::ObjectId_var node_id = PortableServer::string_to_ObjectId(CORBA::string_dup(id));
      return OTGraph::Node::_duplicate(OTGraph::Node::_narrow(poa->id_to_reference(node_id)));
  }

  Node* LiveContext::getNodeFromRef(const OTGraph::Node_var& node_ref) {
    PortableServer::POA_var poa = corba_module->getPOA();
    PortableServer::ServantBase* node_servant = poa->reference_to_servant(OTGraph::Node::_duplicate(node_ref));
    Node* node = dynamic_cast<Node*>(node_servant);
    node->_remove_ref();
      return node;
  }

  OTGraph::Node_var LiveContext::getRefFromNode(Node* node) {
      string id(node->get("ID"));
      PortableServer::ObjectId_var node_id = PortableServer::string_to_ObjectId(id.c_str());
      OTGraph::Node_var node_ref = OTGraph::Node::_narrow((corba_module->getPOA())->id_to_reference(node_id));
      return node_ref;
  }

  OTGraph::Node_var LiveContext::activateNode(Node* node) {
      PortableServer::POA_var poa = corba_module->getPOA();
      CORBA::String_var string_id = CORBA::string_dup(node->get("ID").c_str());
      PortableServer::ObjectId_var corba_id = PortableServer::string_to_ObjectId(string_id);
      poa->activate_object_with_id(corba_id, node);
      OTGraph::Node_var node_ref = OTGraph::Node::_narrow(poa->id_to_reference(corba_id));
      node->_remove_ref();
      std::cout << "Node " << node->get("ID") << " activated with reference count " << node->_refcount_value() << std::endl;
      return node_ref;
    }

  OTGraph::Node_var LiveContext::create_node(const char* _name, const OTGraph::StringTable& _attributes) {
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

      // Add node to the name-Node mapping
      nodes[value->name] = value;
      OTGraph::Node_var node_ref = activateNode(value); 
      unlock();
      return node_ref;
    }

  void LiveContext::connect_nodes(const OTGraph::Node_var& upstreamNode, const OTGraph::Node_var& downstreamNode) {
      lock();
      Node::Ptr upstream_node   = getNodeFromRef(upstreamNode);
      Node*     downstream_node = getNodeFromRef(downstreamNode);
      downstream_node->addChild(*upstream_node);

      // add new edge to list of edges
      edges.push_back(Edge(((Node*) upstream_node), downstream_node));
      unlock();
    }

  void LiveContext::disconnect_nodes(const OTGraph::Node_var& upstreamNode, const OTGraph::Node_var& downstreamNode) {
      lock();
      Node* upstream_node   = getNodeFromRef(upstreamNode);
      Node* downstream_node = getNodeFromRef(downstreamNode);

      NodeVector::iterator parent =
	std::find( upstream_node->parents.begin(),  upstream_node->parents.end(), downstream_node);
      if (parent != upstream_node->parents.end()) {
	upstream_node->parents.erase(parent);
      }
      NodeVector::iterator child =
	std::find( downstream_node->children.begin(),  downstream_node->children.end(), upstream_node);
      if (child != downstream_node->children.end()) {
	// The sender node is the child of the receiver node
	downstream_node->children.erase(child);
      }
      Edge edge(upstream_node, downstream_node);
      EdgeVector::iterator result = std::find( edges.begin(), edges.end(), edge );
      if( result != edges.end())
	{
	  edges.erase( result );
	}
      unlock();
    }

  OTGraph::NodeVector* LiveContext::get_nodes() {
      OTGraph::NodeVector_var node_refs = new OTGraph::NodeVector;
      int len = nodes.size();
      node_refs->length(len);
      int i = 0;
      for (NodeMap::iterator it=nodes.begin(); it != nodes.end(); it++) {
	node_refs[i] = getRefFromNode(it->second);	
	i++;
      }
      return node_refs._retn();
    };

  OTGraph::EdgeVector* LiveContext::get_edges() {
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

  void LiveContext::remove_node(const OTGraph::Node_var& target_ref) {
      lock();
      Node* target  = getNodeFromRef(target_ref);
      std::cout << "Node " << target->get("ID") << " of type " << target->getType() << " being removed with reference count " << target->_refcount_value() << std::endl;

      // Handle child nodes (upstream nodes)
      NodeVector::iterator child_it = target->children.begin();
      while ( child_it != target->children.end())
        {
	  Node* child = (Node *) *child_it;
	  target->removeChild(*child);
	  child_it++;
        }
      // Handle parents
      NodeVector::iterator parent_it = target->parents.begin();
      while ( parent_it != target->parents.end())
        {
	  Node * parent = (Node *) (*parent_it);
	  parent->removeChild(*target);
	  parent_it++;
        }

      // removing node from local list
      string id(target->get("ID"));
      NodeMap::iterator result = nodes.find(id);
      if( result != nodes.end())
        {
	  nodes.erase( result );
        }
      
      // identify module, and instruct module to delete node
      Module * mod = getModuleFromNode(target);
      if (mod != NULL) {
	logPrintI("calling mod->removeNode\n");
	mod->removeNode(target);
	logPrintI("called mod->removeNode\n");
      } else {
	logPrintE("unable to find module associated with node %s\n",id.c_str());
      }

      // deactivate the object before passing the node to the module to be deleted
      deactivateNode(target);


      unlock();
    }

  void LiveContext::run()
    {
      //start();
      int stopflag = stop();
      while ( stopflag == 0 )
        {
	  stopflag=loopOnce();
        }
      //close(); 
    };

  char* LiveContext::getXMLString() {
    return CORBA::string_dup("<foo/>");
  }
  

#endif //USE_LIVE

} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of LiveContext.cxx
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
