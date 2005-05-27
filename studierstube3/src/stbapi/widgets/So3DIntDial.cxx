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
/** So3DIntDial - implementation
  *
  * @author Hermann Wurnig, Gerd Hesina
  *
  * $Id: So3DIntDial.cxx 3781 2004-09-28 11:19:51Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <stbapi/widgets/So3DIntDial.h>
#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>
#include <stbapi/workspace/StbWorkspace.h>

int So3DIntDial::initClassAt = 0;

SO_KIT_SOURCE(So3DIntDial);

float roundFloat(float number)
{
	return floor(number+0.5);
}

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
So3DIntDial::addDialChangeCallback(So3DIntDialCB *funcCB, void *userData)
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
So3DIntDial::removeDialChangeCallback(So3DIntDialCB *funcCB)
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
So3DIntDial::positionChangeCB(void *userData, SoSensor *)
{
	So3DIntDial *root = (So3DIntDial *) userData;
	int i;
	void (*function)(void *userData, So3DIntDial *slider);

	if (root->currentValue.getValue() < root->minValue.getValue())
		root->currentValue.setValue(root->minValue.getValue());
	if (root->currentValue.getValue() > root->maxValue.getValue())
		root->currentValue.setValue(root->maxValue.getValue());

	if((root->getBase3DMode()!=Base3D::BASE3D_SLAVEMODE)||!root->validFlag)
	{
		root->currentSetValue=root->getCurrentValue();
		root->validFlag=TRUE;
	}

	DISABLEDIVFIELD(root->position);
	root->position.setValue(((float) root->currentValue.getValue()-
		root->minValue.getValue())/
	   (root->maxValue.getValue()-root->minValue.getValue()));
	root->oldPos=root->position.getValue();
	ENABLEDIVFIELD(root->position);

	// go through list of func-pointers
	for (i=0;i<root->funcList->getLength();i+=2)
	{		// and call each function
		function = (void (*)(void *, So3DIntDial *))
			(*(root->funcList))[i];
		(*function)((*(root->funcList))[i+1], root);
	}
}

/////////////////////////////////////////////////////////////////////
// Name:		minmaxChangeCB
// Purpose:		This function is called whenever the field minValue
//				or maxValue change. Then the new position of the
//				dial is set.
// Parameters:	userData:	pointer to dial-object
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
So3DIntDial::minmaxChangeCB(void *userData, SoSensor *)
{
	So3DIntDial *root = (So3DIntDial *) userData;
	root->setCurrentValue(root->currentValue.getValue());
}


//###################################################################
//## SETTING AND READING THE POSITION OF THE DIAL
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		setPosition
// Purpose:		This function sets the slider to a new position
// Parameters:	newValue:	new position of slider
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
So3DIntDial::setPosition(float newValue)
{		// newValue has to be between 0 and 1
	int newIntValue;
	
	if (newValue < 0)
		newValue = 0;
	else if (newValue > 1)
		newValue = 1;
		// if it is different to current value, then set it
	
	oldPos=newValue;
	
	newValue*=(maxValue.getValue()-minValue.getValue());
	newValue+=0.5;
	newIntValue = minValue.getValue() + (int) newValue;

	setCurrentValue(newIntValue);
}

/////////////////////////////////////////////////////////////////////
// Name:		getPosition
// Purpose:		This function returns the dials current position
// Parameters:	---
// Returnvalue: position of dial
/////////////////////////////////////////////////////////////////////
float
So3DIntDial::getPosition()
{		
	return oldPos;
}

/////////////////////////////////////////////////////////////////////
// Name:		setCurrentValue
// Purpose:		This function sets the slider to a new position
// Parameters:	newValue:	new value of slider btw. min and max
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
So3DIntDial::setCurrentValue(int newIntValue, SbBool forceUpdate)
{		// newValue has to be between min and max
	
	if (newIntValue < minValue.getValue())
		newIntValue = minValue.getValue();
	else if (newIntValue > maxValue.getValue())
		newIntValue = maxValue.getValue();


	if ((currentValue.getValue() != newIntValue) &&
	    ((callback.getValue()==ALWAYS) || (dragging == FALSE) || (forceUpdate)))
	{
		currentValue.setValue(newIntValue);
		if(getBase3DMode()!=Base3D::BASE3D_SLAVEMODE) currentSetValue=newIntValue;
	}

	DISABLEDIVFIELD(position);
		// correct the position-field in every case
	position.setValue(((float) newIntValue-
		minValue.getValue())/
		(maxValue.getValue()-minValue.getValue()));
	ENABLEDIVFIELD(position);
}

void So3DIntDial::setPositionUpdate(float newValue)
{
    setPosition(newValue);
	if((getBase3DMode()==Base3D::BASE3D_SLAVEMODE)&&validFlag)
	{
		if(((float)currentSetValue-minValue.getValue())/
			(maxValue.getValue()-minValue.getValue())!=getPosition())
        {
            if( snapBackSensor->isScheduled())
                snapBackSensor->unschedule();
			snapBackSensor->setTimeFromNow(SbTime(SNAPBACK_TIME));
			snapBackSensor->schedule(); //schedule a snapBack event
		}
		return;
	}
	if(getBase3DMode()==Base3D::BASE3D_MASTERMODE)
        SETDIALSTATE(this,getCurrentValue());
}
/////////////////////////////////////////////////////////////////////
// Name:		getCurrentValue
// Purpose:		This function returns the dials current value
// Parameters:	---
// Returnvalue: position of dial
/////////////////////////////////////////////////////////////////////
int
So3DIntDial::getCurrentValue()
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
So3DIntDial::setMinMax(int min, int max)
{		
	minValue=min;
	maxValue=max;
	setCurrentValue(currentValue.getValue());
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
So3DIntDial::initClass()
{
	if (So3DIntDial::initClassAt==0)
	{
		So3DIntDial::initClassAt=1;
		SoBase3DDial::initClass();

		// use initialization-macro
		SO_KIT_INIT_CLASS(So3DIntDial, SoBase3DDial, "SoBase3DDial");
	}
}


/////////////////////////////////////////////////////////////////////
// Name:		initFields
// Purpose:		defines the fields used by So3DIntDial
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void 
So3DIntDial::initFields(float radius, float height, int pos, 
	    				int min, int max)
{
	SO_KIT_CONSTRUCTOR(So3DIntDial);

		// declare fields and their default-values
	SO_KIT_ADD_FIELD(minValue,			(min));
	SO_KIT_ADD_FIELD(maxValue,			(max));
	SO_KIT_ADD_FIELD(currentValue,		(pos));

	SO_KIT_INIT_INSTANCE();

	this->height.setValue(height);
	this->radius.setValue(radius);

	position.setValue(((float) pos-min)/(max-min));
	oldPos=position.getValue();
		
	positionSensor = new SoFieldSensor(positionChangeCB, this);
	positionSensor->attach(&currentValue);

	SoFieldSensor *minSensor = 
		new SoFieldSensor(minmaxChangeCB, this);
	minSensor->attach(&minValue);
	SoFieldSensor *maxSensor = 
		new SoFieldSensor(minmaxChangeCB, this);
	maxSensor->attach(&maxValue);
    snapBackSensor->setFunction(snapBack);
    snapBackSensor->setData(this);
}


//###################################################################
//## CONSTRUCTORS/DESTRUCTOR
//###################################################################

/////////////////////////////////////////////////////////////////////
// Name:		So3DIntDial
// Purpose:		Default-Constructor of class So3DIntDial
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DIntDial::So3DIntDial()
{		// init fields
	initFields(5,3,0,0,100);
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DIntDial
// Purpose:		Constructor of class So3DIntDial that sets the dial
//				to a given size
// Parameters:	radius:	radius of dial
//				height: height of dial
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DIntDial::So3DIntDial(float radius, float height)
{	
	initFields(radius, height, 0, 0, 100);
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DIntDial
// Purpose:		Constructor of class So3DIntDial that sets the dial
//				to a given size
// Parameters:	radius:	radius of dial
//				height: height of dial
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DIntDial::So3DIntDial(float radius, float height, int min,
				   int max, int position)
{
	if (position > max) position=max;
	else if (position < min) position=min;
		// init fields
	initFields(radius, height, position, min, max);
}

/////////////////////////////////////////////////////////////////////
// Name:		~So3DIntDial
// Purpose:		Destructor of class So3DIntDial
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DIntDial::~So3DIntDial()
{
}

void So3DIntDial::updateFromNetwork(float newValue)
{
    if( snapBackSensor->isScheduled())
        snapBackSensor->unschedule();
	int intValue=(int)newValue;
//    fprintf(stderr,"so3ddial updatefromnetwork! value=%f\n",newValue);
	currentSetValue=intValue;
    validFlag=TRUE;
	setCurrentValue(intValue,TRUE); //force update
}

void
So3DIntDial::snapBack(void *data, SoSensor *sensor)
{
	So3DIntDial *self=(So3DIntDial*)data;
	if(self->validFlag)
        self->setCurrentValue(self->currentSetValue,TRUE); //force update
}

