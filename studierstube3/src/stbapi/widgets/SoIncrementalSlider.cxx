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
/** source file for SoIncrementalSlider class.
*
* @author Thomas Psik
*
* $Id: SoIncrementalSlider.cxx 4139 2005-03-22 09:59:04Z kaufmann $
* @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoTransform.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTranslation.h>

#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodes/SoCube.h>

#include <stbapi/widgets/SoIncrementalSlider.h>
#include <stbapi/widgets/SoPushButton.h>
#include <stbapi/widgets/SoLabelKit.h>
#include <stbapi/widgets/SoTooltipKit.h>

/*#include <stbapi/styles/SoAbstractWidgetStyle.h>
#include <stbapi/styles/SoSliderStyle.h>
#include <stbapi/styles/SoButtonStyle.h>
*/

#include <stbapi/misc/SoAbortGroup.h>


#include <stbapi/util/ivio.h>

//#define _DEBUG_INCREMENTAL_SLIDER

#if defined(_DEBUG_INCREMENTAL_SLIDER) && defined(_DEBUG)
#define DEBUG_CODE(a) a;
#else
#define DEBUG_CODE(a) ;
#endif


#define ADD SO_KIT_ADD_CATALOG_ENTRY

SO_KIT_SOURCE(SoIncrementalSlider);


/**
* the open inventor initialization
*/
void SoIncrementalSlider::initClass()
{
    SO_KIT_INIT_CLASS(SoIncrementalSlider, SoBehaviorKit, "SoBehaviorKit");
}

/**
* the constructor inits all fields
*/
SoIncrementalSlider::SoIncrementalSlider()
{
	
	SO_KIT_CONSTRUCTOR(SoIncrementalSlider);
	
	// Add the body parts to the catalog...
	//-----------------------+------------+------+---------------+-------+---------------
	//                       |            |      |               | right |
	//                       |            | null |               | Sib-  | is
	//                       |            | By   |               | ling  | public
	//         name          | className  | Def. |  parent Name  | Name  | part
	//-----------------------+------------+------+---------------+-------+---------------
	
	
	ADD(buttonSeparator,	SoSeparator,	FALSE,	this,				\x0, TRUE);
	ADD(buttonAbortGroup,	SoAbortGroup,	FALSE,  buttonSeparator,    \x0, TRUE);
	ADD(upTransform,		SoTransform,	FALSE,  buttonAbortGroup,   \x0, TRUE);
	ADD(up,					SoPushButton,   FALSE,  buttonAbortGroup,   \x0, TRUE);
	ADD(downTransform,		SoTransform,	FALSE,  buttonAbortGroup,   \x0, TRUE);
	ADD(down,				SoPushButton,   FALSE,  buttonAbortGroup,   \x0, TRUE);
	
	ADD(sliderSeparator,	SoSeparator,	FALSE,  this,				\x0, TRUE);
	ADD(sliderAbortGroup,	SoAbortGroup,	FALSE,  sliderSeparator,    \x0, TRUE);
	ADD(sliderTranslation,	SoTranslation,	FALSE,  sliderAbortGroup,   \x0, TRUE);
	ADD(slider,				SoSimpleSlider,	FALSE,  sliderAbortGroup,   \x0, TRUE);

	SO_KIT_ADD_FIELD(value	,			(0.0f));
    SO_KIT_ADD_FIELD(valueIn,			(0.0f));
	SO_KIT_ADD_FIELD(alpha,				(0.0f));
    SO_KIT_ADD_FIELD(alphaIn,			(0.0f));
	SO_KIT_ADD_FIELD(minValue,			(0.0f));
	SO_KIT_ADD_FIELD(maxValue,			(10.0f));
	SO_KIT_ADD_FIELD(cropValueToIncrement,	(FALSE));
	SO_KIT_ADD_FIELD(widthDragger,		(0.05f));
	SO_KIT_ADD_FIELD(scaleDraggerPath,	(0.8f));
	// add fields
	SO_KIT_ADD_FIELD(hideSlider,		(FALSE));
	SO_KIT_ADD_FIELD(hideButtons,		(FALSE));
	SO_KIT_ADD_FIELD(buttonSpace,		(0.15f)); 
	SO_KIT_ADD_FIELD(increment,			(1.0f)); 
	SO_KIT_ADD_FIELD(interval,			(0.2f));

	SO_KIT_ADD_FIELD(startDragging,		());
	SO_KIT_ADD_FIELD(finishedDragging,	());

	
	SO_KIT_INIT_INSTANCE();
	
	upSensor.setData(this);
	upSensor.setFunction( upPressed );
	
	downSensor.setData(this);
	downSensor.setFunction( downPressed );

	alphaSensor.setData(this);
	alphaSensor.setFunction(alphaChangedCB);
	alphaSensor.setPriority(120);

	hideSliderSensor.setData(this);
	hideSliderSensor.setFunction(hideSliderChangedCB);

	hideButtonsSensor.setData(this);
	hideButtonsSensor.setFunction(hideButtonsChangedCB);

	for (int i=0; i<3; i++)
	{
		insideSensor[i].setData(this);
		insideSensor[i].setFunction(insideChangedCB);
	}

	SO_GET_PART(this, "up", SoPushButton)->setPart("label", readNodeFromMemory(defaultUpButtonLabel)->copy());
	SO_GET_PART(this, "down", SoPushButton)->setPart("label", readNodeFromMemory(defaultDownButtonLabel)->copy());

	setUpConnections(TRUE, TRUE);
}


