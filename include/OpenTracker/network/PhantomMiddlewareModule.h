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
/** header file for PhantomMiddleware module.
  *
  * @author Gerhard Reitmayr
  *
  * $Id: PhantomMiddlewareModule.h 760 2004-11-14 18:20:34Z daniel $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section testmodule PhantomMiddlewareModule
 * The PhantomMiddlewareModule handles receiving and sending
 * Phantom Events
 */

#ifndef _PHANTOMMIDDLEWAREMODULE_H
#define _PHANTOMMIDDLEWAREMODULE_H

#ifdef USE_PHANTOMMIDDLEWARE

#include "../dllinclude.h"

#include <vector>

#include <phantom/Exceptions.hh>

#include <OpenTracker/OpenTracker.h>
#include <OpenTracker/network/PhantomMiddlewareSink.h>
#include <OpenTracker/network/PhantomMiddlewareSource.h>
#include <ace/Task.h>

namespace ot {

typedef std::vector<PhantomMiddlewareSink*> PhantomMiddlewareSinkVector;
typedef std::multimap<int, PhantomMiddlewareSource*> PidSourceMultiMap;
typedef PidSourceMultiMap::iterator PidSourceMultiMapIterator;
typedef std::map<std::string, PidSourceMultiMap*> GroupMapping;
typedef std::pair<std::string, PidSourceMultiMap*> GroupMappingPair;
typedef std::pair<int, PhantomMiddlewareSource*> PidSourcePair;

 class OPENTRACKER_API PhantomListener : public ACE_Task_Base
    {
  public:
    PhantomListener (const std::string& group) : _stop(false) { 
      pid_node_mapping = new PidSourceMultiMap;
      msr = new Phantom::Utils::MulticastSocketReceiver(group.c_str());
    };

    virtual ~PhantomListener () {
      delete pid_node_mapping;
      delete msr;
    };

    void Stop() {
      lock();
      _stop = true;
      unlock();
    };

    void Start() {
      lock();
      _stop = false;
      unlock();
    };

    bool isStopping() {
      lock();
      bool stopping = _stop;
      unlock();
      return stopping;
    };
    
    void addNode(PhantomMiddlewareSource* node, int pid) {
      cerr << "adding Node with pid " << pid << endl;
      lock();
      pid_node_mapping->insert(PidSourcePair(pid, node));
      unlock();
    };

    void removeNode(PhantomMiddlewareSource* node) {
      int pid;
      sscanf(node->get("pid").c_str(), " %i", &pid );
      lock();
      // Get the range of iterators which have the key "pid"
      std::pair<PidSourceMultiMapIterator, PidSourceMultiMapIterator> range_it = pid_node_mapping->equal_range(pid);
      PidSourceMultiMapIterator it = range_it.first;
      while (it != range_it.second) {
	PidSourceMultiMapIterator _it = it;
	it++;
	if (_it->second == node) {
	  pid_node_mapping->erase(_it);
	  delete node;
	}
      }
/*       for (PidSourceMultiMapIterator it = range_it.first; it != range_it.second; ++it) { */
/* 	if (it->second == node) { */
/* 	  pid_node_mapping->erase(it); */
/* 	  delete node; */
/* 	} */
/*       } */
      unlock();
    }

    void pushEvent() {
      lock();
      for (PidSourceMultiMap::const_iterator pidsource_it = (*pid_node_mapping).begin(); pidsource_it != (*pid_node_mapping).end(); ++pidsource_it )
	{
	  pidsource_it->second->push();
	}
      unlock();
    }

    virtual int svc (void)
    {
      while (isStopping() == false) {
	Phantom::Utils::UCharMessageReader ucm;
	(*msr) >> ucm;
	
	unsigned char ver;
	short eid;
	unsigned char seq;
	long t1, t2;
	int discard;
	
	ucm >> ver >> eid >> seq >> t1 >> t2 >> discard;
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
	    lock();
	    std::pair<PidSourceMultiMapIterator, PidSourceMultiMapIterator> range_it = pid_node_mapping->equal_range(pid);
	    for (PidSourceMultiMapIterator it = range_it.first; it != range_it.second; ++it) {
	      if (eid == (it->second)->getEventId()) {
		(it->second)->setEvent(x, y, z, theta, t1, t2, eid, source);
	      }
	    }
	    unlock();
	  } catch (PhantomException) {
	    std::cerr << "Caught Phantom Exception: probably attempting to extract from empty buffer" << std::endl;
	  }
	}
      }
      return 0;
    };
    
 protected:
    inline void lock() {lock_.acquire();};
    inline void unlock() {lock_.release();};
    bool _stop;


 private:
    PidSourceMultiMap* pid_node_mapping;
    Phantom::Utils::MulticastSocketReceiver* msr;
  };

typedef std::map<std::string, PhantomListener*> GroupListenerMap;

/**
 * The module and factory to drive the test source nodes. It constructs
 * PhantomMiddlewareSource and PhantomMiddlewareSink nodes via the 
 * NodeFactory interface and pushes events into the tracker tree according 
 * to the nodes configuration.
 * @author Joseph Newman
 * @ingroup network
 */
class OPENTRACKER_API PhantomMiddlewareModule : public Module, public NodeFactory
{
// Members
protected:
    /// list of PhantomMiddlewareModule nodes in the tree
    PhantomMiddlewareSinkVector sinks;
    //GroupMapping                groups;
    GroupListenerMap            listeners;
// Methods

  public:
    /** constructor method. */
 PhantomMiddlewareModule() : Module(), NodeFactory()
    {
      // Empty constructor
    };
    /** Destructor method, clears nodes member. */
    virtual ~PhantomMiddlewareModule();

    virtual void init(StringTable& attributes,  ConfigNode * localTree);

    /** Clears nodes */
    //virtual void clear();
    /** This method is called to construct a new Node. It compares
     * name to the MessageReceiver and MessageSender element name, 
     * and if it matches
     * creates a new TestSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);

    virtual void clear();
      /**
       * closes the module and closes any communication and stops thread.
       */
    virtual void close();
    
    /**
     * starts the receiving threads. It is called after initialisation is done.*/
    virtual void start();

    /**
     * pushes events into the tracker tree. Checks all TestSources and
     * pushes new events, if a TestSource fires.
     */
    virtual void pushEvent();

    //static void runPhantomMessageListener( void * data );
    
    virtual void removeNode(Node *);
 private:
};

OT_MODULE(PhantomMiddlewareModule);

} // namespace ot

#endif //USE_PHANTOMMIDDLEWARE

#endif
