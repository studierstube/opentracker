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
/** This file contains the SoSketchKit.
  * See the class description for details.
  *
  * @author Benjamin A. Schmit
  *
  * $Id: SoSketchKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include "SoSketchKit.h"

#include <Inventor/SbViewportRegion.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTranslation.h>
#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/misc/GRT_defs.h>

#ifdef GRT_FENCE
#include <Inventor/nodes/SoTriangleStripSet.h>
#else
#include <Inventor/nodes/SoLineSet.h>
#endif

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

SO_KIT_SOURCE(SoSketchKit);
BASE3D_SOURCE(SoSketchKit);


/// Macros used for initialization.

void SoSketchKit::constSoSketchKit()
{
    // Create this SoSketchKit:
    SO_KIT_CONSTRUCTOR(SoSketchKit);
    BASE3D_CONSTRUCTOR;

    // Add the catalog parts:
    SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, FALSE,
        this, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(surfaceSwitch, SoSwitch, FALSE,
        topSeparator, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(surfaceSeparator, SoSeparator, FALSE,
        surfaceSwitch, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(surfaceMaterial, SoMaterial, FALSE,
        surfaceSeparator, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(surface, SoCube, FALSE,
        surfaceSeparator, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(sketchSwitch, SoSwitch, FALSE,
        topSeparator, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(sketchMaterial, SoMaterial, FALSE,
        sketchSwitch, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(sketchCoordinates, SoCoordinate3, FALSE,
        sketchSwitch, \x0, FALSE);
#ifdef GRT_FENCE
    SO_KIT_ADD_CATALOG_ENTRY(sketch, SoTriangleStripSet, FALSE,
        sketchSwitch, \x0, FALSE);
#else
    SO_KIT_ADD_CATALOG_ENTRY(sketch, SoLineSet, FALSE,
        sketchSwitch, \x0, FALSE);
#endif
    SO_KIT_ADD_CATALOG_ENTRY(cursorSwitch, SoSwitch, FALSE,
        topSeparator, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(cursorSeparator, SoSeparator, FALSE,
        cursorSwitch, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(cursorTranslation, SoTranslation, FALSE,
        cursorSeparator, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(cursor, SoSeparator, FALSE,
        cursorSeparator, \x0, FALSE);

    // Add enum values:
    SO_KIT_DEFINE_ENUM_VALUE(Visible, HIDDEN);
    SO_KIT_DEFINE_ENUM_VALUE(Visible, VISIBLE);
    SO_KIT_SET_SF_ENUM_TYPE(surfaceVisible, Visible);
    SO_KIT_SET_SF_ENUM_TYPE(sketchVisible, Visible);
    SO_KIT_SET_SF_ENUM_TYPE(cursorVisible, Visible);

    // Add the nodekit's fields:
    SO_KIT_ADD_FIELD(point, (0.0f, 0.0f, 0.0f));
    SO_KIT_ADD_FIELD(numVertices, (0));
    SO_KIT_ADD_FIELD(xSize, (_grt_size[0]));
    SO_KIT_ADD_FIELD(ySize, (_grt_size[1]));
    SO_KIT_ADD_FIELD(height, (_grt_size[2]));
    SO_KIT_ADD_FIELD(threshold, (_grt_threshold));
    SO_KIT_ADD_FIELD(surfaceColor, (_grt_surface));
    SO_KIT_ADD_FIELD(highlightColor, (_grt_highlight));
    SO_KIT_ADD_FIELD(sketchColor, (_grt_sketch));
    SO_KIT_ADD_FIELD(surfaceVisible, (VISIBLE));
    SO_KIT_ADD_FIELD(sketchVisible, (VISIBLE));
    SO_KIT_ADD_FIELD(cursorVisible, (HIDDEN));
    SO_KIT_ADD_FIELD(inside, (FALSE));
    SO_KIT_ADD_FIELD(multiple, (TRUE));

    // Initialize this SoSketchKit:
    SO_KIT_INIT_INSTANCE();

	//---- by hpi
	m_State = INACTIVE;
}


/// This initializes the SoSketchKit.

void SoSketchKit::initSoSketchKit(float myXSize, float myYSize,
    float myHeight)
{
    // surfaceSwitch:
    {
        SoSwitch* sw= (SoSwitch*) getAnyPart("surfaceSwitch", true);
        surfaceEng.in.connectFrom(&surfaceVisible);
        sw->whichChild.connectFrom(&surfaceEng.out);
    }

    // surfaceMaterial:
    {
        SoMaterial* mat= (SoMaterial*) getAnyPart("surfaceMaterial", true);
        mat->diffuseColor.connectFrom(&surfaceColor);
    }

    // surface:
    {
        SoCube* cub= new SoCube();
        setAnyPart("surface", cub);
        xSize.connectFrom(&(cub->width));
        ySize.connectFrom(&(cub->depth));
        height.connectFrom(&(cub->height));
        cub->width.connectFrom(&(xSize));
        cub->depth.connectFrom(&(ySize));
        cub->height.connectFrom(&(height));
        cub->width= myXSize;
        cub->depth= myYSize;
        cub->height= myHeight;
    }

    // sketchSwitch:
    {
        SoSwitch* sw= (SoSwitch*) getAnyPart("sketchSwitch", true);
        sketchEng.in.connectFrom(&sketchVisible);
        sw->whichChild.connectFrom(&sketchEng.out);
    }

    // sketchMaterial:
    {
        SoMaterial* mat= (SoMaterial*) getAnyPart("sketchMaterial", true);
        mat->diffuseColor.connectFrom(&sketchColor);
    }

    // sketchCoordinates:
    {
        SoCoordinate3* coo=
            (SoCoordinate3*) getAnyPart("sketchCoordinates", true);
        coo->point.connectFrom(&point);
        point.connectFrom(&(coo->point));
    }

    // sketch:
    {
#ifdef GRT_FENCE
        SoTriangleStripSet* lin=
            (SoTriangleStripSet*) getAnyPart("sketch", true);
#else
        SoLineSet* lin=
            (SoLineSet*) getAnyPart("sketch", true);
#endif
        lin->numVertices.connectFrom(&numVertices);
        numVertices.connectFrom(&(lin->numVertices));
        clear();
    }

    // cursorSwitch:
    {
        SoSwitch* sw= (SoSwitch*) getAnyPart("cursorSwitch", true);
        cursorEng.in.connectFrom(&cursorVisible);
        sw->whichChild.connectFrom(&cursorEng.out);
    }

    // cursor:
    {
        SoSeparator* cur= (SoSeparator*) getAnyPart("cursor", TRUE);

        // define an example cursor:
        SoCube* cub= new SoCube();
        cub->width.setValue(1.0f);
        cub->height.setValue(height.getValue()/256);
        cub->depth.setValue(0.1f);
        cur->addChild(cub);
        cub= new SoCube();
        cub->width.setValue(0.1f);
        cub->height.setValue(height.getValue()/256);
        cub->depth.setValue(1.0f);
        cur->addChild(cub);
    }

    // additional state information:
    pressCallback= NULL;
    moveCallback= NULL;
    releaseCallback= NULL;
    pressData= NULL;
    moveData= NULL;
    releaseData= NULL;
    active= false;
    root= NULL;
}


/// Extension mechanism for subclassing - key press.

void SoSketchKit::pressImplementation()
{
    // intentionally empty - override to extend.
}


/// Extension mechanism for subclassing - movement.

void SoSketchKit::moveImplementation()
{
    // intentionally empty - override to extend.
}


/// Extension mechanism for subclassing - key release.

void SoSketchKit::releaseImplementation()
{
    // intentionally empty - override to extend.
}


/// This returns the local-to-global matrix.

SbMatrix SoSketchKit::getTransformationMatrix()
{
    /** @warning This only works if root has been set
      *          (by an SoHandle3DEventAction)
      */
    if (root != NULL)
    {		
        SoGetMatrixAction* gma= new SoGetMatrixAction(SbViewportRegion());
        // assumes that the SbViewportRegion doesprintf("LENGTH: %d\n", root->getLength());
		root->ref();
        gma->apply(root);
		root->unrefNoDelete();
        return gma->getMatrix();
    }
    else return SbMatrix::identity();  // fallback	
}


