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
/** source file for SoFitInKit class.
*
* @author Thomas Psik
*
* $Id: SoFitInKit.cxx 4048 2004-11-25 12:36:16Z tomp $
* @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/nodes/SoPickStyle.h>

#include <stbapi/interaction/SoFitInKit.h>

#include <assert.h>

SbViewportRegion SoFitInKit::vp;

SO_KIT_SOURCE(SoFitInKit);

/**
* initClass
*/
void SoFitInKit::initClass()
{
	SO_KIT_INIT_CLASS(SoFitInKit, SoLayoutKit, "SoLayoutKit");
}

/**
* constructor
*/
SoFitInKit::SoFitInKit() : bba(SoFitInKit::vp)
{
	
//	SO_ENABLE(SoPickAction, SoPickStyleElement);
	
	SO_KIT_CONSTRUCTOR(SoFitInKit);
	
	//-----------------------+------------+------+---------------+-------+---------------
	//                       |            |      |               | right |
	//                       |            | null |               | Sib-  | is
	//                       |            | By   |               | ling  | public
	//         name          | className  | Def. |  parent Name  | Name  | part
	//-----------------------+------------+------+---------------+-------+---------------
  	SO_KIT_ADD_CATALOG_ENTRY(translation,          SoTranslation, FALSE, base,      geometry,    TRUE);
	
	SO_NODE_ADD_FIELD(keepAspect, (TRUE));
	SO_NODE_ADD_FIELD(rayPickGeometry, (FALSE));
	SO_NODE_ADD_FIELD(translationVec, (0,0,0));
	SO_NODE_ADD_FIELD(scaleVec, (1,1,1));
	
	SO_KIT_INIT_INSTANCE();
	
	// set up the transparency material
	
	// setup field connections
	geometrySensor.setFunction(geometryChangedCB);
	geometrySensor.setData(this);
	
	geometryNodeSensor.setFunction(geometryChangedCB);
	geometryNodeSensor.setData(this);
    setUpConnections(TRUE, TRUE);
	
	setSearchingChildren(TRUE);
}

/** 
* destructor
*/
SoFitInKit::~SoFitInKit()
{
}


SbBool SoFitInKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
	
	// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoLayoutKit::setUpConnections(onOff, doItAlways);
        // setup the widget
		
		geometrySensor.attach(&geometry);
		if (geometry.getValue() != NULL)
			geometrySensor.schedule();
    }
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.
		geometrySensor.detach();
		
        SoLayoutKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);
	
}


void SoFitInKit::resizeCB() 
{
	adaptToChanges();
}

void SoFitInKit::geometryChangedCB(void *data, SoSensor*)
{
	SoFitInKit* self = (SoFitInKit*)data;
	assert(self != NULL);
	// add node sensor to check change within the node
	if ( self->geometryNodeSensor.getAttachedNode() != self->geometry.getValue())
	{
		self->geometryNodeSensor.detach();
		self->geometryNodeSensor.attach(self->geometry.getValue());
	}
	self->adaptToChanges();
}


void SoFitInKit::adaptToChanges()
{
	assert(geometry.getValue() != NULL);
	bba.apply(geometry.getValue());
	
	SbXfBox3f xfbox=bba.getXfBoundingBox();
	
	if (xfbox == storedBox) return;
	storedBox = xfbox;
	SbVec3f center = xfbox.getCenter();
    SoTranslation * trans = (SoTranslation *)translation.getValue();
	assert(trans != NULL);
	trans->translation.setValue(-center);
	translationVec.setValue(center);
	printf("SoFitInKit::original place: %f %f %f\n",center[0], center[1], center[2]);
	
	float sizeX;
	float sizeY;
	float sizeZ;
	
	xfbox.getSize(sizeX, sizeY, sizeZ);
//	printf("SoFitInKit::original size: %f %f %f\n",sizeX, sizeY, sizeZ);
	scaleVec.setValue(sizeX, sizeY, sizeZ);

	float scaleFac_X = width.getValue()/sizeX;
	float scaleFac_Y = height.getValue()/sizeY;
	float scaleFac_Z = depth.getValue()/sizeZ;

	if (keepAspect.getValue() == TRUE) 
	{
		float allScale = scaleFac_X;
		if (scaleFac_Y < allScale) allScale = scaleFac_Y;
		if (scaleFac_Z < allScale) allScale = scaleFac_Z;
		scaleFac_X = allScale;
		scaleFac_Y = allScale;
		scaleFac_Z = allScale;
	}
	
	SoScale *scaleNode= (SoScale *)scale.getValue();
	assert(scaleNode != NULL);
	scaleNode->scaleFactor.setValue(scaleFac_X, scaleFac_Y, scaleFac_Z);
}

