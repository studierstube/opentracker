  /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
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
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for DwarfModule module.
  *
  * @author Gerhard Reitmayr
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

#include "DwarfModule.h"
#include "DwarfSource.h"
#include "DwarfSink.h"

#ifdef USE_DWARF

#include <cstdio>
#include <cstdlib>

#include <DWARF/Service.h>
#include <DWARF/PoseData.h>
#include <common/cpp/PoseSender.h>
#include <middleware/helpers/corbainit_cpp/corbainit.h>

//using namespace std;
using namespace DWARF;

namespace ot {

// Destructor method

DwarfModule::~DwarfModule()
{
}

void DwarfModule::init(StringTable & attributes, ConfigNode * localTree)
{
    string param = "";
    // passes the service manager address on to CORBA, if given
    if( attributes.containsKey("servicemanager") )
    {
        param = "-DserviceManager=";
        param.append( attributes.get("servicemanager"));
    }
    char * argc = (char *) param.c_str();
    myOrbConnection = CorbaInit::initializeOrb( 1, & argc); 
    
    string name = attributes.get("name");
    // construct and send service description, if required !
    if( attributes.get("senddescription").compare("true") == 0)
    {        
        ServiceManager_var smgr = myOrbConnection->getServiceManager();
        
        ServiceDescription_var sd;
        NeedDescription_var nd;
        AbilityDescription_var ad;
        ConnectorDescription_var pd;                
        
        sd=smgr->newServiceDescription(name.c_str());
        sd->setStartOnDemand(false);
        sd->setStopOnNoUse(false);
        
        unsigned int i;
        for( i = 0;i < localTree->countChildren(); i++)
        {
            ConfigNode * child = (ConfigNode*)localTree->getChild(i);
            StringTable & strTab = child->getAttributes();
            if( child->getType().compare("ability") == 0)
            {
                ad = sd->newAbility(strTab.get("name").c_str());
                ad->setType(strTab.get("type").c_str());
                
                thingIdMap[strTab.get("name")] = strTab.get("thingid");
                thingTypeMap[strTab.get("name")] = strTab.get("thingtype");
                
                unsigned int j;
                for( j = 0; j < child->countChildren(); j++)
                {
                    ConfigNode * subChild = (ConfigNode *) child->getChild(j);
                    StringTable & strTab = subChild->getAttributes();
                    if( subChild->getType().compare("attribute") == 0)
                    {
                        ad->setAttribute(strTab.get("name").c_str(), strTab.get("value").c_str());
                    }
                    else if( subChild->getType().compare("connector") == 0)
                    {
                        pd=ad->newConnector(strTab.get("protocol").c_str());                    
                    }
                }
            }		
            else if( child->getType().compare("need") == 0)
            {
                nd = sd->newNeed(strTab.get("name").c_str());
                nd->setType(strTab.get("type").c_str());
                nd->setPredicate(strTab.get("predicate").c_str());
                nd->setDelegated(strTab.get("delegated").c_str());
                
                unsigned int j;
                for( j = 0; j < child->countChildren(); j++)
                {
                    ConfigNode * subChild = (ConfigNode *)child->getChild(j);
                    StringTable & strTab = subChild->getAttributes();
                    if( subChild->getType().compare("attribute") == 0)
                    {
                        nd->setAttribute(strTab.get("name").c_str(), strTab.get("value").c_str());
                    }
                    else if( subChild->getType().compare("connector") == 0)
                    {
                        pd=nd->newConnector(strTab.get("protocol").c_str());                    
                    }
                }
            }
            else
                ACE_LOG_INFO("ot:DwarfModule got something strange here!\n");
        }        
        smgr->activateServiceDescription(name.c_str());
    }

    service = new PoseSenderService(name.c_str());
    myOrbConnection->registerService(name.c_str(), service->_this());
    service->blockUntilRunning();

    Module::init( attributes, localTree );
}

// This method is called to construct a new Node.

Node * DwarfModule::createNode( const string& name, StringTable& attributes)
{
    if( name.compare("DwarfSource") == 0 )
    {
        DwarfSource * source = NULL;
        return source;
    }
    else if( name.compare("DwarfSink") == 0 )
    {
        DwarfSink * sink = NULL;

        PoseSender * sender = service->getPoseSender(attributes.get("name"));
        if( sender != NULL )
        {
            sink = new DwarfSink( attributes.get("name"));
            sinks.push_back( sink );
            
            sender->setPosAccuracy(0);
            sender->setOriAccuracy(0);
            
            sender->setThingID(& thingIdMap[attributes.get("name")] );
            sender->setThingType(& thingTypeMap[attributes.get("name")] );
        }
        return sink;
    }
    return NULL;
}

// pushes events into the tracker tree.

void DwarfModule::pushState()
{
    // here we would push new events from any needs/DwarfSources into the graph
}

void DwarfModule::pullState()
{
    vector<DwarfSink *>::iterator it;
    for( it = sinks.begin(); it != sinks.end(); it ++ )
    {
        DwarfSink * sink = (*it);
        if( sink->changed )
        {
            PoseSender * sender = service->getPoseSender( sink->name );
            if( sender != NULL )
            {
                // FIXME: better to use the array copy, after switch to doubles !!
                sender->setPos( 0, sink->state.position[0]);
                sender->setPos( 1, sink->state.position[1]);
                sender->setPos( 2, sink->state.position[2]);
                sender->setOri( 0, sink->state.orientation[0]);
                sender->setOri( 1, sink->state.orientation[1]);
                sender->setOri( 2, sink->state.orientation[2]);
                sender->setOri( 3, sink->state.orientation[3]);

                // FIXME: use opentracker time with setTimestamp
                sender->updateTimestamp();

                sender->sendPoseData();
            }
            //  create event here and send it !
            sink->changed = false;
        }
    }
}

} // namespace ot

#endif // USE_DWARF