/// This starts or continues a line.

void SoSketchKit::drawPoint(SbVec3f pt, bool newLine)
{
    int vpp;  // vertices per point
    point.set1Value(point.getNum(), pt);
#ifdef GRT_FENCE
    SbVec3f pt2= pt;
    pt2[1]+= height.getValue()/2;
    point.set1Value(point.getNum(), pt2);
    vpp= 2;
#else
    vpp= 1;
#endif
    if (!newLine)
    {
        if (numVertices.getNum() <= 0) numVertices.
            setValue(vpp);  // first point
        else numVertices.set1Value(numVertices.getNum() - 1,
            *(numVertices.getValues(numVertices.getNum() - 1)) + vpp);
    }
    else numVertices.set1Value(numVertices.getNum(), vpp);
}


/// Static class initialization.

void SoSketchKit::initClass()
{
    SoVisibleSwitchEngine::initClass();

    SO_KIT_INIT_CLASS(SoSketchKit, SoBaseKit, "BaseKit");
    BASE3D_INIT_CLASS(SoSketchKit);
}


/// Default constructor.

SoSketchKit::SoSketchKit()
{
    constSoSketchKit();
    initSoSketchKit(xSize.getValue(), ySize.getValue(), height.getValue());
        // set default values
}


/// Parameterized constructor.

