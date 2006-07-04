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
#include "../tool/disable4786.h"
#include "CORBAModule.h"
#include "CORBASource.h"
#include "CORBASink.h"

#include "../tool/OT_ACE_Log.h"
#include <OT_CORBA.hh>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <ace/Log_Msg.h>

//using namespace std;

// Destructor method

namespace ot {


void CORBAModule::destroyORB()
{
  try {
    //delete myimpl;
    orb->destroy();
  }
  catch(CORBA::SystemException&) {
    cerr << "Caught CORBA::SystemException." << endl;
  }
  catch(CORBA::Exception&) {
    cerr << "Caught CORBA::Exception." << endl;
  }
  catch(omniORB::fatalException& fe) {
    cerr << "Caught omniORB::fatalException:" << endl;
    cerr << "  file: " << fe.file() << endl;
    cerr << "  line: " << fe.line() << endl;
    cerr << "  mesg: " << fe.errmsg() << endl;
  }
  catch(...) {
    cerr << "Caught unknown exception." << endl;
  }
}

// This method initialises the ORB and poa

void CORBAModule::initializeORB(int argc, char **argv)
{
  try {
    // initialize the ORB
    orb = CORBA::ORB_init(argc, argv);
    
    // Initialise the POA.
    objref = orb->resolve_initial_references("RootPOA");
    root_poa = PortableServer::POA::_narrow(objref);
    pman = root_poa->the_POAManager();
    pman->activate();
    
    if (persistent) {
      // Create a new POA with the persistent lifespan policy.
      CORBA::PolicyList pl;
      pl.length(2);
      pl[0] = root_poa->create_lifespan_policy(PortableServer::PERSISTENT);
      pl[1] = root_poa->create_id_assignment_policy(PortableServer::USER_ID);
      
      poa = root_poa->create_POA("persistent poa", pman, pl);
    } else {
      poa = PortableServer::POA::_duplicate(root_poa); // i.e. poa is the same as root_poa
	}
  }
  catch(CORBA::SystemException&) {

    cerr << "Caught CORBA::SystemException." << endl;
  }
  catch(CORBA::Exception&) {
    cerr << "Caught CORBA::Exception." << endl;
  }
  catch(omniORB::fatalException& fe) {
    cerr << "Caught omniORB::fatalException:" << endl;
    cerr << "  file: " << fe.file() << endl;
    cerr << "  line: " << fe.line() << endl;
    cerr << "  mesg: " << fe.errmsg() << endl;
  }
  catch(...) {
    cerr << "Caught unknown exception." << endl;
  }
}

// This method destroys the ORB

  CORBAModule::~CORBAModule()
  {
    sinks.clear();
    destroyORB();
  }
  
  void CORBAModule::init(StringTable& attributes,  ConfigNode * localTree)
  {
    Module::init(  attributes, localTree );
    if( attributes.containsKey("endPoint")) {
      persistent = true;
      // Spoof the command-line arguments
      std::string endpoint = attributes.get("endPoint"); //"giop:tcp:scumble.lce.cl.cam.ac.uk:9999";
      char *av[4]; int ac = 4;
      char arg1[] = "opentracker";
      char arg2[] = "-ORBendPoint";
      char* arg3 = new char[endpoint.length()+1]; // note +1
      endpoint.copy(arg3, std::string::npos);
      char arg4[] = "\0";
      char arg5 = (char) 0;
      av[0] = arg1;
      av[1] = arg2;
      av[2] = arg3;
      av[3] = arg4;
      av[4] = &arg5;
      initializeORB(ac, av);
      delete arg3;
    } else {
      persistent = false;
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
    

// This method is called to remove all nodes

void CORBAModule::clear()
{
  for( CORBASinkVector::iterator it = sinks.begin(); it != sinks.end(); it++ )
    {
      delete *it;
    }
  sinks.clear();
}

// This method is called to construct a new Node.

Node * CORBAModule::createNode( const std::string& name, StringTable& attributes)
{
  if( name.compare("CORBASink") == 0 ) 
    {
      int frequency;
      int num = sscanf(attributes.get("frequency").c_str(), " %i", &frequency );
      if( num == 0 ){
	frequency = 1;
      }
      CosNaming::NamingContextExt::StringName_var string_name = CORBA::string_dup((const char*) attributes.get("name").c_str());
      CORBA::Object_var obj = CORBAUtils::getObjectReference(orb, string_name);
      if (CORBA::is_nil(obj)) {
	LOG_ACE_INFO("Could not obtain a reference to object supposedly bound to %s.\nExiting....\n", (const char*) string_name);
	exit(-1);
      }
      //OT_CORBA::Sink_var sink_ref = OT_CORBA::Sink::_narrow(obj);
      OT_CORBA::Node_var sink_ref = OT_CORBA::Node::_narrow(obj);
      
      CORBASink * sink = new CORBASink( sink_ref , frequency );
      sinks.push_back( sink );
      return sink;
    } 
  else if (name.compare("CORBASource") == 0 ) 
    {
      CosNaming::NamingContextExt::StringName_var name = CORBA::string_dup((const char*) attributes.get("name").c_str());
      CORBASource * source_impl = new CORBASource( );
      
      POA_OT_CORBA::OTSource_tie<CORBASource>* corba_source = new POA_OT_CORBA::OTSource_tie<CORBASource>(source_impl);
      PortableServer::ObjectId_var corba_source_id;
      if (persistent)
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
	  cerr << "Caught CORBA::TRANSIENT" << endl;
	}
      }
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
  
  void CORBASource::setEvent(const OT_CORBA::Event& new_event) 
  {
    lock();
    modified = true;
    CORBAUtils::convertFromCORBAEvent(event, new_event); 
    unlock();
  }
  
} //namespace ot
#endif //USE_CORBA
