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
/** header file for FastTrakModule module.
  *
  * @author Rainer Splechtna
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section fasttrakmodule FastTrakModule (supports Polhemus FastTrak and IsoTrak II)
 *
 * This module provides and drives @ref FastTrakSource nodes that 
 * generate standard events.
 * 
 * The configuration element is called @c FastTrakConfig and has the following 
 * attributes :
 * @li @c type "fasttrak" specifies whether "fasttrak" or "isotrak" is used
 * @li @c device the serial port the tracker is connected to. 
 * @li @c stations number of stations connected to the tracker
 * @li @c hemisphere "1,0,0" the hemisphere the tracker should operate in 
 * @li @c reference-frame "0,0,0,200,0,0,0,200,0" the reference frame of the tracker 
 * @li @c init-string "" other init strings, which will be used to configure 
 *        the tracker at startup (e.g. "x0.9,0.6,1.0,0.9\rv0.3,0.6,0.9,0.9\r")
 *
 * An example configuration element looks like this :
 * @verbatim
 <FastTrakConfig type="isotrak" device="COM1" stations="2" hemisphere="-1,0,0"/>
 @endverbatim
 *
 * Note that to date this driver was only tested with IsoTrak II and WIN32.
 */

#ifndef _FASTTRAKMODULE_H
#define _FASTTRAKMODULE_H

#include "../OpenTracker.h"
#include "../misc/serialcomm.h"

#include <vector>

namespace ot {

typedef std::vector<Node*> NodeVector;

/// this struct just stores a state, and if the state was changed 
typedef struct
{
    /// the local state buffer
    State state;
    /// flag indicating a new value in state
    int newVal;
    
} tmpStationState;

/**
 * developer level information and implementation specifics here
 *
 * @author Rainer Splechtna
 * @ingroup input
 */
class OPENTRACKER_API FastTrakModule : public ThreadModule, public NodeFactory
{
// Members
protected:

    /// flag to stop the thread
    int stop;

    /// pointer to array of tmpStationState storing the state of each station
    tmpStationState *stations;
    /// vector of all created fasttrak source nodes 
    NodeVector nodes;

    /// stores which tracker 
    int trackerType;
    /// port structure for the serial port data
    SerialPort port;
    /// number of stations (sensors) connected to the tracker 
    int numberOfStations;
    /// the hemisphere the tracker should operate in ("1,0,0")
    std::string hemisphere;
    /// the reference frame of the tracker ("0,0,0,200,0,0,0,200,0")
    std::string referenceFrame;
    /// other global config strings sent to the tracker during init-phase
    std::string initString;

// Methods
protected:
    /** this method is the code executed in its own thread, it reads from the 
     *  specified serial port, parses the incoming data and updates the state
     *  of the utilized stations */
    virtual void run();

    /** inits the whole fasttrak according to the parameters stored
     * in the module. It can be used to reset the flock after
     * some communication failure.
     * @return 0 if everything is ok, otherwise < 0 */
    int initFastTrak();
    /** parses the input stream from the FastTrak tracker
     * @param c new charcter read from input stream
     * @param b pointer to char array, where the actual data 
     *          record of a station of the tracker is stored
     * @return -1 if reading of one data record is not completed 
     *         -2 if too much junk was read
     *          0,1 number of station for which a whole data record was read
     */
    int parseRecordFT(char c, char *b);
    /** parses the input stream from the IsoTrak II tracker
     * @param c new charcter read from input stream
     * @param b pointer to char array, where the actual data 
     *          record of a station of the tracker is stored
     * @return -1 if reading of one data record is not completed 
     *         -2 if too much junk was read
     *          0,1 number of station for which a whole data record was read
     */
    int parseRecordIT(char c, char *b);
    /** updates button state for specified station (only used for IsoTrak II)
     * @param stationNr number of station the button is associated with
     * @param button state of button
     */
    void setButtonIT(int stationNr, int button);
    /** converts data record to OpenTracker state values and updates 
     * the state of the specified station.
     * @param stationNr number of station the data record is associated with
     * @param b pointer to array of characters, representing a whole data record
     */
    void convert(int stationNr, char *b);

public:
    /** constructor method. */
    FastTrakModule();

    /** Destructor method, clears nodes member. */
    virtual ~FastTrakModule();
    /**
     * initializes the tracker module. 
     * @param attributes StringTable of elements attribute values. 
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringTable& attributes,  ConfigNode * localTree);
    /** This method is called to construct a new Node. It compares
     * name to the FastTrakSource element name, and if it matches
     * creates a new FastTrakSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);    
    
    virtual void start();
	/**
     * closes serial port */
    virtual void close();
    /**
     * pushes events into the tracker tree. Checks all stations for new data
     * and fires the FastTrakSources, if new data is present.
     */
    virtual void pushState();
};

} // namespace ot

#endif
