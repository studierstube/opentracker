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
/** source file for SoRadioButtonGroup class.
  *
  * @author Thomas Psik
  *
  * $Id: SoRadioButtonGroup.cxx 4030 2004-11-22 10:47:09Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/misc/SoChildList.h>

#include <stbapi/widgets/SoRadioButton.h>
#include <stbapi/widgets/SoRadioButtonGroup.h>

#include <stbapi/util/ivio.h>


SO_NODE_SOURCE(SoRadioButtonGroup);

void SoRadioButtonGroup::initClass()
{
	SO_NODE_INIT_CLASS(SoRadioButtonGroup, SoGroup, "Group");
}

// the constructor

SoRadioButtonGroup::SoRadioButtonGroup()
{
	SO_NODE_CONSTRUCTOR(SoRadioButtonGroup);

	SO_NODE_ADD_FIELD(value, (-1));
	SO_NODE_ADD_FIELD(valueIn, (-1));
	SO_NODE_ADD_FIELD(enable, (TRUE));

    // set the default geometries
    

	enableSensor.setFunction( SoRadioButtonGroup::stateChangedCB );
    enableSensor.setData( this );
    enableSensor.attach( &enable );
    enableSensor.setPriority( 1 );

	valueInSensor.setFunction( SoRadioButtonGroup::valueChangedCB );
    valueInSensor.setData( this );
    valueInSensor.attach( &	valueIn);
    valueInSensor.setPriority( 1 );

}

// the destructor

SoRadioButtonGroup::~SoRadioButtonGroup()
{
}


void SoRadioButtonGroup::stateChangedCB( void * userdata, SoSensor * sensor ) 
{
	((SoRadioButtonGroup*)userdata)->adaptButtonsToState();
}

void SoRadioButtonGroup::valueChangedCB( void * userdata, SoSensor * sensor ) 
{
	SoRadioButtonGroup* group = (SoRadioButtonGroup*)userdata;

	group->value.setValue(group->valueIn.getValue()); // copy valueIn to value field
	group->adaptButtonsToState();
}

void SoRadioButtonGroup::adaptButtonsToState() 
{
	int activeChild = value.getValue();
	int buttonCounter = 0;
	for (int i=0; i<getNumChildren(); i++)
	{
	// dont count children that are not buttons !!
		if (getChild(i)->isOfType( SoRadioButton::getClassTypeId() ) )
		{
			SoRadioButton *radioButton = (SoRadioButton*) getChild(i);
			if (activeChild != buttonCounter) 
			{
				radioButton->on.setValue(false);
			} 
			else
			{
				radioButton->on.setValue(true);
			}
			buttonCounter++;

			radioButton->enable.setValue(enable.getValue());
			radioButton->adaptGeometryToState();
		}
	}
}

void SoRadioButtonGroup::findThisButtonAndActivate(SoRadioButton *radioButton)
{
	int buttonCounter = -1; // first button has id 0 !!

	for (int i=0; i<getNumChildren(); i++)
	{
		if (getChild(i)->isOfType( SoRadioButton::getClassTypeId() ) )
		{
			buttonCounter++;
		}
		if (getChild(i) == radioButton )
		{
			valueIn.setValue(buttonCounter);
			break;
		}
	}
	// is done over valueIn sensor
//	adaptButtonsToState();
}

