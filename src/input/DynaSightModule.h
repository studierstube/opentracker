 /* ========================================================================
  * Copyright (C) 2003  Vienna University of Technology
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
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for DynaSightModule module. Version 1.02
  *
  * @author Alexander Schaelss
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/DynaSightModule.h,v 1.4 2003/08/12 08:49:30 reitmayr Exp $
  * @file                                                                    */
 /* ======================================================================== */

/**
 * @page module_ref Module Reference
 * @section dynasightmodule DynaSightModule
 *
 * The DynaSightModule listens for data from the Origin Instruments DynaSight 
 * Sensor sent via serial port to the host and pushes the state updates into the 
 * tracker tree via @ref DynaSightSource nodes.
 *
 * The DynaSight input module was tested with only one target.
 *
 * It's configuration element has the following attributes:
 * @li @c device the serial port the tracker is connected to. 
 * @li @c lookat defines a point in the local tracker coordinate system (x y z) used
 *        for the calculation of the orientation data. if the option is not specified,
 *        the returned ortientation is always towards the z-axis.
 *
 * An example configuration element looks like this:
 * @verbatim
 <DynaSightConfig device="com1" lookat="0.0 0.8 -1.2"/>
 @endverbatim
 */

#ifndef _DYNASIGHTMODULE_H
#define _DYNASIGHTMODULE_H

#include "../OpenTracker.h"
#include "DynaSightSource.h"
#include "../misc/serialcomm.h"
#include <vector>

/// maximum number of targets that can be processed
const int DYNASIGHT_MAX_TARGETS = 8;
/// maximum length of conversion packet buffer
const int DYNASIGHT_PACKET_MAX_LENGTH = 8;
/// maximum size of serial i/o buffer
const int DYNASIGHT_COMM_BUFFER_SIZE = 4096;
/// constant for conversion of target values to meter
const float SCALE_TO_METER = 20000.0;


/// status values for the tracker data
enum TargetStatus { SEARCH=0, COAST=1, CAUTION=2, TRACK=3 };


/// this structure stores the attributes of a DynaSightSource 
struct Target
{
    int number;
    int modified;
    State state;
    DynaSightSource * source;

    Target( const int number_, DynaSightSource * source_ ) :
        number( number_ ), modified( 0 ), source( source_ )
    {};
};


typedef std::vector<Target *> TargetVector;


/**
 * The module and factory to drive the reception of DynaSight state updates.
 * It builds DynaSightSource nodes that insert data from the DynaSight into
 * the tracker tree. It uses the DynaSight Protocol and runs in a thread.
 * 
 * @author Alexander Schaelss
 * @ingroup input
 */
class OPENTRACKER_API DynaSightModule : public ThreadModule, public NodeFactory
{
// members
protected:
    /// vector containing all targets, which have an associated sourceNode
    TargetVector targets;

    /// flag to stop the thread
    bool stop;
    
    /// is TRUE if the serial port was opened
    bool serialportIsOpen;

    /// is TRUE if we have to calculate the orientation
    bool hasLookAt;

    /// x, y, z coordinate of point to look at [meter]
    float lookAtVector[3];

    /// port structure for the serial port data
    SerialPort port;

// methods
protected:
    /// this method is executed in its own thread and reads data from the DynaSight.
    virtual void run();

public:    
    /** basic constructor */
    DynaSightModule();

    /** destructor */
    virtual ~DynaSightModule();

    /**
     * initializes the DynaSightModule module. 
     * @param attributes refenrence to StringTable containing attribute values
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes, ConfigNode * localTree);

    /** This method is called to construct a new Node. It compares
     * name to the DynaSightSource element name, and if it matches
     * creates a new DynaSightSource node.
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
     * pushes state information into the tree. It checks whether there is new
     * data form the DynaSightSource node, copies it into the nodes and calls
     * push on them.    
     */
    virtual void pushState();             
};

#endif
