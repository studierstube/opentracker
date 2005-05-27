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
/** SoDisplayKit.h contains the implementation of class SoDisplayKit
  *
  * @author Rainer Splechtna, Tamer Fahmy
  *
  * $Id: SoDisplayKit.cxx 3788 2004-09-29 10:15:30Z reitmayr $
  * @file                                                                   
  */
 /* ======================================================================== */

#include <cassert>
#include <cstdlib>
#include <cstring>

#ifdef WIN32
#include <SoWinEnterScope.h>
#define strcasecmp _stricmp
#endif

#include "SoDisplayKit.h"

#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/SoPath.h>
#include <Inventor/nodes/SoFile.h>
#include <Inventor/nodes/SoPointLight.h> 
#include <Inventor/nodes/SoDirectionalLight.h> 
#include <Inventor/nodekits/SoLightKit.h>
#include <Inventor/nodes/SoEnvironment.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoRotation.h>

#include <stbapi/resource/viewer/video/SoVideoBackground.h>
#include <stbapi/workspace/version.h>

#include "SoOffAxisCamera.h"
#include "SoFieldSequentialDisplayMode.h"
#include "SoLineSequentialDisplayMode.h"
#include "SoDualChannelDisplayMode.h"
#include "SoMonoDisplayMode.h"
#include "SoTrackedDisplayControlMode.h"
#include "SoTrackedViewpointControlMode.h"
#include "SoTrackedViewpointMobileDisplayControlMode.h"
#include "SoDesktopControlMode.h"
#include "SoStereoCameraKit.h"

#include SOGUI_CURSOR_H

SO_KIT_SOURCE(SoDisplayKit);

//----------------------------------------------------------------------------
/** Inventor class initialization */

void
SoDisplayKit::initClass()
{
    SO_KIT_INIT_CLASS(SoDisplayKit, SoTrackedArtifactKit, "SoTrackedArtifactKit");
}

//----------------------------------------------------------------------------
/** The constructor inits all fields. */
SoDisplayKit::SoDisplayKit() :
    viewer(NULL)
{
    memset(translation,0,sizeof(translation));
    memset(rotation,0,sizeof(rotation));
 
    SO_KIT_CONSTRUCTOR(SoDisplayKit);

    // Add the body parts to the catalog...
    // SO_KIT_ADD_CATALOG_ENTRY(
    //   name,className,nullByDefault,parentName,rightSiblingName,isPublic

    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(cameraControl, SoCameraControlMode, 
                                      SoDesktopControlMode, FALSE, this, 
                                      \0, TRUE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(display, SoDisplayMode, 
                                      SoMonoDisplayMode, FALSE, this,
                                      \0, TRUE);

    // add fields
    SO_KIT_ADD_FIELD(isRendering, (TRUE)); 
    SO_KIT_ADD_FIELD(stereoCameraKit, (new SoStereoCameraKit)); 
    SO_KIT_ADD_FIELD(xoffset, (0)); 
    SO_KIT_ADD_FIELD(yoffset, (0)); 
    SO_KIT_ADD_FIELD(width, (640)); 
    SO_KIT_ADD_FIELD(height, (480)); 
    SO_KIT_ADD_FIELD(transparencyType, (SoDisplayKit::BLEND)); 
    SO_KIT_ADD_FIELD(headlight, (TRUE)); 
    SO_KIT_ADD_FIELD(headlightIntensity, (1.0f)); 
	SO_KIT_ADD_FIELD(customEnvironment, (NULL)); 
    SO_KIT_ADD_FIELD(backgroundColor, (0,0,0)); 
    SO_KIT_ADD_FIELD(videoBackground, (new SoVideoBackground)); 
    SO_KIT_ADD_FIELD(pixelFormat, (0)); 
	SO_KIT_ADD_FIELD(showMouse, (TRUE));
	SO_KIT_ADD_FIELD(windowBorder, (TRUE));
	SO_KIT_ADD_FIELD(decoration, (TRUE));
	SO_KIT_ADD_FIELD(windowOnTop, (FALSE));
	SO_KIT_ADD_FIELD(stencilBuffer, (FALSE));
 
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SCREEN_DOOR);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, DELAYED_ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, BLEND);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, DELAYED_BLEND);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_BLEND);
#ifdef __COIN__
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, NONE);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_SORTED_TRIANGLE_ADD);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_OBJECT_SORTED_TRIANGLE_BLEND);
    SO_NODE_DEFINE_ENUM_VALUE(TransparencyType, SORTED_LAYERS_BLEND);
#endif
    SO_NODE_SET_SF_ENUM_TYPE(transparencyType, TransparencyType);

    SO_KIT_INIT_INSTANCE();

    setSearchingChildren(TRUE);
}


//----------------------------------------------------------------------------
/** The destructor does nothing */

SoDisplayKit::~SoDisplayKit()
{
    // nil
}


//----------------------------------------------------------------------------
/** Returns a pointer to the viewer associated with this kit. */

SoStudierstubeViewer *
SoDisplayKit::getViewer()
{
    return viewer;
}
  
//----------------------------------------------------------------------------
/** Creates the viewer associated with this kit. */

