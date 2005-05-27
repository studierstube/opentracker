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
/** SoDesktopControlMode.cxx contains the implementation of class SoDesktopControlMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoDesktopControlMode.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <cassert>

#include "SoDesktopControlMode.h"

SO_NODE_SOURCE(SoDesktopControlMode);

//----------------------------------------------------------------------------
// Inventor class initialization 

void 
SoDesktopControlMode::initClass()
{
    SO_NODE_INIT_CLASS(SoDesktopControlMode, SoCameraControlMode, "SoCameraControlMode");
}

//----------------------------------------------------------------------------
// The constructor inits the node 

SoDesktopControlMode::SoDesktopControlMode()
:
examinerMode(FALSE)
{
    SO_NODE_CONSTRUCTOR(SoDesktopControlMode);
}

//----------------------------------------------------------------------------

SoDesktopControlMode::~SoDesktopControlMode()
{
    // nil
}

//----------------------------------------------------------------------------
// Returns TRUE

SbBool
SoDesktopControlMode::isViewing()
{
	return TRUE;
}

//----------------------------------------------------------------------------
// Switch to the left camera view.

void 
SoDesktopControlMode::selectCameraLeft()
{
    assert(camKit);
    camKit->getStereoSwitch()->whichChild = examinerMode ? -1 : 0;
}

//----------------------------------------------------------------------------
// Switch to the right camera view.

void
SoDesktopControlMode::selectCameraRight()
{
    assert(camKit);
	camKit->getStereoSwitch()->whichChild = examinerMode ? -1 : 1;
}

//----------------------------------------------------------------------------
// Returns the position of the left eye in world space.

SbVec3f
SoDesktopControlMode::getEyePositionLeft()
{
	// the eyepointPosition is in world space
	return examinerMode ? camKit->getExaminerCamera()->position.getValue() :
	                      camKit->getCameraLeft()->eyepointPosition.getValue();
} 

//----------------------------------------------------------------------------
// Returns the position of the right eye in world space.

SbVec3f
SoDesktopControlMode::getEyePositionRight()
{
	// the eyepointPosition is in world space
    return examinerMode ? camKit->getExaminerCamera()->position.getValue() :
	                      camKit->getCameraRight()->eyepointPosition.getValue();
} 
    
//----------------------------------------------------------------------------
// Sets camKit to aCamKit and decides on use of examinerMode
  
void
SoDesktopControlMode::setStereoCameraKit(SoStereoCameraKit* aCamKit)
{
//--first remember the camera
    SoCameraControlMode::setStereoCameraKit(aCamKit);
    assert(camKit != NULL);
    
	SoCamera *examinerCam = camKit->getExaminerCamera();
	// if the examiner camera is not the default camera but an perspective or 
    // orthographic camera
	if ((examinerCam->getName() != "defaultExaminerCamera") &&
	    (examinerCam->isOfType(SoPerspectiveCamera::getClassTypeId()) ||
	     examinerCam->isOfType(SoOrthographicCamera::getClassTypeId())))
	{
		examinerMode = TRUE;
		return;
	}

	assert(examinerCam->getName() == "defaultExaminerCamera");

	// read needed values from the cameras in the kit
	SoOffAxisCamera *camLeft, *camRight;
	camLeft = camKit->getCameraLeft();
	camRight = camKit->getCameraRight();
	SbRotation rotLeft, rotRight, rotRef;
	SbVec3f eposLeft, eposRight;
	SbVec3f posLeft, posRight, posRef ;
	eposLeft = camLeft->eyepointPosition.getValue();
	posLeft = camLeft->position.getValue();
	rotLeft = camLeft->orientation.getValue();
	eposRight = camRight->eyepointPosition.getValue();
	posRight = camRight->position.getValue();
	rotRight = camRight->orientation.getValue();

	// build values of referenceCamera for SoGuiExaminerViewer editing
	SoPerspectiveCamera *refCam = (SoPerspectiveCamera *)examinerCam;
	// set position 
	posRef = refCam->position = (eposLeft + eposRight)/2.0f;
    SbRotation diff;	SbVec3f axis;	float angle;
    diff = rotLeft*rotRight.inverse();
    diff.getValue(axis, angle);
    diff.setValue(axis,angle/2.0f);
	rotRef = refCam->orientation = diff * rotRight;
	refCam->aspectRatio = camLeft->size.getValue()[0]/camLeft->size.getValue()[1];
    SbVec3f displayPosRef = (posLeft + posRight)/2.0f;
    float pp = (displayPosRef - posRef).length();
    float ak,gk;		
    ak = (float)(sqrt(pow(pp,2)+ pow(camLeft->size.getValue()[1]/2.0f,2)));
    gk = camLeft->size.getValue()[1]/2.0f;
	refCam->heightAngle = (float)(atan(gk/ak)*2.0f);
	refCam->focalDistance = camLeft->focalDistance;
	refCam->nearDistance = camLeft->nearDistance;
	refCam->farDistance = camLeft->farDistance;
	refCam->viewportMapping = camLeft->viewportMapping;

	// set offsets in camKit
	SbVec3f temp;
	rotRef.inverse().multVec((eposLeft - posRef),temp);
	camKit->eyeOffsetLeft = temp;
	rotRef.inverse().multVec((eposRight - posRef),temp);
	camKit->eyeOffsetRight = temp;

	rotRef.inverse().multVec((posLeft - posRef),temp);
	camKit->displayOffsetLeft = temp;
	rotRef.inverse().multVec((posRight - posRef),temp);
	camKit->displayOffsetRight = temp;

	camKit->displayRotationLeft =  diff;
	camKit->displayRotationRight = diff.inverse();

	// connect refCam to head and display
	camKit->connectHeadTracker(&refCam->position, &refCam->orientation);
	camKit->connectDisplayTracker(&refCam->position, &refCam->orientation);
}

//----------------------------------------------------------------------------
//eof

/* ===========================================================================
    End of SoDesktopControlMode.cxx
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
