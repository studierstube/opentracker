/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* SoSocketModule.h contains the definition of SoSocketModule, a Socket communication class using ACE
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoSocketModule.h 3596 2004-08-10 13:34:54Z tamer $
 * @file
 */
/* ======================================================================== */

#ifndef _SOSOCKETMODULE_H_
#define _SOSOCKETMODULE_H_

// for DLL include
#include <stbapi/stbapi.h>

// general C++ classes
#include <string>

// Inventor classes
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#define SOCKET_DEFAULT_PORT 55555

class TCPServer;
class TCPClient;

/** Inventor class implementing socket communication (server or client)

    This Inventor node provides a convinient way to add socket communication functionality to scene graphs.
    The network and thread functions are based on the ACE library.

    DEF MYNODE1 SoMyNode {
        ...
        myInField = SoSocketModule {
            mode SERVER
            port 12345
            in = USE MYNODE2.myOutputField
        }.out
    }

    TODO: implement UDP as well --> introduce <SoSFEnum connection> field having two possible values: TCP and UDP

    @author Istvan Barakonyi
    @ingroup util
  */
class STBAPI_API SoSocketModule : public SoNode {

    SO_NODE_HEADER(SoSocketModule);

public:
    
    /// Data string to send
    SoMFString in;

    /// Received data string 
    SoMFString out;

    /// Server name or IP address to connect to (either in 'host_name' or 'xxx.xxx.xxx.xxx' format)
    SoSFString address;

    /// Communication port
    SoSFInt32 port;

    /** If SERVER, the component is in server mode i.e. waits for incoming connections
        if CLIENT, the component is in client mode i.e. attempts to connect to the given IP address */
    SoSFEnum mode;
    
    static void initClass();

    SoSocketModule();
    ~SoSocketModule();

protected:

    bool initialized;
    bool connectMode;

    TCPServer *serverTCP;
    TCPClient *clientTCP;

    enum ModeType { SERVER, CLIENT };

    SoFieldSensor *inSensor;
    SoFieldSensor *addressSensor;
    SoFieldSensor *portSensor;

    SoTimerSensor *timerSensor;

    static void portAddressCB(void *data,SoSensor *sensor);
    static void inCB(void *data,SoSensor *sensor);
    static void receiveCB(std::string msg,void *data);

    static void timerSensorCB(void *data, SoSensor *);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOSOCKETMODULE_H_
