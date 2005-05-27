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
/** source file for SoTooltipKit class.
  *
  * @author Thomas Psik
  *
  * $Id: SoTooltipKit.cxx 4047 2004-11-25 12:34:27Z tomp $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoAsciiText.h>

#include <Inventor/nodekits/SoShapeKit.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h> 

#include <stbapi/stbapi.h>
#include <stbapi/util/ivio.h>

#include <stbapi/interaction/SoBehaviorKit.h>
#include <stbapi/widgets/SoTooltipKit.h>
#include <stbapi/widgets/SoLabelKit.h>
#include <stbapi/misc/SoAbortGroup.h>


#include <assert.h>

#define _DEBUG_TOOLTIP

#if defined( _DEBUG_TOOLTIP) && defined(_DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif


SO_KIT_SOURCE(SoTooltipKit);

// ------------------------------------------------------------------------------------------

void SoTooltipKit::initClass()
{
	SO_KIT_INIT_CLASS(SoTooltipKit, SoBaseKit, "BaseKit");
}

// ------------------------------------------------------------------------------------------

SoTooltipKit::SoTooltipKit()
{
	masterTooltip = NULL;

	SO_KIT_CONSTRUCTOR(SoTooltipKit);

	//---------------------------------------+------------+------+---------------+-------+---------------
	//                                       |            |      |               | right |
	//                                       |            | null |               | Sib-  | is
	//                                       |            | By   |               | ling  | public
	//                         name          | className  | Def. |  parent Name  | Name  | part
	//---------------------------------------+------------+------+---------------+-------+---------------
   	SO_KIT_ADD_CATALOG_ENTRY(base,			  SoSeparator ,FALSE, this,			\x0,    TRUE);
   	SO_KIT_ADD_CATALOG_ENTRY(onOffSwitch,	  SoSwitch,	   FALSE, base,			\x0,    TRUE);
  	SO_KIT_ADD_CATALOG_ENTRY(geometry,	      SoSeparator, FALSE, onOffSwitch,	\x0,    TRUE);  
  	SO_KIT_ADD_CATALOG_ENTRY(transform,	      SoTransform, FALSE, geometry,		\x0,    TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(plane,			  SoShapeKit,  FALSE,  geometry,	\x0,    TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(label,			  SoLabelKit,  FALSE,  geometry,	\x0,    TRUE);

	SO_NODE_ADD_FIELD(text,	(""));
	SO_NODE_ADD_FIELD(show, (FALSE));
	SO_NODE_ADD_FIELD(timeOut, (1.2));
	SO_NODE_ADD_FIELD(adaptSize, (TRUE));

	SO_NODE_ADD_FIELD(startShow, ());
	SO_NODE_ADD_FIELD(stopShow, ());

	SO_KIT_INIT_INSTANCE();

	// set default values for tooltip plane
	setPart("transform"				, SoBehaviorKit::getGeometryFromString("SoTransform { rotation 1 0 0 .6 translation 1.5 2.5 -2.0 }"));
	
	setPart("plane.coordinate3"	    , SoBehaviorKit::getGeometryFromString("Coordinate3 { point [-0.5  0 -0.5, -0.5 0 0.5 , 0.5 0 0.5  ,0.5  0 -0.5] }"));
	setPart("plane.shape"			, SoBehaviorKit::getGeometryFromString("FaceSet {}"));
	setPart("plane.localTransform"	, SoBehaviorKit::getGeometryFromString("SoTransform { scaleFactor 10 1 5 }"));
	
	setPart("plane.appearance.material"  , SoBehaviorKit::getGeometryFromString("Material {diffuseColor .8 .8 .5 transparency 0 } "));
	
	//set default values for tooltip text
	setPart("label.font"	 , SoBehaviorKit::getGeometryFromString("Font {size .5}"));
	setPart("label.material" , SoBehaviorKit::getGeometryFromString("Material { diffuseColor 0 0 0 } "));
	setPart("label.transform" ,SoBehaviorKit::getGeometryFromString("Transform { translation 0 .01 0} "));
//	setPart("label.contents" , readMemory("SoSeparator { AsciiText { justification LEFT } }"));
	
	showSensor.setData(this);
	showSensor.setPriority(1);
	showSensor.setFunction(showCB);

	timeInSensor.setData(this);
	timeInSensor.setFunction(timeOutCB);

	setUpConnections(TRUE, TRUE);


}

// ------------------------------------------------------------------------------------------

SoTooltipKit::~SoTooltipKit()
{

}

// ------------------------------------------------------------------------------------------

SbBool SoTooltipKit::setUpConnections( SbBool onOff, SbBool doItAlways )
{
 if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    SoBaseKit::setUpConnections(onOff, doItAlways);
  	
	SoNode *label = getPart("label", TRUE);
	if (label != NULL && label ->isOfType(SoLabelKit::getClassTypeId()))
	{
		SoLabelKit *textNode = (SoLabelKit *)label;
		textNode->text.connectFrom(&text);
		textNode->translations.setValue(0,0.05,0);
	}
	showSensor.attach(&show);
	updateVisibility();

  }
  else {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
    SoBaseKit::setUpConnections(onOff, doItAlways);
	showSensor.detach();

  }
  return !(connectionsSetUp = onOff);
}

// ------------------------------------------------------------------------------------------

void SoTooltipKit::showCB(void *data, SoSensor *sensor)
{
	SoTooltipKit *self = (SoTooltipKit*)data;
	assert(self);
	self->updateVisibility();
}

// ------------------------------------------------------------------------------------------

void SoTooltipKit::updateVisibility()
{
	SoSwitch *geomSwitch = SO_GET_ANY_PART(this, "onOffSwitch", SoSwitch);
	assert (geomSwitch);
	if (show.getValue() == TRUE)
	{
		if (adaptSize.getValue() == TRUE)
		{
			SoNode *label = getPart("label", TRUE);
			if (label != NULL)
			{			
				SbViewportRegion vp;
				SoGetBoundingBoxAction bba(vp);
				bba.apply(label);
			
				SbXfBox3f xfbox=bba.getXfBoundingBox();
					
				SbVec3f center = xfbox.getCenter();
			    SoTransform* transform = (SoTransform*)getPart("plane.localTransform", FALSE);
			//	transform->translation.setValue(-center);
				float sizeX;
				float sizeY;
				float sizeZ;
				
				xfbox.getSize(sizeX, sizeY, sizeZ);
				float overSize = sizeX;
				if (sizeY>sizeX) overSize = sizeY;
				// 20% size of the bigger dimension
				overSize *= 0.2;

				// ignore Z value !! as this is a plane !!
				transform->scaleFactor.setValue(sizeX+overSize, 1.0 ,sizeY+overSize);
			}	
		}
		geomSwitch->whichChild.setValue(0);
		DEBUG_CODE(printf("######### startShow triggered.\n"));
		startShow.setValue();		
	}
	else
	{
		geomSwitch->whichChild.setValue(SO_SWITCH_NONE); 
		stopShow.setValue();
	}
}

// ------------------------------------------------------------------------------------------

void SoTooltipKit::startTimeIn()
{
	timeInSensor.setTimeFromNow(timeOut.getValue());
	timeInSensor.schedule();
}

// ------------------------------------------------------------------------------------------

void SoTooltipKit::stopTimeIn()
{
	if (timeInSensor.isScheduled())
		timeInSensor.unschedule();
	if (masterTooltip != NULL)
	{
		masterTooltip->stopTimeIn();
		return;
	}
	show.setValue(FALSE);
}

// ------------------------------------------------------------------------------------------

void SoTooltipKit::timeOutCB(void *data, SoSensor *sensor)
{
	SoTooltipKit *self = (SoTooltipKit*)data;
	assert(self);
	if (self->masterTooltip != NULL)
	{
		self->masterTooltip->timeOutCB(self->masterTooltip, sensor);
		return;
	}
	self->show.setValue(TRUE);
}

// ------------------------------------------------------------------------------------------

void SoTooltipKit::getBoundingBox(SoGetBoundingBoxAction *action)
{
	return;  
}

// ------------------------------------------------------------------------------------------
/*
void SoTooltipKit::adaptToTemplate(SoTooltipKit *tooltipTempl)
{
	if (tooltipTempl == NULL) return;
	SoNode *testPart = NULL;
	
	if (masterTooltip != NULL) 
	{
		masterTooltip->adaptToTemplate(tooltipTempl);
		return;
	}
	// check if template label has parts set and replace then in current label
	// TODO widget style !!
	// only if the current label is not a SoAsciiText, set the label from the template
	// else do not override with template !!
	testPart = tooltipTempl->getPart("label", FALSE);
	if (testPart != NULL )
	{
		SoLabelKit * labelTempl = (SoLabelKit * )testPart;
		SoLabelKit * label = (SoLabelKit * )getPart("label", FALSE);
		if (label !=  NULL)
		{
			label->adaptToTemplate(labelTempl);
		}
	}


	testPart = tooltipTempl->getPart("plane.appearance.font", FALSE);
	if (testPart != NULL)
	{
		setPart("plane.appearance.font", testPart);
	}

	testPart = tooltipTempl->getPart("plane.appearance.material", FALSE);
	if (testPart != NULL)
	{
		setPart("plane.appearance.material", testPart);
	}

	testPart = tooltipTempl->getPart("plane.transform", FALSE);
	if (testPart != NULL)
	{
		setPart("plane.transform", testPart->copy());
	}
}
*/

void SoTooltipKit::setMasterTooltip(SoTooltipKit *tooltip) 
{
	masterTooltip = tooltip;
}

void SoTooltipKit::addToBase(SoNode *node)
{
	SoSeparator *baseSep = (SoSeparator*)base.getValue();
	baseSep->addChild(node);
} 

