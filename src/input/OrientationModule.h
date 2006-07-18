 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Schall under
  * <Schall@ims.tuwien.ac.at> or write to Gerhard Schall,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for OrientationModule
  *
  * @author Gerhard Schall
  * 
  *
  * @file                                                                   */
 /* ======================================================================= */
/**
 * @page module_ref Module Reference
 * @section OrientationModule OrientationModule 
 *
 * This module implements the device driver for Orientation inertial trackers from FTW.
 * The corresponding source node is @ref OrientationSource.
 * 
 * An example configuration element looks like this:
 * @verbatim
    <OrinentationConfig device="com1"/>
   @endverbatim
*/

#ifndef _ORIENTMODULE_H
#define _ORIENTMODULE_H

#include "../OpenTracker.h"
#include "OrientationSource.h"
#include "../misc/serialcomm.h"


#ifndef OT_NO_ORIENTATION_SUPPORT

namespace ot {

/// maximum number of targets that can be processed
  const int ORIENTATION_MAX_TARGETS = 8;
  /// maximum length of conversion packet buffer
  const int ORIENTATION_PACKET_MAX_LENGTH = 8;
  /// maximum size of serial i/o buffer
  const int ORIENTATION_COMM_BUFFER_SIZE = 4096;


//class OrientationSource;


/**
 * This module simply supports the OrientationSource. 
 *
 * @ingroup input
 * @author Gerhard Schall
 */

 /// this structure stores the attributes of a OrientationSource 
  struct TargetOri
  {
    int modified;
    Event event;
    OrientationSource *source;

    TargetOri( OrientationSource * source_ ) :
      source( source_ ), event()
    {};
  };


  typedef std::vector<TargetOri *> TargetOriVector;






class OPENTRACKER_API OrientationModule : public ThreadModule, public NodeFactory
{
protected:
      
	 /// vector containing all targets, which have an associated sourceNode
      TargetOriVector targets;
      
      
      /// flag to stop the thread
      bool stop;
    
      /// is TRUE if the serial port was opened
      bool serialportIsOpen;

      /// port structure for the serial port data
      SerialPort port;

      // methods
    protected:
      /// this method is executed in its own thread and reads data from the Orientation tracker.
      virtual void run();

    public:    
      /** basic constructor */
      OrientationModule();

      /** destructor */
      virtual ~OrientationModule();

      /**
       * initializes the OrientationModule module. 
       * @param attributes refenrence to StringTable containing attribute values
       * @param localTree pointer to root of configuration nodes tree
       */
      virtual void init(StringTable& attributes, ConfigNode * localTree);

      /** This method is called to construct a new Node. It compares
       * name to the OrientationSource element name, and if it matches
       * creates a new OrientationSource node.
       * @param name reference to string containing element name
       * @param attributes reference to StringTable containing attribute values
       * @return pointer to new Node or NULL. The new Node must be
       *         allocated with new!
       */
      virtual Node * createNode( const std::string& name,  StringTable& attributes);  
     
      /**
       * starts the processing thread. It is called after initialisation is done.
       */
      virtual void start();

      /**
       * closes the module and closes any communication and stops thread.
       */
      virtual void close();
    
      /**
       * pushes event information into the tree. It checks whether there is new
       * data form the OrientationSource node, copies it into the nodes and calls
       * push on them.    
       */
      virtual void pushEvent();   
      
};


}  // namespace ot


#endif // !defined(_ORIENTMODULE_H)
#endif