void
SoDisplayKit::createViewer(SoSeparator* scene, GuiWidget widget)
{
    //-- get stereoCameraKit (from file if needed)
    if (!stereoCameraKit.getValue()->isOfType
        (SoStereoCameraKit::getClassTypeId()))
    {
        if (stereoCameraKit.getValue()->isOfType
            (SoFile::getClassTypeId()))
        {
            SoGroup *fileContent = 
                ((SoFile *)stereoCameraKit.getValue())->copyChildren();
            fileContent->ref();
            // search for SoStereoCameraKit 
            SoSearchAction sAction;
            sAction.setType(SoStereoCameraKit::getClassTypeId());
            sAction.setSearchingAll(TRUE);
            sAction.setInterest(SoSearchAction::FIRST);
            sAction.apply(fileContent);
            SoPath *path = sAction.getPath();
            
            if (path) 
                stereoCameraKit.setValue(path->getTail());
            else
            {
                printf("SoDisplayKit error: no \"stereoCameraKit\""
                       "found in file \"%s\"\n", 
                       ((SoFile *)stereoCameraKit.getValue())
                       ->name.getValue().getString());
                exit(1);
            }
            fileContent->unref();
        }
        else {
            printf("SoDisplayKit error: field \"stereoCameraKit\""
                   "not valid\n");
            exit(1);
        }
    }
    /*    printf("INFO: camera kit = %s\n",
          stereoCameraKit.getValue()->getTypeId().getName().getString());
    */
    //-- info camera control mode
    SoCameraControlMode *test = SO_CHECK_PART(this, "cameraControl", SoCameraControlMode);
    if (test)
    {
        printf("INFO: camera control = %s\n",
               test->getTypeId().getName().getString());
        // connect station of trackedControlMode from displayKit station
        if (test->isOfType(SoTrackedDisplayControlMode::getClassTypeId()))
        {
            ((SoTrackedDisplayControlMode *)test)
                ->station.connectFrom(&station);
        }
        else if (test->isOfType
                 (SoTrackedViewpointControlMode::getClassTypeId()))
        {
            ((SoTrackedViewpointControlMode *)test)
                ->station.connectFrom(&station);
        }
		else if (test->isOfType
                 (SoTrackedViewpointMobileDisplayControlMode::getClassTypeId()))
        {
            ((SoTrackedViewpointMobileDisplayControlMode *)test)
                ->viewpointStation.connectFrom(&station);
        }
    }
    else {
        printf("SoDisplayKit error: field \"cameraControl\" does not exist\n");
        exit(1);
    }

//-- info display mode

#ifdef WIN32
    if (pixelFormat.getValue() > 0)
    {
        SbString envString="OIV_FORCE_PIXEL_FORMAT=";
        envString+=pixelFormat.getValue();
        putenv(envString.getString());
        printf("INFO: forcing pixel format to %d\n", pixelFormat.getValue());
    }
#endif


    if (SO_CHECK_PART(this, "display", SoDisplayMode))
        printf("INFO: display mode = %s\n",
               display.getValue()->getTypeId().getName().getString());
    else
    {
        printf("SoDisplayKit error: field \"display\" does not exist\n");
        exit(1);
    }

//-- create viewer and show it

    printf("INFO: viewer widget at (%d,%d), size = (%d,%d)\n",
           xoffset.getValue(), yoffset.getValue(),
           width.getValue(), height.getValue());

    //-- scene is prepared for adding of custom environment nodes
    if (scene)
    {
        SoSeparator *envHook = new SoSeparator;
        envHook->addChild(scene);
        scene = envHook;
    }
    else {
        scene = new SoSeparator;
    }

    if (!videoBackground.getValue()->isOfType(SoVideoBackground::getClassTypeId()))
    {
        printf("SoDisplayKit error: specified videoBackground is not of type SoVideoBackground!\n");
        exit(1);
    }

    viewer = new SoStudierstubeViewer(widget, 
                                      (SoDisplayMode *)display.getValue(),
                                      (SoCameraControlMode *)cameraControl.getValue(),
                                      (SoStereoCameraKit *)stereoCameraKit.getValue(),
                                      (SoVideoBackground *)videoBackground.getValue(),
                                      scene);

    viewer->setWindowDecoration(windowBorder.getValue());
    if( viewer->isDecoration() != decoration.getValue())
        viewer->setDecoration( decoration.getValue());
	viewer->setWindowOnTop(windowOnTop.getValue());

	if (stencilBuffer.getValue() == TRUE)
	{
        viewer->setStencilBuffer(TRUE);
    }

    viewer->setWindowPosSize(xoffset.getValue(), yoffset.getValue(),
                             width.getValue()  , height.getValue());
    viewer->setStereoMode();
    viewer->setTransparencyType(
        (SoGLRenderAction::TransparencyType)transparencyType.getValue());
    viewer->setTitle(STUDIERSTUBE_VERSION_STRING);
    // scene lighting
    viewer->setHeadlight(headlight.getValue());
    if (viewer->getHeadlight())
        viewer->getHeadlight()->intensity.setValue(headlightIntensity.getValue());


	// add additional environment nodes (lights, ....)
	SoNodeKitListPart *env = new SoNodeKitListPart;
	// allowed types
	env->addChildType(SoLightKit::getClassTypeId());
	env->addChildType(SoLight::getClassTypeId());
	env->addChildType(SoEnvironment::getClassTypeId());
	env->addChildType(SoLightModel::getClassTypeId());
 	for (int i=0;i<customEnvironment.getNum();i++)
		if (customEnvironment[i])
			env->addChild(customEnvironment[i]);
	scene->insertChild(env,0);
	
    viewer->setBackgroundColor(backgroundColor.getValue());

    viewer->show();
    SoGui::show(viewer->getWidget());

    // See FIXME in SoStudierstubeViewer::actualRedraw
    // gerhard 29092004
    // viewer->activateVideoOverlay();

#if defined(WIN32) && defined(USE_TGS)
    int pixelFormat = viewer->getPixelFormat();
    printf("INFO: used pixel Format: %d\n", pixelFormat);
#endif

	// set mouse cursor
	if( showMouse.getValue() == FALSE )
	{
		viewer->setComponentCursor(SoGuiCursor::getBlankCursor());
	}
}

/* ===========================================================================
    End of SoDisplayKit.cxx
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










































