#include <OpenTracker/network/PhantomListener.h>

#ifndef _PHANTOMZONELISTENER_H
#define _PHANTOMZONELISTENER_H


namespace ot {

 class OPENTRACKER_API PhantomZoneListener : public PhantomListener
    {
  public:
      PhantomZoneListener (const std::string& group) : PhantomListener(group) { 
	zid_node_mapping = new ZidSourceMultiMap;
	logPrintI("PhantomZoneListener constructor\n");
    };

    virtual ~PhantomZoneListener () {
      delete zid_node_mapping;
      logPrintI("PhantomZoneListener destructor\n");
    };

    
    void addNode(PhantomMiddlewareSource* node) {
      PhantomZoneSource* zone_source = dynamic_cast<PhantomZoneSource*>(node);
      lock();
      ZidSourcePair pairing(zone_source->getZoneId(), zone_source);
      zid_node_mapping->insert(pairing);
      unlock();
    };

    void removeNode(PhantomMiddlewareSource* node) {
/*       int pid; */
/*       sscanf(node->get("pid").c_str(), " %i", &pid ); */
      lock();
/*       // Get the range of iterators which have the key "pid" */
/*       std::pair<PidSourceMultiMapIterator, PidSourceMultiMapIterator> range_it = pid_node_mapping->equal_range(pid); */
/*       PidSourceMultiMapIterator it = range_it.first; */
/*       while (it != range_it.second) { */
/* 	PidSourceMultiMapIterator _it = it; */
/* 	it++; */
/* 	if (_it->second == node) { */
/* 	  pid_node_mapping->erase(_it); */
/* 	  delete dynamic_cast<PhantomLocationSource*>(node); */
/* 	} */
/*       } */
      unlock();
    }

    void pushEvent() {
      lock();
      for (ZidSourceMultiMap::const_iterator zidsource_it = (*zid_node_mapping).begin(); zidsource_it != (*zid_node_mapping).end(); ++zidsource_it )
	{
	  zidsource_it->second->push();
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
	
	if (ver == 1) {
	  int pid, spacepid;
	  char spacename[128];
	  char type[128];
	  int occ;
	  try {
	    ucm >> spacepid >> pid >> spacename >> occ >> type;
	    std::cerr << spacepid << ", " << pid << ", " << spacename;
	    std::cerr << ", " << occ << ", " << type << std::endl;
	    lock();
	    std::pair<ZidSourceMultiMapIterator, ZidSourceMultiMapIterator> range_it = zid_node_mapping->equal_range(spacepid);
	    for (ZidSourceMultiMapIterator it = range_it.first; it != range_it.second; ++it) {
	      std::cerr << "setEvent of PhantomZoneSource" << std::endl;
	      (it->second)->setEvent(eid, t1, t2, pid, spacename, type);
	    }
	    unlock();

	  } catch (PhantomException) {
	    std::cerr << "Caught Phantom Exception: probably attempting to extract from empty buffer" << std::endl;
	  }

/* 	char source[32]; */
/* 	std::string src; */
/* 	if (eid != 13 && eid !=14) { */
/* 	  try { */
/* 	    ucm >> pid >> x >> y >> z; */
/* 	    ucm >> theta; */
/* 	    if (eid == 24) { */
/* 	      ucm >> source; */
/* 	    } */
/* 	    src = source; */
/* 	    cerr << "pid is " << pid << endl; */
/* 	    lock(); */
/* 	    std::pair<PidSourceMultiMapIterator, PidSourceMultiMapIterator> range_it = pid_node_mapping->equal_range(pid); */
/* 	    for (PidSourceMultiMapIterator it = range_it.first; it != range_it.second; ++it) { */
/* 	      if (eid == (it->second)->getEventId()) { */
/* 		(it->second)->setEvent(x, y, z, theta, t1, t2, eid, source); */
/* 	      } */
/* 	    } */
/* 	    unlock(); */
/* 	  } catch (PhantomException) { */
/* 	    std::cerr << "Caught Phantom Exception: probably attempting to extract from empty buffer" << std::endl; */
/* 	  } */
 	} 
      }
      return 0;
      };
    
    protected:

 private:
    ZidSourceMultiMap* zid_node_mapping;
  };
}

#endif // _PHANTOMZONELISTENER_H
