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
/** source file for SoLayoutKit class.
  *
  * @author Thomas Psik
  *
  * $Id: SoLayoutKit.cxx 4030 2004-11-22 10:47:09Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTranslation.h>

#include <stbapi/interaction/SoLayoutKit.h>

#include <assert.h>

SO_KIT_SOURCE(SoLayoutKit);

/**
 * initClass
 */
void SoLayoutKit::initClass()
{
	SO_KIT_INIT_CLASS(SoLayoutKit, SoBaseKit, "BaseKit");
}

/**
 * constructor
 */
SoLayoutKit::SoLayoutKit()
{
	SO_KIT_CONSTRUCTOR(SoLayoutKit);

	//-----------------------+------------+------+---------------+-------+---------------
	//                       |            |      |               | right |
	//                       |            | null |               | Sib-  | is
	//                       |            | By   |               | ling  | public
	//         name          | className  | Def. |  parent Name  | Name  | part
	//-----------------------+------------+------+---------------+-------+---------------
    SO_KIT_ADD_CATALOG_ENTRY(base,                 SoSeparator, FALSE, this,           \x0,    TRUE);
   	SO_KIT_ADD_CATALOG_ENTRY(scale,                SoScale,     FALSE, base,           \x0,    TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(geometry,			   SoSeparator, FALSE, base,           \x0,    TRUE);

	SO_NODE_ADD_FIELD(width, (1.0));
	SO_NODE_ADD_FIELD(height,(1.0));
	SO_NODE_ADD_FIELD(depth, (1.0));

	SO_KIT_INIT_INSTANCE();
	
	// setup field connections
	widthSensor.setFunction(dimensionsChangedCB);
	widthSensor.setData(this);

	depthSensor.setFunction(dimensionsChangedCB);
	depthSensor.setData(this);
	
	heightSensor.setFunction(dimensionsChangedCB);
	heightSensor.setData(this);

    setUpConnections(TRUE, TRUE);
}

/** 
 * destructor
 */
SoLayoutKit::~SoLayoutKit()
{

}


SbBool SoLayoutKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{

		// todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onOff, doItAlways);
    
		widthSensor.attach(&width);
		depthSensor.attach(&depth);
		heightSensor.attach(&height);
		
		widthSensor.schedule();		
    }
    else {
        // We disconnect BEFORE base class.        
        // Disconnect the field sensors.
		widthSensor.detach();
		depthSensor.detach();
		heightSensor.detach();
		
		SoBaseKit::setUpConnections(onOff, doItAlways);
    }
    return !(connectionsSetUp = onOff);
}



void SoLayoutKit::dimensionsChangedCB(void *data, SoSensor*)
{
	SoLayoutKit* self = (SoLayoutKit*)data;
	assert(self != NULL);
	SoScale *scale = (SoScale *) self->scale.getValue();
	assert(scale != NULL);
	scale->scaleFactor.setValue(self->width.getValue(), self->height.getValue(), self->depth.getValue());
	self->resizeCB();
}

void SoLayoutKit::resizeCB()
{
	
}

void SoLayoutKit::doStyleAction(SoAction *action)
{
  // implemented in PucPipLayout extention
}
