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
/** source file for SoStyleSwitch class.
*
* @author Thomas Psik
*
* $Id: SoStyleSwitch.cxx 4030 2004-11-22 10:47:09Z reitmayr $
* @file                                                                   */
/* ======================================================================= */
#include <assert.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>

#include <stbapi/stbapi.h>
#include <stbapi/util/ivio.h>

#include <stbapi/interaction/SoBehaviorKit.h>


#include "SoStyleSwitch.h"
#include "SoApplyWidgetStyleAction.h"
#include "SoWidgetStyleGroup.h"
#include "SoReApplyWidgetStyleAction.h"

//#define _DEBUG_STYLE_ACTION

#if defined(_DEBUG_STYLE_ACTION) && defined(_DEBUG)
#define DEBUG_CODE(a) a;
#else
#define DEBUG_CODE(a) ;
#endif

SO_KIT_SOURCE(SoStyleSwitch);

/**
* the open inventor initialization
*/
void SoStyleSwitch::initClass()
{

	//SoSimpleWidgetStyle::initClass();

    SO_KIT_INIT_CLASS(SoStyleSwitch, SoBaseKit, "BaseKit");
	SoWidgetStyleGroup::initClass();
	SoApplyWidgetStyleAction::initClass();
	SoReApplyWidgetStyleAction::initClass();
}

/**
* the constructor inits all fields
*/
SoStyleSwitch::SoStyleSwitch()
{
	
	SO_KIT_CONSTRUCTOR(SoStyleSwitch);
		
		// Add the body parts to the catalog...
	//------------------------------------------+------------+------+---------------+-------+---------------
	//                                          |            |      |               | right |
	//                                          |            | null |               | Sib-  | is
	//                                          |            | By   |               | ling  | public
	//                            name          | className  | Def. |  parent Name  | Name  | part
	//------------------------------------------+------------+------+---------------+-------+---------------
	SO_KIT_ADD_CATALOG_ENTRY(stylesGroup,	    SoSwitch,		FALSE,  this,	       \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(widgets,			SoSeparator,	FALSE,  this,		   \x0, TRUE);

	SO_KIT_ADD_FIELD(whichStyle,(0));
	SO_KIT_ADD_FIELD(updateStyle, ());
	
	SO_KIT_INIT_INSTANCE();
	
	whichStyleSensor.setData(this);
	whichStyleSensor.setFunction(SoStyleSwitch::whichStyleChangedCB);

	updateStyleSensor.setData(this);
	updateStyleSensor.setFunction(SoStyleSwitch::updateCB);
	updateStyleSensor.setPriority(1000);

	widgetsSensor.setData(this);
	widgetsSensor.setFunction(SoStyleSwitch::widgetsChangedCB);

	setUpConnections(TRUE, TRUE);
}

// ---------------------------------------------------------------

SbBool SoStyleSwitch::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	// todo take master / slave status into account
    if (!doItAlways) // && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
		SoBaseKit::setUpConnections(onOff, doItAlways); 
        // setup listeners
		SoSwitch *sw = SO_GET_PART(this, "stylesGroup", SoSwitch);
		sw->whichChild.setValue(whichStyle.getValue());
	//	sw->whichChild.connectFrom(&whichStyle);
		whichStyleSensor.attach(&whichStyle);
		updateStyleSensor.attach(&updateStyle);
		widgetsSensor.attach(&widgets);
		updateStyleSensor.schedule();
    }
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.
		whichStyleSensor.detach();
		updateStyleSensor.detach();
		widgetsSensor.detach();

        SoBaseKit::setUpConnections(onOff, doItAlways); 
    }
   return !(connectionsSetUp = onOff);
}

// ---------------------------------------------------------------

SoStyleSwitch::~SoStyleSwitch()
{

}

// ---------------------------------------------------------------

void SoStyleSwitch::whichStyleChangedCB(void *data, SoSensor *)
{
	SoStyleSwitch *self = (SoStyleSwitch*) data;
	assert(self != NULL);
	self->applyStyleToWidgets();

}

// ---------------------------------------------------------------

void SoStyleSwitch::updateCB(void *data, SoSensor *)
{
	SoStyleSwitch *self = (SoStyleSwitch*) data;
	assert(self != NULL);

	self->applyStyleToWidgets();
}

void SoStyleSwitch::applyStyleToWidgets()
{
	SoApplyWidgetStyleAction sa;
	

	// copy value from whichStyle field !!
	SoSwitch *sw = SO_GET_PART(this, "stylesGroup", SoSwitch);
	sw->whichChild.setValue(whichStyle.getValue());
	
	DEBUG_CODE (printf(">>>>>>>>>>>>>>>>> SoStyleSwitch::applyStyleToWidgets to styles\n"););
	sa.retrieveStyle = true;
	sa.apply(SO_GET_PART(this, "stylesGroup", SoSwitch));
	sa.retrieveStyle = false;

	DEBUG_CODE (printf("<<<<<<<<<<<<<<<<< SoStyleSwitch::applyStyleToWidgets to widgets\n"););
	sa.apply(widgets.getValue());
}

// ---------------------------------------------------------------

void SoStyleSwitch::widgetsChangedCB(void *data, SoSensor *)
{
	SoStyleSwitch *self = (SoStyleSwitch*) data;
	assert(self != NULL);
}

// --------------------------------------------------------------------------
/*

  //----------------------------------------------------------------------------
// go through pip sheet content and set default widget geometry, where it has not been overwritten by anyone. 
// Is called by SoContextKit 
void SoPipSheetKit::applyWidgetStyles() 
{
	//
	// SoApplyWidgetStyleAction sa;
    //sa.apply(getSheets());
	//
	applyActionSensor.setFunction(applyWidgetStylesCB);
	applyActionSensor.setData(this);
	applyActionSensor.setTimeFromNow(0.1f);
	applyActionSensor.schedule();
}

void SoPipSheetKit::applyWidgetStylesCB( void *data, SoSensor *sensor) 
{
	if (data == NULL) return;
	SoApplyWidgetStyleAction sa;
   sa.apply(((SoPipSheetKit*)data)->getSheets());
}
  
*/