SoIncrementalSlider::~SoIncrementalSlider()
{
}

// --------------------------------------------------------------------------

SbBool SoIncrementalSlider::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoBehaviorKit::setUpConnections(onOff, doItAlways);
		
		// setup the widget
	
		float intervalValue = (float) interval.getValue().getValue();	
		SoPushButton *button = SO_GET_PART(this, "up", SoPushButton);
		assert(button != NULL);
		upSensor.attach( &button->triggerOut );
		button->repeatTrigger.setValue(TRUE);
		button->repeatTriggerStartInterval.setValue(intervalValue*2.0);
		button->repeatTriggerInterval.setValue(intervalValue);
		button->setUpConnections(onOff, doItAlways);
		insideSensor[0].attach(&button->inside);	

		button = SO_GET_PART(this, "down", SoPushButton);
		assert(button != NULL);
		downSensor.attach( &button->triggerOut );
		button->repeatTrigger.setValue(TRUE);
		button->repeatTriggerInterval.setValue(intervalValue);
		button->repeatTriggerStartInterval.setValue(intervalValue*2.0);
		button->setUpConnections(onOff, doItAlways);
		insideSensor[1].attach(&button->inside);


		SoSimpleSlider *slid = SO_GET_PART(this, "slider", SoSimpleSlider);
		slid->setUpConnections(onOff, doItAlways);
		
		slid->maxValue.connectFrom(&maxValue);
		slid->minValue.connectFrom(&minValue);
		slid->alphaIn.connectFrom(&alphaIn);
		slid->widthDragger.connectFrom(&widthDragger);
		slid->scaleDraggerPath.connectFrom(&scaleDraggerPath);
		slid->valueIn.connectFrom(&valueIn);
		slid->valueIn.setValue(valueIn.getValue());
		slid->setStartCallback(startDraggingCB, this);
		slid->setFinishCallback(finishDraggingCB, this);
		
		value.connectFrom(&slid->value);
		alpha.connectFrom(&slid->alpha);
		
		hideSliderSensor.attach(&hideSlider);
		hideSlider.touch();

		hideButtonsSensor.attach(&hideButtons);
		hideButtons.touch();

		alphaSensor.attach(&alpha);

		insideSensor[2].attach(&slid->inside);
	}
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.
		float intervalValue = (float) interval.getValue().getValue();	
		SoPushButton *button = SO_GET_PART(this, "up", SoPushButton);
		assert(button != NULL);
		upSensor.detach( );
		button->setUpConnections(onOff, doItAlways);
	
		button = SO_GET_PART(this, "down", SoPushButton);
		assert(button != NULL);
		downSensor.detach( );
		button->setUpConnections(onOff, doItAlways);

		SoSimpleSlider *slid = SO_GET_PART(this, "slider", SoSimpleSlider);
		slid->setUpConnections(onOff, doItAlways);
		
		slid->maxValue.disconnect();
		slid->minValue.disconnect();
		slid->alphaIn.disconnect();
		slid->widthDragger.disconnect();
		slid->scaleDraggerPath.disconnect();
		slid->valueIn.disconnect();
		slid->valueIn.disconnect();
		value.disconnect();
		alpha.disconnect();
		
		hideSliderSensor.detach();
		alphaSensor.detach();

		for (int i=0; i<3; i++)
		{
			insideSensor[i].detach();
		}
	
		SoBehaviorKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);
	
}

