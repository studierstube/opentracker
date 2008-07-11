
/** Header file for StylusModule module.
 *
 * @author Stefan Mooslechner
 */

// Additional code for OpenTracker.cxx:
// #include <OpenTracker/prop/PropRegistrationModule.h>
// #ifndef OT_NO_PROPREGISTRATIONMODULE_SUPPORT
//         OT_REGISTER_MODULE(PropRegistrationModule, NULL);
// #endif

#ifndef _PROP_REGISTRATION_MODULE_H
#define _PROP_REGISTRATION_MODULE_H

#include "../OpenTracker.h"


#ifndef OT_NO_PROPREGISTRATIONMOUDLE_SUPPORT


namespace ot {

    class PropRegistration;
    class PropRegistartionSink;

    class OPENTRACKER_API PropRegistrationModule: public Module, public NodeFactory
    {

    protected:
        NodeVector sinks;

        PropRegistration * other_prop_registration;
        bool is_stylus;
        bool is_prop;



        /** Copyconstructor method. */
        PropRegistrationModule(PropRegistrationModule &src){}

    public:
       /** Constructor method. */
       PropRegistrationModule();//{/*other_prop_registration=NULL;*/pitu=0;logPrintW("======Const\n"); std::cout << "xxxxxxxxConst" << std::endl;}

       /** Destructor method. */
       virtual ~PropRegistrationModule(){}

      /** This method is called to construct a new Node. It compares
       *  name to the StylusSink element name, and if it matches
       *  creates the necessary node objects.
       *  @param name reference to string containing element name
       *  @param attributes reference to StringTable containing attribute values
       *  @return pointer to new Node or NULL. */
       virtual Node * createNode( const std::string& name, const StringTable& attributes);
    };

	OT_MODULE(PropRegistrationModule);
}  //namespace ot


#endif //OT_NO__PROPREGISTRATIONMOUDLE_SUPPORT

#endif //_PROP_REGISTRATION_MODULE_H
