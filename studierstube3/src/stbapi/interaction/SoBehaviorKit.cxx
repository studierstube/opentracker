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
/** source file for SoBehaviorKit class.
  *
  * @author Zsolt Marx
  *
  * $Id: SoBehaviorKit.cxx 4137 2005-03-22 09:57:22Z kaufmann $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>

#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoAsciiText.h>

#include <stbapi/misc/SoAbortGroup.h>
#include <stbapi/interaction/SoBehaviorKit.h>
#include <stbapi/widgets/SoLabelKit.h>
#include <stbapi/widgets/SoTooltipKit.h>

#include <assert.h>



#define NO_WARNINGS
//#define _DEBUG_BEHAVIOUR_KIT

#if defined(_DEBUG_BEHAVIOUR_KIT) && defined(_DEBUG)
#undef NO_WARNINGS
#define DEBUG_CODE(a) a;
#else
#define DEBUG_CODE(a) ;
#endif

#if defined(NO_WARNINGS)
#define WARNING_CODE(a) ;
#else
#define WARNING_CODE(a) a;
#endif


#define ADD SO_BEHAVIOR_ADD_CATALOG_ENTRY

SO_BEHAVIOR_SOURCE(SoBehaviorKit);

/**
 * initClass
 */
void SoBehaviorKit::initClass()
{
	SO_BEHAVIOR_INIT_CLASS(SoBehaviorKit, SoLayoutKit, "SoLayoutKit");
}

/**
 * constructor
 */
SoBehaviorKit::SoBehaviorKit()
{
	SO_BEHAVIOR_CONSTRUCTOR(SoBehaviorKit);

	//-----------------------+------------+------+---------------+-------+---------------
	//                       |            |      |               | right |
	//                       |            | null |               | Sib-  | is
	//                       |            | By   |               | ling  | public
	//         name          | className  | Def. |  parent Name  | Name  | part
	//-----------------------+------------+------+---------------+-------+---------------


   	ADD(translationOffset,          SoTranslation,FALSE, base,            scale,    TRUE);
 	ADD(geometrySwitch,       SoSwitch,     FALSE, geometry,        \x0,    TRUE);

    ADD(hysteresisSep,        SoSeparator,  FALSE, base,            \x0,   TRUE);
    ADD(hysteresisMaterial,   SoMaterial,   TRUE,  hysteresisSep,   \x0,   TRUE);
    ADD(invisibility,         SoDrawStyle,  FALSE, hysteresisSep,   \x0,   TRUE);
    ADD(hysteresisSwitch,     SoSwitch,     FALSE, hysteresisSep,   \x0,   TRUE);
 	ADD(hysteresisCube,       SoCube,       FALSE, hysteresisSwitch,\x0,   TRUE);

	ADD(abortGroup,			  SoAbortGroup,FALSE, base,			  translationOffset,    TRUE);
	ADD(label,				  SoLabelKit,  TRUE,  abortGroup,	  \x0,    TRUE);
	ADD(tooltip,			  SoTooltipKit,TRUE,  abortGroup,	  \x0,    TRUE);
	ADD(tooltipDisabled,	  SoTooltipKit,TRUE,  abortGroup,	  \x0,    TRUE);
	
  	
	SO_BEHAVIOR_ADD_FIELD(enable, (TRUE));
    SO_BEHAVIOR_ADD_FIELD(hysteresisBoxScale, (1.0));
	SO_BEHAVIOR_ADD_FIELD(hysteresisBoxScaleHeight, (1.0));
	SO_BEHAVIOR_ADD_FIELD(applyWidgetStyle, (TRUE));
	SO_BEHAVIOR_ADD_FIELD(styleName, (""));
	SO_BEHAVIOR_ADD_FIELD(inside, (FALSE));
	SO_BEHAVIOR_ADD_FIELD(statusBoxText, (""));

	SO_NODE_DEFINE_ENUM_VALUE(LabelPlacing, NONE);
	SO_NODE_DEFINE_ENUM_VALUE(LabelPlacing, TOP);
	SO_NODE_DEFINE_ENUM_VALUE(LabelPlacing, LEFT);
	SO_NODE_DEFINE_ENUM_VALUE(LabelPlacing, RIGHT);
	SO_NODE_DEFINE_ENUM_VALUE(LabelPlacing, BOTTOM);
	SO_NODE_DEFINE_ENUM_VALUE(LabelPlacing, ONWIDGET);
	
	SO_NODE_SET_SF_ENUM_TYPE(labelPlacing, LabelPlacing);
	
	SO_BEHAVIOR_ADD_FIELD( labelPlacing, (NONE));
//	SO_BEHAVIOR_ADD_FIELD( labelPlacingInt, (NONE));

	SO_BEHAVIOR_INIT_INSTANCE();
	
	// set up the transparency material

	SoDrawStyle *inv = new SoDrawStyle();
	inv->style = SoDrawStyle::INVISIBLE;
	setPart("invisibility", inv);

	SoMaterial *hysMat = new SoMaterial();
	hysMat->transparency.setValue(.4);
	setPart("hysteresisMaterial", hysMat);


	SoAbortGroup *abortGrp = new SoAbortGroup();
	abortGrp->abortActions = SoAbortGroup::BBOX;
	setPart("abortGroup", abortGrp);

	oldLabelPtr = NULL;

	// setup field connections

	labelPlacingSensor.setFunction(labelPlacingChangedCB);
	labelPlacingSensor.setData(this);

	labelSensor.setFunction(labelChangedCB);
	labelSensor.setData(this);

	enableSensor.setFunction(enableChangedCB);
	enableSensor.setData(this);

	tooltipSensor.setFunction(tooltipChangedCB);
	tooltipSensor.setData(this);
	
	tooltipDisabledSensor.setFunction(tooltipDisabledChangedCB);
	tooltipDisabledSensor.setData(this);

	selfActivation = FALSE;

    setUpConnections(TRUE, TRUE);
}

