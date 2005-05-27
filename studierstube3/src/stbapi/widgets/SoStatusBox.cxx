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
/** source file for SoStatusBox class.
  *
  * @author Valérie Maquil
  *
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoAsciiText.h>

#include <Inventor/nodekits/SoShapeKit.h>
#include <Inventor/nodekits/SoAppearanceKit.h>

#include <stbapi/stbapi.h>
#include <stbapi/util/ivio.h>

#include <stbapi/interaction/SoBehaviorKit.h>
#include <stbapi/widgets/SoStatusBox.h>

#include <stbapi/misc/SoAbortGroup.h>


#include <assert.h>

#define _DEBUG_STATUSBOX

#if defined( _DEBUG_STATUSBOX) && defined(_DEBUG)
	#define DEBUG_CODE(a) a;
#else
	#define DEBUG_CODE(a) ;
#endif


SO_KIT_SOURCE(SoStatusBox);

// ------------------------------------------------------------------------------------------

void SoStatusBox::initClass()
{
	SO_KIT_INIT_CLASS(SoStatusBox, SoLayoutKit, "SoLayoutKit");
}

// ------------------------------------------------------------------------------------------

SoStatusBox::SoStatusBox()
{
	SO_KIT_CONSTRUCTOR(SoStatusBox);

	//---------------------------------------+------------+------+---------------+-------+---------------
	//                                       |            |      |               | right |
	//                                       |            | null |               | Sib-  | is
	//                                       |            | By   |               | ling  | public
	//                         name          | className  | Def. |  parent Name  | Name  | part
	//---------------------------------------+------------+------+---------------+-------+---------------
	SO_KIT_ADD_CATALOG_ENTRY(onOffSwitch,	  SoSwitch,	   FALSE, geometry,			\x0,    TRUE);
   	SO_KIT_ADD_CATALOG_ENTRY(onGeometry,	  SoSeparator, FALSE, onOffSwitch,		\x0,    TRUE);
   	SO_KIT_ADD_CATALOG_ENTRY(offGeometry,	  SoSeparator, FALSE, onOffSwitch,		\x0,    TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(onPlane,		  SoShapeKit,  FALSE, onGeometry,		\x0,    TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(onLabel,		  SoLabelKit,  FALSE, onGeometry,		\x0,    TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(onContents,	  SoSeparator, FALSE, onGeometry,		\x0,    TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(offPlane,		  SoShapeKit,  FALSE, offGeometry,		\x0,    TRUE);
  	SO_KIT_ADD_CATALOG_ENTRY(offLabel,	      SoLabelKit , FALSE, offGeometry,		\x0,    TRUE);  
	SO_KIT_ADD_CATALOG_ENTRY(offContents,	  SoSeparator, FALSE, offGeometry,		\x0,    TRUE);

	SO_NODE_ADD_FIELD(text,	(""));
	SO_NODE_ADD_FIELD(hideLabel, (FALSE));
	SO_NODE_ADD_FIELD(show, (FALSE));

	SO_KIT_INIT_INSTANCE();

	// set default values for status box plane	
	setPart("onPlane.coordinate3"	    , SoBehaviorKit::getGeometryFromString("Coordinate3 { point [0.5 0 0.5  ,0.5  0 -0.5, -0.5  0 -0.5, -0.5 0 0.5 ] }"));
	setPart("onPlane.shape"			, SoBehaviorKit::getGeometryFromString("FaceSet {}"));
	setPart("onPlane.appearance.material"  , SoBehaviorKit::getGeometryFromString("Material {diffuseColor .8 .8 .5 transparency 0 } "));

	setPart("offPlane.coordinate3"	    , SoBehaviorKit::getGeometryFromString("Coordinate3 { point [-0.5 0 0.5 , 0.5 0 0.5  ,0.5  0 -0.5, -0.5  0 -0.5] }"));
	setPart("offPlane.shape"			, SoBehaviorKit::getGeometryFromString("FaceSet {}"));
	setPart("offPlane.appearance.material"  , SoBehaviorKit::getGeometryFromString("Material {diffuseColor .8 .8 .5 transparency 0 } "));

	//set default values for status box text
	setPart("onLabel.font"	 , SoBehaviorKit::getGeometryFromString("Font { size .8}"));
	setPart("onLabel.material" , SoBehaviorKit::getGeometryFromString("Material { diffuseColor 0 0 0 } "));
	setPart("onLabel.transform" ,SoBehaviorKit::getGeometryFromString("Transform { translation 0 .01 0} "));
	
	setPart("offLabel.font"	 , SoBehaviorKit::getGeometryFromString("Font {size .8}"));
	setPart("offLabel.material" , SoBehaviorKit::getGeometryFromString("Material { diffuseColor 0 0 0 } "));
	setPart("offLabel.transform" ,SoBehaviorKit::getGeometryFromString("Transform { translation 0 .01 0} "));

//	setPart("label.contents" , readMemory("SoSeparator { AsciiText { justification LEFT } }"));
	
	showSensor.setData(this);
	showSensor.setPriority(1);
	showSensor.setFunction(showCB);

	hideLabelSensor.setData(this);
	hideLabelSensor.setFunction(hideLabelCB);

	setUpConnections(TRUE, TRUE);

}

// ------------------------------------------------------------------------------------------

SoStatusBox::~SoStatusBox()
{

}

// ------------------------------------------------------------------------------------------

SbBool SoStatusBox::setUpConnections( SbBool onOff, SbBool doItAlways )
{
 if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    SoLayoutKit::setUpConnections(onOff, doItAlways);

	hideLabelSensor.attach(&hideLabel);
	showSensor.attach(&show);
	updateVisibility();

  }
  else {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
    SoLayoutKit::setUpConnections(onOff, doItAlways);
	showSensor.detach();

  }
  return !(connectionsSetUp = onOff);
}

// ------------------------------------------------------------------------------------------

void SoStatusBox::showCB(void *data, SoSensor *sensor)
{
	SoStatusBox *self = (SoStatusBox*)data;
	assert(self);
	self->updateVisibility();
}


// ------------------------------------------------------------------------------------------

void SoStatusBox::hideLabelCB(void *data, SoSensor *sensor)
{
	SoStatusBox *self = (SoStatusBox*)data;
	assert(self);

	SoNode *onLabel = self->getPart("onLabel", TRUE);
	if (onLabel != NULL && onLabel ->isOfType(SoLabelKit::getClassTypeId()))
	{
		if (self->hideLabel.getValue() == FALSE)
		{
		SoLabelKit *textNode = (SoLabelKit *)onLabel;
		textNode->text.connectFrom(&(self->text));
		textNode->translations.setValue(0,0.05,0);
		}
		else {
		SoLabelKit *textNode = (SoLabelKit *)onLabel;
		textNode->text.disconnect();
		textNode->text.setValue(" ");
		}
	}

}


// ------------------------------------------------------------------------------------------
void SoStatusBox::updateVisibility()
{
	SoSwitch *geomSwitch = SO_GET_ANY_PART(this, "onOffSwitch", SoSwitch);
	assert (geomSwitch);
	if (show.getValue() == TRUE)
	{
		geomSwitch->whichChild.setValue(0);
		//DEBUG_CODE(printf("######### startShow triggered.\n"));	
	}
	else
	{
		geomSwitch->whichChild.setValue(1); 
	}
}

// ------------------------------------------------------------------------------------------
void SoStatusBox::setLabel(SoMFString *sbText)
{
	text.connectFrom(sbText);
}

// ------------------------------------------------------------------------------------------
