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
/** Implementation of class SoPanel
  *
  * @author Valerie Maquil
  *
  * $Id: SoPanel.cxx 3426 2004-07-14 12:11:27Z tamer $
  * @file                                                                   */
 /* ======================================================================= */


#include "SoPanel.h"


#if defined(_DEBUG_PANEL) && defined(DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif


SO_KIT_SOURCE(SoPanel);

//-----------------------------------------------------------------------------------------------------

void SoPanel::initClass()
{
    SO_KIT_INIT_CLASS(SoPanel,  SoWidgetLayoutGroup, "SoWidgetLayoutGroup");
}

//-----------------------------------------------------------------------------------------------------

SoPanel::SoPanel()
{
    SO_KIT_CONSTRUCTOR(SoPanel);


	//--------------------------------------+------------+------+---------------+-------+---------------
	//                                      |            |      |               | right |
	//                                      |            | null |               | Sib-  | is
	//                                      |            | By   |               | ling  | public
	//                      name            | className  | Def. |  parent Name  | Name  | part
	//--------------------------------------+------------+------+---------------+-------+---------------
	
	SO_KIT_ADD_CATALOG_ENTRY(panelLayout,	SoSwitch,	 TRUE, this, \x0, TRUE);
	
	SO_KIT_INIT_INSTANCE();	
  
    SO_KIT_ADD_FIELD( panelIndex, (-1));
	SO_KIT_ADD_FIELD( panelIndexIn, (0));

	panelIndexSensor.setData(this);
	panelIndexSensor.setFunction(SoPanel::panelIndexChangedCB );

	panelIndexInSensor.setData(this);
	panelIndexInSensor.setFunction(SoPanel::panelIndexInChangedCB );

	setUpConnections(TRUE, TRUE);
}

//-----------------------------------------------------------------------------------------------------
	
SbBool SoPanel::setUpConnections(SbBool onOff, SbBool doItAlways)
{
		// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoWidgetLayoutGroup::setUpConnections(onOff, doItAlways);
        // setup the widget
		
		panelIndexSensor.attach( &panelIndex );
	    panelIndexInSensor.attach( &panelIndexIn );

    }
    else {
        // We disconnect BEFORE base class. 
		panelIndexSensor.detach();
		panelIndexInSensor.detach();
       
        // Disconnect the field sensors.
        SoWidgetLayoutGroup::setUpConnections(onOff, doItAlways);


    }
    return !(connectionsSetUp = onOff);
}


//-----------------------------------------------------------------------------------------------------

SoPanel::~SoPanel()
{

}

//-----------------------------------------------------------------------------------------------------

void SoPanel::panelIndexChangedCB( void * data, SoSensor * sensor )
{	
	SoPanel *self = (SoPanel*) data;
	assert(self);

	DEBUG_CODE(printf("SoPanel::panelIndexChanged to [%d]\n",self->panelIndex.getValue()););
	
	SoSwitch *switchLayout = SO_GET_PART(self, "panelLayout", SoSwitch);
	switchLayout->whichChild.setValue(self->panelIndex.getValue());
	
}

//-----------------------------------------------------------------------------------------------------

void SoPanel::panelIndexInChangedCB( void * data, SoSensor * sensor )
{	
	SoPanel *self = (SoPanel*) data;
	assert(self);
	if (self->panelIndex.getValue() != self->panelIndexIn.getValue())
		self->panelIndex.setValue(self->panelIndexIn.getValue());
	 
}

//-----------------------------------------------------------------------------------------------------

void SoPanel::buildPipLayout()
{
	DEBUG_CODE(printf("SoPanel::buildPipLayout \n"););
	SoNodeKitListPart *elementList = SO_GET_PART(this, "elements", SoNodeKitListPart);
	int numChildren = elementList->getNumChildren();

	SoSwitch *switchLayout = SO_GET_PART(this, "panelLayout", SoSwitch);
	//switchLayout->removeAllChildren();

	for (int i= 0; i< numChildren; i++)
	{
			SoLayoutKit *widget = (SoLayoutKit *)elementList->getChild(i);
			switchLayout->addChild(widget);

			widget ->width.setValue(width.getValue());
			widget ->depth.setValue(depth.getValue());	
			widget ->height.setValue(height.getValue());

			if (widget->isOfType(SoWidgetLayoutGroup::getClassTypeId()) )
			{
			((SoWidgetLayoutGroup *)widget )->buildPipLayout();
			}


	}


}

void SoPanel::resizeCB()
{
	//printf("SoPanel::resizeCB() \n");
	//buildPipLayout();
}