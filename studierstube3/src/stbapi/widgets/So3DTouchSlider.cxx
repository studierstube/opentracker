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
/** So3DTouchSlider - implementation
  *
  * @author Hermann Wurnig, Gerd Hesina
  *
  * $Id: So3DTouchSlider.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <stbapi/widgets/So3DTouchSlider.h>
#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>

int So3DTouchSlider::initClassAttemptsSlider = 0;

SO_KIT_SOURCE(So3DTouchSlider);

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
So3DTouchSlider::addPositionChangeCallback(So3DTouchSliderCB *funcCB, void *userData)
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
So3DTouchSlider::removePositionChangeCallback(So3DTouchSliderCB *funcCB)
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
So3DTouchSlider::positionChangeCB(void *userData, SoSensor *)
{
	So3DTouchSlider *root = (So3DTouchSlider *) userData;
	int i;
	void (*function)(void *userData, So3DTouchSlider *slider);

		// if position changes then correct the curretValue-field
	root->position.setValue((root->currentValue.getValue()-
		root->minValue.getValue())/
	   (root->maxValue.getValue()-root->minValue.getValue()));

		// go through list of func-pointers
	for (i=0;i<root->funcList->getLength();i+=2)
	{		// and call each function
		function = (void (*)(void *, So3DTouchSlider *))
			(*(root->funcList))[i];
		(*function)((*(root->funcList))[i+1], root);
	}
}

/////////////////////////////////////////////////////////////////////
// Name:		minmaxChangeCB
// Purpose:		This function is called whenever the field minValue
//				or maxValue change its value. Then position is set to
//				the new value
// Parameters:	userData:	pointer to slider-object
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
So3DTouchSlider::minmaxChangeCB(void *userData, SoSensor *)
{
	So3DTouchSlider *root = (So3DTouchSlider *) userData;
		// if position changes then correct the curretValue-field
	root->setCurrentValue(root->currentValue.getValue());
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
So3DTouchSlider::setPosition(float newValue)
{
		// position has to be between 0 and 1, so change
		// appropriately when neccesary
	if (newValue < 0)
		newValue = 0;
	else if (newValue > 1)
		newValue = 1;

	newValue=newValue*(maxValue.getValue()-minValue.getValue())+
		minValue.getValue();

	setCurrentValue(newValue);
}

/////////////////////////////////////////////////////////////////////
// Name:		getPosition
// Purpose:		This function returns the sliders current position
// Parameters:	---
// Returnvalue: position of slider
/////////////////////////////////////////////////////////////////////
float
So3DTouchSlider::getPosition()
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
So3DTouchSlider::setCurrentValue(float newValue)
{		// newValue has to be between 0 and 1
	if (newValue < minValue.getValue())
		newValue = minValue.getValue();
	else if (newValue > maxValue.getValue())
		newValue = maxValue.getValue();
		// if it is different to current value, then set it
	if ( newValue != currentValue.getValue() )
    {
		currentValue.setValue(newValue);
    }
    
    position.setValue((newValue            - minValue.getValue()) /
			          (maxValue.getValue() - minValue.getValue()));

}

/////////////////////////////////////////////////////////////////////
// Name:		getCurrentValue
// Purpose:		This function returns the dials current value
// Parameters:	---
// Returnvalue: position of dial
/////////////////////////////////////////////////////////////////////
float
So3DTouchSlider::getCurrentValue()
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
So3DTouchSlider::setMinMax(float min, float max)
{		
	minValue=min;
	maxValue=max;
	setCurrentValue(currentValue.getValue());
}


//###################################################################
//## SETTING AND READING THE INCREMENT OF THE SLIDER
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		getNormalizedIncrement
// Purpose:		This function returns the sliders increment setting
//				as a value between 0 and 1
// Parameters:	---
// Returnvalue: normalized increment
/////////////////////////////////////////////////////////////////////
float
So3DTouchSlider::getNormalizedIncrement()
{
	return increment.getValue()/
		(maxValue.getValue()-minValue.getValue());
}

/////////////////////////////////////////////////////////////////////
// Name:		setIncrement
// Purpose:		This function sets the sliders increment-value
// Parameters:	value:	new value for increment
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DTouchSlider::setIncrement(float value)
{
	if (value < 0) increment = 0;
	else increment = value;
}

/////////////////////////////////////////////////////////////////////
// Name:		getIncrement
// Purpose:		This function returns the sliders current increment
// Parameters:	---
// Returnvalue: increment-value of slider
/////////////////////////////////////////////////////////////////////
float
So3DTouchSlider::getIncrement()
{
	return increment.getValue();
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
So3DTouchSlider::initClass()
{
	if (So3DTouchSlider::initClassAttemptsSlider == 0)
	{
		SoBase3DTouchSlider::initClass();

		So3DTouchSlider::initClassAttemptsSlider = 1;

		// use initialization-macro
		SO_KIT_INIT_CLASS(So3DTouchSlider, SoBase3DTouchSlider, "SoBase3DTouchSlider");
	}
}

/////////////////////////////////////////////////////////////////////
// Name:		initFields
// Purpose:		does all constructor-initialization
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DTouchSlider::initFields(float width, float radius,
					   float pos, float min, float max)
{
	if (pos > max) pos = max; 
	else if (pos < min) pos = min;
 	
	SO_KIT_CONSTRUCTOR(So3DTouchSlider);

		// declare fields and their default-values
	SO_KIT_ADD_FIELD(minValue,		(min));
	SO_KIT_ADD_FIELD(maxValue,		(max));
	SO_KIT_ADD_FIELD(currentValue,	(pos));
	SO_KIT_ADD_FIELD(increment,		(0));

	SO_KIT_INIT_INSTANCE();
		// initialize non-field member-variables

	this->width.setValue(width);
	this->radius.setValue(radius);

	position.setValue((pos-min)/(max-min));

	positionSensor = new SoFieldSensor(positionChangeCB, this);
	positionSensor->attach(&currentValue);

	SoFieldSensor *minSensor = new SoFieldSensor(minmaxChangeCB, this);
	minSensor->attach(&minValue);
	SoFieldSensor *maxSensor = new SoFieldSensor(minmaxChangeCB, this);
	maxSensor->attach(&maxValue);
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DTouchSlider
// Purpose:		Default-Constructor of class So3DTouchSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchSlider::So3DTouchSlider()
{
	initFields(50,3,0,0,1);
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DTouchSlider
// Purpose:		Constructor of class So3DTouchSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchSlider::So3DTouchSlider(float width, float radius, float position)
{
	initFields(width, radius, position,0,1);
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DTouchSlider
// Purpose:		Constructor of class So3DTouchSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchSlider::So3DTouchSlider(float width, float radius,
					   float min, float max, float position)
{
	initFields(width, radius, position, min, max);
}

/////////////////////////////////////////////////////////////////////
// Name:		~So3DTouchSlider
// Purpose:		Destructor of class So3DBSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DTouchSlider::~So3DTouchSlider()
{
}



