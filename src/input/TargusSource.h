//----------------------------------------------------------------------
//                Institute for Computer Graphics and Vision
//                  (c) 2002,2003,2004 All Rights Reserved
//----------------------------------------------------------------------
// @author 	Bernhard Reitinger
// created 	Wed Jul 13 16:38:03 2005
//----------------------------------------------------------------------
// $Id$
//----------------------------------------------------------------------

/**
 * @page Nodes Node Reference
 * @section targussource Targussource
 * The Targussource node is a simple EventGenerator that outputs the
 * current position and button event of the TargusModule. It is driven by
 * the @ref targusmodule. 
 * If the mouse does not seem to work, press a button. This should initialize
 * it and it will work. The mouse buttons are mapped to button numbers in
 * the following way : the middle button sets the LSB to 1 and the second 
 * button sets the second to 1.
 *
 * An example element looks like this :
 * @verbatim
 <Targussource/>@endverbatim
*/

#ifndef _TARGUSSOURCE_H
#define _TARGUSSOURCE_H

#include "../OpenTracker.h"

#ifdef USE_TARGUSMODULE

namespace ot {

   /**
    * This class implements a simple source that sets its valid flag in
    * regular intervals and updates any EventObservers. 
    * @author Bernhard Reitinger
    * @ingroup input
    */
   class OPENTRACKER_API TargusSource : public Node {

      // Members
   public: 
      /// the event that is posted to the EventObservers
      Event event;

      // Methods
      /** simple constructor, sets members to initial values */
      TargusSource() : Node()
      {}

      /** tests for EventGenerator interface being present. Is overriden to
       * return 1 always.
       * @return always 1 */
      virtual int isEventGenerator()
      {
	 return 1;
      }
   
      friend class TargetModule;
   };
   
}  // namespace ot

#endif

#endif

//----------------------------------------------------------------------
// End of TargusSource.h
//----------------------------------------------------------------------
// Local Variables:
// mode: c++
// c-basic-offset: 3
// End:
//----------------------------------------------------------------------
