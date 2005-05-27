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
 * abstract base class for synchronised path traveral in Open Inventor
 *  The SoRenderSync node is an abstract class that spwans a thread which
 *  triggers the listenCallback.
 *
 * @author Gottfried Eibner
 *
 * @file                                                                   */
/* ======================================================================= */

#ifndef  _SO_RENDERSYNC_
#define  _SO_RENDERSYNC_

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/SbBasic.h>

#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * This is an abstract class and can not be instantiated.
 * Derived class can use the threadCallback to run the server code.
 *
 * A derived class should support the following methods:
 *
 *  @verbatim virtual void GLRender(SoGLRenderAction *action); @endverbatim
 *   This method provides the synchronization code on the client side.
 *   It is called during GL rendering and should invalidate the cache
 *   element by calling
 *   @verbatim SoCacheElement::invalidate(action->getState()); @endverbatim
 *   to trigger a new SoGLRenderAction. This is important to hold up
 *   communication between server and clients.
 *
 *   @verbatim void setupServerAndClients(void); @endverbatim
 *     This method installs and opens network connection for servers and clients.
 *
 *   @verbatim virtual void serverCode(void); @endverbatim
 *     This method implements the synchronization behaviour on the server side.
 *
 * A clientNum of -1 indicates this node to generate the server thread.
 *
 * @ingroup camera
 */

class STBAPI_API SoRenderSync: public SoNode {

    SO_NODE_ABSTRACT_HEADER(SoRenderSync);

public:

    /** Number of client (setting to -1 forces this node to act as server). */
    SoSFInt32  clientNum;

    /** Determines synchronous/asynchronous rendering for server nodes.
      * Has no effect on client nodes' rendering.*/
    SoSFBool   synchronized;

public:
    virtual void doAction(SoAction *action);
    virtual void GLRender(SoGLRenderAction *action) = 0;
    virtual void callback(SoCallbackAction *action);

public:
    static void  initClass();

protected:
    /// Constructor, protected as described in The Inventor Toolmaker for abstract nodes.
    SoRenderSync();

    /// Destructor, close server thread, servers and clients
    virtual ~SoRenderSync();

    /** Thread function that will be called when the server node build its thread. */
    static void *renderSyncServerThread(void *data);

    /// Field sensor to monitor field changes
    SoFieldSensor *clientNumSensor;

    /** Sets up client/server instances.
      * Triggered by the sensor callback method. */
    void setupServerAndClients(void);

    /** Called by the sensors, when a field was changed */
    static void serverAndClientChangedCB(void *, SoSensor *);

    /** Node's actual frame number */
    unsigned long frameNumber;

    /** Structure for a single packet*/
    typedef struct
    {
        unsigned long  frameNumber;
        unsigned short clientNumber;
        unsigned short retransmit;
    } Packet;

    /** For lazy field evaluation, if fields are changed this flag becomes
        true. It can be used in derived classes to evalute the field's values
        after all fields have changed, instead of evaluating each field changes.
        I.e. this is usefull if you change an IP-address and its port, so
        both changes are detected but only once evaluated. */
    bool hasChanged;

    /** Structure for keeping client statistics,
        used in server thread code and server thread callback code. */
    typedef struct
    {
        unsigned long joinedAtFrame;
        unsigned long framesDropped;
        unsigned long frameNumber;
        bool          active;
    } ClientStatistic;

    /** Method which contains server semantics. */
    virtual void serverCode(void) = 0;

private:
    /** Server side instance, this fields are used by the server only.*/
    /** Id of server thread to service and synchronize clients.
        (used by server only) */
    void *threadID;
    // @hint At the time of implementation there was a conflict of winsock2.h (used by ACE)
    // and winsock.h (some old code buffer.h, sockets.h, SoRenderSync.h) includes
    // when compiling ivutil, so a void* is used instead of ACE_Thread_t type to bypass the
    // conflict.
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif /* _SO_RENDERSYNC_ */