void SoIncrementalSlider::adaptToEnable()
{
	SoSimpleSlider *slid = SO_GET_PART(this, "slider", SoSimpleSlider);
	assert(slid != NULL);
	slid->enable.setValue(enable.getValue());
	SoPushButton *button = SO_GET_PART(this, "up", SoPushButton);
	assert(button != NULL);
	button->enable.setValue(enable.getValue());
	
	button = SO_GET_PART(this, "down", SoPushButton);
	assert(button != NULL);
	button->enable.setValue(enable.getValue());
	
}

// ---------------------------------------------------------------

void SoIncrementalSlider::adaptToTooltips()
{
	if (tooltip.getValue() != NULL) 
	{
		SoTooltipKit *tooltipPtr = (SoTooltipKit *)tooltip.getValue();
		// copy tooltip to sub elements and delete it from this class
		SoPushButton *button = SO_GET_PART(this, "up", SoPushButton);
		assert(button != NULL);

		SoTooltipKit *childTooltip = (SoTooltipKit *)tooltipPtr->copy();
		childTooltip->setMasterTooltip(tooltipPtr);
		button->setPart("tooltip", childTooltip);
		
		button = SO_GET_PART(this, "down", SoPushButton);
		assert(button != NULL);
		button->setPart("tooltip", childTooltip);
		
		SoSimpleSlider *slid = SO_GET_PART(this, "slider", SoSimpleSlider);
		slid->setPart("tooltip", childTooltip);
		
	}

	if (tooltipDisabled.getValue() == NULL) return;

	// copy tooltip to sub elements and delete it from this class

	SoTooltipKit *childTooltip = (SoTooltipKit *)tooltipDisabled.getValue()->copy();
	childTooltip->setMasterTooltip((SoTooltipKit *)tooltipDisabled.getValue());
	
	SoPushButton *button = SO_GET_PART(this, "up", SoPushButton);
	assert(button != NULL);
	button->setPart("tooltipDisabled", childTooltip);
	
	button = SO_GET_PART(this, "down", SoPushButton);
	assert(button != NULL);
	button->setPart("tooltipDisabled", childTooltip);
	
	SoSimpleSlider *slid = SO_GET_PART(this, "slider", SoSimpleSlider);
	slid->setPart("tooltipDisabled", childTooltip);
}


// ---------------------------------------------------------------

