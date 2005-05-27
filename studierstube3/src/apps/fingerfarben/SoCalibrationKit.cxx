/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 

 /* ======================================================================= */
/** 
  * Implements a node for performing position calibration of tracker stations.
  *
  * @author  Flo Ledermann <flo@subnet.at>
  * 
  * $Id: SoCalibrationKit.cxx 4057 2004-11-26 10:46:57Z reitmayr $
  * @file                                                                   
  */
 /* ======================================================================= */

#include <memory>
#include <string>

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoGetMatrixAction.h>

#include <core/Context.h>

#include <stbapi/tracker/TrackerOT.h> 
#include <stbapi/event3d/Stb3DEventGenerator.h>


#include "SoCalibrationKit.h"

using namespace std;

// OpenInventor macros

SO_KIT_SOURCE(SoCalibrationKit);
BASE3D_SOURCE(SoCalibrationKit);
 
// class initialization for OpenInventor

void SoCalibrationKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoCalibrationKit, SoBaseKit, "BaseKit");
    BASE3D_INIT_CLASS(SoCalibrationKit);    
}

// constructor

SoCalibrationKit::SoCalibrationKit()
{
    SO_KIT_CONSTRUCTOR(SoCalibrationKit);

    //--------------------------------name--------------|-type----  |-deftype|nuldef|-parent---------|right sib|public
    SO_KIT_ADD_CATALOG_ENTRY         (base,              SoGroup,             FALSE, this,            \x0,      FALSE );
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(geometry,          SoNode,     SoGroup, FALSE, base,            \x0,      TRUE  );
    SO_KIT_ADD_CATALOG_ENTRY         (highlightSwitch,   SoSwitch,            FALSE, base,            \x0,      FALSE );
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(highlightGeometry, SoNode,     SoGroup, FALSE, highlightSwitch, \x0,      TRUE  );
    SO_KIT_ADD_CATALOG_ENTRY         (boundsSeparator,   SoSeparator,         FALSE, base,            \x0,      FALSE );
    SO_KIT_ADD_CATALOG_ENTRY         (nullDrawStyle,     SoDrawStyle,         FALSE, boundsSeparator, \x0,      FALSE );
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(eventBounds,       SoShape,    SoCube,  FALSE, boundsSeparator, \x0,      TRUE  );
        
    SO_KIT_ADD_FIELD(station, (-1));
    SO_KIT_ADD_FIELD(otNodeName, (""));

    SO_KIT_INIT_INSTANCE();
    
    BASE3D_CONSTRUCTOR;

    SoGroup* myGeom = (SoGroup*)getPart("geometry",false);

    SoMaterial* defaultMaterial = new SoMaterial;
    defaultMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);
    myGeom->addChild(defaultMaterial);

    SoCube *defaultGeometry = new SoCube;
    defaultGeometry->width.setValue(0.01);
    defaultGeometry->height.setValue(0.01);
    defaultGeometry->depth.setValue(0.001);
    myGeom->addChild(defaultGeometry);

    SoGroup* hGeom = (SoGroup*)getPart("highlightGeometry", false);

    SoMaterial* hMaterial = new SoMaterial;
    hMaterial->diffuseColor.setValue(1.0, 1.0, 0.0);
    hMaterial->transparency.setValue(0.8);
    hGeom->addChild(hMaterial);

    SoCube *hCube = new SoCube;
    hCube->width.setValue(0.2);
    hCube->height.setValue(0.2);
    hCube->depth.setValue(0.2);
    hGeom->addChild(hCube);

    ((SoSwitch*)(highlightSwitch.getValue()))->whichChild.setValue(SO_SWITCH_NONE);

    ((SoDrawStyle*)(nullDrawStyle.getValue()))->style = SoDrawStyle::INVISIBLE;

    SoCube* bCube = (SoCube*)getPart("eventBounds", false);
    bCube->width.setValue(0.2);
    bCube->height.setValue(0.2);
    bCube->depth.setValue(0.2);

    timeOut = SbTime(2.0);

    timer.setFunction( timerFired );
    timer.setData( this );

    pOtOffset = NULL;

    
}

// destructor, clears all lists and removes any listeners etc.

SoCalibrationKit::~SoCalibrationKit()
{
    timer.unschedule();

}

