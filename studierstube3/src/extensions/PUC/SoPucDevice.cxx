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
/** Source file defines the entry point for the application dll.
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucDevice.cxx 3823 2004-10-05 12:27:08Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include "PUCBasicLib/PUCDeviceServer.h"
#include "PUCBasicLib/PUCInformationServer.h"

#include "SoPucDevice.h"
#include "SoPucElementBase.h"
#include "SoPucStateBase.h"
#include "SoPucStateInt.h"
#include "SoPucGroup.h"
#include "SoPucCommand.h"

#include "SoGetPucSpecificationAction.h"
#include "SoPucSendFullStateAction.h"
#include "SoPucRequestStateAction.h"
#include <Inventor/nodes/SoGroup.h>

#include <iostream>

using namespace std;

#if defined(_DEBUG_PUC) && defined(_DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoPucDevice);

//----------------------------------------------------------------------------

void SoPucDevice::initClass()
{
    SO_KIT_INIT_CLASS(SoPucDevice, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------

SoPucDevice::SoPucDevice():
requestStateAction(),
sendFullStateAction(),
triggerCommandAction()
{
	deviceServer = NULL;
	printf("SoPucDevice::SoPucDevice\n");
	 SO_KIT_CONSTRUCTOR(SoPucDevice);
    
    //--- partName, partClassName, nullByDefault, parentName, rightName, isPublicPart

	SO_KIT_ADD_CATALOG_LIST_ENTRY(groups,	SoGroup, TRUE, this, \x0, SoPucGroup, TRUE);
		
	SO_NODE_ADD_FIELD(deviceName, ("unnamedDevice"));

	SO_KIT_INIT_INSTANCE();	

	pucInterface = new StbPUCInterface(this);
}

//----------------------------------------------------------------------------

SoPucDevice::~SoPucDevice()
{

}

//----------------------------------------------------------------------------
// not used !!
std::string SoPucDevice::getElementName() const
{
	return "spec";
}

//----------------------------------------------------------------------------

void SoPucDevice::initializeDeviceServer(PUCInformationServer *infoServer, int port)
{
	// TODO _> restart ?!?
	if (deviceServer) 
	{
		delete deviceServer;
	//	return;

	}
	// setup information that is needed by PUCDeviceServer
	deviceServer = new PUCDeviceServer(infoServer->getReactor());
	deviceServer->setPort(port);
	deviceServer->setDeviceName(deviceName.getValue().getString());
	// set callback funtions
	deviceServer->setInterface(pucInterface);
		
	deviceServer->startServer();
	
	// add deviceSever to informationServer
	infoServer->addDevice(deviceServer);

	requestStateAction.setServer(deviceServer);
	sendFullStateAction.setServer(deviceServer);
	triggerCommandAction.setServer(deviceServer);

//	nodeSensor = new SoNodeSensor(nodeSensorCB, this);
	nodeSensor.setData(this);
	nodeSensor.setFunction(nodeSensorCB);
	nodeSensor.setPriority(0);
	nodeSensor.setTriggerPathFlag(TRUE);
	
	startNodeSensor();
	
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

//static 
void SoPucDevice::nodeSensorCB( void *data, SoSensor *sen)
{
	std::cout << "SoPucDevice::nodeSensorCB" << endl;
	SoNodeSensor *nodeSensor = (SoNodeSensor *)sen;
	SoPucDevice *device = (SoPucDevice *)data;
	// TODO only node changed ??
	SoFieldContainer* container = nodeSensor->getTriggerNode();
	std::cout << "container " << container<< endl;

	if( container != NULL) 
	{
		if (container->isOfType(SoPucStateBase::getClassTypeId()))
		{
			SoPucStateBase *elm = (SoPucStateBase *)container;
			std::cout << "SoPucStateBase node changed" << endl;
			device->deviceServer->sendMessageStateChangeNotification(elm->pucName, elm->getStateValue());
		  }
		else if (container->isOfType(SoPucCommand::getClassTypeId()))
		{
			SoPucCommand *elm = (SoPucCommand *)container;
			elm->fireAtWill();
		}

	}
}

void SoPucDevice::startNodeSensor()
{
	nodeSensor.attach(
		((SoSFNode*)
		groups.getValue()->getField("containerNode"))->getValue()
		);
	
}

void SoPucDevice::stopNodeSensor()
{
	nodeSensor.detach();
}



//----------------------------------------------------------------------------

std::string StbPUCInterface::getSpecificationMethod()
{
	DEBUG_CODE(
	std::cout << ":::puc Spec:::" << endl; 
	)
	std::string ret;

	SoGetPucSpecificationAction getSpec;
	getSpec.apply((SoPucDevice*)deviceDataPtr);
	ret = getSpec.getPucSpecification().c_str();

	FILE *file = fopen("PucDefOut.xml","wt");
	fprintf(file,"%s",ret.c_str());
	fclose(file);

	DEBUG_CODE(
	std::cout << ":::puc Spec:::" << ret<< endl; 
	)
	
	return ret;

}

//----------------------------------------------------------------------------
	
void StbPUCInterface::stateChangeReqMethod(std::string stateName, std::string stateValue)
{
	DEBUG_CODE(
	std::cout << ":::puc state change request:::" << endl; 
	std::cout << "state " << stateName << endl; 
	std::cout << "value " << stateValue << endl; 
	)
	SoPucDevice* pd = (SoPucDevice*)deviceDataPtr;

	if (pd->deviceServer) 
	{
		// updates value state with the stateName,
		// sends MessageStateChangeNotification(stateName, stateValue), if success
		pd->requestStateAction.setData(stateName, stateValue);
		pd->requestStateAction.apply(pd);
//		pd->requestStateAlarm.schedule();
	//TODO:
	//	pd->updateActiveIfAction.setData(stateName, stateValue);
	//	pd->updateActiveIfAction.apply(pd);
	}
}

//----------------------------------------------------------------------------
	
void StbPUCInterface::fullStateMethod()
{
	DEBUG_CODE(
	std::cout << ":::puc full state request:::" << endl; 
	)
	SoPucDevice* pd = (SoPucDevice*)deviceDataPtr;

	if (pd->deviceServer) 
	{
		pd->sendFullStateAction.apply(pd);
	}
}

//----------------------------------------------------------------------------

void StbPUCInterface::invokeCommandMethod(std::string command)
{
	SoPucDevice *device = (SoPucDevice *)deviceDataPtr;
	// TODO ...
	DEBUG_CODE(
	std::cout << ":::puc invoke command:::" << endl; 
	)
	SoPucDevice* pd = (SoPucDevice*)deviceDataPtr;
	pd->triggerCommandAction.setData(command);
	pd->triggerCommandAction.apply(pd);
}

