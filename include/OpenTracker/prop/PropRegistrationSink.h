
/** Header file for StylusSink node.
 *
 * @author Stefan Mooslechner
 */

#ifndef _PROP_REGISTRATION_SINK_H
#define _PROP_REGISTRATION_SINK_H

#include "../OpenTracker.h"
#include "PropRegistration.h"


namespace ot {

    class OPENTRACKER_API PropRegistrationSink : public Node
    {
       friend class PropRegistrationModule;

    public:
        PropRegistration & prop_registration;

    protected:
         /** Constructor method. */
         PropRegistrationSink( PropRegistration & prop_registration_ ) :
            Node(), 
            prop_registration( prop_registration_ ){}

    public:
         /** Destructor method. */
         virtual ~PropRegistrationSink(){}

         /** Tests for EventGenerator interface being present. Is overriden to
         * return 1 always.
         * @return always 1 */
         virtual int isEventGenerator()
	      {
            return 1;
	      }
    
         /** This method notifies the object that a new event was generated.
          *  It forwards the event value to the SylusCalibration and passes it
          *  on to its parent.
          *  @param event reference to the new event. Do not change the
          *         event values, make a copy and change that !
          *  @param generator reference to the EventGenerator object that
          *         notified the EventObserver. */
         virtual void onEventGenerated( Event& event, Node& generator)
	      {
            prop_registration.handleEvent( event );
            updateObservers( event );
	      }      
    };

} //namespace ot

#endif //_PROP_REGISTRATION_SINK_H


