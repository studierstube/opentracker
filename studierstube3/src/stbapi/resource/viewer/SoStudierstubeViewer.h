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
/** SoStudierstubeViewer.h contains the definition of class SoStudierstubeViewer
  *
  * @author Dieter Schmalstieg, Rainer Splechtna, Tamer Fahmy
  *
  * $Id: SoStudierstubeViewer.h 3809 2004-10-01 11:39:46Z reitmayr $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOSTUDIERSTUBEVIEWER_H_
#define _SOSTUDIERSTUBEVIEWER_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <stbapi/stbapi.h>
#include SOGUI_H
#include SOGUI_EXAMINERVIEWER_H
#include <Inventor/nodes/SoSeparator.h>
#include <stbapi/util/timerutility.h>
#include "SoStereoCameraKit.h"
#include "SoCameraControlMode.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoDisplayMode;
class SoVideoBackground;

/**
SoStudierstubeViewer is the interface class for rendering in the Studierstube
environment. It provides means for stereoscopic rendering on a variety of
output devices such as large stereo projection screens such as the Virtual
Table or head mounted displays. It also allows to specify various kinds of
camera control, in particular head tracking. These modes can be mixed, matched
and extended to suit an application's needs. The viewer requires three objects
that are handed to the constructor:
- a display mode (SoDisplayMode) object manages display, stereo output etc.
- a camera control mode (SoCameraControl) object manages how the user
  manipulates a camera (head tracking or desktop ...)
- the viewer also requires an SoStereoCameraKit, which contains two cameras
  which the viewer uses for (stereo) rendering.

The SoCameraControlModes and the SoDisplayModes are mutual independent,
so that each combination of SoCameraControlMode and SoDisplayModes is 
possible. In some cases a sepcifc combination of SoCameraControlMode and 
SoDisplayMode requires a little adjustment done to the viewer parameters. 
This adjustment concerns the viewing mode the SoStudierstubeViewer will start 
in and the permission of viewer decoration. These configuration tasks are 
performed by the SoStudierstubeViewer setStereoMode() method.

The class SoStudierstubeViewer is a subclass of SoGuiExaminerViewer, 
so the programmer has access to a rich set of methods for customizing the 
viewer, though not all methods which are inherited are useful in combination 
with the SoStudierstubeViewer. Because the SoStudierstubeViewer configures 
many parameters of the viewer itself according to the requested combination 
of display mode and camera control mode, some methods inherited from 
SoGuiExaminerViewer could be used to override this configuration 
(e.g. viewing, border, stereoOffset, ...) and may alter the behavior of the 
SoStudierstubeViewer, hence it is not recommended to use the following 
methods of the SoStudierstubeViewer: setViewing() (if not desktop control), 
setStereoOffset() (if desktop control).

@author Rainer Splechtna
@ingroup camera
@ingroup workspace
@ingroup core
*/
class STBAPI_API SoStudierstubeViewer : public SoGuiExaminerViewer
{
public:
   /** The constructor of the viewer:
    *    
    * @param aDisplayMode display mode interface object
    * @param aControlMode camera control mode object
    * @param aCamKit camera kit for viewing
    * @param newScene scene graph to be rendered
    */
    SoStudierstubeViewer(GuiWidget widget,
                         SoDisplayMode* aDisplayMode,
                         SoCameraControlMode* aControlMode,
                         SoStereoCameraKit* aCamKit,
                         SoVideoBackground* videoBackground,
                         SoNode* newScene = NULL);
    
    /**
     * Destructor, does some clean up work.
     */
    ~SoStudierstubeViewer();
    
    /**
     * Sets parameters of the viewer (e.g. decoration, ...) are set
     * automatically according to the used combination of display mode
     * and camera control mode. 
     *
     * After the setStereoMode() of the display mode was called, this method
     * calls the isViewing() method of the requested SoCameraControlMode and
     * overrides the viewing-mode and decoration settings accordingly to the
     * returned value of isViewing().
     */
    void setStereoMode();

    /** Calls the setMonoMode() method of the used display mode object.     */
    void setMonoMode();
    
    /**
     * Sets the scene graph to be rendered by this viewer.
     * @param newScene pointer to new scene graph
     */
    virtual void setSceneGraph(SoNode* newScene);

    /**
     * Returns the scene graph rendered by this viewer.
     * @return pointer to new scene graph
     */
    virtual SoNode *getSceneGraph();

    /**
     * Overrides inherited method to set a custom headlight.
     * @param on TRUE  turn headlight on
     *            FALSE turn headlight on
     */
    virtual void setHeadlight(SbBool on);