SoSketchKit::SoSketchKit(float myXSize, float myYSize, float myHeight)
{
    constSoSketchKit();
    initSoSketchKit(myXSize, myYSize, myHeight);
}


/// Destructor

SoSketchKit::~SoSketchKit() {}


/// This function clears any drawings.

void SoSketchKit::clear()
{
    numVertices.deleteValues(0);
    point.deleteValues(0);
}


/// Intersects a drawing with the surface of the SoSketchKit.

SoMFVec3f SoSketchKit::intersect(SoMFVec3f drawing)
{
    SoMFVec3f result= SoMFVec3f();
    SbVec2f res2;
    SbVec3f res3;
    res3[1]= 130*height.getValue()/256;
    for (int i= 0; i < drawing.getNum(); i++)
    {
        res2= posOnSurface(drawing[i]);
        res3[0]= res2[0];
        res3[2]= res2[1];
        result.set1Value(i, res3);
    }
    return result;
}


/// The pen's position on the surface.

SbVec2f SoSketchKit::posOnSurface()
{
    SbVec2f res;
    SbVec3f trn= ((SoTranslation*) getAnyPart("cursorTranslation", true))
        ->translation.getValue();
        // this is the last known position of the cursor
    res[0]= trn[0];
    res[1]= trn[2];
    return res;
}


/// A point's position in surface coordinates.

SbVec2f SoSketchKit::posOnSurface(SbVec3f position)
{
    SbVec3f result;
    SbMatrix mat= getTransformationMatrix();
    mat= mat.inverse();
    mat.multVecMatrix(position, result);
    SbVec2f res2;
    res2[0]= result[0];
    res2[1]= result[2];
    return res2;
}


/// The pen's position in space.

SbVec3f SoSketchKit::posInWorld()
{
    return posInWorld(posOnSurface());
}


/// A surface point's position in world coordinates.

SbVec3f SoSketchKit::posInWorld(SbVec2f position)
{
    SbVec3f pos3, result;
    pos3[0]= position[0];
    pos3[1]= 130*height.getValue()/256;
    pos3[2]= position[1];
    SbMatrix mat= getTransformationMatrix();
    mat.multVecMatrix(pos3, result);
    return result;
}


/// Is a given point on the surface?

