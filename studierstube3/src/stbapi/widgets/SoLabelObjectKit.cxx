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
/** source file for SoLabelObjectKit class.
*
* @author Thomas Psik
*
* $Id: SoLabelObjectKit.cxx 3505 2004-07-20 11:39:36Z tomp $
* @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoAsciiText.h>

#include <stbapi/stbapi.h>
#include <stbapi/util/ivio.h>

#include <stbapi/interaction/SoBehaviorKit.h>
#include <stbapi/widgets/SoLabelObjectKit.h>

#include <assert.h>

SbViewportRegion SoLabelObjectKit::vp;

SO_KIT_SOURCE(SoLabelObjectKit);

// ------------------------------------------------------------------------------------------

void SoLabelObjectKit::initClass()
{
	SO_KIT_INIT_CLASS(SoLabelObjectKit, SoWrapperKit, "WrapperKit");
}

// ------------------------------------------------------------------------------------------

SoLabelObjectKit::SoLabelObjectKit() : bba(SoLabelObjectKit::vp)
{
	SO_KIT_CONSTRUCTOR(SoLabelObjectKit);
	
	//---------------------------------------+------------+------+---------------+-------+---------------
	//                                       |            |      |               | right |
	//                                       |            | null |               | Sib-  | is
	//                                       |            | By   |               | ling  | public
	//                         name          | className  | Def. |  parent Name  | Name  | part
	//---------------------------------------+------------+------+---------------+-------+---------------
   	// all inherited from SoShapeKit

	SO_NODE_ADD_FIELD(text,      	  (""));
	SO_NODE_ADD_FIELD(offset,		  (0,.05,0));
	SO_NODE_ADD_FIELD(resizeToObject, (TRUE));
	SO_NODE_ADD_FIELD(uniformScale,   (TRUE));
	SO_NODE_ADD_FIELD(geometry,       (NULL));
	SO_NODE_ADD_FIELD(dynamicUpdate,  (FALSE));

	SO_NODE_DEFINE_ENUM_VALUE(LabelPlacing, DEFAULT);
	SO_NODE_DEFINE_ENUM_VALUE(LabelPlacing, POS_X);
	SO_NODE_DEFINE_ENUM_VALUE(LabelPlacing, NEG_X);
	SO_NODE_DEFINE_ENUM_VALUE(LabelPlacing, POS_Y);
	SO_NODE_DEFINE_ENUM_VALUE(LabelPlacing, NEG_Y);
	SO_NODE_DEFINE_ENUM_VALUE(LabelPlacing, POS_Z);
	SO_NODE_DEFINE_ENUM_VALUE(LabelPlacing, NEG_Z);
	
	SO_NODE_SET_SF_ENUM_TYPE(placement, LabelPlacing);
	
	SO_NODE_ADD_FIELD(placement, (DEFAULT));
	
	
	SO_KIT_INIT_INSTANCE();
	
	setPart("appearance.font"	, SoBehaviorKit::getGeometryFromString("Font {size .6}  "));
	setPart("appearance.material", SoBehaviorKit::getGeometryFromString("Material { diffuseColor .1 .5 .2 }  "));
	
	setPart("contents"				, readMemory("AsciiText { justification CENTER }"));
	
	setPart("localTransform"	, SoBehaviorKit::getGeometryFromString("Transform {rotation 1 0 0 -1.57 }"));
	
	SoNode *contents = getPart("contents", TRUE);
	if (contents!= NULL && contents->isOfType(SoSeparator::getClassTypeId()))
	{
		SoNode *child = ((SoSeparator*)contents)->getChild(0);
		if (child!= NULL && child->isOfType(SoAsciiText::getClassTypeId()))
		{
			SoAsciiText *textNode = (SoAsciiText *)child;
			textNode->string.connectFrom(&text);
			textNode->justification.setValue(SoAsciiText::CENTER);
		}
	}

	offsetSensor.setData(this);
	offsetSensor.setFunction(SoLabelObjectKit::paramChangedCB);
	placementSensor.setData(this);
	placementSensor.setFunction(SoLabelObjectKit::paramChangedCB);
	resizeToObjectSensor.setData(this);
	resizeToObjectSensor.setFunction(SoLabelObjectKit::paramChangedCB);
	uniformScaleSensor.setData(this);
	uniformScaleSensor.setFunction(SoLabelObjectKit::paramChangedCB);

	geometrySensor.setData(this);
	geometrySensor.setFunction(SoLabelObjectKit::geometryChangedCB);
//	geometryNodeSensor.setData(this);
//	geometryNodeSensor.setFunction(SoLabelObjectKit::geometryChangedCB);
	
	setUpConnections(TRUE, TRUE);
}

// ------------------------------------------------------------------------------------------

SoLabelObjectKit::~SoLabelObjectKit()
{
}

// ------------------------------------------------------------------------------------------

SbBool SoLabelObjectKit::setUpConnections( SbBool onOff, SbBool doItAlways )
{
	if (!doItAlways && connectionsSetUp == onOff)
		return onOff;
	if (onOff) {
		// We connect AFTER base class.
		SoWrapperKit::setUpConnections(onOff, doItAlways);
		offsetSensor.attach(&offset);
		resizeToObjectSensor.attach(&resizeToObject);
		uniformScaleSensor.attach(&uniformScale);
		placementSensor.attach(&placement);
		geometrySensor.attach(&geometry);
		geometrySensor.schedule();
		
	}
	else {
		// We disconnect BEFORE base class.
		// Disconnect the field sensors.
		SoWrapperKit::setUpConnections(onOff, doItAlways);
		offsetSensor.detach();
		resizeToObjectSensor.detach();
		uniformScaleSensor.detach();
		placementSensor.detach();
		geometrySensor.detach();

	}
	return !(connectionsSetUp = onOff);
}

// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------

void SoLabelObjectKit::geometryChangedCB(void * data, SoSensor *)
{
	SoLabelObjectKit* self = (SoLabelObjectKit*)data;
	assert(self != NULL);
	bool updateFlag = FALSE;

	if (self->dynamicUpdate.getValue() == FALSE )
	{
		//	self->geometryNodeSensor.detach();	
			self->geometrySensor.detach();	
	}

	printf("SoLabelObjectKit::geometryChangedCB:: geom: %p\n",self->geometry.getValue());
	updateFlag = TRUE;
	if (updateFlag)
	{
	// 		self->geometryNodeSensor.attach(self->geometry.getValue());
			self->adaptToGeometry();
	}

	if (self->dynamicUpdate.getValue() == FALSE )
	{
		//	self->geometryNodeSensor.detach();	
			self->geometrySensor.detach();	
	}

}

void SoLabelObjectKit::paramChangedCB(void *data, SoSensor*)
{
	SoLabelObjectKit* self = (SoLabelObjectKit*)data;
	assert(self != NULL);
	self->adaptToGeometry();
}


void SoLabelObjectKit::adaptToGeometry()
{
	if (geometry.getValue() == NULL) return;

	
	bba.apply(geometry.getValue());
	
	SbXfBox3f xfbox=bba.getXfBoundingBox();
	
	SbVec3f center = xfbox.getCenter();
	SoTransform *trans = (SoTransform *)getPart("transform", true);
	
	assert(trans != NULL);

	//	printf("SoFitInKit::original place: %f %f %f\n",center[0], center[1], center[2]);
	
	float sizeX;
	float sizeY;
	float sizeZ;
	
	xfbox.getSize(sizeX, sizeY, sizeZ);
	printf("SoLabelObjectKit::original size: %f %f %f\n",sizeX, sizeY, sizeZ);
//	scaleVec.setValue(sizeX, sizeY, sizeZ);
	
	switch ( placement.getValue()) 
	{
	case POS_X: center[0] += sizeX/2.0; break;
	case NEG_X:  center[0] -= sizeX/2.0; break;
	case DEFAULT:  
	case POS_Y: center[1] += sizeY/2.0; break;
	case NEG_Y:  center[1] -= sizeY/2.0; break;
	case POS_Z: center[2] += sizeZ/2.0; break;
	case NEG_Z:  center[2] -= sizeZ/2.0; break;
	case NONE:  
		break;
	}
	SbVec3f offsetScale = offset.getValue();
	offsetScale[0] *= sizeX;
	offsetScale[1] *= sizeY;
	offsetScale[2] *= sizeZ;
	
	center += offsetScale;

	trans->translation.setValue(center);
	printf("SoLabelObjectKit::translation : %f %f %f\n",center[0], center[1], center[2]);
	
	if (resizeToObject.getValue() == TRUE)
	{
		SoTransform *localTrans = (SoTransform *)getPart("localTransform", true);
		assert(localTrans != NULL);
	
		
		if (uniformScale.getValue() == TRUE) 
		{
			float allScale = sizeX;
			if (sizeY < allScale) allScale = sizeY;
			if (sizeZ < allScale) allScale = sizeZ;
			sizeX = allScale;
			sizeY = allScale;
			sizeZ = allScale;
		}
		printf("SoLabelObjectKit::scale : %f %f %f\n",sizeX, sizeY, sizeZ);
		
		trans->scaleFactor.setValue(sizeX, sizeY, sizeZ);
	}
}
// ------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------



