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
/** implementation of AbstractToogleButtonBox widget
  *
  * @author  Thomas Psik
  *
  * $Id: SoTextListBox.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <cassert>

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoMaterial.h>

#include <stbapi/widgets/SoPushButton.h>
#include <stbapi/widgets/SoToggleButton.h>
#include <stbapi/widgets/SoIncrementalSlider.h>
#include <stbapi/widgets/SoLabelKit.h>
#include <stbapi/util/ivio.h>
#include <stbapi/misc/SoAbortGroup.h>

#include <stbapi/widgets/SoTextListBox.h>
/*
#include <stbapi/styles/SoButtonStyle.h>
*/

SO_KIT_SOURCE(SoTextListBox);

void SoTextListBox::initClass()
{
    SO_KIT_INIT_CLASS(SoTextListBox,  SoLabelListBox, "SoLabelListBox");
}

SoTextListBox::SoTextListBox()
{
    SO_KIT_CONSTRUCTOR(SoTextListBox);

	// ??? ask gerhard ???
	//    isBuiltIn = TRUE;

	//--------------------------------------+------------+------+---------------+-------+---------------
	//                                      |            |      |               | right |
	//                                      |            | null |               | Sib-  | is
	//                                      |            | By   |               | ling  | public
	//                      name            | className  | Def. |  parent Name  | Name  | part
	//--------------------------------------+------------+------+---------------+-------+---------------

	SO_KIT_INIT_INSTANCE();	
  
    SO_KIT_ADD_FIELD( values, (""));
	SO_KIT_ADD_FIELD( selectedValues, (""));
	
	values.setNum(0);
	selectedValues.setNum(0);

	valuesSensor.setData(this);
	valuesSensor.setFunction(SoTextListBox::valuesChangedCB );
	
    setUpConnections(TRUE, TRUE);
}

SbBool SoTextListBox::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoLabelListBox::setUpConnections(onOff, doItAlways);
		// never traverse the children of the geometry switch !
		//((SoSwitch *)geometrySwitch.getValue())->whichChild.setValue( -1 );
        // setup the widget
        adaptToConfig();
        // setup listeners
   		valuesSensor.attach( &values );	
    }
    else {
        // We disconnect BEFORE base class.        
   		valuesSensor.detach( );	
   
        SoLabelListBox::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);

}


SoTextListBox::~SoTextListBox()
{

}

void SoTextListBox::valuesChangedCB( void * data, SoSensor * sensor  )
{
	assert( data != NULL );
    SoTextListBox * self = (SoTextListBox *)data;    
	self->adaptToConfig();	
	self->updateLabels();	
	self->adaptToState();	
}

// ---------------------------------------------------------------
void SoTextListBox::updateLabels(void)
{
	// remove all labels
	SoNodeKitListPart * list = (SoNodeKitListPart*)labels.getValue();
    assert(list != NULL);
	while( list->getNumChildren() != 0 )
        list->removeChild( 0 );

	if (values.getNum() == 0)
	{
		printf("SoTextListBox::updateLabels ERROR values.getNum() is 0");
		return;
	}
	int counter = 0;

	for (int i=0; i< values.getNum(); i++)
	{
		if (values[i].getLength() > 0)
		{
			SoLabelKit *label = new SoLabelKit();
			label->setUpConnections(TRUE, TRUE);
			label->text.setValue(values[i]);
			list->addChild( label);		
		}
	}

	states.setNum(list->getNumChildren());

	SoLabelListBox::updateLabels();
}
// ---------------------------------------------------------------

