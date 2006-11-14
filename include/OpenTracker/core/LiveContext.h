#ifndef _LIVECONTEXT_H
#define _LIVECONTEXT_H

#include "../dllinclude.h"

#include <omniORB4/poa.h>

#include <OpenTracker/common/CommonNodeFactory.h>
#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/core/Context.h>
#include <OpenTracker/core/Module.h>
#include <OpenTracker/core/NodeFactoryContainer.h>
#include <OpenTracker/core/RefNode.h>

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

  typedef std::map<std::string, Node *> NodeMap;
  
 class OPENTRACKER_API LiveContext : public Context, 
    public PortableServer::RefCountServantBase {
  private:
    CORBAModule* corba_module;
    int no_nodes;
    NodeMap references;

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

    OTGraph::Node_var activateNode(Node* node) {
      PortableServer::POA_var poa = corba_module->getPOA();
      CORBA::String_var string_id = CORBA::string_dup(node->get("ID").c_str());
      //PortableServer::ObjectId_var corba_id = PortableServer::string_to_ObjectId(CORBA::string_dup(string_id));
      PortableServer::ObjectId_var corba_id = PortableServer::string_to_ObjectId(string_id);
      poa->activate_object_with_id(corba_id, node);
      OTGraph::Node_var node_ref = OTGraph::Node::_narrow(poa->id_to_reference(corba_id));
      return node_ref;
    }

    void deactivateNode(Node* node) {
      string id(node->get("ID"));
      //PortableServer::ObjectId_var node_id = PortableServer::string_to_ObjectId(CORBA::string_dup(id.c_str()));
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
      
      OTGraph::Node_var node_ref = activateNode(value); 
      unlock();
      return node_ref;
    }

    RefNode* createRefNode(const std::string& id, const std::string& def, Node* referenced_node) {
      RefNode * ref = new RefNode( referenced_node );
      StringTable attributes;
      attributes.put("ID", id);
      attributes.put("USE", def);
      const std::string tagName("Ref");

      // add a correctly created DOM_Element to the node here and return
      DOMDocument * doc = ((DOMNode *)(rootNode->parent))->getOwnerDocument();
      std::auto_ptr<XMLCh> tempName ( XMLString::transcode( tagName.c_str()));
      std::auto_ptr<XMLCh> tempNS ( XMLString::transcode(rootNamespace.c_str()));
      DOMElement * el = doc->createElementNS( tempNS.get(), tempName.get());
      ref->setParent( el );
      // set attributes on the element node
      KeyIterator keys(attributes);
      while( keys.hasMoreKeys())
	{
	  const std::string & key = keys.nextElement();
	  ref->put( key, attributes.get( key ));
	  std::auto_ptr<XMLCh> attName ( XMLString::transcode( key.c_str()));
	  std::auto_ptr<XMLCh> attVal ( XMLString::transcode( attributes.get( key ).c_str()));
	  el->setAttributeNS(tempNS.get(), attName.get(), attVal.get());
	}
      ref->type = tagName;
      ref->name = id;
      return ref;
    }

    void connect_node(const OTGraph::Node_var& sendingNode, const OTGraph::Node_var& receivingNode) {
      lock();
      Node* sender   = getNodeFromRef(sendingNode);
      Node* receiver = getNodeFromRef(receivingNode);
      // Test to see whether parent of sender is rootNode
      if (sender->getParent() == rootNode) {
	// The sender node has no functional Node as parent
	// so remove from rootNode and add to the receivingNode
	rootNode->removeChild(*sender);
	receiver->addChild(*sender);
      } else {
	// The sender node already has a functional Node as parent
	// so add the receiving node as a reference
	// First set the DEF attribute of the sender to the same as its ID
	std::string sender_def(sender->get("ID"));
	sender->put("DEF", sender_def);
	// Manufacture a RefNode with appropriate ID and USE attributes
	RefNode * ref = createRefNode(generateNewId(), sender_def, sender );
	// Now set the ref RefNode to be a child of the receiver
	receiver->addChild(*ref);
      }
      unlock();
    }
    
    void add_node(const OTGraph::Node_var& parentNode, const OTGraph::Node_var& newNode) {
      PortableServer::POA_var poa = corba_module->getPOA();
      PortableServer::ServantBase* parent_servant = poa->reference_to_servant(OTGraph::Node::_duplicate(parentNode));
      PortableServer::ServantBase* new_servant = poa->reference_to_servant(OTGraph::Node::_duplicate(newNode));
      Node* parent  = dynamic_cast<Node*>(parent_servant);
      Node* newnode = dynamic_cast<Node*>(new_servant);
      Module * mod = NULL;
      if (newnode->type.compare("Ref") != 0) {
	mod  = getModuleFromNode(newnode);
      }
      if (parent != NULL){
	lock();
	parent->addChild(*newnode);
	if (mod != NULL) {
	  mod->addNode(newnode);
	}
	unlock();
      }
    }

    void remove_node(const OTGraph::Node_var& target_ref) {
      lock();
      Node* target  = getNodeFromRef(target_ref);
      // Handle referencing nodes
      NodeVector::iterator it = target->references.begin();
      for (NodeVector::iterator it = target->references.begin(); it != target->references.end(); it++) {
	Node* ref_parent = ((*it)->getParent());
	if (ref_parent != NULL) {
	  ref_parent->removeChild(*(*it));
	}
	delete (*it); 
      }
      target->references.clear(); // Now clear the references vector

      // Handle child nodes
      Node* child = target->getChild(0);
      if (child != NULL) {
	if ((child->getType()).compare("Ref") == 0) {
	  // Child node is a RefNode, and should NOT have any children
	  Node* referenced_node = ((RefNode*)child)->getReferenced();
	  referenced_node->removeReference(child);
	  target->removeChild(*child);
	  delete child;
	} else {
	  // remove the child from this node
	  target->removeChild(*child);
	  //  and add to rootNode
	  rootNode->addChild(*child);
	}
      }
      // Handle parent
      Node* parent = target->getParent();
      parent->removeChild(*target);

      // deactivate the object before passing the node to the module to be deleted
      deactivateNode(target);
      
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
      XMLCh tempStr[100];
      XMLString::transcode("LS", tempStr, 99);
      DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);
      DOMWriter* theSerializer = ((DOMImplementationLS*)impl)->createDOMWriter();
      
      if (theSerializer->canSetFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true))
	theSerializer->setFeature(XMLUni::fgDOMWRTDiscardDefaultContent, true);
      
      if (theSerializer->canSetFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true))
	theSerializer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
      
      try {
	XMLCh * xmlstring = theSerializer->writeToString(*((DOMNode *)(rootNode->parent)));
	char * tmpstring = XMLString::transcode( xmlstring );
	char * corba_string = CORBA::string_dup(tmpstring);
	theSerializer->release();
	XMLString::release( & xmlstring );
	XMLString::release( & tmpstring );
	return corba_string;
      }
      catch (const XMLException& toCatch) {
	char* message = XMLString::transcode(toCatch.getMessage());
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
      }
      catch (const DOMException& toCatch) {
	char* message = XMLString::transcode(toCatch.msg);
	cout << "Exception message is: \n"
	     << message << "\n";
	XMLString::release(&message);
      }
      catch (...) {
	cout << "Unexpected Exception \n" ;
      }
      theSerializer->release();
      
      return CORBA::string_dup("");
    }

  protected:
    ConfigNode* configNode;
  };
}
#endif //_LIVECONTEXT_H
