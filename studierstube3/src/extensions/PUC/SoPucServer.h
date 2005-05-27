/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
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
  * PROJECT: Studierstube
  * ======================================================================== */

 /* ======================================================================= */
/** Header file for API definitions for the application dll of the Invisible Person
  * application.
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucServer.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef SoPucServer_H
#define SoPucServer_H

#include "PUCBasicLib/PUCInformationServer.h"

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFFloat.h>

#include <Inventor/nodekits/SoBaseKit.h>

#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "PUC.h"
#include <string>

/**
 Represents the PUC server in the OIV scene graph. There can be only one PUC Server at one machine. All SoPucDevices must be added to the 
 SoPucServer::devices group or they will not be visible to the clients. It is allowed to have mulitple SoPucServers in the scene graph, but note that
 only the name of the first SoPucServer will be visible to the clients.

  CATALOG PARTS

  All Parts	
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>devices </td><td>SoPucDevice</td><td>SoPucDevice</td><td>YES</td></tr>	
  </table>

 
File Format / defaults:

@verbatim
SoPucServer {
    SoSFString			serverName		"Studierstube PUC" # name of the device, that will be visible at the clients
    }
}
@endverbatim

See:
	SoPucDevice, PUCBasicLib

  @ingroup puc
 */
class PUC_API SoPucServer : public SoBaseKit
{
    SO_KIT_HEADER(SoPucServer);
    SO_KIT_CATALOG_ENTRY_HEADER(devices);

public:
	SoSFString serverName;	///< name of the server that will be displayed at the clients 
	
	///
    static void initClass(); 
    
	///
	SoPucServer();	
	
	void startNodeSensor();
	void stopNodeSensor();


protected:
	static const int startDevicePortRange;   ///< set to 5280, this is the start port number for the PUC devices

	virtual ~SoPucServer();

	virtual SbBool setUpConnections( SbBool onOff, SbBool doItAlways = FALSE );
  
	static void devicesSensorCB(void *data, SoSensor *sensor);
	static void serverNameCB(void *data, SoSensor *sensor);		///< updates the serverName in the PUCInformationServer

	void checkNewDeviceData();
	void checkNewServerData();

	SoNodeSensor devicesSensor;		///< sensor that is fired when devices are changed calls SoPucServer::devicesSensorCB
	SoFieldSensor serverNameSensor; ///< sensor that is fired if SoPucServer::serverName is changed calls SoPucServer::serverNameCB

	static PUCInformationServer pucServer;	///< connection to the PUCBasicLib
};

//----------------------------------------------------------------------------

#endif // SoPucServer_H