void SoIncrementalSlider::resizeWidgetCB()
{
	DEBUG_CODE(printf("SoIncrementalSlider::resizeWidgetCB()\n"););
	SoBehaviorKit::resizeWidgetCB();
	
	((SoScale*)scale.getValue())->scaleFactor.setValue(1,1,1);
	// set up geometry for sub widgets
	float widthV = widgetDim[0];
	//widthV = 1.0f;
	float buttonSp = buttonSpace.getValue();

	if (hideButtons.getValue() == TRUE)
		buttonSp = 0.0f;
	
	SoSimpleSlider *slid = SO_GET_PART(this, "slider", SoSimpleSlider);
	assert(slid != NULL);
	DEBUG_CODE(printf("widthV %f buttonSp %f slider width %f\n", widthV, buttonSp, (1.0-buttonSp*2.0)*widthV));

	slid->width.setValue((1.0-buttonSp*2.0)*widthV );//(1.0-buttonSp*2.0)*widthV);
	slid->height.setValue(widgetDim[1]);//widgetDim[1]);
	slid->depth.setValue(widgetDim[2]); //;
	
	SoTranslation *sliderTrans = SO_GET_PART(this, "sliderTranslation", SoTranslation);
	assert(sliderTrans  != NULL);
	sliderTrans ->translation.setValue(widgetMove[0], widgetMove[1], widgetMove[2]);
	
	if (hideButtons.getValue() == FALSE)
	{
		SoTransform *trans = SO_GET_PART(this, "upTransform", SoTransform);
		assert(trans != NULL);
		trans->translation.setValue(widgetMove[0]+(-.5+buttonSp/2.0)*widthV, widgetMove[1], widgetMove[2]);
		
		SoPushButton *button = SO_GET_PART(this, "up", SoPushButton);
		assert(button != NULL);
		button->width.setValue(buttonSp*widthV);
		button->height.setValue(widgetDim[1]);
		button->depth.setValue(widgetDim[2]);
		//	button->enable.setValue(FALSE);

		trans = SO_GET_PART(this, "downTransform", SoTransform);
		assert(trans);
		// watch out to include up transform in calculation (no separator !!)
		trans->translation.setValue((1-buttonSp)*widthV, widgetMove[1], 0);

		button = SO_GET_PART(this, "down", SoPushButton);
		assert(button != NULL);
		button->width.setValue(buttonSp*widthV);
		button->height.setValue(widgetDim[1]);
		button->depth.setValue(widgetDim[2]);
		//	button->enable.setValue(FALSE);
	}
	

}

// ---------------------------------------------------------------

void SoIncrementalSlider::startDraggingCB(void* userData, SoDraggerKit*)
{
	SoIncrementalSlider *self = (SoIncrementalSlider*) userData;
	assert(self);
	// fire startDragging trigger
	self->startDragging.setValue();
}
	
void SoIncrementalSlider::finishDraggingCB(void* userData, SoDraggerKit*)
{
	SoIncrementalSlider *self = (SoIncrementalSlider*) userData;
	assert(self);
	// fire finishDragging trigger
	self->finishedDragging.setValue();
}

// ---------------------------------------------------------------

void SoIncrementalSlider::upPressed( void * data, SoSensor * sensor  )
{
	DEBUG_CODE(printf("SoIncrementalSlider::upPressed"););

	SoIncrementalSlider *self = (SoIncrementalSlider*) data;
	assert(self);
	// fire startDragging trigger
	self->startDragging.setValue();
	float value = self->value.getValue();
	SoSimpleSlider *slid = SO_GET_PART(self, "slider", SoSimpleSlider);
	slid->valueIn.setValue(value - self->increment.getValue());	
	// fire finishDragging trigger
	self->finishedDragging.setValue();
}

void SoIncrementalSlider::downPressed( void * data, SoSensor * sensor  )
{
	DEBUG_CODE(printf("SoIncrementalSlider::downPressed"););
	SoIncrementalSlider *self = (SoIncrementalSlider*) data;
	assert(self);
	// fire startDragging trigger
	self->startDragging.setValue();
	
	float value = self->value.getValue();
	SoSimpleSlider *slid = SO_GET_PART(self, "slider", SoSimpleSlider);
	slid->valueIn.setValue(value + self->increment.getValue());	
	
	// fire finishDragging trigger
	self->finishedDragging.setValue();

}