/** 
 * destructor
 */
SoBehaviorKit::~SoBehaviorKit()
{
	delete inspector;
}


SbBool SoBehaviorKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{

		// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoLayoutKit::setUpConnections(onOff, doItAlways);
        // setup the widget
   		inspector = new StbEventInspector(this);

		// connect width, height, depth to the appropriate scale values
	
		labelPlacingSensor.attach(&labelPlacing);
		labelSensor.attach(&label);
		enableSensor.attach(&enable);
		tooltipSensor.attach(&tooltip);
		tooltipDisabledSensor.attach(&tooltipDisabled);

		
		SoLabelKit *label = (SoLabelKit *)getPart("label", FALSE);
		if (label!= NULL)
			label->setUpConnections(onOff, doItAlways);
		
		labelPlacing.touch();
		enable.touch();
		
		// set up the event receiver list
		eventReceiverList = new SoChildList(this);

    }
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.
		labelPlacingSensor.detach();
		labelSensor.detach();

		tooltipSensor.detach();
		tooltipDisabledSensor.detach();

		delete eventReceiverList;
		eventReceiverList = NULL;

		SoLabelKit *label = (SoLabelKit *)getPart("label", FALSE);
		if (label!= NULL)
			label->setUpConnections(onOff, doItAlways);

        SoLayoutKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);

}


void SoBehaviorKit::labelChangedCB(void *data, SoSensor*)
{
	SoBehaviorKit* self = (SoBehaviorKit*)data;
	assert(self != NULL);
	if (self->label.getValue() != self->oldLabelPtr)
	{
		self->oldLabelPtr = self->label.getValue();
		self->adaptToLabelPlacing();
	}
}

void SoBehaviorKit::labelPlacingChangedCB(void *data, SoSensor*)
{
	SoBehaviorKit* self = (SoBehaviorKit*)data;
	assert(self != NULL);
	DEBUG_CODE(printf("SoBehaviorKit::labelPlacingChangedCB\n"));
//	self->labelPlacing.setValue(self->labelPlacingInt.getValue());
	self->adaptToLabelPlacing();
	self->resizeWidgetCB();
}

