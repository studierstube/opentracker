
/** Header file for StylusSink node.
 *
 * @author Stefan Mooslechner
 */

#ifndef _STYLUSSINK_H
#define _STYLUSSINK_H

#ifndef OT_NO_PROPREGISTRATIONMOUDLE_SUPPORT
#include "../OpenTracker.h"
#include "StylusCalibration.h"


namespace ot {

    class OPENTRACKER_API StylusSink : public Node
    {
       friend class StylusModule;

    public:
        StylusCalibration & calibration;

    protected:
         /** Constructor method. */
         StylusSink( StylusCalibration & calibration_ ) :
            Node(), 
            calibration( calibration_ ){}

    public:
         /** Destructor method. */
         virtual ~StylusSink(){}

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
            calibration.handleEvent( event );
            updateObservers( event );
	      }      
    };

} //namespace ot
#endif //OT_NO_PROPREGISTRATIONMOUDLE_SUPPORT
#endif //_STYLUSSINK_H


