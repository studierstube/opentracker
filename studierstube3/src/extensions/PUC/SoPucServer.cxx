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
/** Header file for API definitions of SoPucServer
  *
  * @author Thomas Psik <tomp@ims.tuwien.ac.at>
  *
  * $Id: SoPucServer.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoPucServer.h"
#include "SoPucDevice.h"
#include "SoPucElementBase.h"
#include "SoPucStateBase.h"
#include "SoPucGroup.h"
#include "SoPucCommand.h"

#include "SoPucStateInt.h"
#include "SoPucStateFloat.h"
#include "SoPucStateFixed.h"
#include "SoPucStateEnumerated.h"
#include "SoPucStateBool.h"

#include "SoPucStateString.h"

#include "SoGetPucSpecificationAction.h"
#include "SoPucSendFullStateAction.h"
#include "SoPucRequestStateAction.h"
#include "SoPucTriggerCommandAction.h"

#include "SoPucActiveIfNode.h"
#include "SoPucActiveIfBase.h"
#include "SoPucActiveIfClause.h"
#include "SoPucActiveIfOperator.h"

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoGroup.h>

#include <iostream>

using namespace std;
//#include <Inventor/actions/SoGLRenderAction.h>
#ifdef USE_DIV
#include <stbapi/div/divmain.h>
#endif

#if defined(_DEBUG_PUC) && defined(DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif


PUCInformationServer SoPucServer::pucServer;

const int SoPucServer::startDevicePortRange = 5280;
//----------------------------------------------------------------------------


SO_KIT_SOURCE(SoPucServer);

//----------------------------------------------------------------------------

void SoPucServer::initClass()
{
	SoPucElementBase::initClass();
	SoPucGroup::initClass();
	SoPucCommand::initClass();
	SoPucDevice::initClass();

	SoPucStateBase::initClass();
	SoPucStateInt::initClass();
	SoPucStateFloat::initClass();
	SoPucStateEnumerated::initClass();
	SoPucStateString::initClass();
	SoPucStateFixed::initClass();
	SoPucStateBool::initClass();
	
	SoPucActiveIfNode::initClass();
	SoPucActiveIfBase::initClass();

	SoPucActiveIfClause::initClass();
	SoPucActiveIfOperator::initClass();

	SoGetPucSpecificationAction::initClass();
	SoPucSendFullStateAction::initClass();
	SoPucRequestStateAction::initClass();
    SoPucTriggerCommandAction::initClass();

	SO_KIT_INIT_CLASS(SoPucServer, SoBaseKit, "BaseKit");
	printf("SoPucServer::initClass\n");
}

//----------------------------------------------------------------------------

SoPucServer::SoPucServer()
{
	 SO_KIT_CONSTRUCTOR(SoPucServer);
    
    //--- partName, partClassName, nullByDefault, parentName, rightName, isPublicPart

	SO_KIT_ADD_CATALOG_LIST_ENTRY(devices,	SoGroup, TRUE, this, \x0, SoPucDevice, FALSE);
		
	SO_NODE_ADD_FIELD(serverName, ("Studierstube PUC"));

	SO_KIT_INIT_INSTANCE();	
	
	serverNameSensor.setFunction(serverNameCB);
	serverNameSensor.setData(this);
	serverNameSensor.attach( &serverName);
	
	devicesSensor.setFunction(devicesSensorCB);
	devicesSensor.setData(this);
	
	pucServer.setServerName(serverName.getValue().getString());
	pucServer.run();

	setUpConnections(TRUE, TRUE);
	enableNotify(TRUE);

}

//----------------------------------------------------------------------------

SoPucServer::~SoPucServer()
{

}

SbBool SoPucServer::setUpConnections( SbBool onOff, SbBool doItAlways )
{
 if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    SoBaseKit::setUpConnections(onOff, doItAlways);
    
    // do your own special setup here !
    // connect fields
/*	if (devices.getValue() != NULL)
		devicesSensor.attach(devices.getValue());
  */
  	// check data and setup information in the nodes
	SoNodeKitListPart *lp = SO_GET_ANY_PART(this ,"devices",SoNodeKitListPart);
	int num = lp->getNumChildren();
 	cout<< "groups found [" <<num << "]" << endl;		
	for (int i= 0; i< num; i++) 
	{
		SoPucDevice *dv = (SoPucDevice *)lp->getChild(i);
		#ifdef USE_DIV
		CDivMain::globalDisableTree( dv);
		#endif
		dv->initializeDeviceServer(&pucServer, startDevicePortRange + i);
	}

  }
  else {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
    //translation.disconnect();
    devicesSensor.detach();

    SoBaseKit::setUpConnections(onOff, doItAlways);
  }
  return !(connectionsSetUp = onOff);
}

//----------------------------------------------------------------------------
void SoPucServer::startNodeSensor()
{
	SoNodeKitListPart *lp = SO_GET_ANY_PART(this ,"devices",SoNodeKitListPart);
	int num = lp->getNumChildren();
	for (int i= 0; i< num; i++) 
	{
		SoPucDevice *dv = (SoPucDevice *)lp->getChild(i);
		dv->startNodeSensor();
	}

}

//----------------------------------------------------------------------------

void SoPucServer::stopNodeSensor()
{
	SoNodeKitListPart *lp = SO_GET_ANY_PART(this ,"devices",SoNodeKitListPart);
	int num = lp->getNumChildren();
 	cout<< "groups found [" <<num << "]" << endl;		
	for (int i= 0; i< num; i++) 
	{
		SoPucDevice *dv = (SoPucDevice *)lp->getChild(i);
		dv->stopNodeSensor();
	}

}

//----------------------------------------------------------------------------


void SoPucServer::devicesSensorCB(void *data, SoSensor *sensor)
{
	((SoPucServer *)data)->checkNewDeviceData();
}

void SoPucServer::serverNameCB(void *data, SoSensor *sensor)
{
	DEBUG_CODE(
	std::cout << "serverName changed to ["<< ((SoPucServer  *)data)->serverName.getValue().getString() << "] " << endl; 
	)

	((SoPucServer*)data)->pucServer.setServerName(((SoPucServer  *)data)->serverName.getValue().getString());
}
 
//----------------------------------------------------------------------------

void SoPucServer::checkNewDeviceData() 
{
	DEBUG_CODE(
	std::cout << "checkNewDeviceData " << endl;
	);
	
	// TODO !!!! What ??

}
 
//----------------------------------------------------------------------------

void SoPucServer::checkNewServerData()
{

}

//----------------------------------------------------------------------------
