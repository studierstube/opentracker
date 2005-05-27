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
  * Implements a node class to perform position and orientation calibration of the virtual camera.
  *
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoDisplayCalibrationKit.cxx 4057 2004-11-26 10:46:57Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>

#include <core/Context.h>

#include <stbapi/tracker/TrackerOT.h> 
#include <stbapi/event3d/Stb3DEventGenerator.h>

#include "SoDisplayCalibrationKit.h"

// OpenInventor macros

SO_KIT_SOURCE(SoDisplayCalibrationKit);
BASE3D_SOURCE(SoDisplayCalibrationKit);
 
// class initialization for OpenInventor

void SoDisplayCalibrationKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoDisplayCalibrationKit, SoBaseKit, "BaseKit");
    BASE3D_INIT_CLASS(SoDisplayCalibrationKit);    
}

// constructor

SoDisplayCalibrationKit::SoDisplayCalibrationKit()
{
    SO_KIT_CONSTRUCTOR(SoDisplayCalibrationKit);

    //--------------------------------name--------------|-type----  |-deftype|nuldef|-parent---------|right sib|public
    SO_KIT_ADD_CATALOG_ENTRY         (baseSwitch,        SoSwitch,            FALSE, this,            \x0,      FALSE );
    SO_KIT_ADD_CATALOG_ENTRY         (geomGroup,         SoGroup,             FALSE, baseSwitch,      \x0,      FALSE );
    SO_KIT_ADD_CATALOG_ENTRY         (calibSeparator,    SoSeparator,         FALSE, geomGroup,       \x0,      FALSE );
    SO_KIT_ADD_CATALOG_ENTRY         (calibXf,           SoMatrixTransform,   FALSE, calibSeparator,  \x0,      FALSE );
    SO_KIT_ADD_CATALOG_ENTRY         (calibTranslation,  SoTranslation,       FALSE, calibSeparator,  \x0,      TRUE  );
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(calibGeometry,     SoNode,     SoGroup, FALSE, calibSeparator,  \x0,      TRUE  );
    SO_KIT_ADD_CATALOG_ENTRY         (stationSeparator,  SoSeparator,         FALSE, geomGroup,       \x0,      FALSE );
    SO_KIT_ADD_CATALOG_ENTRY         (stationMover,      SoTransform,         FALSE, stationSeparator,\x0,      FALSE );
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(stationGeometry,   SoNode,     SoGroup, FALSE, stationSeparator,\x0,      TRUE  );
        
    SO_KIT_ADD_FIELD(station, (-1));
    SO_KIT_ADD_FIELD(otNodeName, (""));
	SO_KIT_ADD_FIELD(timeOut, (2.0));

    SO_KIT_INIT_INSTANCE();
    
    BASE3D_CONSTRUCTOR;

    // build the reference geometry for calibration

    SoTranslation* calibTrans = (SoTranslation*)getPart("calibTranslation",false);
    //calibTrans->translation.setValue(0.0,0.0,0.0);

    SoSeparator* calibGeom = (SoSeparator*)getPart("calibGeometry",false);

    SoMaterial* defaultMaterial = new SoMaterial;
    defaultMaterial->diffuseColor.setValue(1.0, 1.0, 0.0);
    defaultMaterial->transparency.setValue(0.5);
    calibGeom->addChild(defaultMaterial);

    SoCube *defaultGeometry = new SoCube;
    defaultGeometry->width.setValue(0.05);
    defaultGeometry->height.setValue(0.05);
    defaultGeometry->depth.setValue(0.001);
    calibGeom->addChild(defaultGeometry);

    SoMaterial* cMaterial = (SoMaterial*) defaultMaterial->copy();
    cMaterial->transparency.setValue(0.0);
    calibGeom->addChild(cMaterial);

    SoSeparator* cornerSep = new SoSeparator;

    SoTranslation* cTrans = new SoTranslation;
    cTrans->translation.setValue(-0.024, 0.024, 0.0);
    cornerSep->addChild(cTrans);

    SoCube* corner = new SoCube;
    corner->width.setValue(0.002);
    corner->height.setValue(0.002);
    corner->depth.setValue(0.002);
    cornerSep->addChild(corner);

    SoRotation* csideRot = new SoRotation;
    csideRot->rotation.setValue(SbVec3f(0.0,0.0,1.0), 1.5707963);

    calibGeom->addChild(cornerSep);
    calibGeom->addChild(csideRot);
    calibGeom->addChild(cornerSep);
    calibGeom->addChild(csideRot);
    calibGeom->addChild(cornerSep);
    calibGeom->addChild(csideRot);
    calibGeom->addChild(cornerSep);

    // build the geometry for the station overlay

    SoSeparator* stationGeom = (SoSeparator*)getPart("stationGeometry", false);

    SoMaterial* hMaterial = new SoMaterial;
    hMaterial->diffuseColor.setValue(1.0, 0.0, 0.0);
    hMaterial->transparency.setValue(0.7);
    stationGeom->addChild(hMaterial);

    SoSeparator* arrowSep = new SoSeparator;
    SoTranslation* arrowoffset = new SoTranslation;
    arrowoffset->translation.setValue(0.0, 0.03, 0.0);
    arrowSep->addChild(arrowoffset);

    SoCone* arrow = new SoCone;
    arrow->bottomRadius.setValue(0.003);
    arrow->height.setValue(0.015);
    arrowSep->addChild(arrow);

    stationGeom->addChild(arrowSep);
   
    SoSeparator* sideGroup = new SoSeparator;

    SoTranslation* offset = new SoTranslation;
    offset->translation.setValue(0.0, 0.024, 0.0);
    sideGroup->addChild(offset);

    SoCube *hCube = new SoCube;
    hCube->width.setValue(0.05);
    hCube->height.setValue(0.002);
    hCube->depth.setValue(0.002);
    sideGroup->addChild(hCube);

    SoRotation* sideRot = new SoRotation;
    sideRot->rotation.setValue(SbVec3f(0.0,0.0,1.0), 1.5707963);

    stationGeom->addChild(sideGroup);
    stationGeom->addChild(sideRot);
    stationGeom->addChild(sideGroup);
    stationGeom->addChild(sideRot);
    stationGeom->addChild(sideGroup);
    stationGeom->addChild(sideRot);
    stationGeom->addChild(sideGroup);

    timer.setFunction( timerFired );
    timer.setData( this );

    active = FALSE;
    ((SoSwitch*)(baseSwitch.getValue()))->whichChild.setValue(SO_SWITCH_NONE);

    pOtOffset = NULL;

    
}

