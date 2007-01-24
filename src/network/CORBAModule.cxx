/* ======================================================================== 
 * Copyright (C) 2006  Graz University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact 
 * Dieter Schmalstieg
 * <schmalstieg@icg.tu-graz.ac.at>
 * Graz University of Technology, 
 * Institut for Computer Graphics and Vision,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** source file for CORBAModule module.
  *
  * @author Joseph Newman
  *
  * $Id: CORBAModule.cxx 897 2005-12-03 20:23:38Z jfn $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef USE_CORBA

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/network/CORBAModule.h>
#include <OpenTracker/network/CORBASource.h>
#include <OpenTracker/network/CORBASink.h>
#include <OpenTracker/network/CORBATransform.h>

#include <OpenTracker/tool/OT_ACE_Log.h>
#include <OpenTracker/skeletons/OT_CORBA.hh>

#ifdef USE_OMNIEVENTS
#include <omniEvents/CosEventComm.hh>
#include <omniEvents/CosEventChannelAdmin.hh>
#include <OpenTracker/skeletons/OT_EventChannel.hh>
#include <OpenTracker/network/PushCons.h>
#include <OpenTracker/network/PushSupp.h>
#ifdef USE_SHARED
#include <OpenTracker/network/SharedEngineNode.h>
#endif //USE_SHARED
#endif //ENABLE_OMNIEVENTS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <ace/Log_Msg.h>

//using namespace std;

// Destructor method

namespace ot {

	OT_MODULE_REGISTER_FUNC(CORBAModule){
		OT_MODULE_REGISTRATION_DEFAULT(CORBAModule, "CORBAConfig" );
	}	


 PortableServer::POAManager_var CORBAModule::pman;
 CORBA::ORB_var CORBAModule::orb;
 PortableServer::POA_var CORBAModule::poa;
 PortableServer::POA_var CORBAModule::root_poa;
 bool CORBAModule::persistent;
 bool CORBAModule::initialised;




void CORBAModule::destroyORB()
{
  try {
    //delete myimpl;
    orb->destroy();
  }
  catch(CORBA::SystemException&) {
    logPrintE("Caught CORBA::SystemException when trying to destroy ORB.\n");
  }
  catch(CORBA::Exception&) {
    logPrintE("Caught CORBA::Exception when trying to destroy ORB");
  }
  catch(omniORB::fatalException& fe) {
    logPrintE("Caught omniORB::fatalException when trying to destroy ORB:");
    logPrintE("  file: %s", fe.file());
    logPrintE("  line: %s", fe.line());
    logPrintE("  mesg: %s", fe.errmsg());
  }
  catch(...) {
    logPrintE("Caught unknown exception while trying to destroy ORB.");
  }
}

// This method initialises the ORB and poa

void CORBAModule::initializeORB(int argc, char **argv)
{
  try {
    // initialize the ORB
    orb = CORBA::ORB_init(argc, argv);
    if (CORBA::is_nil(orb)) {
      logPrintE("Orb is nil. Exiting...\n");
    } else {
      logPrintI("Orb successfully initialised\n");
    }
    CORBAModule::initialised = true;
    
    // Initialise the POA.
    CORBA::Object_var objref = orb->resolve_initial_references("RootPOA");
    CORBAModule::root_poa = PortableServer::POA::_narrow(objref);
    pman = root_poa->the_POAManager();
    pman->activate();
    
    if (CORBAModule::persistent) {
      // Create a new POA with the persistent lifespan policy.
      CORBA::PolicyList pl;
      pl.length(2);
      pl[0] = CORBAModule::root_poa->create_lifespan_policy(PortableServer::PERSISTENT);
      pl[1] = CORBAModule::root_poa->create_id_assignment_policy(PortableServer::USER_ID);
      //pl[2] = CORBAModule::root_poa->create_implicit_activation_policy (PortableServer::NO_IMPLICIT_ACTIVATION);
      poa = CORBAModule::root_poa->create_POA("persistent poa", pman, pl);
      //poa = PortableServer::POA::_duplicate(CORBAModule::root_poa); // i.e. poa is the same as root_poa      
    } else {
      poa = PortableServer::POA::_duplicate(CORBAModule::root_poa); // i.e. poa is the same as root_poa
    }
  }
  catch(CORBA::SystemException&) {
    logPrintE("Caught CORBA::SystemException.");
  }
  catch(CORBA::Exception&) {
    logPrintE("Caught CORBA::Exception.");
  } 
  catch(omniORB::fatalException& fe) {
    logPrintE("Caught omniORB::fatalException:");
    logPrintE("  file: %s", fe.file());
    logPrintE("  line: %s", fe.line());
    logPrintE("  mesg: ", fe.errmsg());
  }
  catch(...) {
    logPrintE("Caught unknown exception.");
  }
}

// This method destroys the ORB

  CORBAModule::~CORBAModule()
  {
    
    //sinks.clear();
    clear();
    //if (initialised) {
    //  destroyORB();
    //}
  }
  
  void CORBAModule::init(StringTable& attributes,  ConfigNode * localTree)
  {
    logPrintI("CORBAModule is being initialised\n");
    Module::init(  attributes, localTree );
    if( attributes.containsKey("endPoint")) {
      cerr << "persistent" << endl;
      CORBAModule::persistent = true;
      // Spoof the command-line arguments
      std::string endpoint = attributes.get("endPoint"); //"giop:tcp:scumble.lce.cl.cam.ac.uk:9999";
      char *av[5]; int ac = 4;
      char arg1[] = "opentracker";
      char arg2[] = "-ORBendPoint";
      char arg3[endpoint.length()+1]; // note +1
      strcpy(arg3, endpoint.c_str());
      char arg4[] = "\0";
      char arg5 = (char) 0;
      av[0] = &arg1[0];
      av[1] = &arg2[0];
      av[2] = &arg3[0];
      av[3] = &arg4[0];
      av[4] = &arg5;
      initializeORB(ac, av);
    } else {
      cerr << "non-persistent" << endl;
      CORBAModule::persistent = false;
      // Spoof the command-line arguments
      char *av[3]; int ac = 2;
      char arg1[] = "opentracker";
      char arg2[] = "\0";
      char arg3 = (char) 0;
      av[0] = arg1;
      av[1] = arg2;
      av[2] = &arg3;
      initializeORB(ac, av);
    }
  }
    
  void CORBAModule::removeNode(const Node * node) {
    cerr << "CORBAModule deleting node " << node->get("ID");
    cerr << " of type " << node->getType() << endl;
    if ((node->getType().compare("CORBASink") == 0) || (node->getType().compare("CORBATransform") == 0)) {
      CORBASinkVector::iterator result = std::find( sinks.begin(), sinks.end(), node );
        if( result != sinks.end())
        {
          //delete *result;
	  sinks.erase( result );
	  return;
        } else {
	  logPrintE("Node with ID %s not in sinks CORBAModule sinks vector\n", node->get("ID").c_str());
	}
    } else if (node->getType().compare("CORBASource") == 0) {
      SourceNodeMap::iterator result = sources.find((CORBASource*) node);
      if (result != sources.end()) {
	OT_CORBA::OTSource_var corba_source_ref = result->second;
	// deactivate object
	PortableServer::ObjectId_var corba_source_id = getPOA()->reference_to_id(corba_source_ref);
	//delete result->first;
      } else {
	logPrintE("Node not present in SourceNodeMap");
      }
      return;
    } else if (node->getType().compare("PushSupp") == 0) {
      // TODO: handle removal of PushSupp nodes...
      PushSuppVector::iterator result =  std::find( pushsupps.begin(), pushsupps.end(), node );//pushsupps.find((PushSupp*) node);
      if (result != pushsupps.end()) {
	pushsupps.erase( result );
	//delete *result;
	return;
      }
    } else if (node->getType().compare("SharedEngineNode") == 0) {
      // TODO: handle removal of PushSupp nodes...
      SharedEngineNodeVector::iterator result =  std::find( sharedengines.begin(), sharedengines.end(), node );//pushsupps.find((PushSupp*) node);
      if (result != sharedengines.end()) {
	sharedengines.erase( result );
	//delete *result;
	return;
      }
    } else if (node->getType().compare("PushCons") == 0) {
      // TODO: handle removal of PushCons nodes...
      PushConsVector::iterator result = std::find( pushconsumers.begin(), pushconsumers.end(), node );//pushconsumers.find((PushCons*) node);
      if (result != pushconsumers.end()) {
	// disconnect the PushCons node
	CORBAUtils::disconnectPushConsumer(proxy_pushsupplier_map[*result]);
	proxy_pushsupplier_map.erase(*result);
	pushconsumers.erase(result);
      	//delete *result;
      }
    } 
  }

// This method is called to remove all nodes

void CORBAModule::clear()
{
  // for( CORBASinkVector::iterator it = sinks.begin(); it != sinks.end(); it++ )
//     {
//       delete *it;
//     }
  sinks.clear();
  // for (SourceNodeMap::iterator source_it=sources.begin(); source_it!=sources.end(); ++source_it) 
//     {
//       OT_CORBA::OTSource_var corba_source_ref = source_it->second;
//       // deactivate object
//       PortableServer::ObjectId_var corba_source_id = getPOA()->reference_to_id(corba_source_ref);
      
      
//       delete source_it->first;
//       //have to sort out reference counting - it may be sufficient to delete the servant
//       //delete source_it->second; //have to sort out reference counting
//     }
  sources.clear();
#ifdef USE_OMNIEVENTS
//   for( PushSuppVector::iterator it = pushsupps.begin(); it != pushsupps.end(); it++ )
//     {
//       delete *it;
//     }
  pushsupps.clear();
  for( PushConsVector::iterator it = pushconsumers.begin(); it != pushconsumers.end(); it++ )
    {
      // disconnect the PushCons node
      CORBAUtils::disconnectPushConsumer(proxy_pushsupplier_map[*it]);
      // have to sort out the reference counting wrt to the proxy_pushsupplier_map
      //delete *it;
    }
  pushconsumers.clear();
  proxy_pushsupplier_map.clear();
  
#endif //USE_OMNIEVENTS
}

// This method is called to construct a new Node.
Node * CORBAModule::createNode( const std::string& name, StringTable& attributes)
{
  if( name.compare("CORBASink") == 0 ) 
    {
      int frequency;
      int num = sscanf(attributes.get("frequency").c_str(), " %i", &frequency );
      if( num <= 0 ) {
	frequency = 1;
      } else {
	frequency = num;
      }
      CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup((const char*) attributes.get("name").c_str());
      CORBA::Object_var obj = CORBAUtils::getObjectReference(orb, string_name);
      if (CORBA::is_nil(obj)) {
	logPrintI("Could not obtain a reference to object supposedly bound to %s.\nExiting....\n", (const char*) string_name);
	exit(-1);
      }
      OT_CORBA::Node_var sink_ref = OT_CORBA::Node::_narrow(obj);
      
      CORBASink * sink = new CORBASink( sink_ref , frequency );
      sinks.push_back( sink );
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:Build CORBASink node\n")));
      return sink;
    } 
  else if( name.compare("CORBATransform") == 0 ) 
    {
      CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup((const char*) attributes.get("name").c_str());
      CORBA::Object_var obj = CORBAUtils::getObjectReference(orb, string_name);
      if (CORBA::is_nil(obj)) {
	logPrintI("Could not obtain a reference to object supposedly bound to %s.\nExiting....\n", (const char*) string_name);
	exit(-1);
      }
      OT_CORBA::TransformNode_var sink_ref = OT_CORBA::TransformNode::_narrow(obj);
      
      CORBATransform * transform = new CORBATransform( sink_ref );
      sinks.push_back( transform );
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:Build CORBATransform node\n")));
      return transform;
    } else if (name.compare("CORBASource") == 0 ) {
      CosNaming::NamingContextExt::StringName_var name = CORBA::string_dup((const char*) attributes.get("name").c_str());
      CORBASource * source_impl = new CORBASource( );
      
      POA_OT_CORBA::OTSource_tie<CORBASource>* corba_source = new POA_OT_CORBA::OTSource_tie<CORBASource>(source_impl);
      PortableServer::ObjectId_var corba_source_id;
      if (CORBAModule::persistent)
	{
	  corba_source_id = CORBAUtils::getObjectId(orb, name);
	  poa->activate_object_with_id(corba_source_id, corba_source);
	} 
      else {
	corba_source_id = poa->activate_object(corba_source);
      }
      OT_CORBA::OTSource_var corba_source_ref = corba_source->_this();
      CORBA::Object_var obj = CORBA::Object::_narrow(corba_source_ref);
      CORBAUtils::bindObjectReferenceToName(orb, obj, name);
      
      sources[source_impl] = corba_source_ref;
      
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:Build CORBASource node\n")));
      return source_impl;
    }
#ifdef USE_OMNIEVENTS
  else if (name.compare("PushCons") == 0 ) 
    {
      CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup((const char*) attributes.get("name").c_str());
      CORBA::Object_var obj = CORBAUtils::getObjectReference(orb, string_name);
      if (CORBA::is_nil(obj)) {
	logPrintI("Could not obtain a reference to event channel supposedly bound to %s.\nExiting....\n", (const char*) string_name);
	exit(-1);
      }
      CosEventChannelAdmin::EventChannel_var channel;
      channel = CosEventChannelAdmin::EventChannel::_narrow(obj);

      PushCons * pushcons_impl = new PushCons( );
      if (pushcons_impl == NULL) {
	logPrintE("pushcons_impl is NULL exiting...\n");
	exit(-1);
      } else {
	logPrintE("pushcons_impl is not NULL. Carrying on...\n");
      }
      
      POA_OT_EventChannel::PushConsNode_tie<PushCons>* pushcons_source = new POA_OT_EventChannel::PushConsNode_tie<PushCons> (pushcons_impl);
      
      // get Consumer Admin
      CosEventChannelAdmin::ConsumerAdmin_var consumer_admin = CORBAUtils::getConsumerAdmin(channel);

      // get Proxy Supplier
      CosEventChannelAdmin::ProxyPushSupplier_var proxy_supplier = CORBAUtils::getProxyPushSupplier(consumer_admin);

      PortableServer::ObjectId_var corba_source_id = 
	poa->activate_object(pushcons_source);
      OT_EventChannel::PushConsNode_var pushcons_ref = 
	pushcons_source->_this();
      CosEventComm::PushConsumer_var consumer_ref = 
	CosEventComm::PushConsumer::_narrow(pushcons_ref);
      if (CORBA::is_nil(consumer_ref)) {
	logPrintE("Could not narrow down to CosEventComm::PushConsumer\n");
	exit(-1);
      }
      CORBAUtils::connectPushConsumer(proxy_supplier, consumer_ref);
      pushconsumers.push_back( pushcons_impl );
      //CORBAUtils::connectPushConsumer(proxy_supplier, pushcons_ref);
      proxy_pushsupplier_map[pushcons_impl] = proxy_supplier;
			     
      return pushcons_impl;
    }
  else if (name.compare("PushSupp") == 0 ) 
    {
      CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup((const char*) attributes.get("name").c_str());
      CORBA::Object_var obj = CORBAUtils::getObjectReference(orb, string_name);
      if (CORBA::is_nil(obj)) {
	logPrintI("Could not obtain a reference to event channel supposedly bound to %s.\nExiting....\n", (const char*) string_name);
	exit(-1);
      }
      CosEventChannelAdmin::EventChannel_var channel;
      channel = CosEventChannelAdmin::EventChannel::_narrow(obj);      
      PushSupp * pushsupp = new PushSupp( channel );
      pushsupps.push_back( pushsupp );
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:Build PushSupp node\n")));
      return pushsupp;
    }   
#ifdef USE_SHARED
  else if (name.compare("SharedEngineNode") == 0 ) 
    {
      CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup((const char*) attributes.get("name").c_str());
      CORBA::Object_var obj = CORBAUtils::getObjectReference(orb, string_name);
      if (CORBA::is_nil(obj)) {
	logPrintI("Could not obtain a reference to event channel supposedly bound to %s.\nExiting....\n", (const char*) string_name);
	exit(-1);
      }
      CosEventChannelAdmin::EventChannel_var channel;
      channel = CosEventChannelAdmin::EventChannel::_narrow(obj);      
      SharedEngineNode * shared_engine = new SharedEngineNode( channel );
      sharedengines.push_back( shared_engine );
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("ot:Build SharedEngineNode node\n")));
      return shared_engine;
    }
#endif //USE_SHARED
#endif
  return NULL;
}
// pushes events into the tracker tree.

  void CORBAModule::pushEvent()
  {
    for (SourceNodeMap::const_iterator source_it=sources.begin(); source_it!=sources.end(); ++source_it) 
      {
	try {
	  source_it->second->push();
	} catch (CORBA::TRANSIENT) {
	  logPrintE("Caught CORBA::TRANSIENT");
	}
      }
#ifdef USE_OMNIEVENTS
  for( PushConsVector::iterator it = pushconsumers.begin(); it != pushconsumers.end(); it++ )
    {
      (*it)->push();
    }
#endif //USE_OMNIEVENTS
  }
  
  void CORBASource::push()
  {
    lock();
    if (modified) {
      updateObservers( event );
      modified = false;
    }
    unlock();
  }
  
  void CORBASource::setEvent(const OT_CORBA::Event& corba_event) 
  {
    lock();
    modified = true;
    Event new_event(corba_event);
    event = new_event;
    unlock();
  }

#ifdef USE_OMNIEVENTS
  void PushCons::disconnect_push_consumer () 
  {
    logPrintI("Push Consumer: disconnected.\n");
  }

  void PushCons::push(const CORBA::Any& data) 
  {
    //logPrintI("data received\n");
    const OT_CORBA::Event* new_event;
    if (data >>= new_event) {
      lock();
      modified = true;
      OT_CORBA::Event copy_of_new_event = OT_CORBA::Event(*new_event);
      //CORBAUtils::convertFromCORBAEvent(event, copy_of_new_event);
      event = Event(copy_of_new_event);
      unlock();
    }
  }

  void PushCons::push()
  {
    lock();
    if (modified) {
      updateObservers( event );
      modified = false;
    }
    unlock();
  }
  
#endif //USE_OMNIEVENTS

} //namespace ot
#endif //USE_CORBA
