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
/** source file for SoTextBox class.
*
* @author Thomas Psik
*
* $Id$
* @file                                                                   */
/* ======================================================================= */

#include <cassert>

#include <iostream>

#include <Inventor/nodes/SoAsciiText.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoRotation.h>


#include <Inventor/sensors/SoFieldSensor.h>

#include <Inventor/misc/SoChildList.h>

#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>

#include <stbapi/widgets/SoTextBox.h>
#include <stbapi/widgets/SoLabelKit.h>


#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoTextBox);

void SoTextBox::initClass()
{
	SO_BEHAVIOR_INIT_CLASS(SoTextBox, SoBehaviorKit, "SoBehaviorKit");
}

// the constructor

SoTextBox::SoTextBox()
{
	SO_BEHAVIOR_CONSTRUCTOR(SoTextBox);
	
	//-------------------------+------------+------+---------------+-------+---------------
	//                         |            |      |               | right |
	//                         |            | null |               | Sib-  | is
	//                         |            | By   |               | ling  | public
	//         name            | className  | Def. |  parent Name  | Name  | part
	//-------------------------+------------+------+---------------+-------+---------------
	ADD(rotation,					  SoRotation ,FALSE,  base,	       translation, TRUE);
	ADD(font,						  SoFont	 ,FALSE,  base,	       rotation,    TRUE);
	ADD(normalGeometry,               SoSeparator, FALSE, geometrySwitch, \x0,    TRUE);
	ADD(highlightedGeometry,          SoSeparator, FALSE, geometrySwitch, \x0,    TRUE);
	ADD(disabledGeometry,             SoSeparator, FALSE, geometrySwitch, \x0,    TRUE);
	
    SO_KIT_ADD_FIELD(actionEnterGestures, (new SoEnterGesture()));
    SO_KIT_ADD_FIELD(actionExitGestures, (new SoExitGesture()));
	
	SO_BEHAVIOR_ADD_FIELD(highlighted, (false) );
	
	SO_BEHAVIOR_ADD_FIELD(text, (""));
	
	
	SO_BEHAVIOR_INIT_INSTANCE();
	
	highlighted.setValue(false);
	
	adaptGeometryToState();
	
    hlSensor.setFunction( SoTextBox::stateChangedCB );
    hlSensor.setData( this );
    hlSensor.setPriority( 1 );
	
    enableSensor.setPriority( 1 );
		
	SoAsciiText *text = new SoAsciiText();
	SoSeparator *sep = (SoSeparator *)getPart("normalGeometry",TRUE);
	assert(sep != NULL);
	sep->addChild(text);
	sep = (SoSeparator *)getPart("highlightedGeometry",TRUE);
	assert(sep != NULL);
	sep->addChild(text);
	sep = (SoSeparator *)getPart("disabledGeometry",TRUE);
	assert(sep != NULL);
	sep->addChild(text);
	
	SoFont *fontV = (SoFont *)font.getValue();
	assert(fontV != NULL);
	fontV->size.setValue(.4);

	SoRotation *rot = (SoRotation *)rotation.getValue();
	assert(rot!=NULL);
	rot->set("rotation 1 0 0 -1.57");
}

// the destructor

SoTextBox::~SoTextBox()
{
}

SbBool SoTextBox::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	
	// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoBehaviorKit::setUpConnections(onOff, doItAlways);
        // setup the widget
		hlSensor.attach( &highlighted );
		SoSeparator *sep = NULL;
		
		sep = (SoSeparator *)getPart("normalGeometry",TRUE);
		if (sep != NULL ) 
			connectAsciiTextToValue(sep);	
		sep = (SoSeparator *)getPart("highlightedGeometry",TRUE);
		if (sep != NULL ) 
			connectAsciiTextToValue(sep);	
		sep = (SoSeparator *)getPart("normalGeometry",TRUE);
		if (sep != NULL ) 
			connectAsciiTextToValue(sep);	
		
		
    }
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.
		hlSensor.detach( );
		SoBehaviorKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);
	
}

// ---------------------------------------------------------------

void SoTextBox::resizeWidgetCB()
{
//	DEBUG_CODE(
		printf("SoTextBox::resizeWidgetCB()\n");
	//);
	SoBehaviorKit::resizeWidgetCB();
		
	float widthV = widgetDim[0];
	float heightV = widgetDim[1];
	float depthV = widgetDim[2];

	//widthV = 1.0f;
	float uniformScaleV = widthV;
	if (depthV < uniformScaleV) uniformScaleV = depthV;

	// dont scale Z direction - as SoAsciiText is flat anyhow
	SbVec3f dim(uniformScaleV, uniformScaleV, uniformScaleV);

	SoScale *scalePart = SO_GET_PART(this, "scale", SoScale);
	assert(scalePart != NULL);

	scalePart->scaleFactor.setValue(dim);
	printf("set scale to %f %f %f\n",dim[0],dim[1],dim[2]);

}

// ---------------------------------------------------------------


void SoTextBox::connectAsciiTextToValue(SoSeparator *sep)
{
	SoAsciiText *textNode = NULL;
	int idx = 0;

	while (textNode == NULL && idx < sep->getNumChildren())
	{

		SoNode *child = sep->getChild(idx);
		if (child != NULL && child->isOfType(SoAsciiText::getClassTypeId()))
		{
			textNode = (SoAsciiText *)child;
		}
	}

	if (textNode!= NULL) 
	{
		textNode->string.connectFrom(&text);
		textNode->justification.setValue(SoAsciiText::CENTER);
	}
	else
	{
		// SoDebugMessage("could not find a SoAsciiText in SoTextBox::connectAsciiTextToValue\n");
	}
}



/**
* the handle3DEvent method
*/
void SoTextBox::handle3DEvent(SoHandle3DEventAction *h3a)
{
//	DEBUG_CODE(printf("SoTextBox::handle3DEvent %s\n",getName().getString()););
	
	if(detectAction(actionEnterGestures))
	{
		highlighted.setValue(TRUE);
		setHysteresis(TRUE);
	}
	else if(detectAction(actionExitGestures))
	{
		highlighted.setValue(FALSE);
		setHysteresis(FALSE);
	}

}

/**
* adapts the button's geometry to the button's state
*/

void SoTextBox::adaptGeometryToState()
{
	// We could directly calculate the value of 'whichChild' from
	// the values of 'highlighted' and 'on', but to separate
	// into several cases is clearer.
	
	if (enable.getValue())
	{
		if (highlighted.getValue()) 
			getGeometrySwitch()->whichChild = HIGHLIGHTED;
		else 
			getGeometrySwitch()->whichChild = NORMAL;
	}
	else
	{
		getGeometrySwitch()->whichChild = DISABLED;
		
	}
	
}


void SoTextBox::stateChangedCB( void * userdata, SoSensor * sensor )
{
    assert( userdata != NULL );
    SoTextBox * self = (SoTextBox *) userdata;
    self->adaptGeometryToState();
}



void SoTextBox::adaptToEnable()
{
	adaptGeometryToState();
}

// -------------------------------------------------------------------------------