bool SoSketchKit::isInside(SbVec2f position)
{
    return ((position[0] >= -xSize.getValue()/2)
        && (position[0] <= xSize.getValue()/2)
        && (position[1] >= -ySize.getValue()/2)
        && (position[1] <= ySize.getValue()/2));
}


/// Is a given point inside the SoSketchKit?

bool SoSketchKit::isInside(SbVec3f position)
{
    SbVec3f pos;
    SbMatrix mat= getTransformationMatrix();
    mat= mat.inverse();
    mat.multVecMatrix(position, pos);

    // important: allow more space along 2nd axis (height) -
    // else drawing becomes nearly impossible!
    return ((pos[0] >= -xSize.getValue()/2)
        && (pos[0] <= xSize.getValue()/2)
        && (pos[1] >= -height.getValue()*2)
        && (pos[1] <= height.getValue()*2)
        && (pos[2] >= -ySize.getValue()/2)
        && (pos[2] <= ySize.getValue()/2));
}


/// Sets a callback on key press (start line).

void SoSketchKit::setPressCallback(SoSketchKitCB* cb, void* userData= NULL)
{
    pressCallback= cb;
    pressData= userData;
}


/// Sets a callback on movement (continue line).

void SoSketchKit::setMoveCallback(SoSketchKitCB* cb, void* userData= NULL)
{
    moveCallback= cb;
    moveData= userData;
}


/// Sets a callback on key release (finish line).

void SoSketchKit::setReleaseCallback(SoSketchKitCB* cb, void* userData= NULL)
{
    releaseCallback= cb;
    releaseData= userData;
}


/// Removes a callback on key press (start line).

void SoSketchKit::removePressCallback()
{
    pressCallback= NULL;
    pressData= NULL;
}


/// Removes a callback on movement (continue line).

void SoSketchKit::removeMoveCallback()
{
    moveCallback= NULL;
    moveData= NULL;
}


/// Removes a callback on key release (finish line).

void SoSketchKit::removeReleaseCallback()
{
    releaseCallback= NULL;
    releaseData= NULL;
}


/// This does 3D event handling.

void SoSketchKit::handle3DEvent(SoHandle3DEventAction* h3a)
{
    // isInterested() has been called before this, so root probably does not
    // need to be updated.	
    doAction(h3a);
}


/// Tells if we are interested in an event.

SbBool SoSketchKit::isInterested(SoHandle3DEventAction* h3a)
{
    // capture the root path
    if (m_State == INACTIVE) root= (SoPath*) ((SoHandle3DEventAction*) h3a)->getPathFromRoot()->copy();
        // it isn't sufficient to do this only once, so do it continuously

    // show interest if there:
    return isInside(h3a->getEvent()->getTranslation());
}


/// This does general event handling.

