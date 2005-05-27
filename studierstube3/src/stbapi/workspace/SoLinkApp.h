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
/** SoLinkApp.cxx contains the class definition of SoLinkApp
  *
  * @author Erich Pohn, Dieter Schmalstieg
  *
  * $Id: SoLinkApp.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOLINKAPP_H_
#define _SOLINKAPP_H_

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTrigger.h> 
#include <Inventor/fields/SoSFRotation.h> 
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/sensors/SoOneShotSensor.h> 
#include <Inventor/sensors/SoFieldSensor.h> 

#include <stbapi/stbapi.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/util/ivio.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoContextKit;

/** 
  * SoLinkApp serves for managing slide shows.
  * Sequences of applications (slides) can be loaded,shown and deleted.
  *
  * @author Erich Pohn,Dieter Schmalstieg
  *
  *  File syntax:
  *@verbatim
  SoLinkApp {
      fileName         ""
      workDir          ""
      position         0 0 0
      orientation      0 0 1 0
      size             1 1 1
      keepCurrent      REMOVE
      loadImmediately  FALSE
      showImmediately  FALSE
      description      ""
      locale           ""
      trigger
  }
  @endverbatim
  * @ingroup workspace
  */
class STBAPI_API SoLinkApp : public SoNode
{
SO_NODE_HEADER(SoLinkApp);

protected:

    /** constructor. */
    SoLinkApp();

    /** destructor. */
    virtual ~SoLinkApp();

    /** CallBack for loading application
      */
    static void linkAppCB(void *data, SoSensor *sensor);
    static void linkImmedCB(void *data, SoSensor *sensor);
    /** CallBack for setting position of application
      */
    static void setPosCB(void *data, SoSensor *sensor);

public:
    enum 
    {
        KEEP      = 0,
        INVISIBLE = 1,
        REMOVE    = 2
    };

    /** Inventor class initialization */
    static void initClass();

    /** 
      * name of application
      */
    SoSFString fileName;

    /** 
      * working directory of application
      */
    SoSFString workDir;

    /** 
      * absolute Position of scene's window. If this field is set
      * to be ignored, the position of the new window is not changed.
      */
    SoSFVec3f position;

    /** 
      * absolute Rotation of scene's window. If this field is set
      * to be ignored, the rotation of the new window is not changed.
      */
    SoSFRotation rotation;

    /**
      * absolute size of scene's window. If this field is set
      * to be ignored, the size of the new window is not changed.
      */
    SoSFVec3f size;

    /** 
      * if true, the current scene is supposed to stay after loading 
      * the linked scene
      */
    SoSFEnum keepCurrent;

    /** 
      * if true, the link is loaded without requiring user interaction 
      * first.This controls preloading and "caching" of slides.
      */
    SoSFBool loadImmediately;

    /** 
      * if true, the link is shown also as soon as the application
      * is loaded.
      */
    SoSFBool showImmediately;

    /**
      * connect this field from any other field to trigger the
      * loading and display of the linked application.
      */
    SoSFTrigger trigger;

    /** 
      * Text describing the content of the link
      */
    SoSFString description;

    /**
      * locale to load the application into. If not set or the locale given
      * here does not exist, the application is loaded into the system
      * default locale.
      */
    SoSFName locale;
private:

    /**
      * pointer to context after loading of application 
      */
    SoContextKit*     context;
    static SoLinkApp* activeLink;

    SoFieldSensor*    linkAppSensor;
    SoFieldSensor*    setPosSensor;
    SoOneShotSensor*  linkImmedSensor;

    static void loadApp(SoLinkApp* self);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

//----------------------------------------------------------------------------
#endif//_SOLINKAPP_H_

