 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for NetworkSourceModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/NetworkSourceModule.h,v 1.5 2001/03/27 05:36:13 reitmayr Exp $
  * @file                                                                    */
 /* ======================================================================== */

/**
 * @page module_ref Module Reference
 * @section networksourcemodule NetworksourceModule
 * The NetworkSourceModule listens for data from the Network sent to 
 * multicast groups and pushes the state updates into the tracker tree
 * via NetworkSource nodes. It has no attributes and need not be present
 * in the configuration section to operate.
 */

#ifndef _NETWORKSOURCEMODULE_H
#define _NETWORKSOURCEMODULE_H

#include "../OpenTracker.h"
#include "Network.h"
#include "NetworkSource.h"

struct Station
{
    int number;    
    State state;
    int modified;
    NetworkSource * source;
};

typedef std::vector<Station *> StationVector;

struct MulticastReceiver;

typedef std::vector<MulticastReceiver *> ReceiverVector;

class ACE_Thread_Manager;        
/**
 * The module and factory to drive the reception of network state updates.
 * It builds NetworkSource nodes that insert data from the network into
 * the tracker tree. It uses the Flexible Network Protocol from the 
 * studierstube. It uses its a thread per multicast group to receive data.
 *
 * @author Gerhard Reitmayr
 */
class OPENTRACKER_API NetworkSourceModule : public Module, public NodeFactory
{
// members
protected:    
    
    /// ACE Thread manager
    ACE_Thread_Manager * manager;
    /// multicast socket
    //ACE_SOCK_Dgram_Mcast socket;
    /// list of groups to listen for
    ReceiverVector groups;
    
// methods
protected:
    /** Converts num floats from network byte order.
     * @param floats pointer to source data
     * @param result pointer to result array
     * @param num number of floats to convert
     */
    static void convertFloatsNToHl(float* floats, float* result, int num);
    /** the work method for the module thread. This is executed by the new
     * module thread. In this class it does nothing but subclasses should
     * override it to add their implementation. */
    static void run( void * data );
    
public:    
     NetworkSourceModule();

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