// ---------------------------------------------------------------

void SoIncrementalSlider::alphaChangedCB(void *data, SoSensor *sensor)
{
	SoIncrementalSlider *self = (SoIncrementalSlider*) data;
	assert(self);
	if (self->cropValueToIncrement.getValue() == TRUE)
	{
		float alpha = self->alpha.getValue() ;
		float min = self->minValue.getValue() ;
		float scale = self->maxValue.getValue() - min;
		float value_min = alpha * scale;
		// crop value (without min) to increment and loop back to alphaIn
		value_min -= fmod(value_min, self->increment.getValue());
	//	value += min;
	 	SoSimpleSlider *slid = SO_GET_PART(self, "slider", SoSimpleSlider);
		slid->alphaIn.setValue(value_min/scale);
	}
}

// ---------------------------------------------------------------

void SoIncrementalSlider::hideSliderChangedCB(void *data, SoSensor *)
{
	SoIncrementalSlider *self = (SoIncrementalSlider*) data;
	assert(self);
	SoSFBitMask *abortgrpBM = &(((SoAbortGroup*)self->sliderAbortGroup.getValue())->abortActions);
	if (self->hideSlider.getValue()== TRUE)
	{
		if (self->hideButtons.getValue()== TRUE)
			self->hideButtons.setValue(FALSE);

		abortgrpBM->setValue(SoAbortGroup::RENDER | SoAbortGroup::EVENT3D);
	}
	else
		abortgrpBM->setValue(SoAbortGroup::NONE);

}

// ---------------------------------------------------------------

void SoIncrementalSlider::hideButtonsChangedCB(void *data, SoSensor *)
{
	SoIncrementalSlider *self = (SoIncrementalSlider*) data;
	assert(self);
	SoSFBitMask *abortgrpBM = &(((SoAbortGroup*)self->buttonAbortGroup.getValue())->abortActions);
	if (self->hideButtons.getValue()== TRUE)
	{
		if (self->hideSlider.getValue()== TRUE)
			self->hideSlider.setValue(FALSE);

		abortgrpBM->setValue(SoAbortGroup::RENDER | SoAbortGroup::EVENT3D);
	}
	else
		abortgrpBM->setValue(SoAbortGroup::NONE);
	
	// resize widget to set new visual style (buttons / no buttons)
	self->resizeWidgetCB();
}

// ---------------------------------------------------------------

void SoIncrementalSlider::insideChangedCB(void *data, SoSensor *sensor)
{
	SoIncrementalSlider *self = (SoIncrementalSlider*) data;
	assert(self);

	SoFieldSensor *fSensor = (SoFieldSensor *)sensor;
	SoBehaviorKit *curWidget = (SoBehaviorKit *)fSensor->getAttachedField()->getContainer();

	self->inside.setValue(curWidget->inside.getValue());
}


// ---------------------------------------------------------------

const char *SoIncrementalSlider::defaultUpButtonLabel =
"	SoLabelKit {"
"	 appearance SoAppearanceKit {  material SoMaterial {diffuseColor .2 .8 .2	} }"
" 	 contents SoSeparator { SoShapeKit { coordinate3 Coordinate3 { point [0.4 0.4 0 , -0.4 0 0  , 0.4  -0.4 0] }"
"    shape FaceSet {}"
"    }}"
"  }";


const char *SoIncrementalSlider::defaultDownButtonLabel =
"SoLabelKit {"
"	 appearance SoAppearanceKit {  material SoMaterial {diffuseColor .2 .8 .2	} }"
" 	 contents SoSeparator { SoShapeKit { coordinate3 Coordinate3 { point [0.4 0.4 0 , -0.4 0 0  , 0.4  -0.4 0] }"
"	 coordinate3 Coordinate3 { point [-0.4  -0.4 0, 0.4 0 0, -0.4 0.4 0] }"
"    shape FaceSet {}"
"    }}"
"}";
