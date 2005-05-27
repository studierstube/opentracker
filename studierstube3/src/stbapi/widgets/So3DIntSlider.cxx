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
/** So3DIntSlider - implementation
  *
  * @author Hermann Wurnig, Gerd Hesina, Jan Prikryl
  *
  * $Id: So3DIntSlider.cxx 3781 2004-09-28 11:19:51Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/widgets/So3DIntSlider.h>
#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>
#include <stbapi/workspace/StbWorkspace.h>

int So3DIntSlider::initClassAttemptsInt = 0;

SO_KIT_SOURCE(So3DIntSlider);

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
So3DIntSlider::addPositionChangeCallback(So3DIntSliderCB *funcCB, void *userData)
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
So3DIntSlider::removePositionChangeCallback(So3DIntSliderCB *funcCB)
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
So3DIntSlider::positionChangeCB(void *userData, SoSensor *)
{
    So3DIntSlider *root = (So3DIntSlider *) userData;
    int i;
    void (*function)(void *userData, So3DIntSlider *slider);

    if((root->getBase3DMode()!=Base3D::BASE3D_SLAVEMODE)||!root->validFlag)
    {
        root->currentSetValue=root->getCurrentValue();
        root->validFlag=TRUE;
    }

    DISABLEDIVFIELD(root->position);
    root->position.setValue(((float) root->currentValue.getValue()-
                             root->minValue.getValue())/
                            (root->maxValue.getValue()-root->minValue.getValue()));
    ENABLEDIVFIELD(root->position);

    // go through list of func-pointers
    for (i=0;i<root->funcList->getLength();i+=2)
    {		// and call each function
        function = (void (*)(void *, So3DIntSlider *))
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
So3DIntSlider::minmaxChangeCB(void *userData, SoSensor *)
{
    So3DIntSlider *root = (So3DIntSlider *) userData;
    // if position changes then correct the curretValue-field
    root->position.setValue(((float) root->currentValue.getValue()-
                             root->minValue.getValue())/
                            (root->maxValue.getValue()-root->minValue.getValue()));
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
So3DIntSlider::setPosition(float newValue)
{
    // position has to be between 0 and 1, so change
    // appropriately when neccesary
    if (newValue < 0)
        newValue = 0;
    else if (newValue > 1)
        newValue = 1;

    newValue *= (maxValue.getValue()-minValue.getValue());
    setCurrentValue(minValue.getValue() + (int) newValue);
}

/////////////////////////////////////////////////////////////////////
// Name:		getPosition
// Purpose:		This function returns the sliders current position
// Parameters:	---
// Returnvalue: position of slider
/////////////////////////////////////////////////////////////////////
float
So3DIntSlider::getPosition()
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
So3DIntSlider::setCurrentValue(int newValue, SbBool forceUpdate)
{		// newValue has to be between 0 and 1
    if (newValue > maxValue.getValue()) newValue=maxValue.getValue();
    else if (newValue < minValue.getValue()) newValue=minValue.getValue();

    if ((newValue != currentValue.getValue()) &&
        ((callback.getValue()==ALWAYS) || (dragging == FALSE) || (forceUpdate)))
    {
        currentValue.setValue(newValue);
        if(getBase3DMode()!=Base3D::BASE3D_SLAVEMODE) currentSetValue=newValue;
    }

    float newFloatValue = ((float) newValue-minValue.getValue())/
        (maxValue.getValue()-minValue.getValue());
    DISABLEDIVFIELD(position);
    position.setValue(newFloatValue);
    ENABLEDIVFIELD(position);
}

void So3DIntSlider::setPositionUpdate(float newValue)
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
        SETSLIDERSTATE(this,getCurrentValue());
}

/////////////////////////////////////////////////////////////////////
// Name:		getCurrentValue
// Purpose:		This function returns the dials current value
// Parameters:	---
// Returnvalue: position of dial
/////////////////////////////////////////////////////////////////////
int
So3DIntSlider::getCurrentValue()
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
So3DIntSlider::setMinMax(int min, int max)
{		
    minValue=min;
    maxValue=max;
    setCurrentValue(getCurrentValue());
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
So3DIntSlider::getNormalizedIncrement()
{
    return ((float) increment.getValue())/
        (maxValue.getValue()-minValue.getValue());
}

/////////////////////////////////////////////////////////////////////
// Name:		setIncrement
// Purpose:		This function sets the sliders increment-value
// Parameters:	value:	new value for increment
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DIntSlider::setIncrement(int value)
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
int
So3DIntSlider::getIncrement()
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
So3DIntSlider::initClass()
{
    if (So3DIntSlider::initClassAttemptsInt == 0)
    {
        SoBase3DSlider::initClass();

        So3DIntSlider::initClassAttemptsInt = 1;

        // use initialization-macro
        SO_KIT_INIT_CLASS(So3DIntSlider, SoBase3DSlider, "SoBase3DSlider");
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
So3DIntSlider::copy(SbBool copyConnections) const
{
    // use copy-function of SoNode for copying all public fields
    So3DIntSlider *newSlider = 
        (So3DIntSlider *) SoNode::copy(copyConnections);

    // and copy the rest explicitly
    newSlider->bodyHL = bodyHL;
    newSlider->sliderHL = sliderHL;
    newSlider->disableHL = disableHL;
    newSlider->position = position;
    newSlider->funcList = funcList;

    // return pointer to copy of node
    return newSlider;
}

/////////////////////////////////////////////////////////////////////
// Name:		initFields
// Purpose:		does all constructor-initialization
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
void
So3DIntSlider::initFields(float width, float radius,
                          int pos, int min, int max)
{
    if (pos > max) pos=max;
    else if (pos < min) pos=min;
	
	SO_KIT_CONSTRUCTOR(So3DIntSlider);

		// declare fields and their default-values
	SO_KIT_ADD_FIELD(minValue,		(min));
	SO_KIT_ADD_FIELD(maxValue,		(max));
	SO_KIT_ADD_FIELD(currentValue,	(pos));
	SO_KIT_ADD_FIELD(increment,		(0));

	SO_KIT_INIT_INSTANCE();
		// initialize non-field member-variables
	validFlag=FALSE;

	setPosition(pos);

	this->width.setValue(width);
	this->radius.setValue(radius);

	position.setValue((int) (((float)(pos-min))/(max-min)));

	SoFieldSensor *positionSensor = new SoFieldSensor(positionChangeCB, this);
	positionSensor->attach(&currentValue);

	SoFieldSensor *minSensor = new SoFieldSensor(minmaxChangeCB, this);
	minSensor->attach(&minValue);
	SoFieldSensor *maxSensor = new SoFieldSensor(minmaxChangeCB, this);
	maxSensor->attach(&maxValue);
    snapBackSensor->setFunction(snapBack);
    snapBackSensor->setData(this);
}


/////////////////////////////////////////////////////////////////////
// Name:		So3DIntSlider
// Purpose:		Default-Constructor of class So3DIntSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DIntSlider::So3DIntSlider()
{
    initFields(50,3,0,0,100);
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DIntSlider
// Purpose:		Constructor of class So3DIntSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DIntSlider::So3DIntSlider(float width, float radius,
                             float position)
{
    initFields(width, radius, (int)position, 0, 100);
}

/////////////////////////////////////////////////////////////////////
// Name:		So3DIntSlider
// Purpose:		Constructor of class So3DIntSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DIntSlider::So3DIntSlider(float width, float radius,
                             int min, int max, float position)
{
    initFields(width, radius, (int)position, min, max);
}

/////////////////////////////////////////////////////////////////////
// Name:		~So3DIntSlider
// Purpose:		Destructor of class So3DBSlider
// Parameters:	---
// Returnvalue: ---
/////////////////////////////////////////////////////////////////////
So3DIntSlider::~So3DIntSlider()
{
}

void So3DIntSlider::updateFromNetwork(float newValue)
{
    int intValue=(int)newValue;
//    fprintf(stderr,"so3dslider updatefromnetwork! value=%f\n",newValue);
    currentSetValue=intValue;
    validFlag=TRUE;
	setCurrentValue(intValue,TRUE); //force update
}

void
So3DIntSlider::snapBack(void *data, SoSensor *sensor)
{
	So3DIntSlider *self=(So3DIntSlider*)data;
	if(self->validFlag)
        self->setCurrentValue(self->currentSetValue,TRUE); //force update
}

/* ===========================================================================
    End of So3DIntSlider.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */
