//----------------------------------------------------------------------
//                Institute for Computer Graphics and Vision
//                  (c) 2002,2003,2004 All Rights Reserved
//----------------------------------------------------------------------
// @author 	Alexander Bornik
// created 	Mon Sep 19 14:40:41 2005
//----------------------------------------------------------------------
// $Id$
//----------------------------------------------------------------------

/**
 * @page module_ref Module Reference
 * @section targusmodule LinmouseModule
 * The LinmouseModule provides the @ref linmousesource nodes that 
 * generate button press events for mouse type devices device. This
 */

#ifndef _LINMOUSEMODULE_H
#define _LINMOUSEMODULE_H

// this is a linux implementation !!!
#ifndef WIN32

#include "../OpenTracker.h"
#include "../core/StringTable.h"

#include <cstdlib>
#include <cstdio>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <asm/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#include <linux/input.h>
#include <iostream>
#include <string>

#define USE_LINMOUSEMODULE
#ifdef USE_LINMOUSEMODULE

/**
 * The module and factory to drive the TargusSource node.
 * @author Bernhard Reitinger
 * @ingroup input
 */

namespace ot {

   class OPENTRACKER_API LinmouseModule : public ThreadModule, public NodeFactory
   {
      // Members
   protected:
      /// list of LinmouseSource nodes in the tree
      NodeVector sources;
      
      // file descriptor
      int fd;
      // flag whether the thread should stop
      int stop;

      // Methods
      
      /**
       * Mainlopp */
      void run(); 

   public:
      /** constructor method. */
      LinmouseModule();

      /** Destructor method, clears nodes member. */
      virtual ~LinmouseModule();

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
      /**
       * initializes the LinmouseModule. 
       * @param attributes StringMap of elements attribute values. Should be
       *        possibly , but is not for convenience.
       * @param localTree pointer to root of configuration nodes tree
	*/
    virtual void init(StringTable& attributes, ConfigNode * localTree);


   };

} // namespace ot

#endif

// WIN32
#endif

#endif

//----------------------------------------------------------------------
// End of LinmouseModule.h
//----------------------------------------------------------------------
// Local Variables:
// mode: c++
// c-basic-offset: 3
// End:
//----------------------------------------------------------------------