    /**
     * Returns whether the headlight is turned on or off.
     * @return TRUE  headlight is turned on
     * FALSE headlight is turned off
     */
    SbBool isHeadlight() const;

    /**
     *  Returns the SoDirectionalLight-instance used as headlight.
     * @return Pointer to SoDirectionalLight-instance used as headlight.
     * (NULL if headlight isn't created yet)
     */
    SoDirectionalLight *getHeadlight() const;

    /**
     * This method turns the decorations of a window on/off.
     * @param on TRUE  turn decorations on
     *        on FALSE turn decorations off
     */
    void setWindowDecoration(SbBool on);

    /**
     * This method sets a widget on top of all others and keeps it there.
     * @param on TRUE  set on top
     *        on FALSE keep as it is
     */
    void setWindowOnTop(SbBool on);
    
    /**
     * Returns whether the window decoration are turned on or off.
     * @return TRUE  decorations are turned on
     *         FALSE decorations are turned off
     */
    inline SbBool isWindowDecoration() const { return this->isWindowDecorationActive; }


    /**
     * This method sets the position and the size of a window on the desktop.
     * @param x x position of the window on the desktop
     * @param y y position of the window on the desktop
     * @param width window width in pixels
     * @param height window height in pixels
     */
    void setWindowPosSize(int x, int y, int width, int height);
    
    /**
     * Returns used display mode object.
     * @return used display mode object
     */
    SoDisplayMode* getDisplayMode();

    /**
     * Returns used camera control mode object
     * @return used camera control mode object
     */
    SoCameraControlMode* getCameraControlMode();

    /**
     * Returns used camera kit.
     * @return used camera kit
     */
    SoStereoCameraKit* getStereoCameraKit();

    /**
     * Registers a function, which is called before and after the viewer's actualRedraw is performed.
     * The second parameter of redrawCB specifies, whether the call is executed before (TRUE) or 
     * after (FALSE) the viewer's actualRedraw.
     * @param redrawCB a pointer to the function, which should be called
     * @param data     is used as the first parameter of redrawCB (user data)
    */
    void setActualRedrawCB(void (*redrawCB)(void *, SbBool), void *data);

    /**
     * This static method stores a pointer to the given handler.
     *
     * The external handler is called, when the viewer gets an event,
     * which is not consumed by itself.
     * note: order of event processing is as follows:
     * SoStudierstubeViewer -> externalHandler -> SoGuiExaminerViewer
     *
     * @param handler the external handler to be called, when an event 
     * is not consumed by the viewer itself
     */        
    static void setExternalHandler(int (*handler)(const SoEvent * event, 
                                   SoStudierstubeViewer * const viewer));

    /** Activate rendering of a video image as background of the viewer */
    void activateVideoOverlay();                          
    
    /** flag that indicates if the video background mode was activated */
    SbBool isVideoActive;
    
    /** Video background object */
    SoVideoBackground* videoBackground;

protected:
    /**
     * The actual redraw method is overloaded for stereo rendering.
     *
     * This method uses the selectCameraXXX (from SoCameraControlMode) and
     * selectBufferXXX (from SoDisplayMode) methods to control the rendering
     * of the right and left eye view.
     */
    virtual void actualRedraw();

    virtual void createViewerButtons(GuiWidget parent, SbPList * buttonList); 
    virtual SbBool processSoEvent(const SoEvent * const event);
    virtual void sizeChanged(const SbVec2s & size);
                
private:
    /** Stores a flag to indicate if window decorations are activated */
    SbBool isWindowDecorationActive;

    /** The headlight managed by the viewer */
    SoDirectionalLight *headlight;

    /** Display mode object     */
    SoDisplayMode* displayMode;

    /** Camera control mode object */
    SoCameraControlMode* controlMode; 

    /** Scene graph root */
    SoSeparator* root;

    /** Performance measurement timer */
    TimerUtility* frameRateTimer;

    /// internal flag to initialize video background later than construction time
    bool initVideo;

    /**
     * A pointer to the funtion, which is called at the beginning and the end 
     * of the viewer's actualRedraw method.
     */
    void (*redrawCB)(void *, SbBool preOrPost);

    /**
     * A pointer (to user data), which is used as the first parameter of
     * the redrawCB.
     */
    void *redrawCBData;

    /** Stores pointer to external handler. */ 
    static int (*externalHandler)(const SoEvent * event, 
                                  SoStudierstubeViewer * const viewer);
};

//----------------------------------------------------------------------------
#endif//_SOSTUDIERSTUBEVIEWER_H_

