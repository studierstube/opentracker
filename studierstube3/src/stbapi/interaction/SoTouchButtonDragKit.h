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
//  PROJECT:    Studierstube
//  CONTENT:    3D Dragger for "magic wand" (no button)
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    jp      Jan Prikryl
// ===========================================================================
//  HISTORY:
//
//  20-Sep-00 11:53:42  jp  last modification 
//  20-Sep-00 11:58:25  jp  copied template from SoDragKit.h
// ===========================================================================

#ifndef _SOTOUCHBUTTONDRAGKIT_H_
#define _SOTOUCHBUTTONDRAGKIT_H_

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <stbapi/event3d/Base3D.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class STBAPI_API SoTouchButtonDragKit;

typedef void SoTouchButtonDragKitCB(void* userData, SoTouchButtonDragKit* dragKit);

//----------------------------------------------------------------------------

class STBAPI_API SoTouchButtonDragKit
:
    public SoBaseKit, public Base3D 
{
    SO_KIT_HEADER(SoTouchButtonDragKit);
    BASE3D_HEADER;
    
    // (hidden) root separator
    SO_KIT_CATALOG_ENTRY_HEADER(base);
    // SoTransform - used for dragging
    SO_KIT_CATALOG_ENTRY_HEADER(transform);
    // SoSwitch - used for switching between normal and highlighted geometry
    SO_KIT_CATALOG_ENTRY_HEADER(highlightSwitch);
    //SoSeparator - add your content here
    SO_KIT_CATALOG_ENTRY_HEADER(content);
    //SoSeparator - add contend for highlighted display here
    SO_KIT_CATALOG_ENTRY_HEADER(highlightedContent);

public:
    SoSFFloat highlightRadius;          //"sensitive" radius from bbox center
    SoSFFloat hysteresis;               //hysteresis for the "sensitive" radius
    SoSFVec3f translation;              //current translation, local coords
    SoSFRotation rotation;              //current rotation,    local coords

    SoSFBool translationOn;             //turn whole translation on/off
    SoSFBool rotationOn;                //turn whole rotation on/off

    SoSFBool xTranslationOn;            //x component can be modified
    SoSFBool yTranslationOn;            //y component can be modified
    SoSFBool zTranslationOn;            //z component can be modified
    SoSFBool xRotationOn;               //x axis rotation possible
    SoSFBool yRotationOn;               //y axis rotation possible
    SoSFBool zRotationOn;               //z axis rotation possible
    
    static void initClass(              //static class initialization
        );
    SoTouchButtonDragKit(                          //constructor
        );
    ~SoTouchButtonDragKit(                         //destructor
        );
	SbBool isInterested(                //true if node interested in 3d event
        SoHandle3DEventAction* h3a      //action owning 3d event
        );
	void handle3DEvent(                 //deal with 3d event
        SoHandle3DEventAction* h3a      //action owning 3d event
        );
    void setPressCallback(
        SoTouchButtonDragKitCB* cb,
        void* userData = NULL
        );
    void setReleaseCallback(
        SoTouchButtonDragKitCB* cb,
        void* userData = NULL
        );
    void setMoveCallback(
        SoTouchButtonDragKitCB* cb,
        void* userData = NULL
        );
    void removePressCallback(
        );
    void removeReleaseCallback(
        );
    void removeMoveCallback(
        );
    

private:
    void writeBack(                     //write back to fields
        SbVec3f trans,                  //modified translation
        SbRotation rot                  //modified rotation
        );
    void update(
        SbVec3f transIn,
        SbRotation rotIn,
        SbVec3f& transOut,
        SbRotation& rotOut
        );

	SbBool     highlighted;             // true if geometry highlighted
    SbBool     doDragging;              // true if manipulating the geometry
    SbVec3f    trans0L;                 // original kit translation, local
    SbRotation rot0L;                   // original kit rotation,    local
    SbMatrix   globalToLocal;           // global to local transformation
    SbVec3f    pickOffsetL;             // offset to pen-kit origin, local

    SoTouchButtonDragKitCB * pressCallback;
    SoTouchButtonDragKitCB * releaseCallback;
    SoTouchButtonDragKitCB * moveCallback;

    void * pressCallbackData;
    void * releaseCallbackData;
    void * moveCallbackData;
    
}; //class SoTouchButtonDragKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOTOUCHBUTTONDRAGKIT_H_

