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
/** SoDisplayKit.h contains the definition of class SoDisplayKit
  *
  * @author Rainer Splechtna, Tamer Fahmy
  *
  * $Id: SoDisplayKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SODISPLAYKIT_H_
#define _SODISPLAYKIT_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoMFNode.h>

#include <stbapi/resource/SoTrackedArtifactKit.h>

#include "SoStudierstubeViewer.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

//----------------------------------------------------------------------------
/** SoDisplayKit is a nodekit representing a HMD and its associated
SoStudierstubeViewer.

This nodekit inherits from SoTrackedArtifact and adds fields for storing
certain viewer parameters and methods for creating and accessing the viewer.
@author Rainer Splechtna
@ingroup camera
@ingroup core
 */
class STBAPI_API SoDisplayKit : public SoTrackedArtifactKit
{
    SO_KIT_HEADER(SoDisplayKit);
    SO_KIT_CATALOG_ENTRY_HEADER(cameraControl);
    SO_KIT_CATALOG_ENTRY_HEADER(display);

public:
    /** This enum just replicates the names and values of the 
        SoGLRenderAction TransparencyType-enum, to be able to use
        the enum names as values for the transparencyType-field of this class
        in an IV-file.
    */
    enum TransparencyType {
        SCREEN_DOOR         = SoGLRenderAction::SCREEN_DOOR,
	    ADD                 = SoGLRenderAction::ADD,
	    DELAYED_ADD         = SoGLRenderAction::DELAYED_ADD,
	    SORTED_OBJECT_ADD   = SoGLRenderAction::SORTED_OBJECT_ADD,
	    BLEND               = SoGLRenderAction::BLEND,
	    DELAYED_BLEND       = SoGLRenderAction::DELAYED_BLEND,
	    SORTED_OBJECT_BLEND = SoGLRenderAction::SORTED_OBJECT_BLEND,
#ifdef __COIN__
        NONE                                = SoGLRenderAction::NONE,
        SORTED_OBJECT_SORTED_TRIANGLE_ADD   = SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_ADD,
        SORTED_OBJECT_SORTED_TRIANGLE_BLEND = SoGLRenderAction::SORTED_OBJECT_SORTED_TRIANGLE_BLEND,
        SORTED_LAYERS_BLEND = SoGLRenderAction::SORTED_LAYERS_BLEND
#endif
    };

    /** A field used for storing whether the user shall have a viewer.
        TRUE:  a viewer shall be created.
        FALSE: no viewer shall be created.
        Only used for storing, i.e.changing this fields value won't trigger
        an action.
      */
    SoSFBool  isRendering;

    /** The SoStereoCameraKit used by the viewer.
        The nodeField must either contain a node of type SoStereoCameraKit,
        or a fileNode specifying a file, which contains a SoStereoCameraKit.
      */
    SoSFNode  stereoCameraKit;

    /** Specifies the position of the upper-left corner of the viewer window.
        (in pixels) 
        note: (0,0) is the upper-left corner of the screen.
     */
    SoSFInt32 xoffset, yoffset;

    /** Specifies the width and height of the viewer window (in pixels). */
    SoSFInt32 width, height;

    /** Specifies whether the viewer's headlight is turned on or off. */ 
    SoSFBool  headlight;

    /** Specifies the intensity of the viewer's headlight (0.0 - 1.0). */
    SoSFFloat  headlightIntensity;

    /** Specifies the transparency type, the viewer shall use.
        The default value is BLEND.
        note: see documentation of SoGLRenderAction for description
              of all possible transparencyTypes
    */
    SoSFEnum transparencyType;

	/** This multiple-value field may contain any additonal nodes,
	    which specify the environment of the scene, which is passed
		to the createViewer() method.
	    Valid types for this field are:
	    SoLightKit, SoLight, SoEnvironment, SoLightModel 
        @warning The specified environment nodes will only be added to the
                 scene passed as parameter of the createViewer() method.
                 If the setSceneGraph(SoNode *new_Scene) method of the 
                 SoStudierstubeViewer is called, the environment nodes 
                 specified here will not affect the new_Scene.
    */
	SoMFNode   customEnvironment;

    /** Specifies the color of the viewer's background. */
    SoSFColor backgroundColor;

    /** The SoVideoBackground used by the viewer.
        The nodeField must contain a node of type SoVideoBackgroud.
      */
    SoSFNode videoBackground;

    /** Specify the pixel format, which shall be forced.
        note: this value is only used for the WIN32-TGS version of Studierstube
    */
    SoSFInt32 pixelFormat;

	/** Specifies visibility of the mouse cursor in within the window. default
	 * value is TRUE.
	 * @note works only with Qt currently */
	SoSFBool showMouse;

	/** Specifies whether the window should be decorated or not
	 * (have borders, title bar, etc. ). Default value is TRUE. 
	 * This is independend of the display mode.*/
	SoSFBool windowBorder;

	/** Specifies whether the viewer should be decorated or not, 
	 * this enables the buttons and wheel widgets around the render area.
	 * Default value is TRUE. 
	 * This is independend of the display mode.*/	
	SoSFBool decoration;

	/** Specifies whether the viewer should be on top of all other windows or not, 
	 * Default value is FALSE. 
	 * This is independend of the display mode.*/	
	SoSFBool windowOnTop;

	/** Specifies whether to enable the stencil buffer of the viewer.
	 * Default value is FALSE. */
	SoSFBool stencilBuffer;

    /** The constructor inits all fields
      */
    SoDisplayKit();

    /** The destructor does nothing */
    ~SoDisplayKit();

    /** Inventor class initialization */
    static void initClass();

    /** Creates the viewer associated with this kit.
        The viewer is configured by the fields and parts of this kit.
         be sure to specify all needed fields before calling this method.
        @param scene the scene-grpah to be rendered by the viewer
		@param widget the widget used by the viewer
    */
    void createViewer(SoSeparator* scene, GuiWidget widget);

    /** Returns a pointer to the viewer associated with this kit. 
        @return the SoStudierstubeViewer associated with this kit. 
     */
    SoStudierstubeViewer *getViewer();  

    /** keyboard tracker translation */
    float translation[3][3];

    /** keyboard tracker rotation */
    float rotation[3][3];

private:
    /** viewer instance associated with this kit. */
    SoStudierstubeViewer* viewer; 
};

//----------------------------------------------------------------------------
#endif//_SODISPLAYKIT_H_

