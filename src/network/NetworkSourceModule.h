 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for NetworkSourceModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/NetworkSourceModule.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                    */
 /* ======================================================================== */

/**
 * @page modules Module Reference
 * @section networksourcemodule NetworksourceModule
 * The NetworkSourceModule listens for data from the Network sent to 
 * multicast groups and pushes the state updates into the tracker tree
 * via NetworkSource nodes. It has no attributes and need not be present
 * in the configuration section to operate.
 */

#ifndef _NETWORKSOURCEMODULE_H
#define _NETWORKSOURCEMODULE_H

#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram_Mcast.h>

#include "../core/OpenTracker.h"
#include "../core/ThreadModule.h"
#include "../core/NodeFactory.h"
#include "Network.h"
#include "NetworkSource.h"

typedef struct 
{
    int number;    
    State state;
    int modified;
    NetworkSource * source;
} Station;

typedef std::vector<Station *> StationVector;

typedef struct
{
    ACE_INET_Addr address;
    string group;
    int port;
    StationVector sources;
} MulticastReceiver;

typedef std::vector<MulticastReceiver *> ReceiverVector;
        
/**
 * The module and factory to drive the reception of network state updates.
 * It builds NetworkSource nodes that insert data from the network into
 * the tracker tree. It uses the Flexible Network Protocol from the 
 * studierstube. It uses its own thread to receive data. 
 *
 * @author Gerhard Reitmayr
 */
class NetworkSourceModule : public ThreadModule, public NodeFactory
{
// members
protected:    
    /// multicast socket
    ACE_SOCK_Dgram_Mcast socket;
    /// list of groups to listen for
    ReceiverVector groups;
    /// buffer for incoming package
    FlexibleTrackerDataRecord buffer;
    
// methods
protected:
    /** Converts num floats from network byte order.
     * @param floats pointer to source data
     * @param result pointer to result array
     * @param num number of floats to convert
     */
    void convertFloatsNToHl(float* floats, float* result, int num);
    /** the work method for the module thread. This is executed by the new
     * module thread. In this class it does nothing but subclasses should
     * override it to add their implementation. */
    virtual void run();
    
public:    
     /** This method is called to construct a new Node. It compares
     * name to the NetworkSource element name, and if it matches
     * creates a new NetworkSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringMap& attributes);        
    /**
     * opens the sockets needed for communication and
     * starts the receive thread. It is called after initialisation is done.*/
    virtual void start();
    /**
     * closes the module and closes any communication sockets and stops thread.*/
    virtual void close();    
    /**
     * pushes state information into the tree. It checks whether there is new
     * data for any NetworkSource node, copies it into the nodes and calls
     * push on them.    
     */
    virtual void pushState();             
};

#endif
