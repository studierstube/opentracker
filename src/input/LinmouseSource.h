//----------------------------------------------------------------------
//                Institute for Computer Graphics and Vision
//                  (c) 2002,2003,2004 All Rights Reserved
//----------------------------------------------------------------------
// @author 	Alexander Bornik
// created 	Mon Sep 19 16:38:03 2005
//----------------------------------------------------------------------
// $Id$
//----------------------------------------------------------------------

/**
 * @page Nodes Node Reference
 * @section linmousesource Linmousesource
 * The Linmouse node is a simple EventGenerator that outputs the
 * current position and button state of the LinmouseModule. It is driven by
 * the @ref linmousemodule. 
 *
 * An example element looks like this :
 * @verbatim
 <Linmousesource/>@endverbatim
*/

#ifndef _LINMOUSESOURCE_H
#define _LINMOUSESOURCE_H

#include "../OpenTracker.h"

#ifdef USE_LINMOUSEMODULE

namespace ot {

   /**
    * This class implements a simple source that sets its valid flag in
    * regular intervals and updates any EventObservers. 
    * @author Alexander Bornik
    * @ingroup input
    */
   class OPENTRACKER_API LinmouseSource : public Node {

      // Members
   public:   
      /// source device name
      std::string devname;
      /// the state that is posted to the EventObservers
      State state;
      /// a flag to indicate whether it was changed during processing
      int changed;

      // Methods
      /** simple constructor, sets members to initial values */
      LinmouseSource( const std::string &devname_) : Node(), devname(devname_), changed(1)
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
// End of LinmouseSource.h
//----------------------------------------------------------------------
// Local Variables:
// mode: c++
// c-basic-offset: 3
// End:
//----------------------------------------------------------------------