void SoBehaviorKit::adaptToLabelPlacing()
{
	SoScale *scalePart = SO_GET_PART(this, "scale", SoScale);
	assert(scalePart != NULL);

	SoTranslation *translPart = SO_GET_PART(this, "translationOffset", SoTranslation);
	assert(translPart != NULL);

	SbVec3f dim(width.getValue(), height.getValue(), depth.getValue());
	SbVec3f move(0, 0, 0);

	SoLabelKit *labelPart = (SoLabelKit *)getPart("label", FALSE);
	if (labelPart!= NULL)
	{
		SbVec3f moveLabel(0, .12, 0);
		SbVec3f moveLabel2(0, .12, 0);

		if (labelPlacing.getValue() == LEFT)

		{
			dim[0] = dim[0] / 2.0f;	
			move[0] = dim[0] / 2.0f;
		}
		else if (labelPlacing.getValue() == RIGHT)
		{
			dim[0] = dim[0] / 2.0f;	
			move[0] = -dim[0] / 2.0f;
		}
		else if (labelPlacing.getValue() == TOP)
		{
			dim[2] = dim[2] / 2.0f;	
			move[2] = dim[2] / 2.0f;

		}
		else if (labelPlacing.getValue() == BOTTOM)
		{
			dim[2] = dim[2] / 2.0f;	
			move[2] = -dim[2] / 2.0f;
		}
		else // ONWIDGET || NONE if (labelPlacing.getValue() == ONWIDGET)
		{
			moveLabel[1] = 1.05;
			setLabel2Translation(moveLabel2[1]);
		}
		
		moveLabel[0] = -move[0];
		moveLabel[2] = -move[2];
		moveLabel2[0] = -move[0];
		moveLabel2[2] = -move[2];

		labelSensor.detach();
		labelPart->resizeToWidgetCB(dim, moveLabel, moveLabel2);
		labelSensor.attach(&label);
	}
	
	widgetDim = dim;
	widgetMove = move;
	
	scalePart->scaleFactor.setValue(dim);
//	printf("scale set to %f %f %f\n",dim[0],dim[1],dim[2]);
	translPart->translation.setValue(move);
}

void SoBehaviorKit::setLabel2Translation(float &moveLabel2_1)
{
	moveLabel2_1 = 1.05;
}


void SoBehaviorKit::resizeCB()
{
	resizeWidgetCB();
}

void SoBehaviorKit::resizeWidgetCB()
{
	adaptToLabelPlacing();
}


void SoBehaviorKit::enableChangedCB(void *data, SoSensor*)
{
	SoBehaviorKit* self = (SoBehaviorKit*)data;
	assert(self != NULL);
	self->adaptToEnable();
}

void SoBehaviorKit::adaptToEnable()
{
	// nil should be implemented by subclasses
}

/**
 * handles the low level event, the application programmer
 * can use the event inspector to check for certain gestures
 * in the event.
 */
void SoBehaviorKit::handle3DEvent(SoHandle3DEventAction *h3a)
{
//	printf("SoBehaviorKit::handle3DEvent called\n");
}
/*
void SoBehaviorKit::inspect(const SoHandle3DEventAction *h3a)
{
	inspector->examineEvent((SoHandle3DEventAction *) h3a);
}
*/
/**
 * returns whether the event ocurred in the kit's bounding
 * box
 */
SbBool SoBehaviorKit::isInside(const SoHandle3DEventAction *h3a)
{

	return inspector->isInside();
}


void SoBehaviorKit::examineEvent(const SoHandle3DEventAction *h3a)
{
	inspector->examineEvent((SoHandle3DEventAction *) h3a);
}

bool SoBehaviorKit::detectAction(SoMFNode &gestures)
{
  for(int i = 0; i < gestures.getNum(); ++i)
  {
    SoGesture *gesture = (SoGesture*)(gestures[i]);
    if(inspector->detect(gesture))
      return(true);
  }
  return(false);
}

// --------------------------------------------------------------------------

