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
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for FOBModule module.
  *
  * @author
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/FOBModule.h,v 1.7 2002/01/24 17:31:07 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section fobmodule FOBModule (Flock of Birds)
 * 
 * The FOBModule is a device driver for the Ascension Flock of Birds tracker. It
 * supports single as well as multi port configurations and the extended range
 * transmitter. It provides the FOBSource source nodes in the data flow graph. 
 * It uses a configuration element called @c FOBConfig which in turn contains one
 * element called @c Bird for each bird in the setup. Only configured birds will
 * be used. 
 * The @c FOBConfig element has the following attributes :
 *
 * @li @c mode ( multi | single ) defines whether the flock is used in single mode
 *         (one serial comm port to the master bird), or multi mode (a serial port
 *         to each bird).
 * @li @c master the number of the master bird
 * @li @c transmitter if the transmitter is attached to another bird than the master,
 *         set the bird number here (i.e. for an ERT)
 * @li @c hemisphere ( forward | rear | upper | lower | left | right ) defines which
 *         hemisphere to set on the flock.
 * @li @c referenceframe a transformation of the position data
 * @li @c anglealign a transformation of the rotation data
 * @li @c scale ( 36 | 72 ) set the extend of the receivers
 * 
 * The @c Bird element is used to set the birds used in the setup. If the single
 * mode is used, only the master bird needs a device configured. Otherwise all
 * birds need their serial devices set.
 * The @c Bird element has the following attributes :
 * 
 * @li @c number the number of the bird
 * @li @c dev the device name of the bird (i.e. COM1, /dev/ttyS0)
 *
 * An example configuration element looks like this :
 * @verbatim
<FOBConfig mode="multi" master="1" scale="72">
    <Bird number="1" dev="COM1"/>
    <Bird number="2" dev="COM2"/>
</FOBConfig>@endverbatim
 *
 */

#ifndef _FOBMODULE_H
#define _FOBMODULE_H

#include "../OpenTracker.h"

#include <map>

class Bird;
/**
 * developer level information and implementation specifics here
 *
 * @author 
 * @ingroup input
 */
class OPENTRACKER_API FOBModule : public ThreadModule, public NodeFactory
{
// Members
protected:

    /// the hemisphere the tracker should operate in
    enum Hemisphere { FORWARD, REAR, UPPER, LOWER, LEFT, RIGHT } hemisphere;

    /** the serial line mode used, either multi (each bird connected via a serial) or single
     * (all birds via a single serial port). */ 
    enum Mode { MULTI, SINGLE } mode;

    /// number of the master bird
    int master;
    
    /// number of the erc transmitter, if present
    int transmitter;
    
    /// scale factor 
    float scale;
    
    /// associative array of Bird data structures
    std::map<int, Bird *> birds;

    /// flag to stop the thread
    bool stop;

// Methods
protected:
    /** this method is executed in its own thread and reads data from the flock. */
    virtual void run(); 

public:
    /** constructor method. */
    FOBModule();

    /** Destructor method, clears nodes member. */
    virtual ~FOBModule();
    /**
     * initializes the tracker module. For each configured tracker it
     * allocates a Bird structure and stores the configuration information.
     * @param attributes StringTable of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes,  ConfigNode * localTree);
    /** This method is called to construct a new Node. It compares
     * name to the FOBSource element name, and if it matches
     * creates a new FOBSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);    
    
    /** this method initializes the flock and starts the processing thread. */
    virtual void start();
	/**
     * closes the flock again */
    virtual void close();
    /**
     * pushes events into the tracker tree. Checks all trackers for new data
     * and fires the FOBSources, if new data is present.
     */
    virtual void pushState();

private :
   /** inits the whole flock according to the parameters stored
     * in the module. It can be used to reset the flock after
     * some communication failure.
     * @return 0 if everything is, otherwise the FOB error code of the operation
     *  that went wrong. */
    int initFoB();

    /** resets all birds by toggling the modem lines. Does the right thing 
     * depending on the mode. 
     * @return the FOB error code */
    int resetBirds();

    /** sets the report mode of all birds to POSITION/QUATERNION.
     * @return the FOB error code */
    int setReportMode();

    /** issues the Next Transmitter command to the master bird, if another
     * transmitter is set. This is used to activate an ERC transmitter as well.
     * @return the FOB error code */
    int setNextTransmitter();

    /** starts the stream mode to receive data from the birds.
     * @return the FOB error code */
    int startStreamMode();
};

#endif

