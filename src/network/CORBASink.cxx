#include <OpenTracker/network/CORBASink.h>
#include <OpenTracker/network/CORBAModule.h>
#include <OpenTracker/skeletons/OT_CORBA.hh>

namespace ot {

  char* CORBASink::get_attribute(const char* _key) {
    ACE_Guard<ACE_Thread_Mutex> mutexlock(*mutex);
    if (strcmp(_key, "uri") == 0) {
      return CORBAModule::getORB()->object_to_string(corba_sink);
    } else {	
      throw OTGraph::UnsupportedAttribute();
    }
  }

  void CORBASink::set_attribute(const char* _key, const char* _value) {
      if (strcmp(_key, "uri") == 0) {
	CORBA::Object_var obj = CORBAModule::getORB()->string_to_object(_key);
	if (CORBA::is_nil(obj)) {
	    throw OTGraph::InvalidAttribute();	    
	} else {
	  ACE_Guard<ACE_Thread_Mutex> mutexlock(*mutex);
	  corba_sink = OT_CORBA::OTEntity::_narrow(obj);
	}
      } else {
	throw OTGraph::UnsupportedAttribute();
      }
    }

} // namespace ot

