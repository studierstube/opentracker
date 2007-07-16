#include <OpenTracker/network/CORBASink.h>
#include <OpenTracker/network/CORBAModule.h>
#include <OpenTracker/skeletons/OT_CORBA.hh>

namespace ot {

  char* CORBASink::get_attribute(const char* _key) {
    if (strcmp(_key, "uri") == 0) {
      ACE_Guard<ACE_Thread_Mutex> mutexlock(*refmutex);
      CORBA::ORB_var orb = CORBAModule::getORB();
      //CORBA::String_var uri = orb->object_to_string(OT_CORBA::OTEntity::_duplicate(corba_sink));
      //OT_CORBA::OTEntity_var entity = OT_CORBA::OTEntity::_duplicate(corba_sink);
      //OT_CORBA::OTEntity_var entity = corba_sink;
      char* uri = orb->object_to_string(entity);
      return uri;
    } else {	
      throw OTGraph::UnsupportedAttribute();
    }
  }

  void CORBASink::set_attribute(const char* _key, const char* _value) {
      if (strcmp(_key, "uri") == 0) {
	CORBA::Object_var obj = CORBAModule::getORB()->string_to_object(_value);
	OT_CORBA::OTEntity_var entity_ = OT_CORBA::OTEntity::_narrow(obj);
	if (CORBA::is_nil(entity)) {
	    throw OTGraph::InvalidAttribute();	    
	} else {
	  ACE_Guard<ACE_Thread_Mutex> mutexlock(*refmutex);
	  //CORBA::release(corba_sink);
	  entity = OT_CORBA::OTEntity::_duplicate(entity_);
	}
      } else {
	throw OTGraph::UnsupportedAttribute();
      }
    }

} // namespace ot

