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
/** source file for PhantomMiddleware module.
  *
  * @author Joseph Newman
  *
  * $Id: PhantomMiddleware.cxx 897 2005-12-03 20:23:38Z jfn $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef USE_PHANTOMMIDDLEWARE

// this will remove the warning 4786
#include <OpenTracker/tool/disable4786.h>
#include <OpenTracker/network/PhantomMiddlewareModule.h>
#include <OpenTracker/network/PhantomMiddlewareSource.h>
#include <phantom/EventIdQuery.hh>
#include <phantom/Exceptions.hh>

#include <OpenTracker/tool/OT_ACE_Log.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include <ace/Log_Msg.h>
#include <ace/Thread.h>

//using namespace std;

// Destructor method

namespace ot {

  OT_MODULE_REGISTER_FUNC(PhantomMiddlewareModule) {
    OT_MODULE_REGISTRATION_DEFAULT(PhantomMiddlewareModule, "PhantomMiddlewareConfig" );
  }	
  
  // destructor cleans up any allocated memory
  PhantomMiddlewareModule::~PhantomMiddlewareModule()
  {
    LOG_ACE_INFO("PhantomMiddlewareModule destructor\n");
  }

  void PhantomMiddlewareModule::init(StringTable& attributes, ConfigNode * localTree)
  {    
    LOG_ACE_INFO("PhantomMiddlewareModule::init\n");
  } // init

  Node * PhantomMiddlewareModule::createNode( const std::string & name, StringTable & attributes )
  {
    LOG_ACE_INFO("PhantomMiddlewareModule::createNode\n");
    if( name.compare("PhantomMiddlewareSink") == 0 ) 
      {
	int frequency;
	int num = sscanf(attributes.get("frequency").c_str(), " %i", &frequency );
	if( num == 0 || frequency == 0){
	  frequency = 1;
	} 
	const char* group = attributes.get("group").c_str();
	int pid;
	num = sscanf(attributes.get("pid").c_str(), " %i", &pid );
	short eid  =  (short) atoi(attributes.get("eid").c_str());
	std::cerr << "eid extracted from StringTable:" << attributes.get("eid") << std::endl;
	//std::string id = attributes.get("id");
	//Phantom::Database::EventIdQuery eidq(id);
	//eid = eidq.GetId();
	std::string source = attributes.get("source");//.c_str();
	PhantomMiddlewareSink* sink;
	if (source == "") {
	  LOG_ACE_INFO("new PhantomMiddlewareSink( %s, %d, %d, %d )", group, frequency, pid, (int) eid );
	  sink = new PhantomMiddlewareSink( group, frequency, pid, eid );
	} else {
	  LOG_ACE_INFO("new PhantomMiddlewareSink( %s, %d, %d, %d, %s )", group, frequency, pid, (int) eid, source );
	  sink = new PhantomMiddlewareSink( group, frequency, pid, eid, source );
	}
	num = sscanf(attributes.get("eid").c_str(), " %h", &eid );
	sinks.push_back( sink );
	return sink;
      } else if ( name.compare("PhantomMiddlewareSource") == 0 ) {
	//const char* group = attributes.get("group").c_str();
	std::string group = attributes.get("group");
	int pid;
	sscanf(attributes.get("pid").c_str(), " %i", &pid );
	short eid  =  (short) atoi(attributes.get("eid").c_str());
	std::string source_description = attributes.get("source");
	PhantomMiddlewareSource* source;
	if  (source_description == "") {
	  std::cerr << "PhantomMiddlewareSource: " << pid << ", " << eid << ", " << source_description << std::endl;
	  source = new PhantomMiddlewareSource( group.c_str(), pid, eid );
	} else {
	  std::cerr << "PhantomMiddlewareSource with source: " << pid << ", " << eid << ", " << source_description << std::endl;
	  source = new PhantomMiddlewareSource( group.c_str(), pid, eid, source_description);
	}
	GroupMapping::iterator g = groups.find(group);
	if (g == groups.end()) {
	  PidSourceMultiMap* multi_mapping = new PidSourceMultiMap;
	  groups[group] = multi_mapping;
	  multi_mapping->insert(PidSourcePair(pid, source));
	} else {
	  g->second->insert(PidSourcePair(pid, source));
	}
	LOG_ACE_INFO("Returning PhantomMiddlewareSource*\n");
	return source;
      } 
    return NULL;
  };

  void PhantomMiddlewareModule::runPhantomMessageListener( void * data )
  {
    GroupMappingPair* group_map_pairing = (GroupMappingPair *) data;
    const char* group = group_map_pairing->first.c_str();
    Phantom::Utils::MulticastSocketReceiver msr(group);
    short locpreeid;
    while (1) {
      Phantom::Utils::UCharMessageReader ucm;
      msr >> ucm;
      
      unsigned char ver;
      short eid;
      unsigned char seq;
      long t1, t2;
      int discard;

      ucm >> ver >> eid >> seq >> t1 >> t2 >> discard;
      //      if (eid==6) {
	int pid;
	float x,y,z, theta;
	char source[32];
	std::string src;
	if (eid != 13 && eid !=14) {
	  try {
	    ucm >> pid >> x >> y >> z;
	    ucm >> theta;
	    if (eid == 24) {
	      ucm >> source;
	    }
	    src = source;
	    PidSourceMultiMap* multi_map = group_map_pairing->second;
	    std::pair<PidSourceMultiMapIterator, PidSourceMultiMapIterator> range_it = multi_map->equal_range(pid);
	    for (PidSourceMultiMapIterator it = range_it.first; it != range_it.second; ++it) {
	      if (eid == (it->second)->getEventId()) {
		(it->second)->setEvent(x, y, z, theta, t1, t2, eid, source);
	      }
	    }
	  } catch (PhantomException) {
	    std::cerr << "Caught Phantom Exception: probably attempting to extract from empty buffer" << std::endl;
	  }
	}
	//      }
    }
  }

  
  void PhantomMiddlewareModule::start()
  {
    for(GroupMapping::const_iterator group_it = groups.begin(); group_it != groups.end(); ++group_it )
      {
	ACE_Thread::spawn((ACE_THR_FUNC)PhantomMiddlewareModule::runPhantomMessageListener, (void *) (GroupMappingPair *) &(*group_it));
      }

  }
  
  void PhantomMiddlewareModule::close()
  {
    LOG_ACE_INFO("PhantomMiddlewareModule::close()\n");
  }

  void PhantomMiddlewareModule::clear()
  {
    for( PhantomMiddlewareSinkVector::iterator it = sinks.begin(); it != sinks.end(); it++ )
      {
	delete *it;
      }
    sinks.clear();
    // TODO!!!!!!!! Clean up code for sources
  }
  

  void PhantomMiddlewareModule::pushEvent()
  {        
    for(GroupMapping::const_iterator group_it = groups.begin(); group_it != groups.end(); ++group_it )
      {
	PidSourceMultiMap* multi_map = group_it->second;
	for (PidSourceMultiMap::iterator pidsource_it = (*multi_map).begin(); pidsource_it != (*multi_map).end(); ++pidsource_it )
	  {
	    pidsource_it->second->push();
	  }
      }
  }
  
} //namespace ot
#endif //USE_PHANTOMMIDDLEWARE
