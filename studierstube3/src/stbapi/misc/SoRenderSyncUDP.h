/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
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
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: stbapi
 * ======================================================================== */
/**
 * class for synchronised path traveral in Open Inventor
 *  The SoRenderSyncUDP node blocks a SoGLRenderAction traversal. It sends a
 * message to the UDP-server it is registered with and waits until the
 * server returns a message. The server handles all registered clients and
 * synchronize their buffer swaps when all clients have reached a render sync
 * node.
 *  The server client (client number -1) can also run asynchronous from the
 * rest of the cluster. Thus the server and its client can run for monitoring
 * a dedicated render cluster.
 *  Only a server client will also build up the server.
 *
 * Message flow:
 * Client: UDP sender, Multicast receiver.
 * Server: UDP receiver, Multicast sender.
 *
 *  A client sends an UDP messages (frame number and client number) when a 
 * GLRenderAction reaches the sync node.
 *  The Server receives the UDP message and blocks all its clients until all
 * of them have sent an UDP message. It compares the frame numbers of the
 * clients with its own synchronizing frame number. If the numbers are equal
 * the server sends a multicast message with the new frame number and a client
 * number of -1. If the numbers differ all clients try to catch up the actual
 * frame number.
 *  In that the UDP messages are not reliable the server never knows if a
 * client is busy or unplugged. So if no client responds after 5 seconds, the
 * server sends a retransmit packet.
 *
 *
 * @author Gottfried Eibner
 * @see SoRenderSync heavily based on SoRenderSync by Alexander Bornik
 *
 * @file                                                                   */
/* ======================================================================= */

#ifndef  _SO_RENDERSYNC_UDP_
#define  _SO_RENDERSYNC_UDP_

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbBasic.h>

#include <stbapi/stbapi.h>

#include <stbapi/misc/SoRenderSync.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * This class syncronizes render traversals between different instances via
 * the network. It is typically put at the end of the scene graph to
 * work as a barrier that waits for all other instances to arrive at the
 * same node before continuing.
 *
 * an example looks like this :
 @verbatim
 SoSeparator {
    SoRenderSyncUDP {
        multicastAddress "224.0.0.1"
        multicastPort    1500
        interfaceAddress "192.168.128.2"
        udpServerAddress "192.168.128.1"
        udpServerPort    2001
        clientNum        0
        synchronized     TRUE
    }
 }
 @endverbatim
 * A clientNum of -1 indicates this node as server.
 *
 * @ingroup camera
 */

class UDPServer;
class UDPClient;
class MulticastReceiver;
class MulticastSender;

class STBAPI_API SoRenderSyncUDP: public SoRenderSync {

    SO_NODE_HEADER(SoRenderSyncUDP);

public:

    SoRenderSyncUDP();

    /** Multicast address and port all clients will listen to. */
    SoSFString multicastAddress;
    SoSFInt32  multicastPort;

    /** Interface IP-address for outgoing messages (to server). */
    SoSFString interfaceAddress;  //@remark does not work with UDP-datagram addresses...

    /** Server address and port all clients will send a message when they traversed this node. */
    SoSFString serverAddress;
    SoSFInt32  serverPort;

public:
    virtual void GLRender(SoGLRenderAction *action);

public:
    static void  initClass();

private:
    /// Destructor, close server thread, servers and clients
    virtual ~SoRenderSyncUDP();

    /// Field sensor to monitor field changes
    SoFieldSensor *multicastAddrSensor;
    SoFieldSensor *multicastPortSensor;
    SoFieldSensor *serverAddrSensor;
    SoFieldSensor *serverPortSensor;

    /** Sets up client/server instances.
      * Triggered by the sensor callback method. */
    void setupServerAndClients(void);

    /** Called by the sensors, when a field was changed */
    static void serverAndClientChangedCB(void *, SoSensor *);

    /** Method which contains server semantics. */
    virtual void serverCode(void);

    /** Server side communication instances */
    UDPServer         *server;  /// UDP server for receiving clients' waiting for synchornization.
    MulticastSender   *sender;  /// Multicast sender to send unblocking signal to all clients.

    /** Client side communication instances */
    UDPClient         *client;  /// UDP client to notify server of blocking rendering.
    MulticastReceiver *receiver;/// Multicast receiver to receive server's unblocking signal.
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif /* _SO_RENDERSYNC_UDP_ */
