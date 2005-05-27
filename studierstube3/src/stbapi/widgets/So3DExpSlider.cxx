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
/** So3DExpSlider - implementation
  *
  * @author Hermann Wurnig, Gerd Hesina
  *
  * $Id: So3DExpSlider.cxx 3781 2004-09-28 11:19:51Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/widgets/So3DExpSlider.h>
#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>
#include <stbapi/workspace/StbWorkspace.h>

int So3DExpSlider::initClassAtExp = 0;

SO_KIT_SOURCE(So3DExpSlider);

//###################################################################
//## CALLBACK-MANAGEMENT FUNCTIONS
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		addPositionChangeCallback
// Purpose:		This function adds the given pointer funcCB to the
//				list of function-pointer, which are called when
//				the position of the slider changes
// Parameters:	funcCB	: pointer to callback-function	
//				userData: void-pointer which is used as a parameter
//						  for the callback-function
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DExpSlider::addPositionChangeCallback(So3DExpSliderCB *funcCB, void *userData)
{
		// append function-pointer and data-pointer to list
	funcList->append((void *) funcCB);
	funcList->append(userData);
}

/////////////////////////////////////////////////////////////////////
// Name:		removePositionChangeCallback
// Purpose:		This function removes the given pointer funcCB from
//				the list of function-pointer, which are called when
//				the position of the slider changes
// Parameters:	funcCB	: pointer to callback-function	
// Returnvalue: TRUE if removing pointer was successful, else FALSE 
/////////////////////////////////////////////////////////////////////
SbBool
So3DExpSlider::removePositionChangeCallback(So3DExpSliderCB *funcCB)
{
	int i;	
		// try to find the pointer in the list
	i=funcList->find((void*)funcCB);
		// if not found return FALSE
	if (i==-1) 
		return FALSE;
		// remove function-pointer, data-pointer and return TRUE
	funcList->remove(i);
	funcList->remove(i);
	return TRUE;
}


//###################################################################
//## EVENT-CALLBACK FUNCTIONS 
//###################################################################


/////////////////////////////////////////////////////////////////////
// Name:		positionChangeCB
// Purpose:		This function is called whenever the field position
//				changes its value. Then all registered callback-
//				functions in funcList have to be called
// Parameters:	userData:	pointer to slider-object
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
So3DExpSlider::positionChangeCB(void *userData, SoSensor *)
{
	So3DExpSlider *root = (So3DExpSlider *) userData;
	int i;
	void (*function)(void *userData, So3DExpSlider *slider);
	float hVal = (root->getCurrentValue()-root->minValue.getValue())/
		(root->maxValue.getValue()-root->minValue.getValue());

	if((root->getBase3DMode()!=Base3D::BASE3D_SLAVEMODE)||!root->validFlag)
	{
		root->currentSetValue=root->getCurrentValue();
		root->validFlag=TRUE;
	}

	DISABLEDIVFIELD(root->position);
	root->position.setValue((exp(1-hVal)-M_E)/(1-M_E));
	ENABLEDIVFIELD(root->position);
	
		// go through list of func-pointers
	for (i=0;i<root->funcList->getLength();i+=2)
	{		// and call each function
		function = (void (*)(void *, So3DExpSlider *))
			(*(root->funcList))[i];
		(*function)((*(root->funcList))[i+1], root);
	}
}


//###################################################################
//## SETTING AND READING THE POSITION OF THE SLIDER
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setPosition
// Purpose:		This function sets the movable part of the slider 
//				to a given position
// Parameters:	newValue:	new position of slider
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DExpSlider::setPosition(float newValue)
{
		// position has to be between 0 and 1, so change
		// appropriately when neccesary
	if (newValue < 0)
		newValue = 0;
	else if (newValue > 1)
		newValue = 1;

	float hVal=1-log(M_E-newValue*(M_E-1));

	setCurrentValue(minValue.getValue() + hVal*(
			maxValue.getValue()-minValue.getValue()));
}

/////////////////////////////////////////////////////////////////////
// Name:		getPosition
// Purpose:		This function returns the sliders current position
// Parameters:	---
// Returnvalue: position of slider
/////////////////////////////////////////////////////////////////////
float
So3DExpSlider::getPosition()
{
	return position.getValue();
}

/////////////////////////////////////////////////////////////////////
// Name:		setCurrentValue
// Purpose:		This function sets the slider to a new position
// Parameters:	newValue:	new value of slider btw. min and max
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
So3DExpSlider::setCurrentValue(float newValue, SbBool forceUpdate)
{		// newValue has to be between 0 and 1
	if (newValue < minValue.getValue())
		newValue = minValue.getValue();
	else if (newValue > maxValue.getValue())
		newValue = maxValue.getValue();
		// if it is different to current value, then set it
	if ((newValue != currentValue.getValue()) &&
		((callback.getValue()==ALWAYS) || (dragging == FALSE) || (forceUpdate)))
	{
		currentValue.setValue(newValue);
		if(getBase3DMode()!=Base3D::BASE3D_SLAVEMODE) currentSetValue=newValue;
	}
	float hVal = (newValue-minValue.getValue())/
		(maxValue.getValue()-minValue.getValue());

	newValue = (exp(1-hVal)-M_E)/(1-M_E);

	DISABLEDIVFIELD(position);
	position.setValue(newValue);
	ENABLEDIVFIELD(position);
}

void So3DExpSlider::setPositionUpdate(float newValue)
{
    setPosition(newValue);
	if((getBase3DMode()==Base3D::BASE3D_SLAVEMODE)&&validFlag)
	{
    	float hVal = (currentSetValue-minValue.getValue())/
	    	(maxValue.getValue()-minValue.getValue());
        hVal = (exp(1-hVal)-M_E)/(1-M_E);
		if(hVal!=getPosition())
        {
            if( snapBackSensor->isScheduled())
                snapBackSensor->unschedule();
			snapBackSensor->setTimeFromNow(SbTime(SNAPBACK_TIME));
			snapBackSensor->schedule(); //schedule a snapBack event
		}
		return;
	}
	if(getBase3DMode()==Base3D::BASE3D_MASTERMODE)
        SETSLIDERSTATE(this, getCurrentValue());
}

/////////////////////////////////////////////////////////////////////
// Name:		getCurrentValue
// Purpose:		This function returns the dials current value
// Parameters:	---
// Returnvalue: position of dial
/////////////////////////////////////////////////////////////////////
float
So3DExpSlider::getCurrentValue()
{		
	return currentValue.getValue();
}

/////////////////////////////////////////////////////////////////////
// Name:		setMinMax
// Purpose:		This function sets the minValue and maVaue-fields
// Parameters:	min: value for minValue-field
//				max: value for maxvalue-field
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DExpSlider::setMinMax(float min, float max)
{		
	minValue=min;
	maxValue=max;
	setCurrentValue(getCurrentValue());
}


//###################################################################
//## INITIALIZATION FUNCTIONS 
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		initClass
// Purpose:		sets up type- and file-format name-information for
//				the class
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
So3DExpSlider::initClass()
{
	if (So3DExpSlider::initClassAtExp == 0)
	{
		So3DSlider::initClass();
		So3DExpSlider::initClassAtExp = 1;

		// use initialization-macro
		SO_KIT_INIT_CLASS(So3DExpSlider, So3DSlider, "So3DSlider");
	}
}

/////////////////////////////////////////////////////////////////////
// Name:		copy
// Purpose:		creates a copy of an instance of a node
// Parameters:	copyConnections: if TRUE, connections to fields of
//				the class are copied as well
// Returnvalue: pointer to copy of node
/////////////////////////////////////////////////////////////////////
SoNode *
So3DExpSlider::copy(SbBool copyConnections) const
{
		// use copy-function of SoNode for copying all public fields
	So3DExpSlider *newSlider = 
		(So3DExpSlider *) SoNode::copy(copyConnections);

	return newSlider;
}

/////////////////////////////////////////////////////////////////////
// Name:		initFields
// Purpose:		does all constructor-initialization
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DExpSlider::initFields(float width, float radius,
					   float pos, float min, float max)
{
	if (pos > max) pos = max; 
	else if (pos < min) pos = min;
 	
	SO_KIT_CONSTRUCTOR(So3DExpSlider);

	SO_KIT_INIT_INSTANCE();
		// initialize non-field member-variables

	validFlag=FALSE;
	this->currentValue.setValue(pos);
	this->width.setValue(width);
	this->radius.setValue(radius);

	setMinMax(min,max);

	positionSensor->detach();
	positionSensor->attach(&currentValue);
	positionSensor->setFunction(positionChangeCB);
	positionSensor->setData(this);
    snapBackSensor->setFunction(snapBack);
    snapBackSensor->setData(this);
}


/////////////////////////////////////////////////////////////////////
// Name:		So3DExpSlider
// Purpose:		Default-Constructor of class So3DExpSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DExpSlider::So3DExpSlider()
{
	initFields(50,3,0,0,1);
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DExpSlider
// Purpose:		Constructor of class So3DExpSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DExpSlider::So3DExpSlider(float width, float radius,
					   float position)
{
	if (position > 1) position=1;
	else if (position < 0) position=0;
	
	initFields(width, radius, position,0,1);
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DExpSlider
// Purpose:		Constructor of class So3DExpSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DExpSlider::So3DExpSlider(float width, float radius,
					   float min, float max, float position)
{
	if (position > 1) position=1;
	else if (position < 0) position=0;

	initFields(width, radius, position, min, max);
}

/////////////////////////////////////////////////////////////////////
// Name:		~So3DExpSlider
// Purpose:		Destructor of class So3DBSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DExpSlider::~So3DExpSlider()
{
}

void
So3DExpSlider::snapBack(void *data, SoSensor *sensor)
{
	So3DExpSlider *self=(So3DExpSlider*)data;
	if(self->validFlag)
        self->setCurrentValue(self->currentSetValue,TRUE); //force update
}