// destructor, clears all lists and removes any listeners etc.

SoDisplayCalibrationKit::~SoDisplayCalibrationKit()
{
    timer.unschedule();
}

/*
Use setUpConnections() to connect to opentracker
*/
SbBool SoDisplayCalibrationKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (onOff) {
        // We connect AFTER base class.
        // SoDragger::setUpConnections(onOff, doItAlways);

	    // get the opentracker node for calibration
	    TrackerOT* pOT = (TrackerOT*)Stb3DEventGenerator::getTracker();
        if (pOT != NULL) {
   	        Context* otContext = pOT->getContext();
            if (otContext != NULL) {
                pOtOffset = (StaticTransformation*) (otContext->findNode(otNodeName.getValue().getString()));
            }
        }
        if (pOtOffset == NULL) {
            fprintf(stderr, "SoDisplayCalibrationKit.setUpConnections(): OpenTracker node \"%s\" NOT found!\n", otNodeName.getValue().getString());
        }

    }
    else {
        //nothing to disconnect
    }
    return !onOff;
}


// we are interested in all 3D events by our stations

SbBool SoDisplayCalibrationKit::isInterested(SoHandle3DEventAction *h3a)
{    
    if (active) {
        return TRUE;
    }

    So3DEvent *ev = (So3DEvent*)h3a->getEvent();
    if (ev->getStation() == station.getValue()) {
        return TRUE;
    }

    return FALSE;

}

