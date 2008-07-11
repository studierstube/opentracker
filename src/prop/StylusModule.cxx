
/** Source file for StylusModule module.
 *
 *  @author Stefan Mooslechner
 */  

#include <OpenTracker/prop/StylusModule.h>
#include <OpenTracker/prop/StylusSink.h>


#ifndef OT_NO_STYLUSMODULE_SUPPORT

namespace ot {
	OT_MODULE_REGISTER_FUNC(StylusModule){
		OT_MODULE_REGISTRATION_DEFAULT(StylusModule, "StylusConfig" );
	}

    Node * StylusModule::createNode( const std::string& name, const StringTable& attributes)
    {
      if( name.compare("StylusSink") == 0 )
      {             
         int samples;
         if ((attributes.get("samples", &samples)) == 0 ) 
            samples = 0;
      
         bool show_scatter = false;
         if ( (attributes.get("showScatter") == "true") )
            show_scatter = true;

         bool save_data = false;
         if ( (attributes.get("saveData") == "true") )
            save_data = true;

         StylusCalibration * calibration;
         calibration = new StylusCalibration( samples, attributes.get("file"), show_scatter, save_data );
         StylusSink * sink = new StylusSink(*calibration );
         sinks.push_back( sink );
         return sink;
      }
      return NULL;
    }
} //namespace ot


#else
#pragma message(">>> OT_NO_STYLUSMODULE_SUPPORT")
#endif //OT_NO_STYLUSMODULE_SUPPORT

