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
/** header file for UltraTrakModule module.
  *
  * @author Rainer Splechtna
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/input/UltraTrakModule.h,v 1.1 2002/10/31 19:53:06 splechtna Exp $
  * @file                                                                    */
 /* ======================================================================== */

/**
 * @page module_ref Module Reference
 * @section UltraTrakModule UltraTrakModule
 * The UltraTrakModule listens for data from the UltraTrak sent via UDP
 * to a specified port of the host and pushes the state updates into the 
 * tracker tree via @ref UltraTrakSource nodes. 
 * It's configuration element has the following attributes :
 * @li @c port port to listen to
 * @li @c positionMapping defines mapping of position components (x,y,z) from ultratrak to opentracker
 * @li @c orientationMapping defines mapping of quaternion components (x,y,z) from ultratrak to opentracker
 * 0=x 1=y 2=z component of ultratrak
 * @li @c invertPosition defines, which components of the position should be inverted
 * @li @c invertOrientation defines, which components of the quaternion should be inverted
 * 0=don't invert 1=invert component
 * An example configuration element looks like this:
 * @verbatim
< UltraTrakConfig port="12345"
  positionMapping="1 0 2"	
  orientationMapping="2 0 1"
  invertPosition="0 1 1"
  invertOrientation="0 0 0"
	  />@endverbatim
 */

#ifndef _ULTRATRAKMODULE_H
#define _ULTRATRAKMODULE_H

#include "../OpenTracker.h"
#include "UltraTrakSource.h"

#include <ace/SOCK_Dgram.h>

/// this structure stores the attributes of an UltratrakSource 
struct Station
{
    int number;    
    State state;
    int modified;
    UltraTrakSource * source;

    Station( const int number_, UltraTrakSource * source_ ) :
        number( number_ ), source( source_ ), modified( 0 )
    {};
};


typedef std::vector<Station *> StationVector;

/// maximum number of stations that can be processed
const int ultraTrakMaxUnits = 16;

/// structure of an UltraTrakStationData section
struct UltraTrakStationData
{
    char dummy1;
    char boardNumber;
    char updateFlag;
    char stationNumber;
#ifdef ERRORCODE
    char errorCode;
#endif
    char dummy2;
    char position[3][4];
    char orientation[4][4];
};
	
/// structure of a whole UltraTrak frame 
struct UltraTrakFrameData
{
    char header[56];
    UltraTrakStationData stationData[ultraTrakMaxUnits];
};


     
/**
 * The module and factory to drive the reception of UltraTrak state updates.
 * It builds UltraTrakSource nodes that insert data from the UltraTrak into
 * the tracker tree. It uses the UltraTrak Protocol and runs in a thread.
 * 
 * @author Rainer Splechtna
 * @ingroup Network
 */
class OPENTRACKER_API UltraTrakModule : public ThreadModule, public NodeFactory
{
// members
protected:    
    
	/// vector containing all stations, which have an associated sourceNode
	StationVector stations;
	/// buffer for incoming package
    UltraTrakFrameData buffer;
	/// port to listen for ultratrak packages
    int port;
    int stop;

	int positionMapping[3];
	int invertPosition[3];
	int orientationMapping[3];
	int invertOrientation[3];
    
// methods
protected:
    /** Converts num floats from Network byte order.
     * @param floats pointer to source data
     * @param result pointer to result array
     * @param num number of floats to convert
     */
    static void convertFloatsNToHl(float* floats, float* result, int num);
    /** The work method for this module's thread, i.e. waiting for and processing data
	    sent from the UltraTrak server */
    void run();
	int parseVector(const std::string & line, int * val);
	void initMappping(int *mapping);
	void initInversion(int *inversion);
	void calcInversion(int *inversion);
	void correctData(float* d, int *mapping, int *inversion);

public:    
    /** basic constructor */
     UltraTrakModule();

     /** destructor */
     virtual ~UltraTrakModule();
    /**
     * initializes the UltraTrak module. 
     * @param attributes refenrence to StringTable containing attribute values
     * @param localTree pointer to root of configuration nodes tree
     */
	 virtual void init(StringTable& attributes, ConfigNode * localTree);
    /** This method is called to construct a new Node. It compares
     * name to the UltraTrakSource element name, and if it matches
     * creates a new UltraTrakSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. 
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
     * data for any UltraTrakSource node, copies it into the nodes and calls
     * push on them.    
     */
    virtual void pushState();             
};

#endif
