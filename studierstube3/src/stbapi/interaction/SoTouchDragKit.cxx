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
/** 3D Dragger for "magic wand" (no button)
  *
  * @author Jan Prikryl
  *
  * $Id: SoTouchDragKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <cassert>

#include <Inventor/SbBox.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include "SoTouchDragKit.h"

SO_KIT_SOURCE(SoTouchDragKit);
BASE3D_SOURCE(SoTouchDragKit);

//----------------------------------------------------------------------------

void
SoTouchDragKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoTouchDragKit, SoBaseKit, "BaseKit");
    BASE3D_INIT_CLASS(SoTouchDragKit);
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoTouchDragKit::SoTouchDragKit()
:
    highlighted(FALSE),
    doDragging(FALSE),
    pressCallback(NULL),
    releaseCallback(NULL),
    moveCallback(NULL),
    pressCallbackData(NULL),
    releaseCallbackData(NULL),
    moveCallbackData(NULL)
{
    SO_KIT_CONSTRUCTOR(SoTouchDragKit);
    //-----------------------name-----|-type-------|        nuldef|parent           |right sibling|     public
    SO_KIT_ADD_CATALOG_ENTRY(base,      SoSeparator,        FALSE,  this,           \x0,                TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(transform, SoTransform,        FALSE,  base,           \x0,                TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(highlightSwitch, SoSwitch,     FALSE,  base,           \x0,                FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(highlightedContent,SoSeparator,TRUE,   highlightSwitch,\x0,                TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(content  , SoSeparator,        FALSE,  highlightSwitch,highlightedContent, TRUE);
    
    SO_KIT_ADD_FIELD(highlightRadius, (1));
    SO_KIT_ADD_FIELD(hysteresis, (0.1));
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
    SO_KIT_INIT_INSTANCE();
    
    BASE3D_CONSTRUCTOR;
    
    globalToLocal.makeIdentity();
    SoTransform* xf = (SoTransform*)transform.getValue();
    xf->translation.connectFrom(&translation);
    xf->rotation.connectFrom(&rotation);
    
    SoSwitch* sw = (SoSwitch*)highlightSwitch.getValue();
    sw->whichChild = 0;

    setSearchingChildren(TRUE);
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoTouchDragKit::~SoTouchDragKit()
{
    // nil
}

//-----------------------------------------------------------------------------
// returns: true if the node is interested in the 3d event of the given action
// h3a: action owning the 3d event

SbBool 
SoTouchDragKit::isInterested(SoHandle3DEventAction *h3a)
{
    // get underlying event
    So3DEvent* ev = (So3DEvent*) h3a->getEvent();
    
    // test the bounding sphere
    SbViewportRegion vp;
    SoGetBoundingBoxAction bba(vp);

	SoFullPath *fp = (SoFullPath*)h3a->getPathFromRoot();
	fp->ref();
    bba.apply(fp);
    SoGetMatrixAction ma(vp);
    ma.apply(fp);
	fp->unrefNoDelete();

    globalToLocal = ma.getInverse();
    SbXfBox3f xfbox = bba.getXfBoundingBox();
    
    SbVec3f centL, tranL;
    globalToLocal.multVecMatrix(xfbox.getCenter(),    centL);
    globalToLocal.multVecMatrix(ev->getTranslation(), tranL);
    centL -= tranL;
    SbBool interested = centL.length() < highlightRadius.getValue();
    
    if (interested)
    {
        // initialize dragging 
        doDragging = FALSE;
    }

    return interested;
}

//-----------------------------------------------------------------------------
// SoTouchDragKit::handle3DEvent(SoHandle3DEventAction* h3a)
// handle the 3d event passed by the action
//-----------------------------------------------------------------------------

void 
SoTouchDragKit::handle3DEvent(SoHandle3DEventAction* h3a)
{
    // get access to the higlight switch
    SoSwitch* sw = (SoSwitch*)highlightSwitch.getValue();
    
    // event
    So3DEvent* ev = (So3DEvent*)h3a->getEvent();

    // first test whether the pen is still in the boundingbox
    // of the node
    SbViewportRegion vp;
    SoGetBoundingBoxAction bba(vp);

	SoFullPath *fp = (SoFullPath*)h3a->getPathFromRoot();
	fp->ref();
    bba.apply(fp);
	fp->unrefNoDelete();
    SbXfBox3f xfbox = bba.getXfBoundingBox();

    SbVec3f centL, tranL;
    globalToLocal.multVecMatrix(xfbox.getCenter(),    centL);
    globalToLocal.multVecMatrix(ev->getTranslation(), tranL);
    centL -= tranL;
    SbBool inside = centL.length() < (highlightRadius.getValue() + hysteresis.getValue());

    if ( inside )
    {
        // yes, the pen shall manipulate the object
        if ( doDragging == FALSE )
        {
            // highlight the content and grab all coming
            // events
            highlighted = TRUE;
            sw->whichChild = SO_SWITCH_ALL;
            h3a->setGrabber(this);
            h3a->setHandled();
            // as the pen has moved into the geometry,
            // we shall now manipulate the object until
            // it moves out again
            trans0L     = translation.getValue();
            rot0L       = rotation.getValue();
            pickOffsetL = trans0L - tranL;
            // process callbacks
            if (pressCallback) pressCallback(pressCallbackData, this);
            // we manupulate the content until pen moves out
            // of the boundingbox
            doDragging = TRUE;
        }
        else
        {
            // manipulating the object
            SbVec3f    transRes;
            SbRotation rotRes;
            update(ev->getTranslation(), ev->getRotation(), transRes, rotRes);
            writeBack(transRes, rotRes);
            h3a->setHandled();
            if (moveCallback) moveCallback(moveCallbackData, this);
        }
    }
    else
    {
        // nope, no maniplation anymore, so clear the flags
        doDragging = FALSE;
        highlighted = FALSE;
        sw->whichChild = 0;
        h3a->releaseGrabber();
        h3a->setHandled();
        // release callback
        if(releaseCallback) releaseCallback(releaseCallbackData, this);
    }
}

//-----------------------------------------------------------------------------

void 
SoTouchDragKit::update(SbVec3f  transIn, SbRotation  rotIn,
                       SbVec3f& transOut,SbRotation& rotOut)
{
    SbRotation relRotL;

    // the raotation is determined by an offset vertor with respect
    // to the initial direction 
    SbVec3f    transInL;
    globalToLocal.multVecMatrix(transIn, transInL);
    
    SbVec3f    currentOffsetL = trans0L - transInL;
    SbRotation newRot(pickOffsetL, currentOffsetL);
    
    // we have to take (x|y|z)RotationOn into account (not tested)

    const float * oldValue = newRot.getValue();
    float         newValue[4], oldLen, newLen;

    newValue[0] = xRotationOn.getValue() ? oldValue[0] : 0;
    newValue[1] = yRotationOn.getValue() ? oldValue[1] : 0;
    newValue[2] = zRotationOn.getValue() ? oldValue[2] : 0;
    newValue[3] = oldValue[3];

    oldLen      = oldValue[0] * oldValue[0] + 
                  oldValue[1] * oldValue[1] +
                  oldValue[2] * oldValue[2];
    newLen      = newValue[0] * newValue[0] + 
                  newValue[1] * newValue[1] + 
                  newValue[2] * newValue[2];

    if( newLen == 0.0 ) 
    {
        // the constrained rotation is not a rotation anymore
        relRotL = SbRotation::identity();
    } 
    else 
    {
        float k      = (float)(sqrt( oldLen / newLen ));
        newValue[0] *= k;
        newValue[1] *= k;
        newValue[2] *= k;
        relRotL.setValue( newValue );
    }

    rotOut = rot0L * relRotL;
}

//-----------------------------------------------------------------------------

void
SoTouchDragKit::writeBack(SbVec3f trans, SbRotation rot)
{
    /*
    float tmp[3];
    tmp[0] = xTranslationOn.getValue() ? trans[0] : translation.getValue()[0];
    tmp[1] = yTranslationOn.getValue() ? trans[1] : translation.getValue()[1];
    tmp[2] = zTranslationOn.getValue() ? trans[2] : translation.getValue()[2];
    
    if(translationOn.getValue()) translation = SbVec3f(tmp);
    */
    if(rotationOn.getValue())    rotation = rot;
}

//-----------------------------------------------------------------------------

void
SoTouchDragKit::setPressCallback(SoTouchDragKitCB* cb, void* userData)
{
    pressCallback = cb;
    pressCallbackData = userData;
}

//-----------------------------------------------------------------------------

void
SoTouchDragKit::setReleaseCallback(SoTouchDragKitCB* cb, void* userData)
{
    releaseCallback = cb;
    releaseCallbackData = userData;
}

//-----------------------------------------------------------------------------

void
SoTouchDragKit::setMoveCallback(SoTouchDragKitCB* cb, void* userData)
{
    moveCallback = cb;
    moveCallbackData = userData;
}

//-----------------------------------------------------------------------------

void
SoTouchDragKit::removePressCallback()
{
    setPressCallback(NULL);
}

//-----------------------------------------------------------------------------

void
SoTouchDragKit::removeReleaseCallback()
{
    setReleaseCallback(NULL);
}

//-----------------------------------------------------------------------------

void
SoTouchDragKit::removeMoveCallback()
{
    setMoveCallback(NULL);
}

//-----------------------------------------------------------------------------
//eof

