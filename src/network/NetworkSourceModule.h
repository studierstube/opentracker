 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for NetworkSourceModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/NetworkSourceModule.h,v 1.3 2001/01/29 17:16:44 reitmayr Exp $
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

//#include <ace/Thread.h>
#include <ace/Thread_Manager.h>
#include <ace/Synch.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram_Mcast.h>

#include "../OpenTracker.h"
// #include "../core/Module.h"
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
    ACE_SOCK_Dgram_Mcast socket;
    /// Mutex to synchronize access to Station data
    ACE_Thread_Mutex mutex;
    /// buffer for incoming package
    FlexibleTrackerDataRecord buffer;
    StationVector sources;
    string group;
    int port;
    int stop;
} MulticastReceiver;

typedef std::vector<MulticastReceiver *> ReceiverVector;
        
/**
 * The module and factory to drive the reception of network state updates.
 * It builds NetworkSource nodes that insert data from the network into
 * the tracker tree. It uses the Flexible Network Protocol from the 
 * studierstube. It uses its a thread per multicast group to receive data.
 *
 * @author Gerhard Reitmayr
 */
class NetworkSourceModule : public Module, public NodeFactory
{
// members
protected:    
    
    /// ACE Thread manager
    ACE_Thread_Manager manager;
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
     NetworkSourceModule() : 
      Module(), NodeFactory(), manager()
     {
     };

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