void SoDisplayCalibrationKit::handle3DEvent(SoHandle3DEventAction *h3a)
{
    So3DEvent *ev = (So3DEvent*)h3a->getEvent();

    if (ev->getStation() == station.getValue()){
        //a move event from our station -> update mover
        //this is basically the SoStationKit code...

        SbViewportRegion vp;
        SoGetMatrixAction ma(vp);
        SoPath* fullPath = (SoPath*)(h3a->getPathFromRoot());
        ma.apply(fullPath);
        SbMatrix globalTransform = ma.getInverse();

        // transform the rotation part of the 3D event
        SbMatrix rotMatrix;
        ev->getRotation().getValue(rotMatrix);
        SbMatrix resMatrix;
        resMatrix = rotMatrix * globalTransform;
        SbVec3f t,s; SbRotation rot, so;
        resMatrix.getTransform( t, rot, s, so );

        // transfrom the position part of the 3D event
        SbVec3f pos;
        globalTransform.multVecMatrix( ev->getTranslation(), pos);    

        // compute the new pose based on the transformed event data
        SoTransform *mover = ((SoTransform*)(stationMover.getValue()));
        mover->translation.setValue(pos);
        mover->rotation.setValue(rot);

        lastPos = ev->getTranslation();
        lastRot = ev->getRotation();
    }

    if (active && ev->getType() == So3DEvent::ET_BUTTON0_EVENT && ev->getButton(0)) {
        // button pressed in active mode:
        // do the actual calibration
        //printf("SoDisplayCalibrationKit: calibrating...\n\n");

        this->setSearchingChildren(TRUE);

        SbViewportRegion vp;
        SoGetMatrixAction ma(vp);
        SoPath* fullPath = (SoPath*)(h3a->getPathFromRoot()->copy());

        SoSearchAction sa;
        sa.setInterest(SoSearchAction::FIRST);
        sa.setNode(getPart("calibGeometry",false));
        sa.apply(this);

        fullPath->append(sa.getPath());
        ma.apply(fullPath);
        SbMatrix globalTransform = ma.getMatrix();

        SbVec3f calibPos, s; SbRotation calibRot, so;
        globalTransform.getTransform(calibPos, calibRot, s, so);

        // printf("SoDisplayCalibrationKit: reference position: [%f,%f,%f]\n", calibPos[0], calibPos[1], calibPos[2]);
        // printf("SoDisplayCalibrationKit: tracked position: [%f,%f,%f]\n", lastPos[0], lastPos[1], lastPos[2]);

        SbMatrix camXf;
        //cam in referenzpunkt
        camXf.setTranslate(calibPos * -1.0f);

        //drehen um rotation des tags
        SbMatrix rotMat;
        rotMat.setRotate(lastRot);
        camXf *= rotMat;

        //offset
        SbMatrix transMat;
        transMat.setTranslate(lastPos - calibPos);
        camXf *= transMat;

        //cam zurueck
        transMat.setTranslate(calibPos);
        camXf *= transMat;

        SbVec3f offset;
        SbRotation rotOffset;

	    camXf.getTransform(offset, rotOffset, s, so);

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

	        float* tRot = pOtOffset->getRotation();
            SbRotation totalRot(tRot);
            totalRot *= rotOffset;
            const float* sRot = totalRot.getValue();
	        tRot[0] = sRot[0];
	        tRot[1] = sRot[1];
	        tRot[2] = sRot[2];
	        tRot[3] = sRot[3];

            printf("\nSoDisplayCalibrationKit: %s.translation: [%f,%f,%f]\n\n", otNodeName.getValue().getString(), tOffset[0], tOffset[1], tOffset[2]);
            printf("\nSoDisplayCalibrationKit: %s.rotation: [%f,%f,%f,%f]\n\n", otNodeName.getValue().getString(), tRot[0], tRot[1], tRot[2],tRot[3]);
        }       
        else {
            fprintf(stderr, "SoDisplayCalibrationKit: OpenTracker node \"%s\" NOT found!\n", otNodeName.getValue().getString());
        }
        return;
    }
    if (ev->getStation() != station.getValue()) {
        return;
    }

    // we received an event from our station
    // remember this position as last one

    //set visible
    ((SoSwitch*)(baseSwitch.getValue()))->whichChild.setValue(0);

    // reset timeOut timer
    if( timeOut.getValue() > 0.0 )
    {
        timer.unschedule();
        timer.setTimeFromNow( timeOut.getValue());
        timer.schedule();
    }
    active = TRUE;

}

void SoDisplayCalibrationKit::timerFired(void *data, SoSensor *sensor)
{
    SoDisplayCalibrationKit* self = (SoDisplayCalibrationKit*)data;

    self->active = FALSE;
    ((SoSwitch*)(self->baseSwitch.getValue()))->whichChild.setValue(SO_SWITCH_NONE);

}
