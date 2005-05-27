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
/** Header file for API definitions of SoPucDevice
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucDevice.h 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef SoPucDevice_H
#define SoPucDevice_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFNode.h>

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>
#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "PUC.h"
#include "SoPucRequestStateAction.h"
#include "SoPucSendFullStateAction.h"
#include "SoPucTriggerCommandAction.h"

#include "PUCBasicLib/PUCInterface.h"
#include <string>

class SoGetPucSpecificationAction;
class PUCInformationServer;
class PUCDeviceServer;
class SoPucDevice;

/**
*/
// internal class to define the PUCInterface behavior and create the interface to PucBasicLib

class StbPUCInterface : public PUCInterface
{
public:
	StbPUCInterface (SoPucDevice *device) { setDataPtr(device); }

	virtual std::string getSpecificationMethod();

	virtual void stateChangeReqMethod(std::string, std::string);

	virtual void fullStateMethod();

	virtual void invokeCommandMethod(std::string);
};

//----------------------------------------------------------------------------

/**
 Represents a PUC device in the OIV scene graph. It is possible to have multiple devices in the PUC Server. The states of the devices can not
 influence each other. Each SoPucDevice must be added to the SoPucServer::devices field, if not the device will not be visible from the clients.
 A PUC device contains a list of groups (SoPucGroup).

  CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>groups </td><td>SoPucGroup</td><td>SoPucGroup</td><td>YES</td></tr>
  </table>


File Format / defaults:

@verbatim
SoPucDevice {
    SoSFString			deviceName		"unnamed" # name of the device, that will be visible at the clients
    }
}
@endverbatim

See:
	SoPucServer, SoPucGroup, PUCBasicLib

  @ingroup puc
 */

class PUC_API SoPucDevice : public SoBaseKit
{
    SO_KIT_HEADER(SoPucDevice);
    SO_KIT_CATALOG_ENTRY_HEADER(groups);

public:
	SoSFString deviceName;

    static void initClass();
    SoPucDevice();

	virtual SbBool	isOverride() const {return TRUE;}
	virtual std::string getElementName() const;

	void initializeDeviceServer(PUCInformationServer *, int port);

	void startNodeSensor();
	void stopNodeSensor();
protected:

	StbPUCInterface *pucInterface;

//	static std::string getPucSpecification(void *data);
	static void	stateChangeReq(void *data, std::string stateName, std::string stateValue);
	static void fullStateReq(void *data);

	static void nodeSensorCB(void *data, SoSensor *sen);

	void sendFullState();

	virtual ~SoPucDevice();

	PUCDeviceServer *deviceServer;

	SoPucRequestStateAction requestStateAction;
	SoPucSendFullStateAction sendFullStateAction;
	SoPucTriggerCommandAction triggerCommandAction;
	SoNodeSensor nodeSensor;

	friend SoGetPucSpecificationAction;
	friend StbPUCInterface;
};

//----------------------------------------------------------------------------

#endif // SoPucDevice_H