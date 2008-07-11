
/** Header file for StylusModule module.
 *
 * @author Stefan Mooslechner
 */

// Additional code for OpenTracker.cxx:
// #include <OpenTracker/prop/StylusModule.h>
// #ifndef OT_NO_STYLUSMODULE_SUPPORT
//        OT_REGISTER_MODULE(StylusModule, NULL);
// #endif

#ifndef _STYLUSMODULE_H
#define _STYLUSMODULE_H

#include "../OpenTracker.h"


#ifndef OT_NO_STYLUSMODULE_SUPPORT


namespace ot {

    class StylusCalibration;
    class StylusSink;

    class OPENTRACKER_API StylusModule: public Module, public NodeFactory
    {

    protected:
        NodeVector sinks;

        /** Copyconstructor method. */
        StylusModule(StylusModule &src){}

    public:
       /** Constructor method. */
       StylusModule(){}

       /** Destructor method. */
       virtual ~StylusModule(){}

      /** This method is called to construct a new Node. It compares
       *  name to the StylusSink element name, and if it matches
       *  creates the necessary node objects.
       *  @param name reference to string containing element name
       *  @param attributes reference to StringTable containing attribute values
       *  @return pointer to new Node or NULL. */
       virtual Node * createNode( const std::string& name, const StringTable& attributes);
    };

	OT_MODULE(StylusModule);
}  //namespace ot


#endif //OT_NO__STYLUSMODULE_SUPPORT

#endif //_STYLUSMODULE_H
