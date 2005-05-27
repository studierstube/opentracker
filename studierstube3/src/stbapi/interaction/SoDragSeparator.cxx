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
/** Class definition for SoDragSeparator
  *
  * @author Gerd Hesina, Hermann Wurnig
  *
  * $Id: SoDragSeparator.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/interaction/SoDragSeparator.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <stbapi/workspace/StbWorkspace.h> 
SO_NODE_SOURCE(SoDragSeparator);
BASE3D_SOURCE(SoDragSeparator);

//-----------------------------------------------------------------------------
// static class initialization. call before first use

void
SoDragSeparator::initClass()
{
    SO_NODE_INIT_CLASS(SoDragSeparator, SoSeparator, "Separator");

    BASE3D_INIT_CLASS(SoDragSeparator);
}

//-----------------------------------------------------------------------------
// constructor

SoDragSeparator::SoDragSeparator()
{
    SO_NODE_CONSTRUCTOR(SoDragSeparator);

    SO_NODE_ADD_FIELD(translation, (0,0,0));
    SO_NODE_ADD_FIELD(rotation, (SbRotation::identity()));
    SO_NODE_ADD_FIELD(dragButton, (STB_PRIMARY_BUTTON));
    SO_NODE_ADD_FIELD(dragging, (FALSE));

    BASE3D_CONSTRUCTOR;

    dragging = FALSE;
    startDrag = FALSE;
    stopDrag = FALSE;
    pressFunc = NULL;
    pressData = NULL;
    releaseFunc = NULL;
    releaseData = NULL;
    validFlag = FALSE;
	snapBackSensor=new SoAlarmSensor(snapBack, this);
}

//-----------------------------------------------------------------------------
// destructor. delete snapBackSensor

SoDragSeparator::~SoDragSeparator()
{
    if(snapBackSensor) delete snapBackSensor;
}

//-----------------------------------------------------------------------------
// returns: true if the node is interested in the 3d event of the given action
// h3a: action owning the 3d event

SbBool 
SoDragSeparator::isInterested(SoHandle3DEventAction *h3a)
{
    // startDrag or stopDrag are flags indicating we have to switch drag
    // mode immediately so in this case the node "fakes" interest in any
    // event to get handleEvent called
    if(startDrag||stopDrag) return TRUE; 

          // get underlying event
	So3DEvent *ev = (So3DEvent*) h3a->getEvent();
          // get path to button

	if ((ev->getType() == dragButton.getValue()+1) &&
    	(ev->getButton(dragButton.getValue()) == TRUE))

	{
    	const SoPath *rootPath = h3a->getPathFromRoot();
    	SoPath *currPath = rootPath->copy();

    	currPath->ref();

    	static SbViewportRegion vp;

    	static SoGetBoundingBoxAction *bba=new SoGetBoundingBoxAction(vp);
    	bba->apply((SoFullPath*)currPath);
        	// calculate the buttons bounding box

    	currPath->unref();

        	// calculate the non-axis alined bounding-box
    	SbXfBox3f xfbox=bba->getXfBoundingBox();
        	// if position of pen is outside again

    	if(xfbox.intersect(ev->getTranslation())==FALSE)
    	{        // then not interested
        	return FALSE;
    	}
    	return TRUE;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// handle the 3d event passed by the action
// h3a: action owning the 3d event

void 
SoDragSeparator::handle3DEvent(SoHandle3DEventAction *h3a)
{
    DISABLEDIVNODE(this);
	// get underlying event
    So3DEvent *ev = (So3DEvent*) h3a->getEvent();
        // get path to button

    SbVec3f objectTranslation;
    SbRotation objectRotation;
    SbVec3f penTranslation;
    SbRotation penRotation;

        // get current object translation/rotation
    objectTranslation=(translation.getValue());
    objectRotation=(rotation.getValue());
        // get current pen translation/rotation
    penTranslation=(ev->getTranslation());
    penRotation=(ev->getRotation());

    SbMatrix rotationMatrix;
	rotationMatrix.makeIdentity();
    SbVec3f hVec;

    // BUTTON PRESSED
    if (dragging.getValue() == FALSE)
    {
    	if (((ev->getType() == dragButton.getValue()+1) &&
            (ev->getButton(dragButton.getValue()) == TRUE)) ||
            startDrag)
    	{
			if (snapBackSensor->isScheduled()) snapBackSensor->unschedule();
        	transOffset=(objectTranslation - penTranslation);
        	(penRotation.inverse()).getValue(oldMatrixInverse);
        	oldMatrixInverse.multVecMatrix(transOffset, oldTranslation);
        	oldRotation=objectRotation*(penRotation.inverse());
        	h3a->setGrabber(this);
            startDrag = FALSE;
        	dragging.setValue(TRUE);
            //dragging = TRUE;
            if(pressFunc) pressFunc(pressData,this);
    	}
    }
    else
    {        // BUTTON RELEASED
    	if (((ev->getType() == dragButton.getValue()+1) &&
        	(ev->getButton(dragButton.getValue()) == FALSE)) ||
            stopDrag)
    	{        
        	h3a->releaseGrabber();
            stopDrag = FALSE;
            dragging.setValue(FALSE);
            //dragging = FALSE;
            if(getBase3DMode()==Base3D::BASE3D_MASTERMODE)
			{
    	        SETDRAGSEPARATORSTATE(this, translation.getValue(), rotation.getValue());
			}
            else if(getBase3DMode()==Base3D::BASE3D_SLAVEMODE)
			{
			    if (snapBackSensor->isScheduled()) snapBackSensor->unschedule();
			    snapBackSensor->setTimeFromNow(SbTime(SNAPBACK_TIME));
			    snapBackSensor->schedule();
			}
            if(releaseFunc) releaseFunc(releaseData,this);
        }
            // PEN MOVED
        if (ev->getType() == So3DEvent::ET_MOVE_EVENT) 
        {
            penRotation.getValue(rotationMatrix);
            rotationMatrix.multVecMatrix(oldTranslation, hVec);
            translation.setValue(hVec + penTranslation);
            rotation.setValue(oldRotation * penRotation);
        }
    }
    h3a->setHandled();
	ENABLEDIVNODE(this);
}

//-----------------------------------------------------------------------------
// set the callback executed when geometry picked
// func: callback function
// data: user supplied data

void
SoDragSeparator::setPressCallback(SoDragSeparatorCB* func, void* data)
{
    pressFunc = func;
    pressData = data;
}

//-----------------------------------------------------------------------------
// remove the callback executed when geometry picked

void
SoDragSeparator::removePressCallback()
{
    pressFunc = NULL;
    pressData = NULL;
}

//-----------------------------------------------------------------------------
// set the callback executed when geometry dropped
// func: callback function
// data: user supplied data

void
SoDragSeparator::setReleaseCallback(SoDragSeparatorCB* func, void* data)
{
    releaseFunc = func;
    releaseData = data;
}

//-----------------------------------------------------------------------------
// remove the callback executed when geometry dropped

void
SoDragSeparator::removeReleaseCallback()
{
    releaseFunc = NULL;
    releaseData = NULL;
}

//-----------------------------------------------------------------------------
// immediately go into dragging mode

void
SoDragSeparator::startDragging() 
{
    startDrag = TRUE; 
}

//-----------------------------------------------------------------------------
// immediately exit into dragging mode

void 
SoDragSeparator::stopDragging() 
{
    stopDrag = TRUE; 
}

void
SoDragSeparator::updateFromNetwork(SbVec3f trans,
                             SbRotation rot)
{
	if (snapBackSensor->isScheduled()) snapBackSensor->unschedule();
    translation=trans;
	rotation=rot;
    validFlag=TRUE;
	currentSetTranslation=trans;
	currentSetRotation=rot;
}

void
SoDragSeparator::snapBack(void *data, SoSensor *sensor)
{
	SoDragSeparator *self=(SoDragSeparator*)data;
	if(!self->validFlag)
	{
	    self->translation=self->oldTranslation;
	    self->rotation=self->oldRotation;	 
	}
	else
	{
	    self->translation=self->currentSetTranslation;
	    self->rotation=self->currentSetRotation;
	}
}


//-----------------------------------------------------------------------------
//eof
