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
/**
 * implementation of SoDragKit
 *
 * @author Dieter Schmalstieg, Gerhard Reitmayr, Jan Prikryl, Hannes Kaufmann
 *
 * $Id: SoDragKit.cxx 4089 2004-12-13 16:42:12Z reitmayr $
 * @file                                                                   */
/* ======================================================================= */

#include <cassert>
#include <Inventor/SbBox.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include "SoDragKit.h"

SO_KIT_SOURCE(SoDragKit);
BASE3D_SOURCE(SoDragKit);

//----------------------------------------------------------------------------

void
SoDragKit::initClass(void)
{
  SO_KIT_INIT_CLASS(SoDragKit, SoBaseKit, "BaseKit");
  BASE3D_INIT_CLASS(SoDragKit);
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoDragKit::SoDragKit()
:
validFlag(FALSE),
//pressCallback(NULL),
//releaseCallback(NULL),
//moveCallback(NULL),
//pressCallbackData(NULL),
//releaseCallbackData(NULL),
//moveCallbackData(NULL),
highlighted(FALSE),
allowExternalUpdate(TRUE)
{
  SO_KIT_CONSTRUCTOR(SoDragKit);
  //-----------------------name-----|-type-------|            nuldef|parent                   |right sibling|         public
  SO_KIT_ADD_CATALOG_ENTRY(base,               SoSeparator, FALSE, this,
    \x0,                TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(material,           SoMaterial,  FALSE, base,
    \x0,                TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(transform,          SoTransform, FALSE, base,
    \x0,                TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(highlightSwitch,    SoSwitch,    FALSE, base,
    \x0,                FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(highlightedContent, SoSeparator, TRUE, highlightSwitch,
    \x0,                TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(content,            SoSeparator, FALSE, highlightSwitch,
    highlightedContent, TRUE);
  
  SO_KIT_ADD_FIELD(translation, (0,0,0));
  SO_KIT_ADD_FIELD(rotation, (0,0,0,1));
  SO_KIT_ADD_FIELD(translationOn, (TRUE));
  SO_KIT_ADD_FIELD(rotationOn, (TRUE));
  SO_KIT_ADD_FIELD(xTranslationOn, (TRUE));
  SO_KIT_ADD_FIELD(yTranslationOn, (TRUE));
  SO_KIT_ADD_FIELD(zTranslationOn, (TRUE));
  SO_KIT_ADD_FIELD(xRotationOn, (TRUE));
  SO_KIT_ADD_FIELD(yRotationOn, (TRUE));
  SO_KIT_ADD_FIELD(zRotationOn, (TRUE));
  SO_KIT_ADD_FIELD(dragButton, (0));
  SO_KIT_ADD_FIELD(dragging, (FALSE));
  SO_KIT_ADD_FIELD(draggingStation, (-1));
  SO_KIT_INIT_INSTANCE();
  
  BASE3D_CONSTRUCTOR;
  
  globalToLocal.makeIdentity();

  SoSwitch* sw = (SoSwitch*)highlightSwitch.getValue();
  sw->whichChild = 0;

  snapBackSensor = new SoAlarmSensor(snapBack, this);
  translationSensor = new SoFieldSensor(externalUpdate, this);
  translationSensor->setPriority(0);
  translationSensor->attach(&translation);
  rotationSensor = new SoFieldSensor(externalUpdate, this);
  rotationSensor->setPriority(0);
  rotationSensor->attach(&rotation);
  setSearchingChildren(TRUE);

  setUpConnections(TRUE, TRUE);
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoDragKit::~SoDragKit()
{
  if(snapBackSensor)
    delete snapBackSensor;
  if(translationSensor)
    delete translationSensor;
  if(rotationSensor)
    delete rotationSensor;
}

SbBool SoDragKit::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    // todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onOff, doItAlways);

        SoTransform* xf = (SoTransform*)transform.getValue();
        xf->translation.connectFrom(&translation);
        xf->rotation.connectFrom(&rotation);
    } else {
        // We disconnect BEFORE base class.
        SoBaseKit::setUpConnections(onOff, doItAlways);        
    }
    return !(connectionsSetUp = onOff);
}

//-----------------------------------------------------------------------------
// returns: true if the node is interested in the 3d event of the given action
// h3a: action owning the 3d event

SbBool 
SoDragKit::isInterested(SoHandle3DEventAction *h3a)
{
  So3DEvent* ev = (So3DEvent*) h3a->getEvent(); // get underlying event
  
  SbViewportRegion vp; 
  SoGetBoundingBoxAction bba(vp);
  
  h3a->getPathFromRoot()->ref();
  bba.apply((SoFullPath*)h3a->getPathFromRoot());
  h3a->getPathFromRoot()->unrefNoDelete();
  
  SbXfBox3f xfbox   = bba.getXfBoundingBox();
  SbBool interested = xfbox.intersect(ev->getTranslation());
  
  // if ((ev->getType() == dragButton.getValue()+1) &&
  // (ev->getButton(dragButton.getValue())))
  
  return interested;
  
  // else return FALSE;
}

//-----------------------------------------------------------------------------
// SoDragKit::handle3DEvent(SoHandle3DEventAction* h3a)
// handle the 3d event passed by the action
// h3a: action owning the 3d event
// state diagram
// not dragging >>--- DRAGGER_PRESS ---->> dragging >>---DRAGGER_MOVE--|
//              <<----DRAGGER_RELEASE---<<          <<-----------------/
// on DRAGGER_PRESS :
// - store original local translation and rotation in trans0L, rot0L
// - store original pen position in world coordinates in startTransG, startRotG
// - compute the transformation matrix a long the path to the SoDragKit and store
//   in globalToLocal
// - compute the local original pen position in startTransL by multiplying with
//   globalToLocal
// - compute the offset between the pen position and the SoDragKits children in
//   local coordinates and store in pickOffsetL
// all this data is assumed to be constant through out the dragging.
//
// on DRAGGER_MOVE or DRAGGER_RELEASE :
// - compute the relative pen translation since DRAGGER_PRESS in world coordinates
//   relTransG = endTransG - startTransG
// - compute the relative pen rotation since DRAGGER_PRESS in world coordinates
//   relRotG = startRotG.inverse() * endRotG ( relRotG = endRotG o startRotG^(-1))
// - transfrom the relative pen rotation into local coordinates
//   relRotL = globalToLocal.inverse() * relRotG * globalToLocal; 
//   ( relRotL = globalToLocal o relRotG o globalToLocal^(-1) )
// - transform the global relative pen translation to local coordinates using
//   only the affine part ( no translations ).
// - rotate the children around the pen position by applying the relative pen
//   rotation to the offset vector and take the difference to the original 
//   offset vector. ( result = ( relrotL o pickOffsetL ) - OffsetL )
// - add local relative pen translation and rotational movement to the local
//   translation
// - now apply the rotational constraints. In quaternion format the components
//   of the rotation axis are set to 0, if rotation is restricted around.
//   afterwards the quaternion is normalized again.
// - the new rotation rot1L is then the local relative pen rotation after the old
//   rotation : rot1L = rot0L * relRotL ( rot1L =  relRotL o rot0L )
// - write back the new values testing for constraints

void 
SoDragKit::handle3DEvent(SoHandle3DEventAction* h3a)
{
  const int DRAGGER_RELEASE = 0;
  const int DRAGGER_PRESS   = 1;
  const int DRAGGER_MOVE    = 2;
  
  DISABLEDIVNODE(this); 
  SoSwitch* sw = (SoSwitch*)highlightSwitch.getValue();
  
  // compute event code
  So3DEvent* ev = (So3DEvent*)h3a->getEvent();
  int evType = -1; // default = no event
  if(ev->getType() == 0) 
    evType = DRAGGER_MOVE;
  else if(ev->getType() == dragButton.getValue()+1) // button event
    evType = ev->getButton(dragButton.getValue());
  
  if (!dragging.getValue() && evType == DRAGGER_MOVE)
  {
    if (!highlighted) {
      highlighted = TRUE;
      sw->whichChild = SO_SWITCH_ALL;
      h3a->setGrabber(this);
      h3a->setHandled();
    } else {
      if (!this->isInterested(h3a)) {
        highlighted = FALSE;
        sw->whichChild = 0;
        h3a->releaseGrabber();
        h3a->setHandled();
      }
    }
  } else if (!dragging.getValue() && evType == DRAGGER_PRESS)
  {
    if (snapBackSensor->isScheduled())
        snapBackSensor->unschedule();
    dragging.setValue(TRUE);
    draggingStation.setValue(ev->getStation());
    trans0L  = translation.getValue();
    rot0L    = rotation.getValue();
    startTransG  = ev->getTranslation();
    startRotG    = ev->getRotation();
    SbViewportRegion vp;
    SoGetMatrixAction ma(vp);

    h3a->getPathFromRoot()->ref();
    ma.apply((SoFullPath*)h3a->getPathFromRoot());
    h3a->getPathFromRoot()->unrefNoDelete();

    globalToLocal = ma.getInverse();
    SbVec3f startTransL;
    globalToLocal.multVecMatrix(startTransG,startTransL);
    pickOffsetL = trans0L-startTransL;
    h3a->setGrabber(this);
    h3a->setHandled();
    ENABLEDIVNODE(this);
    //if(pressCallback) pressCallback(pressCallbackData, this);
    triggerPressCallback();
  }
  else if(dragging.getValue() && evType == DRAGGER_RELEASE)
  {
    SbVec3f transRes; SbRotation rotRes;
    update(ev->getTranslation(), ev->getRotation(), transRes, rotRes);
    if(getBase3DMode()==Base3D::BASE3D_MASTERMODE) { ENABLEDIVNODE(this); }
    writeBack(transRes, rotRes);
    if(getBase3DMode()==Base3D::BASE3D_MASTERMODE)
    {
      DISABLEDIVNODE(this);
      SETDRAGKITSTATE(this, translation.getValue(), rotation.getValue());
    }
    else if(getBase3DMode()==Base3D::BASE3D_SLAVEMODE)
    {
      if (snapBackSensor->isScheduled())
        snapBackSensor->unschedule();
      snapBackSensor->setTimeFromNow(SbTime(SNAPBACK_TIME));
      snapBackSensor->schedule();
    }
    h3a->setHandled();
    dragging.setValue(FALSE);
    draggingStation.setValue(-1);
    highlighted = FALSE;
    sw->whichChild = 0;
    h3a->releaseGrabber();
    ENABLEDIVNODE(this);
    //if(releaseCallback) releaseCallback(releaseCallbackData, this);
    triggerReleaseCallback();
  }
  else if(dragging.getValue() && evType == DRAGGER_MOVE)
  {
    SbVec3f transRes; SbRotation rotRes;
    update(ev->getTranslation(), ev->getRotation(), transRes, rotRes);
    writeBack(transRes, rotRes);
    h3a->setHandled();
    ENABLEDIVNODE(this);
    //if(moveCallback) moveCallback(moveCallbackData, this);
    triggerMoveCallback();
  }
  ENABLEDIVNODE(this);
}

//-----------------------------------------------------------------------------

void 
SoDragKit::update(SbVec3f  transIn, SbRotation  rotIn,
                  SbVec3f& transOut,SbRotation& rotOut)
{
  SbVec3f    endTransG, relTransG, trans1L, tmp;
  SbRotation endRotG,   relRotG,   rot1L,   relRotL;
  endTransG  = transIn;
  endRotG    = rotIn;
  relTransG  = endTransG-startTransG;
  relRotG    = startRotG.inverse() * endRotG;    
  //    relRotL    = globalToLocal.inverse() * relRotG * globalToLocal;
  
  SbMatrix relRotGMatrix;
  relRotG.getValue(relRotGMatrix);
  SbMatrix theMatrix;
  theMatrix = globalToLocal.inverse() * relRotGMatrix * globalToLocal;
  
  SbVec3f t; SbVec3f s; SbRotation so;
  theMatrix.getTransform(t,relRotL,s,so);
  
  globalToLocal.multDirMatrix(relTransG,trans1L);
  trans1L   += trans0L;
  relRotL.multVec(pickOffsetL,tmp);
  trans1L   += (tmp-pickOffsetL);
  
  // rotation part has to take (x|y|z)RotationOn into account
  const float* rotValue = relRotL.getValue();
  float newValue[4], oldLen, newLen;
  newValue[0] = xRotationOn.getValue() ? rotValue[0] : 0;
  newValue[1] = yRotationOn.getValue() ? rotValue[1] : 0;
  newValue[2] = zRotationOn.getValue() ? rotValue[2] : 0;
  newValue[3] = rotValue[3];
  oldLen      = rotValue[0] * rotValue[0] +  rotValue[1] * rotValue[1] +
    rotValue[2] * rotValue[2];
  newLen      = newValue[0] * newValue[0] + newValue[1] * newValue[1] + 
    newValue[2] * newValue[2];
  if( newLen == 0.0 ) // the constrained rotation is not a rotation anymore
  { 
    relRotL = SbRotation::identity();
  } 
  else 
  {
    float k      = sqrt( oldLen / newLen );
    newValue[0] *= k;
    newValue[1] *= k;
    newValue[2] *= k;
    relRotL.setValue( newValue );
  }
  rot1L      = rot0L * relRotL;
  
  transOut   = trans1L;
  rotOut     = rot1L;
}

//-----------------------------------------------------------------------------

void
SoDragKit::writeBack(SbVec3f trans, SbRotation rot)
{
  allowExternalUpdate = FALSE;
  float tmp[3];
  tmp[0] = xTranslationOn.getValue() ? trans[0] : translation.getValue()[0];
  tmp[1] = yTranslationOn.getValue() ? trans[1] : translation.getValue()[1];
  tmp[2] = zTranslationOn.getValue() ? trans[2] : translation.getValue()[2];
  
  if(translationOn.getValue()) translation = SbVec3f(tmp);
  if(rotationOn.getValue())    rotation = rot;
  allowExternalUpdate = TRUE;
}

//-----------------------------------------------------------------------------

void
SoDragKit::setPressCallback(SoDragKitCB* cb, void* userData)
{
  pressCallback.push_back(cb);
  pressCallbackData.push_back(userData);
}

//-----------------------------------------------------------------------------

void
SoDragKit::setReleaseCallback(SoDragKitCB* cb, void* userData)
{
  releaseCallback.push_back(cb);
  releaseCallbackData.push_back(userData);
}

//-----------------------------------------------------------------------------

void
SoDragKit::setMoveCallback(SoDragKitCB* cb, void* userData)
{
  moveCallback.push_back(cb);
  moveCallbackData.push_back(userData);
}

//-----------------------------------------------------------------------------

void
SoDragKit::removePressCallback(SoDragKitCB* cb, void* userData)
{
  for(unsigned int i = 0; i < pressCallback.size(); ++i)
    if(cb == pressCallback[i] && userData == pressCallbackData[i])
	{
	  pressCallback[i] = pressCallback.back();
	  pressCallbackData[i] = pressCallbackData.back();
	  pressCallback.pop_back();
	  pressCallbackData.pop_back();
	}
}

//-----------------------------------------------------------------------------

void
SoDragKit::removeReleaseCallback(SoDragKitCB* cb, void* userData)
{
  for(unsigned int i = 0; i < releaseCallback.size(); ++i)
    if(cb == releaseCallback[i] && userData == releaseCallbackData[i])
	{
	  releaseCallback[i] = releaseCallback.back();
	  releaseCallbackData[i] = releaseCallbackData.back();
	  releaseCallback.pop_back();
	  releaseCallbackData.pop_back();
	}
}

//-----------------------------------------------------------------------------

void
SoDragKit::removeMoveCallback(SoDragKitCB* cb, void* userData)
{
  for(unsigned int i = 0; i < moveCallback.size(); ++i)
    if(cb == moveCallback[i] && userData == moveCallbackData[i])
	{
	  moveCallback[i] = moveCallback.back();
	  moveCallbackData[i] = moveCallbackData.back();
	  moveCallback.pop_back();
	  moveCallbackData.pop_back();
	}
}

//-----------------------------------------------------------------------------

void
SoDragKit::triggerPressCallback()
{
  OnPress();
  for(unsigned int i = 0; i < pressCallback.size(); ++i)
    pressCallback[i](pressCallbackData[i], this);
}

//-----------------------------------------------------------------------------

void
SoDragKit::triggerReleaseCallback()
{
  OnRelease();
  for(unsigned int i = 0; i < releaseCallback.size(); ++i)
    releaseCallback[i](releaseCallbackData[i], this);
}

//-----------------------------------------------------------------------------

void
SoDragKit::triggerMoveCallback()
{
  OnMove();
  for(unsigned int i = 0; i < moveCallback.size(); ++i)
    moveCallback[i](moveCallbackData[i], this);
}

//-----------------------------------------------------------------------------

void
SoDragKit::updateFromNetwork(SbVec3f trans,
                             SbRotation rot)
{
  allowExternalUpdate = FALSE;
  if (snapBackSensor->isScheduled())
    snapBackSensor->unschedule();
  translation=trans;
  rotation=rot;
  validFlag=TRUE;
  currentSetTranslation=trans;
  currentSetRotation=rot;
  allowExternalUpdate = TRUE;
}

void
SoDragKit::snapBack(void *data, SoSensor *sensor)
{
  SoDragKit *self=(SoDragKit*)data;
  self->allowExternalUpdate = FALSE;
  if(!self->validFlag)
  {
    self->translation=self->trans0L;
    self->rotation=self->rot0L;	 
  }
  else
  {
    self->translation=self->currentSetTranslation;
    self->rotation=self->currentSetRotation;
  }
  self->allowExternalUpdate = TRUE;
}

void
SoDragKit::externalUpdate(void *data, SoSensor *sensor)
{
  SoDragKit *self=(SoDragKit*)data;
  if(self->allowExternalUpdate ||
	   self->getBase3DMode()==Base3D::BASE3D_MASTERMODE)
  {
    if (self->snapBackSensor->isScheduled())
        self->snapBackSensor->unschedule();
    self->validFlag=TRUE;
    if(sensor == (SoSensor*) self->translationSensor)
      self->currentSetTranslation=self->translation.getValue();
    else if(sensor == (SoSensor*)self->rotationSensor)
      self->currentSetRotation=self->rotation.getValue();
  }
}

/* ===========================================================================
End of SoDragKit.cxx
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

