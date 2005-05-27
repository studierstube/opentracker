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
/** SoStereoCameraKit.cxx contains the implementation of class SoStereoCameraKit
  *
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoStereoCameraKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#include <cassert>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoWriteAction.h>
#include <stbapi/tracker/SoTrakEngine.h>
#include <Inventor/engines/SoTransformVec3f.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <stbapi/context/SoContext.h>
#include <stbapi/context/StudierstubeContext.h>

#include "SoStereoCameraKit.h"
#include "SoOffAxisCamera.h"
#include "MultRotRot.h"

SO_KIT_SOURCE(SoStereoCameraKit);

//----------------------------------------------------------------------------
// Inventor class initialization.

void
SoStereoCameraKit::initClass()
{
    SO_KIT_INIT_CLASS(SoStereoCameraKit, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------
// The constructor inits the fields of the kit. 

SoStereoCameraKit::SoStereoCameraKit()
{
    SO_KIT_CONSTRUCTOR(SoStereoCameraKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    //   name,className,nullByDefault,parentName,rightSiblingName,isPublic

    SO_KIT_ADD_CATALOG_ENTRY(
        xfSep,    SoTransformSeparator,FALSE,this,     \0,FALSE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(
	    examinerCam, SoCamera, SoPerspectiveCamera, TRUE, xfSep, \0, FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(
        camXf,    SoTransform,         TRUE,xfSep,    \0,FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(
        camSwitch,SoSwitch,            FALSE,xfSep,    \0,FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(
        groupLeft,  SoGroup,     FALSE,camSwitch,\0, FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(
        contextLeft,  SoContext,   FALSE,groupLeft,\0, TRUE );    
    SO_KIT_ADD_CATALOG_ENTRY(
        camLeft,  SoOffAxisCamera,     FALSE,groupLeft,\0,TRUE );
    SO_KIT_ADD_CATALOG_ENTRY(
        groupRight,  SoGroup,     FALSE,camSwitch,\0, FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(
        contextRight,  SoContext,   FALSE,groupRight,\0, TRUE );    
    SO_KIT_ADD_CATALOG_ENTRY(
        camRight, SoOffAxisCamera,     FALSE,groupRight,\0,TRUE );

	// add fields
	SO_KIT_ADD_FIELD(eyeOffsetLeft, (0.0, 0.0, 0.0)); 
	SO_KIT_ADD_FIELD(eyeOffsetRight, (0.0, 0.0, 0.0)); 
	SO_KIT_ADD_FIELD(displayOffsetLeft, (0.0, 0.0, 0.0)); 
	SO_KIT_ADD_FIELD(displayOffsetRight, (0.0, 0.0, 0.0)); 
	SO_KIT_ADD_FIELD(displayRotationLeft, (SbVec3f( 0.f, 0.f, 1.f ), 0.f )); 
	SO_KIT_ADD_FIELD(displayRotationRight, (SbVec3f( 0.f, 0.f, 1.f ), 0.f )); 


    SO_KIT_INIT_INSTANCE();

    assert(SO_GET_ANY_PART(this, "camLeft", SoOffAxisCamera));
	getExaminerCamera()->setName("defaultExaminerCamera");

    // The call to selectCameraLeft(); used to be here. It did not alwyas
    // work as expected though (clipping plane problems).
    
    setSearchingChildren(TRUE);

    SoContext * context = SO_GET_ANY_PART(this, "contextLeft", SoContext);
    context->mode.setValue( SoContext::ADD );
    context->value.setValue( StudierstubeContext::Left );
    context->index.setValue( StudierstubeContext::Eye );

    context = SO_GET_ANY_PART(this, "contextRight", SoContext);
    context->mode.setValue( SoContext::ADD );
    context->value.setValue( StudierstubeContext::Right );
    context->index.setValue( StudierstubeContext::Eye );
}


//----------------------------------------------------------------------------
// The destructor does nothing.

SoStereoCameraKit::~SoStereoCameraKit()
{
    // nil
}


//----------------------------------------------------------------------------
void
SoStereoCameraKit::disconnectHeadTracker()
{
	getCameraLeft()->eyepointPosition.disconnect();
	getCameraRight()->eyepointPosition.disconnect();
}


//----------------------------------------------------------------------------
void SoStereoCameraKit::connectHeadTracker(SoTrakEngine *tracker)
{
	assert(tracker);

	disconnectHeadTracker();

	// use engine to create tracker to world transformation matrix
	SoComposeMatrix *ctw = new SoComposeMatrix;
	ctw->translation.connectFrom(&tracker->translation);
	ctw->rotation.connectFrom(&tracker->rotation);

	connectHeadTrackerStep2(ctw);
}



//----------------------------------------------------------------------------
void
SoStereoCameraKit::connectHeadTracker(SoSFVec3f *trackerTranslation,
												 SoSFRotation *trackerRotation)
{
	disconnectHeadTracker();

	// use engine to create tracker to world transformation matrix
	SoComposeMatrix *ctw = new SoComposeMatrix;
	ctw->translation.connectFrom(trackerTranslation);
	ctw->rotation.connectFrom(trackerRotation);

	connectHeadTrackerStep2(ctw);
}


//----------------------------------------------------------------------------
void
SoStereoCameraKit::connectHeadTrackerStep2(SoComposeMatrix *ctw)
{
	// use engines to transform offsets
	SoTransformVec3f *teL, *teR;
	teL = new SoTransformVec3f;
	teL->vector.connectFrom(&eyeOffsetLeft);
	teL->matrix.connectFrom(&ctw->matrix);

	teR = new SoTransformVec3f;
	teR->vector.connectFrom(&eyeOffsetRight);
	teR->matrix.connectFrom(&ctw->matrix);

	// connect eyepointPositions to transformed offsets
	getCameraLeft()->eyepointPosition.connectFrom(&teL->point);	
	getCameraRight()->eyepointPosition.connectFrom(&teR->point);		
}



//----------------------------------------------------------------------------

void
SoStereoCameraKit::disconnectDisplayTracker()
{
	getCameraLeft()->position.disconnect();
	getCameraRight()->position.disconnect();
	getCameraLeft()->orientation.disconnect();
	getCameraRight()->orientation.disconnect();
}


//----------------------------------------------------------------------------
void
SoStereoCameraKit::connectDisplayTracker(SoSFVec3f *trackerTranslation,
													 SoSFRotation *trackerRotation)
{
	disconnectDisplayTracker();

	// use engine to create tracker to world transformation matrix
	SoComposeMatrix *ctw = new SoComposeMatrix;
	ctw->translation.connectFrom(trackerTranslation);
	ctw->rotation.connectFrom(trackerRotation);
	
	connectDisplayTrackerStep2(ctw);

	// use engines to calculate rotations	
	MultRotRot *mdrL = new MultRotRot;
	mdrL->rotationA.connectFrom(&displayRotationLeft);
	mdrL->rotationB.connectFrom(trackerRotation);

	MultRotRot *mdrR = new MultRotRot;
	mdrR->rotationA.connectFrom(&displayRotationRight);
	mdrR->rotationB.connectFrom(trackerRotation);

	// connect orientations to calculated rotations
	getCameraLeft()->orientation.connectFrom(&mdrL->product);
	getCameraRight()->orientation.connectFrom(&mdrR->product);

}



//----------------------------------------------------------------------------
void
SoStereoCameraKit::connectDisplayTracker(SoTrakEngine *tracker)
{
	assert(tracker);

	disconnectDisplayTracker();

	// use engine to create tracker to world transformation matrix
	SoComposeMatrix *ctw = new SoComposeMatrix;
	ctw->translation.connectFrom(&tracker->translation);
	ctw->rotation.connectFrom(&tracker->rotation);
	
	connectDisplayTrackerStep2(ctw);

	// use engines to calculate rotations	
	MultRotRot *mdrL = new MultRotRot;
	mdrL->rotationA.connectFrom(&displayRotationLeft);
	mdrL->rotationB.connectFrom(&tracker->rotation);

	MultRotRot *mdrR = new MultRotRot;
	mdrR->rotationA.connectFrom(&displayRotationRight);
	mdrR->rotationB.connectFrom(&tracker->rotation);

	// connect orientations to calculated rotations
	getCameraLeft()->orientation.connectFrom(&mdrL->product);
	getCameraRight()->orientation.connectFrom(&mdrR->product);

}


//----------------------------------------------------------------------------
void
SoStereoCameraKit::connectDisplayTrackerStep2(SoComposeMatrix *ctw)
{
	// use engines to transform offsets
	SoTransformVec3f *tdoL, *tdoR;
	tdoL = new SoTransformVec3f;
	tdoL->vector.connectFrom(&displayOffsetLeft);
	tdoL->matrix.connectFrom(&ctw->matrix);

	tdoR = new SoTransformVec3f;
	tdoR->vector.connectFrom(&displayOffsetRight);
	tdoR->matrix.connectFrom(&ctw->matrix);

	// connect positions to transformed offsets
	getCameraLeft()->position.connectFrom(&tdoL->point);	
	getCameraRight()->position.connectFrom(&tdoR->point);		
}


//----------------------------------------------------------------------------
// Switches to camera for left eye.

void
SoStereoCameraKit::selectCameraLeft()
{
    SO_GET_ANY_PART(this, "camSwitch", SoSwitch)->whichChild = 0;
}


//----------------------------------------------------------------------------
// Switches to camera for right eye.

void
SoStereoCameraKit::selectCameraRight()
{
    SO_GET_ANY_PART(this, "camSwitch", SoSwitch)->whichChild = 1;
}

//----------------------------------------------------------------------------
// Returns left camera.

SoOffAxisCamera*
SoStereoCameraKit::getCameraLeft()
{
    return SO_GET_PART(this, "camLeft", SoOffAxisCamera );
}

//----------------------------------------------------------------------------
// Returns right camera.

SoOffAxisCamera*
SoStereoCameraKit::getCameraRight()
{
    return SO_GET_PART(this, "camRight", SoOffAxisCamera );
}

//----------------------------------------------------------------------------
// Returns TRUE, if left camera is active.

SbBool 
SoStereoCameraKit::isActiveCameraLeft()
{
    return getStereoSwitch()->whichChild.getValue() == 0 ? TRUE : FALSE;
}    

//----------------------------------------------------------------------------
// Returns TRUE, if right camera is active.

SbBool 
SoStereoCameraKit::isActiveCameraRight()
{
    return getStereoSwitch()->whichChild.getValue() == 1 ? TRUE : FALSE;
}    

//----------------------------------------------------------------------------
// Returns "the rack", i.e. the camXf part.

SoTransform*
SoStereoCameraKit::getTransform()
{
    return SO_GET_ANY_PART(this,"camXf",SoTransform);
}

//----------------------------------------------------------------------------
// Returns the camera, which is presented to the SoGuiExaminerViewer.
  
SoCamera*
SoStereoCameraKit::getExaminerCamera()
{
    return SO_GET_ANY_PART(this,"examinerCam",SoCamera);
}

//----------------------------------------------------------------------------
// Returns the camera switch.

SoSwitch*
SoStereoCameraKit::getStereoSwitch()
{
    return SO_GET_ANY_PART(this,"camSwitch",SoSwitch);
}

//----------------------------------------------------------------------------

//eof

/* ===========================================================================
    End of SoStereoCameraKit.cxx
   ===========================================================================
    Automatic Emacs configuration follows.
    Local Variables:
    mode:c++
    c-basic-offset: 4
    eval: (c-set-offset 'substatement-open 0)
    eval: (c-set-offset 'case-label '+)
    eval: (c-set-offset 'statement 'c-lineup-runin-statements)
    eval: (setq indent-tabs-mode nil)
    End:
=========================================================================== */