/*
USE setUpConnections() to walk the tree checking for TrackedArtifactKits !
and connect to opentracker
*/
SbBool SoCalibrationKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (onOff) {
        // We connect AFTER base class.
        //SoDragger::setUpConnections(onOff, doItAlways);

	    //get the opentracker node for calibration
	    TrackerOT* pOT = (TrackerOT*)Stb3DEventGenerator::getTracker();
        if (pOT != NULL) {
   	        Context* otContext = pOT->getContext();
            if (otContext != NULL) {
                pOtOffset = (StaticTransformation*) (otContext->findNode(otNodeName.getValue().getString()));
            }
        }
        if (pOtOffset == NULL) {
            fprintf(stderr, "SoCalibrationKit.setUpConnections(): OpenTracker node \"%s\" NOT found!\n", otNodeName.getValue().getString());
        }

    }
    else {
        //nothing to disconnect

        // We disconnect BEFORE base class.
        //SoDragger::setUpConnections(onOff, doItAlways);
    }
    return !onOff;
}

void SoCalibrationKit::handle3DEvent(SoHandle3DEventAction *h3a)
{    
    ((SoSwitch*)(highlightSwitch.getValue()))->whichChild.setValue(0);

    So3DEvent *ev = (So3DEvent*)h3a->getEvent();
    int eventStation = ev->getStation();

    //printf("Event Received. Station:%d Button0:%d Button1:%d Button2:%d\n", ev->getStation(),ev->getButton(So3DEvent::BUTTON0),ev->getButton(So3DEvent::BUTTON1),ev->getButton(So3DEvent::BUTTON2));

    if (eventStation != station.getValue()){
        return;
    }

    if( timeOut.getValue() > 0.0 )
    {
        timer.unschedule();
        timer.setTimeFromNow( timeOut);
        timer.schedule();
    }

    if (ev->getButton(So3DEvent::BUTTON0)==0 ) {
        return;
    }

    //printf("### Event accepted");

    //check whether in calibration mode

    //calculate own position in world-coords
    // get the transformation from the root 
    SbViewportRegion vp;
    SoGetMatrixAction ma(vp);
    h3a->getPathFromRoot()->ref();
    ma.apply((SoFullPath*)h3a->getPathFromRoot());
	h3a->getPathFromRoot()->unrefNoDelete();

    SbMatrix globalTransform = ma.getMatrix();

    SbVec3f calibPos, s; SbRotation rot, so;
    globalTransform.getTransform(calibPos, rot, s, so);

    //get station position in world coords
    SbVec3f stationPos = ev->getTranslation();
	SbRotation stationRot = ev->getRotation();

    //calculate difference
	SbVec3f offset = calibPos - stationPos;

    //transform to station coords
	stationRot.invert();
	stationRot.multVec(offset, offset);

    //printf("SoCalibrationKit: Event-Station offset: [%f,%f,%f]\n", offset[0], offset[1], offset[2]);

    if (pOtOffset == NULL){
    	//get the opentracker node for calibration
	    TrackerOT* pOT = (TrackerOT*)Stb3DEventGenerator::getTracker();
        if (pOT != NULL) {
   	        Context* otContext = pOT->getContext();
            if (otContext != NULL) {
                pOtOffset = (StaticTransformation*) (otContext->findNode(otNodeName.getValue().getString()));
            }
        }
    }

    if (pOtOffset != NULL){
        //write to OT calibration node
	    float* tOffset = pOtOffset->getTranslation();
	    tOffset[0] += offset[0];
	    tOffset[1] += offset[1];
	    tOffset[2] += offset[2];
        printf("SoCalibrationKit: %s.translation: [%f,%f,%f]\n", otNodeName.getValue().getString(), tOffset[0], tOffset[1], tOffset[2]);
    }
    else {
        fprintf(stderr, "SoCalibrationKit: OpenTracker node \"%s\" NOT found!\n", otNodeName.getValue().getString());
    }


}

SbBool SoCalibrationKit::isInterested(SoHandle3DEventAction *h3a)
{
    So3DEvent *ev = (So3DEvent*)h3a->getEvent();
    int eventStation = ev->getStation();
    
    if (ev->getType() == So3DEvent::ET_ENTER_EVENT){
        //printf("ENTER event \n");
    }

    if (eventStation == station.getValue()) {
        return true;
    }

    return false;
}


void SoCalibrationKit::timerFired(void *data, SoSensor *sensor)
{
    SoCalibrationKit* self = (SoCalibrationKit*)data;
    ((SoSwitch*)(self->highlightSwitch.getValue()))->whichChild.setValue(SO_SWITCH_NONE);
}
