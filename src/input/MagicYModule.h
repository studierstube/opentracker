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
/** header file for MagicYModule module.
  *
  * @author Christoph Traxler
  *
  * $Id$
  * @file                                                                    */
 /* ======================================================================== */

/**
 * @page module_ref Module Reference
 * @section MagicYModule MagicYModule
 * The MagicYModule listens for data from the MagicY sent via TCP/IP
 * to a specified port of the host and pushes the state updates into the 
 * tracker tree via @ref MagicYSource nodes. 
 * It's configuration element has the following attributes :
 * @li @c IP IP of the MagicY server
 * @li @c port port to listen to
 * @li @c positionMapping defines mapping of position components (x,y) from MagicY to OpenTracker
 * @li @c invertPosition defines, which components of the position should be inverted
 * @li @c average, when set to 1 the average of all points delivered by the MagicY server is calculated
 *
 * An example configuration element looks like this:
 * @verbatim
<MagicYConfig	IP="123.45.67.89"
				port="12345"
				positionMapping="1 0"	
				invertPosition="0 1"
/>@endverbatim
 */
#ifndef _MAGICYMODULE_H
#define _MAGICYMODULE_H

#include "../OpenTracker.h"
#include "MagicYSource.h"

#include <ace/SOCK_Dgram.h>
#include <ace/SOCK_Connector.h>
#include <ace/INET_Addr.h>
#include <ace/Handle_Set.h> 

namespace ot {

/// this structure stores the attributes of an MagicY 
class MagicY
{
  public:
    int number;    
	bool average;
    State state;
    int modified;
    MagicYSource * source;

    MagicY( const int number_, const bool average_, MagicYSource * source_ ) :
        number( number_ ), average( average_ ), modified( 0 ), source( source_ )
    {
		state.position[0] = 0;
		state.position[1] = 0;
		state.position[2] = 0;

		state.orientation[0] = 0;
		state.orientation[1] = 0;
		state.orientation[2] = 0;
		state.orientation[3] = 0;

		state.button = 0;
		state.confidence = 0.0f;
	};
};

// struct to store points for intermediate processing (prediction, etc.)
struct MagicPoint
{
	int x, y;
    bool trigger;

	MagicPoint(const int x_, const int y_) : x(x_), y(y_), trigger(0) {};
	MagicPoint(const int x_, const int y_, const bool trigger_) : x(x_), y(y_), trigger(trigger_) {};
	MagicPoint(const MagicPoint &src) : x(src.x), y(src.y), trigger(src.trigger) {};
};

// struct to store ports and offsets for data streams for extra screens
struct Screen
{
	ACE_SOCK_Stream socket;
	ACE_INET_Addr address;
	int x_offset, y_offset;
	bool connected;

	Screen(const int port_, const std::string hostname_, const int x_offset_, const int y_offset_)
	{
		address.set((u_short)port_, hostname_.c_str()); 
		x_offset = x_offset_; 
		y_offset = y_offset_;
		connected = false;
	};
}; 

typedef std::vector<MagicY *> MagicYVector;
typedef std::vector<Screen *> ScreenVector;
typedef std::vector<MagicPoint> PointVector;

/// maximum number of stations that can be processed
const int magicYMaxUnits = 1000;

/**
 * The module and factory to drive the reception of MagicY state updates.
 * It builds MagicYSource nodes that insert data from the MagicY into
 * the tracker tree. It uses the MagicY protocol and runs in a thread.
 * 
 * @author Christoph Traxler
 * @ingroup input
 */
class OPENTRACKER_API MagicYModule : public ThreadModule, public NodeFactory
{
// members
protected:    
    
	/// vector containing all MagicYs, which have an associated sourceNode
	MagicYVector magicYs;
	/// intermediate MagicY point array for preprocessing
	PointVector points;
	/// sockets and offsets for streams from multiple screens
	ScreenVector screens;
	/// IP to listen for MagicY packages
	std::string hostname;

	ACE_SOCK_Connector connector;
	ACE_Handle_Set readHandles;

    int stop;
	int positionMapping[3];
	int invertPosition[3];
	float z_value;
	float orientation[4];
    
// methods
protected:

    /** The work method for this module's thread, i.e. waiting for and processing data
	    sent from the MagicY server */
	int connect();
	void setSelect();
	int receive();
	int stillConnected(); 
	void disconnect();
    void run();
	int parseVector(const std::string & line, int * val);
	int parseVector(const std::string & line, float * val);
	int parseScreens(const std::string & line);
	void initMappping(int *mapping);
	void initInversion(int *inversion);
	void initOrientation(float *orientation);
	void calcInversion(int *inversion);
	void calcMapping(int *mapping);
	void correctData(float* d, int *mapping, int *inversion);

public:    
    /** basic constructor */
     MagicYModule();

     /** destructor */
     virtual ~MagicYModule();
    /**
     * initializes the MagicY module. 
     * @param attributes refenrence to StringTable containing attribute values
     * @param localTree pointer to root of configuration nodes tree
     */
	 virtual void init(StringTable& attributes, ConfigNode * localTree);
    /** This method is called to construct a new Node. It compares
     * name to the MagicYSource element name, and if it matches
     * creates a new MagicYSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new node or NULL. 
	 */
     virtual Node * createNode( const std::string& name,  StringTable& attributes);        
    /**
     * starts the receive thread. It is called after initialisation is done.
	 */
    virtual void start();
    /**
     * closes the module and closes any communication sockets and stops thread.
	 */
    virtual void close();    
    /**
     * pushes state information into the tree. It checks whether there is new
     * data for any MagicYSource node, copies it into the nodes and calls
     * push on them.    
     */
    virtual void pushState();             
};

} // namespace ot

#endif
