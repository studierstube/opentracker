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
/** source file for SoRadioButton class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoRadioButton.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/misc/SoChildList.h>

#include <stbapi/widgets/SoRadioButton.h>
#include <stbapi/widgets/SoRadioButtonGroup.h>
#include <stbapi/widgets/radioButtonGeometry/onString.h>
#include <stbapi/widgets/radioButtonGeometry/offString.h>
#include <stbapi/widgets/radioButtonGeometry/highlightedOnString.h>
#include <stbapi/widgets/radioButtonGeometry/highlightedOffString.h>

#include <stbapi/interaction/SoEnterGesture.h>
#include <stbapi/interaction/SoExitGesture.h>
#include <stbapi/interaction/SoMoveGesture.h>
#include <stbapi/interaction/SoButtonPressGesture.h>
#include <stbapi/util/ivio.h>

#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoRadioButton);

void SoRadioButton::initClass()
{
	SO_BEHAVIOR_INIT_CLASS(SoRadioButton, SoButtonKit, "SoButtonKit");
}

// the constructor

SoRadioButton::SoRadioButton()
{
	SO_BEHAVIOR_CONSTRUCTOR(SoRadioButton);

	SO_BEHAVIOR_INIT_INSTANCE();

    // set the default geometries
    setPart("onGeometry",			  getGeometryFromString(radioButtonOnString) );
    setPart("offGeometry",			  getGeometryFromString(radioButtonOffString) );
    setPart("highlightedOnGeometry",  getGeometryFromString(radioButtonHighlightedOnString) );
    setPart("highlightedOffGeometry", getGeometryFromString(radioButtonHighlightedOffString) );
	setPart("externalOnGeometry",	  getGeometryFromString(radioButtonHighlightedOnString) );
	setPart("disabledOnGeometry",	  getGeometryFromString(SoButtonKit::disabledOnString) );
	setPart("disabledOffGeometry",	  getGeometryFromString(SoButtonKit::disabledOffString) );


}

// the destructor

SoRadioButton::~SoRadioButton()
{
}


// turn the other radio buttons under the same group node off

void SoRadioButton::turnSiblingsOff(SoHandle3DEventAction *h3a)
{
	// set up a search action (juhu, es geht was weiter)

	SoFullPath *pathToButton = (SoFullPath *) h3a->getCurPath();

	SoGroup *parentGroup = NULL;

	for (int i=0; i<pathToButton->getLength(); i++)
	{
		if (pathToButton->getNodeFromTail(i)->isOfType( SoGroup::getClassTypeId() ))
		{
			parentGroup = (SoGroup *) pathToButton->getNodeFromTail(i);
			// check for new style -> just call the SoRadioButtonGroup function if the button is in one.
			if (parentGroup->isOfType( SoRadioButtonGroup::getClassTypeId() ) )
			{
				SoRadioButtonGroup* radioParentGroup = (SoRadioButtonGroup*)parentGroup;
				radioParentGroup->findThisButtonAndActivate(this);
				// that's it.
				return;
			}
			// else do it the "old" way ....
			break;
		}
	}

	if (parentGroup)
	{
		for (int i=0; i<parentGroup->getNumChildren(); i++)
		{
			if (parentGroup->getChild(i)->isOfType( SoRadioButton::getClassTypeId() ) )
			{
				SoRadioButton *radioSibling = (SoRadioButton*) parentGroup->getChild(i);

				if (radioSibling != this) radioSibling->turnOff();
			}
		}
	}
}


void SoRadioButton::handle3DEvent(SoHandle3DEventAction *h3a)
{
	// should not handle events when disabled
	if (enable.getValue() == FALSE) return;

	if(detectAction(actionEnterGestures))
	{
 		highlighted = true;
		setHysteresis(true);
	}
	else if(detectAction(actionExitGestures))
	{
 		highlighted = false;
		setHysteresis(false);
	}


	// toggle the button's on/off state

	if(detectAction(actionStartGestures))
	{
		on = true;
		turnSiblingsOff(h3a);
    
		if(pressCallback) pressCallback(pressCallbackData, this);
 	}


 /*
	SoEnterGesture enter;
	SoExitGesture exit;

	// to prevent the button's geometry to switch abruptly
	// between different geometry states, while this method
	// is executed, we set the state variables (on,highlighted)
	// first and adapt the geometry finally.

	// treat the button's highlight state

	if (inspector->detect(&enter))
	{
 		highlighted = true;
		setHysteresis(true);
	}
	else if (inspector->detect(&exit))
	{
		highlighted = false;
		setHysteresis(false);
	}

	// toggle the button's on/off state

	if (inspector->detect(actionStartGesture))
	{
		on = true;
		turnSiblingsOff(h3a);

        if(pressCallback) pressCallback(pressCallbackData, this);
	}
*/
	adaptGeometryToState();

}
