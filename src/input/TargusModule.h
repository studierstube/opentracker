//----------------------------------------------------------------------
//                Institute for Computer Graphics and Vision
//                  (c) 2002,2003,2004 All Rights Reserved
//----------------------------------------------------------------------
// @author 	Bernhard Reitinger
// created 	Wed Jul 13 14:40:41 2005
//----------------------------------------------------------------------
// $Id$
//----------------------------------------------------------------------

/**
 * @page module_ref Module Reference
 * @section targusmodule TargusModule
 * The TargusModule provides the @ref targussource nodes that 
 * generate button press events for the wireless device. This
 * class relies on the code provided by xbindkeys (http://hocwp.free.fr/xbindkeys/xbindkeys.html)
 */

#ifndef _TARGUSMODULE_H
#define _TARGUSMODULE_H

// this is a linux implementation !!!
#ifndef WIN32

#include "../OpenTracker.h"
#include "../core/StringTable.h"
#include "../misc/XKeys.h"

#include <X11/Xlib.h>
#include <iostream>
#include <string>

#define USE_TARGUSMODULE
#ifdef USE_TARGUSMODULE

/**
 * The module and factory to drive the TargusSource node.
 * @author Bernhard Reitinger
 * @ingroup input
 */

namespace ot {

   class OPENTRACKER_API TargusModule : public Module, public NodeFactory
   {
      // Members
   protected:
      /// list of TargusSource nodes in the tree
      NodeVector nodes;

      // Methods
   public:
      /** constructor method. */
      TargusModule() : Module(), NodeFactory()
      {};

      /** Destructor method, clears nodes member. */
      virtual ~TargusModule();

      /** This method is called to construct a new Node. It compares
       * name to the TargusSource element name, and if it matches
       * creates a new TargusSource node.
       * @param name reference to string containing element name
       * @attributes refenrence to StringTable containing attribute values
       * @return pointer to new Node or NULL. The new Node must be
       *         allocated with new ! */
      virtual Node * createNode( const std::string& name,  StringTable& attributes);

      /**
       * closes the X keyboard source */
      virtual void close();

      /**
       * opens the X keyboard source
       */
      virtual void start();

      /**
       * pushes events into the tracker tree. Checks all TargusSources and
       * pushes new events, if a TargusSource fires. The events store
       * structure with position and status of the buttons.
       */
      virtual void pushEvent();

   protected:
      // holds the display pointer
      Display *display;

      // Stores all information about the key mappings
      XKeys *xkeys;
   };

} // namespace ot

#endif

// WIN32
#endif

#endif

//----------------------------------------------------------------------
// End of TargusModule.h
//----------------------------------------------------------------------
// Local Variables:
// mode: c++
// c-basic-offset: 3
// End:
//----------------------------------------------------------------------