SbBool SoBehaviorKit::isInterested()
{
	return inspector->isInterested();
}

SoSwitch* SoBehaviorKit::getGeometrySwitch()
{
	return ((SoSwitch*) geometrySwitch.getValue());
}

// --------------------------------------------------------------------------

SoSwitch* SoBehaviorKit::getHysteresisSwitch()
{
	return ((SoSwitch*) hysteresisSwitch.getValue());
}

SoCube* SoBehaviorKit::getHysteresisCube()
{
	return ((SoCube*) hysteresisCube.getValue());
}

void SoBehaviorKit::setHysteresis(bool on)
{
	SoTooltipKit *tooltipKit = (SoTooltipKit *)getPart("tooltip", FALSE);
	SoTooltipKit *tooltipDisabledKit = (SoTooltipKit *)getPart("tooltipDisabled", FALSE);

    if (on)
    {
        SbViewportRegion vp;
        SoGetBoundingBoxAction bba(vp);

        // tell Open Inventor not to reflect the scene graph changes in the display.
        enableNotify(false);

        // memorize the actual geometry
        int actualGeometry = getGeometrySwitch()->whichChild.getValue();

        // make the switch behave like a group
        getGeometrySwitch()->whichChild = SO_SWITCH_ALL;

        // and now calculate the union bounding box of all children
        getGeometrySwitch()->ref();
        bba.apply(getGeometrySwitch());
        getGeometrySwitch()->unrefNoDelete();

        // change everything back again
        getGeometrySwitch()->whichChild = actualGeometry;

        // put the bounding box into the hysteresis
        float sizeX;
        float sizeY;
        float sizeZ;
        SbXfBox3f bbox = bba.getXfBoundingBox();
        bbox.getSize(sizeX, sizeY, sizeZ);

        getHysteresisCube()->width.setValue(hysteresisBoxScale.getValue() * sizeX);
		// SoCube is defined centered in 0/0/0 while the widgets geom is  defined centered at .5/.5/.5
		// The hysteresis cube is not moved but scaled (it continues throught the pip though)
        getHysteresisCube()->height.setValue(hysteresisBoxScale.getValue() * sizeY * hysteresisBoxScaleHeight.getValue());
        getHysteresisCube()->depth.setValue(hysteresisBoxScale.getValue() * sizeZ);

        getHysteresisSwitch()->whichChild = 0;

        enableNotify(true);
		if (enable.getValue() == TRUE)
		{
			if (tooltipKit != NULL) tooltipKit->startTimeIn();
		}
		else
		{
			if (tooltipDisabledKit != NULL) tooltipDisabledKit->startTimeIn();
		}

		inside.setValue(TRUE);
    }
    else
    {
        getHysteresisSwitch()->whichChild = SO_SWITCH_NONE;
		if (enable.getValue() == TRUE)
		{
			if (tooltipKit != NULL) tooltipKit->stopTimeIn();
		}
		else
		{
			if (tooltipDisabledKit != NULL) tooltipDisabledKit->stopTimeIn();
		}

		inside.setValue(FALSE);
    }
}

// --------------------------------------------------------------------------

void SoBehaviorKit::tooltipChangedCB(void *data, SoSensor*)
{
	SoBehaviorKit* self = (SoBehaviorKit*)data;
	assert(self != NULL);
	// TODO ... find out what has changed ??
	//	self->adaptToTooltips();
}

void SoBehaviorKit::tooltipDisabledChangedCB(void *data, SoSensor*)
{
	SoBehaviorKit* self = (SoBehaviorKit*)data;
	assert(self != NULL);
	// TODO ... find out what has changed ??
//	self->adaptToTooltips();
}

void SoBehaviorKit::adaptToTooltips()
{	
	// nil should be implemented by subclasses
}


// --------------------------------------------------------------------------

SoNode *SoBehaviorKit::getGeometryFromString(char *string)
{
    // Read from the string
    SoInput in;
    in.setBuffer(string, strlen(string) );

    SoNode *result;
    SoDB::read(&in, result);

    return result;
}

