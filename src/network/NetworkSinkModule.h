 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for NetworkSinkModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/network/NetworkSinkModule.h,v 1.3 2001/01/29 17:16:44 reitmayr Exp $
  * @file                                                                    */
 /* ======================================================================== */

/**
 * @page module_ref Module Reference
 * @section networksinkmodule NetworkSinkModule
 * The NetworkSinkModule graps states from the tracker tree and sends them
 * to other hosts via multicast groups. The receiving groups addresses are
 * set in the NetworkSink nodes. It implements the Flexible Network protocol
 * from the Studierstube.
 * It has the following attributes :
 * @li @c name a string idenfifying the server
 *
 * An example configuration element looks like this :
 * @verbatim
<NetworkSinkConfig name="TrackSpatz Server"/>@endverbatim
 */

#ifndef _NETWORKSINKMODULE_H
#define _NETWORKSINKMODULE_H

#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>

#include "../OpenTracker.h"
#include "Network.h"
#include "NetworkSink.h"

/** a list of MulticastGroup records */
typedef std::vector<MulticastGroup *> GroupVector;

/** a list of NetworkSink nodes */
typedef std::vector<NetworkSink *> SinkVector;
        
/**
 * The module and factory to drive the transmission of tracker states via
 * the network. It grabs the states from various NetworkSink nodes and 
 * distributes them to specified multicast groups.
 *
 * @author Gerhard Reitmayr
 */
class NetworkSinkModule : public Module, public NodeFactory
{
// members
protected:
    /// list of NetworkSink nodes
    SinkVector nodes;
    /// list of MulticastGroup objects
    GroupVector groups;
    /// server name
    string serverName;
    /// Datagram socket to use for sending data
    ACE_SOCK_Dgram socket;

//methods
protected:    
    /** converts floats to network byte order.
     * @param floats pointer to floats to convert
     * @param result pointer where to store the result ( can be floats )
     * @param num number of floats to convert */
    void convertFloatsHToNl(float* floats, float* result, int num);
    
public:
    /**
     * initializes the tracker module. 
     * @param attributes StringMap of elements attribute values. Should be
     *        possibly , but is not for convenience.
     * @param localTree pointer to root of configuration nodes tree
     */
    virtual void init(StringMap& attributes,  Node * localTree);

    /** This method is called to construct a new Node. It compares
     * name to the NetworkSink element name, and if it matches
     * creates a new NetworkSink node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringMap& attributes);    
    /**
     * This method is called after initialisation is finished and before the
     * main loop is started. It opens the sockets needed for communication. */
    virtual void start();
    /**
     * closes the module and closes any communication sockets. */
    virtual void close();    
     /**
     * pulls state information out of the tracker tree. It checks the 
     * modified member on each NetworkSink node and if a new value is stored
     * it inserts it into the apropriate network data buffer. Then it sends
     * any non-empty data buffers to the network.
     */
    virtual void pullState();      
};

#endif
