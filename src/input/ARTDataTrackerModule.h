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
/** header file for ARTDataTrackerModule module.
*
* @author Christopher Schmidt
*
* $Id$
* @file                                                                   */
/* ======================================================================= */

/**
* @page module_ref Module Reference
* @section artdatatrackermodule ARTDataTrackerModule
* The ARTDataTrackerModule is a device driver module for the optical tracking
* system by ART Data. It uses @ref artdatatrackersource nodes to input data into
* the tracking tree. It does something useful as soon as at least one @ref artdatatrackersource node is created.
* It is configured using the configuration element 'ARTDataTrackerConfig'. This
* element has the following attributes :
* @li @c maxbodies  the maximal number of different objects tracked
* @li @c port the port number to listen on, the ART tracker should be configured to send to this number
* 
* An example configuration element looks like this :
* @verbatim
<ARTDataTrackerConfig maxbodies="10" port="12346"/>@endverbatim
*
* Some hints to configuring the ART Tracker host to work with this driver :
* @li When you calibrate the rigid body, you need to choose the "due to
*     body" setting.
* @li Go to the general settings and make sure they are set as displayed in this picture :
*     @image html artsettings.jpg
*/

#ifndef _ARTDATATRACKERMODULE_H
#define _ARTDATATRACKERMODULE_H

#include "../dllinclude.h"
#include "../OpenTracker.h"

#include <vector>

#include "ARTDataTrackerSource.h"

typedef std::vector<Node*> NodeVector;

class ACE_SOCK_Dgram;

/**
* The module and factory to drive the ARTDataTrackersource nodes. It constructs
* ARTDataTrackerSource nodes via the NodeFactory interface and pushes events into
* the tracker tree according to the nodes configuration.
* @author Christopher Schmidt
* @ingroup input
*/
class OPENTRACKER_API ARTDataTrackerModule : public ThreadModule, public NodeFactory
{
	// Members
	
protected:
    
    /// list of ARTDataTrackerSource nodes in the tree
    NodeVector sources;
	/// port number
	int port;
	/// stop flag 
	int stop;
	/// pointer to socket class
	ACE_SOCK_Dgram * socket;
	
private:

	// Methods
	
protected:
	/**
	* Mainloop */
	void run();
	
public:

   	typedef struct BodyRecord		// Structur for the 6d Bodies
	{
		unsigned long id;			// Body ID taken from the Datagramm
		float quality;				// Quality taken from the Datagramm (not used by DTrack in this Version of DTrack)
		float location[3];			// Array for the loaction of the Body (s0 s1 s2)
		float eulerAngles[3];		// Array for the Eulerangles
		float rotationMatrix[9];	// Array for the Rotation Matrix
		float orientation[4];		// Array for the Quaternion (calculated by ARTDataTrackerModule) pushed in the Nodetree
		float position[3];			// Array for the position (location) pushed in the Nodetree
		bool  valid;				// Flag is true if body is tracked by DTrack
	} BodyRecord;
	
	typedef struct MarkerRecord     // Structur for the 3d Markers
	{
		unsigned long id;			// Marker ID taken from the Datagramm
		float quality;				// Quality taken from the Datagramm (not used by DTrack in this Version of DTrack)
		float location[3];			// Array for the loaction of the Body (s0 s1 s2)		
	} MarkerRecord;

	int		bodyID;
	int		maxBodyNumber;
	int		receiveBufferSize;
	char	*receiveBuffer;



	/** constructor method. */
    ARTDataTrackerModule();
	
	/** Destructor method, clears nodes member. */
    virtual ~ARTDataTrackerModule();
    
	/** This method is called to construct a new Node. It compares
	* name to the ARTDataTrackerSource element name, and if it matches
	* creates a new ARTDataTrackerSource node.
	* @param name reference to string containing element name
	* @attributes refenrence to StringMap containing attribute values
	* @return pointer to new Node or NULL. The new Node must be
	*         allocated with new ! */
    virtual Node * createNode( const std::string& name,  StringTable& attributes);
	
	
	/**
	* This method is called after initialisation is finished and before the
	* main loop is started.*/
    virtual void start();
	
	/**
	* Convert the Data from the Arrays to the Quaternion Format */
	virtual void convert();
	
	/**
	* Close */
	virtual void close();
	
    /**
	* pushes events into the tracker tree. Checks all source nodes for
	* new states and pushes them into the tracker tree.
	*/
    virtual void pushState();
	
    /**
	* initializes the ARTDataTrackerModule. 
	* @param attributes StringMap of elements attribute values. Should be
	*        possibly , but is not for convenience.
	* @param localTree pointer to root of configuration nodes tree
	*/
    virtual void init(StringTable& attributes, ConfigNode * localTree);
	
};

#endif
