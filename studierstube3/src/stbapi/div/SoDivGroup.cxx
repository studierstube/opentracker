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
/**
 * source file for the SoDivGroup node
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoDivGroup.cxx 3504 2004-07-20 11:34:20Z tomp $
 * @file                                                                   */
/* ======================================================================= */

#include <stbapi/div/SoDivGroup.h>
#ifndef DIV_TEST
#include <stbapi/div/StbDiv.h>
#else
#include <stbapi/div/divmain.h>
#endif

#include <string>
#include <assert.h>

SO_NODE_SOURCE(SoDivGroup);

// static method to initialize class for OpenInventor.
void SoDivGroup::initClass()
{
    SO_NODE_INIT_CLASS(SoDivGroup, SoGroup, "Group");
}

SoDivGroup::SoDivGroup() 
: div( NULL )
{
    SO_NODE_CONSTRUCTOR(SoDivGroup);
    SO_NODE_ADD_FIELD(active,(FALSE));    
    SO_NODE_ADD_FIELD(getMaster,(/**/));    
    SO_NODE_ADD_FIELD(initFromMaster,(TRUE));    
    SO_NODE_ADD_FIELD(isMaster,(FALSE));    
    SO_NODE_ADD_FIELD(multicastGroup,(""));    
    SO_NODE_ADD_FIELD(port,(0));
    SO_NODE_ADD_FIELD(nic, (""));

    isBuiltIn = TRUE;

    activeObs.setPriority(1);
    activeObs.setData( this );
    activeObs.setFunction( SoDivGroup::activeChangedCB );
    activeObs.attach( &active );

    masterObs.setPriority(1);
    masterObs.setData( this );
    masterObs.setFunction( SoDivGroup::getMasterTriggeredCB );
    masterObs.attach( &getMaster );
}

SoDivGroup::~SoDivGroup()
{
    if( div != NULL )
        delete div;
    activeObs.detach();
    masterObs.detach();
}

void SoDivGroup::activeChangedCB( void * data, SoSensor * sensor )
{    
    assert(data);
    SoDivGroup * self = (SoDivGroup *)data;
#if 0
    printf("SoDivGroup %x - %s::active changed to %d, mc %s:%d, master %d, init %d\n", self,
           self->getName().getString(), self->active.getValue(), self->multicastGroup.getValue().getString(),
           self->port.getValue(), self->isMaster.getValue(), self->initFromMaster.getValue());
#endif
    if( self->active.getValue() == TRUE )
    {
        if( self->div != NULL )  // we are already active, ignore signal
            return;
        // if the SoDivGroup does not have a name, create one so that it will
        // work correctly.
        if( self->getName() == "" )
        {
            // replace all occurences of . with _ to make the name a valid one
            std::string validAddress(self->multicastGroup.getValue().getString());
            unsigned int idx = validAddress.find(".", 0);
            for ( ;idx != std::string::npos; idx = validAddress.find(".", idx))
                validAddress.replace(idx,1,"_");

            SbString name("DIV_GROUP_");
            name += validAddress.c_str();
            name += "_";
            name += SbString( self->port.getValue());
            self->setName( name );
        }        

		const char * nic = NULL;
		if (self->nic.getValue().getLength() > 0) 
			nic = self->nic.getValue().getString();

        self->div = 
#ifndef DIV_TEST
            new StbDiv( 
#else
            new CDivMain(
#endif
            self->multicastGroup.getValue().getString(), 
            self->port.getValue(), 
            self->isMaster.getValue() == TRUE ? CDivMain::MODE_MASTER : CDivMain::MODE_SLAVE, 
            self->initFromMaster.getValue() == TRUE,
			nic
        );
        self->div->disableField( & self->active );
        self->active.setDefault( TRUE );
        self->div->disableField( & self->getMaster );
        self->div->disableField( & self->initFromMaster );
        self->div->disableField( & self->isMaster );
        self->div->disableField( & self->multicastGroup );
        self->div->disableField( & self->port );
        self->div->setMasterChangeCB( SoDivGroup::masterChangedCB, self );
        if( self->isMaster.getValue() == TRUE )
            self->div->shareNode( self );
    }
    else
    {
        if( self->div == NULL )  // we are inactive, ignore signal
            return;
        self->div->disableDiv();
        delete self->div;
        self->div = NULL;
    }
}

void SoDivGroup::getMasterTriggeredCB( void * data, SoSensor * sensor )
{
    assert(data);
    SoDivGroup * self = (SoDivGroup *)data;
#if 0
    printf("SoDivGroup %s::masterTriggered active is %d\n",
           self->getName().getString(), self->active.getValue());
#endif
    if( self->active.getValue())
    {
        self->div->getMasterMode();
    }
    else
    {
        self->isMaster.setValue( TRUE );
    }
}

void SoDivGroup::masterChangedCB( void * data, CDivMain * div )
{
    assert(data);
	SoDivGroup * self = (SoDivGroup *)data;
#if 0
	printf("SoDivGroup %s::masterChangedCB new mode is %d\n",
           self->getName().getString(), div->getMode() == CDivMain::MODE_MASTER );    
#endif
    self->isMaster.setValue( div->getMode() == CDivMain::MODE_MASTER );
}
