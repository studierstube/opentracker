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
/** source file for SoShapePlaneKit class.
  *
  * @author Thomas Psik
  *
  * $Id: SoShapePlaneKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoAsciiText.h>


#include <stbapi/stbapi.h>

#include <stbapi/interaction/SoBehaviorKit.h>
#include <stbapi/widgets/SoShapePlaneKit.h>
#include <stbapi/widgets/SoLabelKit.h>
#include <stbapi/misc/SoAbortGroup.h>


#include <assert.h>

SO_KIT_SOURCE(SoShapePlaneKit);

// ------------------------------------------------------------------------------------------

void SoShapePlaneKit::initClass()
{
	SO_KIT_INIT_CLASS(SoShapePlaneKit, SoBaseKit, "BaseKit");
}

// ------------------------------------------------------------------------------------------

SoShapePlaneKit::SoShapePlaneKit()
{
	SO_KIT_CONSTRUCTOR(SoShapePlaneKit);

	//---------------------------------------+------------+------+---------------+-------+---------------
	//                                       |            |      |               | right |
	//                                       |            | null |               | Sib-  | is
	//                                       |            | By   |               | ling  | public
	//                         name          | className  | Def. |  parent Name  | Name  | part
	//---------------------------------------+------------+------+---------------+-------+---------------
   	SO_KIT_ADD_CATALOG_ENTRY(base,			  SoSeparator ,FALSE, this,				\x0,    TRUE);
   	SO_KIT_ADD_CATALOG_ENTRY(onOffSwitch,	  SoSwitch,	   FALSE, base,			\x0,    TRUE);
  	SO_KIT_ADD_CATALOG_ENTRY(geometry,	      SoSeparator, FALSE, onOffSwitch,		\x0,    TRUE);
//  	SO_KIT_ADD_CATALOG_ENTRY(transform,	      SoTransform, FALSE, geometry,			\x0,    TRUE);
//	SO_KIT_ADD_CATALOG_ENTRY(planeGeometry,	  SoSeparator, FALSE, geometry,			\x0,    TRUE);
//	SO_KIT_ADD_CATALOG_ENTRY(material,		  SoMaterial,  FALSE,  planeGeometry,	\x0,    TRUE);
//	SO_KIT_ADD_CATALOG_ENTRY(scale,			  SoScale,	   FALSE,  planeGeometry,	\x0,    TRUE);
//	SO_KIT_ADD_CATALOG_ENTRY(plane,			  SoSeparator, FALSE,  planeGeometry,	\x0,    TRUE);

	SO_KIT_ADD_CATALOG_ENTRY(plane,			  SoShapeKit, FALSE ,  geometry,	\x0,    TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(label,			  SoLabelKit,  FALSE,  geometry,	\x0,    TRUE);
  
	SO_NODE_ADD_FIELD(text,	(""));
	SO_NODE_ADD_FIELD(show, (FALSE));
	SO_NODE_ADD_FIELD(timeOut, (1.2));

	SO_KIT_INIT_INSTANCE();

	// set default values for tooltip plane
	setPart("plane.transform"	    , SoBehaviorKit::getGeometryFromString("SoTransform { translation 4.5 2.5 4.5 }"));
	setPart("plane.coordinate3"	    , SoBehaviorKit::getGeometryFromString("Coordinate3 { point [-0.5  0 -0.5, -0.5 0 0.5 , 0.5 0 0.5  ,0.5  0 -0.5] }"));
	setPart("plane.shape"			, SoBehaviorKit::getGeometryFromString("FaceSet {}"));
	setPart("plane.localTransform"	, SoBehaviorKit::getGeometryFromString("SoTransform { scaleFactor 10 1 5 }"));
	
	setPart("plane.appearance.material"  , SoBehaviorKit::getGeometryFromString("Material {diffuseColor 1 1 .5 transparency .4 } "));
	
	//setPart("transform"	     , SoBehaviorKit::getGeometryFromString("SoTransform { translation 4.5 2.5 4.5 }"));
	//setPart("material"       , SoBehaviorKit::getGeometryFromString("Material {diffuseColor 1 1 .5 transparency .2 } "));
	//setPart("plane"			 , SoBehaviorKit::getGeometryFromString("Separator {Coordinate3 { point [-0.5  0 -0.5, -0.5 0 0.5 , 0.5 0 0.5  ,0.5  0 -0.5] } FaceSet { } }"));
	//setPart("scale"			 , SoBehaviorKit::getGeometryFromString("Scale { scaleFactor 10 1 5 }"));
	
	//set default values for tooltip text
	setPart("label.font"	 , SoBehaviorKit::getGeometryFromString("Font {size .5}"));
	setPart("label.textNode" , SoBehaviorKit::getGeometryFromString("AsciiText { justification LEFT } "));
	setPart("label.material" , SoBehaviorKit::getGeometryFromString("Material { diffuseColor 0 0 0 } "));
	setPart("label.transform" ,SoBehaviorKit::getGeometryFromString("Transform { translation -4 .01 -1 rotation 1 0 0 -1.57} "));
	
	SoAsciiText *textNode = (SoAsciiText *)getPart("label.textNode", FALSE);
	if (textNode)
		textNode->string.connectFrom(&text);

	showSensor.setData(this);
	showSensor.setPriority(1);
	showSensor.setFunction(showCB);
	showSensor.attach(&show);

	timeInSensor.setData(this);
	timeInSensor.setFunction(timeOutCB);

	updateVisibility();

}

// ------------------------------------------------------------------------------------------

SoShapePlaneKit::~SoShapePlaneKit()
{

}



// ------------------------------------------------------------------------------------------

void SoShapePlaneKit::showCB(void *data, SoSensor *sensor)
{
	SoShapePlaneKit *self = (SoShapePlaneKit*)data;
	assert(self);
	self->updateVisibility();
}

// ------------------------------------------------------------------------------------------

void SoShapePlaneKit::updateVisibility()
{
	SoSwitch *geomSwitch = SO_GET_ANY_PART(this, "onOffSwitch", SoSwitch);
	assert (geomSwitch);
	if (show.getValue() == TRUE)
	{
		geomSwitch->whichChild.setValue(0); 
	}
	else
	{
		geomSwitch->whichChild.setValue(SO_SWITCH_NONE); 
	}
}

// ------------------------------------------------------------------------------------------

void SoShapePlaneKit::startTimeIn()
{
	timeInSensor.setTimeFromNow(timeOut.getValue());
	timeInSensor.schedule();
}

// ------------------------------------------------------------------------------------------

void SoShapePlaneKit::stopTimeIn()
{
	timeInSensor.unschedule();
	show.setValue(FALSE);
}

// ------------------------------------------------------------------------------------------

void SoShapePlaneKit::timeOutCB(void *data, SoSensor *sensor)
{
	SoShapePlaneKit *self = (SoShapePlaneKit*)data;
	assert(self);
	self->show.setValue(TRUE);
}

// ------------------------------------------------------------------------------------------

void SoShapePlaneKit::getBoundingBox(SoGetBoundingBoxAction *action)
{
	return;  
}