

#include <OpenTracker/prop/PropRegistrationModule.h>
#include <OpenTracker/prop/PropRegistrationSink.h>


#ifndef OT_NO_PROPREGISTRATIONMOUDLE_SUPPORT

namespace ot {
	OT_MODULE_REGISTER_FUNC(PropRegistrationModule){
		OT_MODULE_REGISTRATION_DEFAULT(PropRegistrationModule, "PropRegistrationConfig" );
	}

   PropRegistrationModule::PropRegistrationModule()
   {
      other_prop_registration = 0;
   }

   Node * PropRegistrationModule::createNode( const std::string& name, const StringTable& attributes)
   {
      if( name.compare("PropRegistrationSink") == 0 )
      {
         PropRegistration * prop_registration;
         is_stylus = false;
         is_prop = false;  

         if ( (attributes.get("isStylus") == "true") ) // build stylus node
         {
            if ( attributes.get("isProp") == "true" )
            {
               logPrintE("Error while parsing PropRegistration: \n");
               logPrintE("'isStylus=\"true\"' and 'isProp=\"true\"' found in one tag.\n");
               exit (-1);
            }
            if ( !attributes.containsKey("propfile") )
            {
               logPrintE("Error while parsing PropRegistration: \n");
               logPrintE("No propfile found. Impossible to continue.\n");
               exit (-1);
            }

            is_stylus = true;
            prop_registration = new PropRegistration(true, other_prop_registration,attributes.get("propfile"),attributes.get("dtrack"));
         }
         else if ( (attributes.get("isProp") == "true") ) // build prop node
         {
            if ( attributes.get("isStylus") == "true" )
            {
               logPrintE("Error while parsing PropRegistration: \n");
               logPrintE("'isStylus=\"true\"' and 'isProp=\"true\"' found in one tag.\n");
               exit (-1);
            }
            if ( attributes.containsKey("propfile") || attributes.containsKey("dtrack") )
            {
               logPrintW("In prop node: \"propfile\" or \"dtrack\" found.\n");
               logPrintW("This attributes are ignored.\n");
            }

            is_prop = true;  
            prop_registration = new PropRegistration(false, other_prop_registration, "", "");
         }
         else // error - not a stylus node and not a prop node
         {
            logPrintE("Error while parsing PropRegistrationSink: \n");
            logPrintE("'isStylus=\"true\"' or 'isProp=\"true\"' not found.\n");
            logPrintE("One of this attributes are essential.\n");
            exit (-1);
         }

         if (other_prop_registration == 0)
         {
            other_prop_registration = prop_registration;
         }
         else
         {
            if (is_stylus == other_prop_registration->is_stylus) // both nodes are stylus or both are prop
            {
               logPrintE("Two nodes of the same type are not allowed.\n");
               exit (-1);
            }
         }

         PropRegistrationSink * sink = new PropRegistrationSink(*prop_registration );
         sinks.push_back( sink );
         return sink;
      }
      return 0;
   }
} //namespace ot


#else
#pragma message(">>> OT_NO_PROPREGISTRATIONMOUDLE_SUPPORT")
#endif //OT_NO_PROPREGISTRATIONMOUDLE_SUPPORT

