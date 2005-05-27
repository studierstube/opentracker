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
/* SoSocketModule.cxx contains the implementation of SoSocketModule, a Socket communication class using ACE
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoSocketModule.cxx 3596 2004-08-10 13:34:54Z tamer $
 * @file
 */
/* ======================================================================== */

// Stb classes
#include <stbapi/misc/TCPServerClient.h>

// Inventor classes
#include <Inventor/elements/SoCacheElement.h>

// own classes
#include "SoSocketModule.h"

SO_NODE_SOURCE(SoSocketModule);

void SoSocketModule::initClass()
{
    SO_NODE_INIT_CLASS(SoSocketModule, SoNode, "Node");
}

SoSocketModule::SoSocketModule()
{
    SO_NODE_CONSTRUCTOR(SoSocketModule);
    
    SO_NODE_ADD_FIELD(in, (""));
    SO_NODE_ADD_FIELD(out, (""));
    SO_NODE_ADD_FIELD(address, ("localhost"));
    SO_NODE_ADD_FIELD(port, (SOCKET_DEFAULT_PORT));
    SO_NODE_ADD_FIELD(mode, (SERVER));

    SO_NODE_DEFINE_ENUM_VALUE(ModeType,SERVER);
    SO_NODE_DEFINE_ENUM_VALUE(ModeType,CLIENT);
    SO_NODE_SET_SF_ENUM_TYPE(mode,ModeType);

    // not initialized yet
    initialized=false;

    // initialize servers to NULL
    serverTCP=NULL;
    clientTCP=NULL;

    // create and attach field sensors
    inSensor=new SoFieldSensor(inCB,this);
    portSensor=new SoFieldSensor(portAddressCB,this);
    addressSensor=new SoFieldSensor(portAddressCB,this);

    // timer sensor
    timerSensor=new SoTimerSensor(timerSensorCB,this);

    inSensor->attach(&in);
    portSensor->attach(&port);
    addressSensor->attach(&address);
}

SoSocketModule::~SoSocketModule()
{
    if (serverTCP) delete serverTCP;
    if (clientTCP) delete clientTCP;

    delete inSensor;
    delete portSensor;
    delete addressSensor;

    delete timerSensor;
}

void SoSocketModule::portAddressCB(void *data,SoSensor *sensor)
{
    SoSocketModule *module=(SoSocketModule*)data;

    if (module->port.getValue()<0) {
        printf("SOCKETMODULE: error: wrong port number %d\n",module->port.getValue());
        return;
    }
    
    if (module->timerSensor->isScheduled()) module->timerSensor->unschedule();
    if (module->serverTCP) delete module->serverTCP;
    if (module->clientTCP) delete module->clientTCP;
    module->initialized=false;
    
    if (module->mode.getValue()==SoSocketModule::SERVER) {        
        module->serverTCP=new TCPServer;
        module->serverTCP->setReceiveCB(&receiveCB);
        module->serverTCP->setUserData(module);
        if (module->initialized=(module->serverTCP->open(module->port.getValue())!=-1)){
            printf("SOCKETMODULE: listening at port %d...\n",module->port.getValue());
        }
    }
    else if (module->mode.getValue()==SoSocketModule::CLIENT) {
        if (!(module->address.getValue().getLength())) {
            printf("SOCKETMODULE: error: no address has been specified\n");
            return;
        }
        module->clientTCP=new TCPClient(module->address.getValue().getString(),module->port.getValue());
        module->clientTCP->setReceiveCB(&receiveCB);
        module->clientTCP->setUserData(module);
        printf("SOCKETMODULE: connecting to %s @ port %d...\n",
            module->address.getValue().getString(),module->port.getValue());
        module->timerSensor->schedule();
    }
}

void SoSocketModule::inCB(void *data,SoSensor *sensor)
{
    SoSocketModule *module=(SoSocketModule*)data;
    if (!module->initialized) return;
    SbString value;
    module->in.get(value);
    if (value.getLength()) {
        if (module->mode.getValue()==SoSocketModule::SERVER) module->serverTCP->send(value.getString());
        else module->clientTCP->send(value.getString());
    }
}

void SoSocketModule::receiveCB(std::string msg,void *data)
{
    SoSocketModule *module=(SoSocketModule*)data;
    module->out.set(msg.c_str());    
}

void SoSocketModule::timerSensorCB(void *data, SoSensor *)
{
    SoSocketModule *module=(SoSocketModule*)data;
    
    if (!module->initialized){
        if (module->clientTCP!=NULL && (module->initialized=(module->clientTCP->open()!=-1))){
            printf("SOCKETMODULE: connected to %s @ port %d\n",
                module->address.getValue().getString(),module->port.getValue());
        }
    }
    else {
        if (!(module->initialized=(module->clientTCP->receive()!=-1))){
            delete module->clientTCP;
            module->clientTCP=new TCPClient(module->address.getValue().getString(),module->port.getValue());
            module->clientTCP->setReceiveCB(&receiveCB);
            module->clientTCP->setUserData(module);
            printf("SOCKETMODULE: connecting to %s @ port %d...\n",
                module->address.getValue().getString(),module->port.getValue());
        }
    }
}
