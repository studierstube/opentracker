 /* ========================================================================
  * Copyright (C) 2003  Vienna University of Technology
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
/** SoBillboardKit - implementation
  *
  * @author  Hannes Kaufmann, Erich Pohn, Dieter Schmalstieg
  *
  * $Id: SoBillboardKit.cxx 4092 2004-12-14 16:15:55Z reitmayr $
  * @file                                                                   */
 /* ======================================================================= */

#include "SoBillboardKit.h"

#include <stbapi/context/StudierstubeContext.h>
#include <stbapi/context/SoContextElement.h>

SO_KIT_SOURCE(SoBillboardKit);

//----------------------------------------------------------------------------
/** Inventor class initialization */
//----------------------------------------------------------------------------
void SoBillboardKit::initClass(void) {
	SO_KIT_INIT_CLASS(SoBillboardKit, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------
/** constructor */
//----------------------------------------------------------------------------
SoBillboardKit::SoBillboardKit() {

    SO_KIT_CONSTRUCTOR(SoBillboardKit);

	SO_KIT_ADD_CATALOG_ENTRY(root,    SoSeparator, FALSE, this, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(cbNode,  SoCallback,  FALSE, root, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(transform,SoTransform,  FALSE, root, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(content, SoSeparator, FALSE, root, \x0, TRUE);

	SO_KIT_INIT_INSTANCE();

    SoCallback* cb = SO_GET_ANY_PART(this, "cbNode", SoCallback);
    cb->setCallback(billboardCB,this);

	SoTransform* trans = SO_GET_ANY_PART(this, "transform", SoTransform);
	CDivMain::globalDisableNode(trans);
	
    setSearchingChildren(TRUE);
}

//----------------------------------------------------------------------------
/** destructor */
//----------------------------------------------------------------------------
SoBillboardKit::~SoBillboardKit() {
}

//----------------------------------------------------------------------------
/** returns position of active eye */
//----------------------------------------------------------------------------
SbVec3f SoBillboardKit::getActiveEyepoint(int userId)
{
    SoStereoCameraKit* ck;
    ck = SoUserManagerKit::getInstance()->getUserKit(userId)->getDisplay()
             ->getViewer()->getStereoCameraKit();

	// return middle eye position (between left and right eye)
	// add translation of SoOffAxisCamera Transform node 
	// this transform node is only not null in a tracked display mode
	SoTransform* camTransform = SoUserManagerKit::getInstance()->getUserKit(userId)->getDisplay()->getViewer()->getStereoCameraKit()->getTransform();
	SbMatrix cm, icm;
    camTransform->getTranslationSpaceMatrix(cm, icm);
	SbVec3f trans, dummy2;
	SbRotation dummy1, dummy3;
	cm.getTransform(trans, dummy1, dummy2, dummy3);

	return (ck->getCameraRight()->eyepointPosition.getValue() + ck->getCameraLeft()->eyepointPosition.getValue())/2 + trans;
}

//----------------------------------------------------------------------------
/** Callback for calculating rotation */
//----------------------------------------------------------------------------
void SoBillboardKit::billboardCB(void *userData, SoAction *action) {

    // do only for rendering
    if(action->getTypeId() != SoGLRenderAction::getClassTypeId()) return; 

	SoBillboardKit *self = (SoBillboardKit *)userData;

    self->performRotation(action->getState());
}

//
// private method that appends the needed rotation to the state
//
void
SoBillboardKit::performRotation(SoState * state) const
{
	// in our case we always want rotaxis == SbVec3f(0.0f, 0.0f, 0.0f)
    // 1. Compute the billboard-to-viewer vector.
    // 2. Rotate the Z-axis of the billboard to be collinear with the
    //    billboard-to-viewer vector and pointing towards the viewer's position.
    // 3. Rotate the Y-axis of the billboard to be parallel and oriented in the
    //    same direction as the Y-axis of the viewer.
	
	SbRotation rot;
	const SbMatrix & mm = SoModelMatrixElement::get(state);
	SbMatrix imm = mm.inverse();
	SbVec3f toviewer;
	
	// Always a perspective projection in our case
	SbVec3f worldpos(0.0f, 0.0f, 0.0f);
	mm.multVecMatrix(worldpos, worldpos);
	int userId = atoi(SoContextElement::get(state, StudierstubeContext::User).getString());
	toviewer = getActiveEyepoint(userId) - worldpos;
	imm.multDirMatrix(toviewer, toviewer);
	(void) toviewer.normalize();
	// rotate z-axis to current viewer (=toviewer)
	rot.setValue(SbVec3f(0.f, 0.0f, 1.0f), toviewer);
	//printf("Toviewer lokal: %f %f %f \n", toviewer[0],toviewer[1],toviewer[2]);
	
	SbMatrix cm, icm;
	SbVec3f yaxis(0.0f, 1.0f, 0.0f);
	SbVec3f yaxisLC, yaxisRotWC;
	// rotate y-axis with same rotation rot as above
	rot.multVec(yaxis,yaxisLC);

	// calculate rotation around z-axis from y to up-position
#ifdef  __COIN__
	SbViewVolume vv = SoUserManagerKit::getInstance()->getUserKit(userId)->getDisplay()->getViewer()->getStereoCameraKit()->getCameraLeft()->getViewVolume(0.0);
	// use getViewUp which is implemented in Coin in SbViewVolume for use in the VRMLBillboard node
	SbVec3f viewup = vv.getViewUp();
	SoTransform* camTransform = SoUserManagerKit::getInstance()->getUserKit(userId)->getDisplay()->getViewer()->getStereoCameraKit()->getTransform();
    camTransform->getRotationSpaceMatrix(cm, icm);
	SbRotation camOr(cm);
	camOr.multVec(viewup, yaxisRotWC);
#else
	// combine (=multiply) cameraOrientation with the orientation inside 
	// the SoTransform node of the SoOffAxisCamera to get the final orientation of
	// the camera also for tracked display modes
	// In non-tracked display modes the SoTransform node has a zero rotation
	SbRotation camOr = SoUserManagerKit::getInstance()->getUserKit(userId)->getDisplay()->getViewer()->getStereoCameraKit()->getCameraLeft()->orientation.getValue();
	SoTransform* camTransform = SoUserManagerKit::getInstance()->getUserKit(userId)->getDisplay()->getViewer()->getStereoCameraKit()->getTransform();
    camTransform->getRotationSpaceMatrix(cm, icm);
	SbRotation camOr2(cm);
	camOr = camOr*camOr2;
	// rotate y-axis with combined camera rotation 
	camOr.multVec(yaxis, yaxisRotWC);
#endif

	imm.multDirMatrix(yaxisRotWC, yaxisRotWC);
	// calculate "correct" angle between those two vectors and rotate around toviewer axis
	// this also avoids certain "jumps" in the rotation angle
	SbVec3f p1 = toviewer.cross(yaxisLC);
	SbVec3f p2 = toviewer.cross(yaxisRotWC);
	SbRotation rot2 = SbRotation(p1,p2);

	// combine rot (for z-axis) and rot2 (for correct y-axis alignment) to get final rotation
    rot = rot * rot2;
	//    SoModelMatrixElement::rotateBy(state, (SoNode*) this, rot);
	// set rotation of internal transform node
	SoTransform* trans = (SoTransform*)transform.getValue();
    if(rot != trans->rotation.getValue())
    {
        trans->rotation.setValue(rot);
    }
}
