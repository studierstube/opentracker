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
/** So3DSensor - implementation
  *
  * @author Gerd Hesina, Dieter Schmalstieg
  *
  * $Id: So3DSensor.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <stbapi/interaction/So3DSensor.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
 
SO_NODE_SOURCE(So3DSensor);
BASE3D_SOURCE(So3DSensor);

//-----------------------------------------------------------------------------
// static class initialization. call before first use

void
So3DSensor::initClass()
{
    SO_NODE_INIT_CLASS(So3DSensor, SoSeparator, "Separator");
    BASE3D_INIT_CLASS(So3DSensor);
}

//-----------------------------------------------------------------------------
// constructor

So3DSensor::So3DSensor()
{
    SO_NODE_CONSTRUCTOR(So3DSensor);
    SO_NODE_ADD_FIELD(translation, (0,0,0));
    SO_NODE_ADD_FIELD(rotation, (SbRotation::identity()));
    SO_NODE_ADD_FIELD(dragButton, (STB_PRIMARY_BUTTON));
    BASE3D_CONSTRUCTOR;

    pressFunc = NULL;
    pressData = NULL;
    releaseFunc = NULL;
    releaseData = NULL;
    moveFunc = NULL;
    moveData = NULL;
}

//-----------------------------------------------------------------------------
// destructor. does nothing

So3DSensor::~So3DSensor() {}

//-----------------------------------------------------------------------------
// returns: true if the node is interested in the 3d event of the given action
// h3a: action owning the 3d event

SbBool 
So3DSensor::isInterested(SoHandle3DEventAction *h3a)
{
	So3DEvent *ev = (So3DEvent*) h3a->getEvent();
	if (ev->getType() == dragButton.getValue()+1 &&
    	ev->getButton(dragButton.getValue()))
	{
    	const SoPath *rootPath = h3a->getPathFromRoot();
    	SoPath *currPath = rootPath->copy();
    	currPath->ref();
    	static SbViewportRegion vp;
    	static SoGetBoundingBoxAction *bba=new SoGetBoundingBoxAction(vp);
    	bba->apply((SoFullPath*)currPath);
    	currPath->unref();
    	SbXfBox3f xfbox=bba->getXfBoundingBox();
    	return xfbox.intersect(ev->getTranslation());
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
// handle the 3d event passed by the action
// h3a: action owning the 3d event

void 
So3DSensor::handle3DEvent(SoHandle3DEventAction *h3a)
{
    static SbBool dragging = FALSE;
    So3DEvent *ev = (So3DEvent*) h3a->getEvent();
	if (ev->getType() == dragButton.getValue()+1)
    {
        translation = h3a->getEvent()->getTranslation();
        rotation = h3a->getEvent()->getRotation();
        if(ev->getButton(dragButton.getValue()))
        {
            h3a->setGrabber(this);
            dragging = TRUE;
            if(pressFunc) pressFunc(pressData,this);
        }
        else 
        {
            h3a->releaseGrabber();
            dragging = FALSE;
            if(releaseFunc) releaseFunc(releaseData,this);
        }
        h3a->setHandled();
    }
    else if(ev->getType() == So3DEvent::ET_MOVE_EVENT)
    {
        if(dragging)
        {
            translation = h3a->getEvent()->getTranslation();
            rotation = h3a->getEvent()->getRotation();
            if(moveFunc) moveFunc(moveData,this);
        }
    }
}

//-----------------------------------------------------------------------------
// set the callback executed when geometry picked
// func: callback function
// data: user supplied data

void
So3DSensor::setPressCallback(So3DSensorCB* func, void* data)
{
    pressFunc = func;
    pressData = data;
}

//-----------------------------------------------------------------------------
// remove the callback executed when geometry picked

void
So3DSensor::removePressCallback()
{
    pressFunc = NULL;
    pressData = NULL;
}

//-----------------------------------------------------------------------------
// set the callback executed when geometry dropped
// func: callback function
// data: user supplied data

void
So3DSensor::setReleaseCallback(So3DSensorCB* func, void* data)
{
    releaseFunc = func;
    releaseData = data;
}

//-----------------------------------------------------------------------------
// remove the callback executed when geometry dropped

void
So3DSensor::removeReleaseCallback()
{
    releaseFunc = NULL;
    releaseData = NULL;
}

//-----------------------------------------------------------------------------
// set the callback executed when geometry moved
// func: callback function
// data: user supplied data

void
So3DSensor::setMoveCallback(So3DSensorCB* func, void* data)
{
    moveFunc = func;
    moveData = data;
}

//-----------------------------------------------------------------------------
// remove the callback executed when geometry moved

void
So3DSensor::removeMoveCallback()
{
    moveFunc = NULL;
    moveData = NULL;
}

//-----------------------------------------------------------------------------
//eof