void SoSketchKit::doAction(SoAction* action)
{
    ///////////////////////////
    // define some variables //
    ///////////////////////////

// this was put into the header by hpi    enum { INACTIVE=0, SLEEPING, DRAWING, WAITING } state= INACTIVE;
        // current state
    enum { NO_ACTION=0, MOVE_IN, MOVE_OUT, PRESS, RELEASE } trans= NO_ACTION;
        // transitional action
    SbVec3f pos;  // current cursor position (on surface)
    bool move= false;  // check for movement (movement event occured)

    /////////////////////////
    // evaluate transition //
    /////////////////////////

    // check for So3DEvent (3d):
    if (action->isOfType(SoHandle3DEventAction::getClassTypeId()))
    {
        So3DEvent* event= (So3DEvent*) ((SoHandle3DEventAction*) action)->
            getEvent();

        // button event:
        if (event->getType() == So3DEvent::ET_BUTTON0_EVENT)
        {
            if (event->getButton(So3DEvent::BUTTON0)) trans= PRESS;
            else trans= RELEASE;
        }

        // move event:
        if (event->getType() == So3DEvent::ET_MOVE_EVENT)
        {			
            pos= event->getTranslation();			
            if (inside.getValue() && !isInside(pos)) trans= MOVE_OUT;
			if (!inside.getValue() && isInside(pos)) trans= MOVE_IN;
            if (isInside(pos)) move= true;
        }

		if (event->getType() == So3DEvent::ET_EXIT_EVENT)
			printf("EXIT EVENT!\n");
    }

    ///////////////////////
    // move if necessary //
    ///////////////////////

    if (move) {

        // initialize necessary variables:
        SbVec2f pos2= posOnSurface(pos);
        float pos3[3]= { pos2[0], 129*height.getValue()/256, pos2[1] };
        SoTranslation* ctr= (SoTranslation*) getAnyPart("cursorTranslation",
            true);

        // should movement be detected (compare to threshold)?
        SbVec3f trn= ctr->translation.getValue();
        if ((pow(pos3[0] - trn[0], 2) + pow(pos3[2] - trn[2], 2)) >=
            pow(threshold.getValue(), 2))
        {
            // yes, set current coordinates:
            ctr->translation.setValue(pos3);

            if (active)  // should we draw?
            {
                // yes, draw:
                pos3[1]= 130*height.getValue()/256;
                drawPoint(pos3, false);
            }

            // execute callbacks:
            moveImplementation();  // internal extension mechanism
            if (moveCallback != NULL) moveCallback(moveData, this);
        }
    }

    ///////////////////////
    // state transitions //
    ///////////////////////

	if (trans != NO_ACTION)
	{		
		switch(m_State)
		{
		case INACTIVE:
			if (trans == MOVE_IN)
			{
				inside = true;
				((SoMaterial*) getAnyPart("surfaceMaterial", true))->diffuseColor.connectFrom(&highlightColor);
				((SoHandle3DEventAction*) action)->setGrabber(this);
                ((SoHandle3DEventAction*) action)->setHandled();
				m_State = SLEEPING;
			}
			break;

		case SLEEPING:
			if (trans == MOVE_OUT)
			{
				inside = false;
				((SoMaterial*) getAnyPart("surfaceMaterial", true))->diffuseColor.connectFrom(&surfaceColor);            
                ((SoHandle3DEventAction*) action)->releaseGrabber();
				((SoHandle3DEventAction*) action)->setHandled();
				m_State = INACTIVE;
			}
			else if (trans == PRESS)
			{
				active = true;
				if (!multiple.getValue()) clear();
				SbVec3f pos3= ((SoTranslation*) getAnyPart("cursorTranslation", true))->translation.getValue();
                pos3[1]= 130*height.getValue()/256;
                drawPoint(pos3, true);
                // execute callback functions:
                pressImplementation();  // internal extension mechanism
                if (pressCallback != NULL) pressCallback(pressData, this);				
                ((SoHandle3DEventAction*) action)->setHandled();
				m_State = DRAWING;
			}
			break;

		case DRAWING:
			if (trans == MOVE_OUT)
			{
				inside = false;
				((SoMaterial*) getAnyPart("surfaceMaterial", true))->diffuseColor.connectFrom(&surfaceColor);
				((SoHandle3DEventAction*) action)->setHandled();
				m_State = WAITING;
			}
			else if (trans == RELEASE)
			{
				active = false;
				releaseImplementation();  // internal extension mechanism
                if (releaseCallback != NULL) releaseCallback(releaseData, this);                
				((SoHandle3DEventAction*) action)->setHandled();				
				m_State = SLEEPING;
			}
			break;

		case WAITING:
			if (trans == MOVE_IN)
			{
				inside = true;
				((SoMaterial*) getAnyPart("surfaceMaterial", true))->diffuseColor.connectFrom(&highlightColor);
				((SoHandle3DEventAction*) action)->setHandled();
				m_State = DRAWING;
			}
			else if (trans == RELEASE)
			{
				active = false;
				releaseImplementation();  // internal extension mechanism
                if (releaseCallback != NULL) releaseCallback(releaseData, this);
                ((SoHandle3DEventAction*) action)->releaseGrabber();
                ((SoHandle3DEventAction*) action)->setHandled();
				m_State = INACTIVE;
			}
			break;
		}
	}
}
