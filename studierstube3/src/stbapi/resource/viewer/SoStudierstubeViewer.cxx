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
/** SoStudierstubeViewer.cxx contains the implementation of class SoStudierstubeViewer
 *
 * @author Dieter Schmalstieg, Rainer Splechtna, Gerd Hesina, Tamer Fahmy
 *
 * $Id: SoStudierstubeViewer.cxx 3810 2004-10-01 15:04:57Z reitmayr $
 * @file                                                                   
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#define XK_Escape VK_ESCAPE
#define XK_End VK_END
#define XK_Insert VK_INSERT
#define XK_x 'X'

#include <windows.h>
#endif

#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <iostream>

#include "SoStudierstubeViewer.h"
#include "SoDisplayMode.h"
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/resource/viewer/video/SoVideoBackground.h>

#include SOGUI_H
#include SOGUI_EXAMINERVIEWER_H

#ifdef USE_SOQT
#include <qwidget.h>
#elif USE_SOXT
#include <X11/IntrinsicP.h>
#endif

#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/SbPList.h>

#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/engines/SoCalculator.h>

#include <stbapi/util/GLtools.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/resource/viewer/SoTrackedViewpointControlMode.h>
#include <stbapi/resource/viewer/SoTrackedViewpointMobileDisplayControlMode.h>
#include <stbapi/resource/viewer/SoTrackedDisplayControlMode.h>
#include <stbapi/resource/viewer/SoRotateFromTo.h>

#include <stbapi/workspace/SoLocaleManagerKit.h>
#include <stbapi/workspace/SoLocaleKit.h>

using namespace std;

int (*SoStudierstubeViewer::externalHandler)(const SoEvent * event, SoStudierstubeViewer * const viewer) = NULL;

//----------------------------------------------------------------------------
// The constructor of the viewer.
SoStudierstubeViewer::SoStudierstubeViewer(GuiWidget widget,
                                           SoDisplayMode* aDisplayMode,
                                           SoCameraControlMode* aControlMode,
                                           SoStereoCameraKit* aCamKit,
                                           SoVideoBackground* videoBackground,
                                           SoNode* newScene) :
    SoGuiExaminerViewer(widget, "Studierstube", 
                        TRUE, SoGuiFullViewer::BUILD_ALL),
    initVideo(false),
    isVideoActive(FALSE),
    videoBackground(videoBackground),
    isWindowDecorationActive(TRUE),
    headlight(NULL),
    displayMode(aDisplayMode),
    controlMode(aControlMode),
    redrawCB(NULL),
    redrawCBData(NULL)
{
    assert(displayMode);
    assert(controlMode);
    assert(aCamKit);

    controlMode->setStereoCameraKit(aCamKit); //control mode must know camera

    // the constructed scene graph looks as follows:
    //               root
    //                |
    //            +-----+---------+-------------+
    //            |     |         |             |
    //          xfSep xfSep (backCallback)  newScene
    //          |        |
    //   +----------+    ---+----------+
    //   |          |       |          |  
    // transform camKit  transform  headlight
    //
    // note: the double xfSep is due to some caching problem in OpenInventor with
    //       nested xfSeps :(. (see the source of ModelMatrixElement for some ideas:). 

    root = new SoSeparator;
    root->ref();
    
    SoTransformSeparator *cameraPlatform = new SoTransformSeparator;
    SoTransform *transform = new SoTransform;
    root->addChild( cameraPlatform );
    cameraPlatform->addChild(transform);
    cameraPlatform->addChild(aCamKit);    
    SoTransform  *transform2 = new SoTransform;
    SoTransformSeparator *lightPlatform = new SoTransformSeparator;
    root->addChild(lightPlatform); // hook for headlight     
    lightPlatform->addChild(transform2);
    root->addChild(new SoSeparator); // hook for video background
    root->addChild(new SoSeparator); // hook for scene
    setSceneGraph(newScene);

    int station = -1;
    if (controlMode->isOfType( SoTrackedViewpointControlMode::getClassTypeId())) {
        station = ((SoTrackedViewpointControlMode *)controlMode)->station.getValue();
    }
    else if (controlMode->isOfType( SoTrackedViewpointMobileDisplayControlMode::getClassTypeId())) {
        station = ((SoTrackedViewpointMobileDisplayControlMode *)controlMode)->viewpointStation.getValue();
    }
    else if (controlMode->isOfType( SoTrackedDisplayControlMode::getClassTypeId())) {
        station = ((SoTrackedDisplayControlMode *)controlMode)->station.getValue();
    }

    if (station != -1) {
        SoUserKit * userKit = SoUserManagerKit::getInstance()->getUserKit(
            SoUserManagerKit::getInstance()->getUserFromStation( station ));
        if (userKit)
        {
            SoLocaleKit * locale = SoLocaleManagerKit::getInstance()->getLocaleFromNode( userKit );
            if (locale) {
                station = locale->station.getValue();
                transform->translation.connectFrom( Stb3DEventGenerator::getTranslationField(station));
                transform->rotation.connectFrom( Stb3DEventGenerator::getRotationField(station));
                transform2->translation.connectFrom( Stb3DEventGenerator::getTranslationField(station));
                transform2->rotation.connectFrom( Stb3DEventGenerator::getRotationField(station));
            }
        }
    }

//--set up timer
    static int instanceCounter;
    SbString instanceString = "VIEWER";
    instanceString += SbString(instanceCounter);
    frameRateTimer = new TimerUtility(instanceString.getString(),5000.0);
    instanceCounter++;

    SoGLRenderAction* renderAction = this->getGLRenderAction();
    renderAction->setSmoothing(TRUE);
	// in order to support COINs SORTED_LAYER_BLEND transparency Mode, 
	// the Alpha bits must be set

#ifdef __COIN__    
	this->setAlphaChannel(TRUE);
#endif
}

//----------------------------------------------------------------------------
// Destructor, does some clean up work.
SoStudierstubeViewer::~SoStudierstubeViewer()
{
    setMonoMode();
    root->unref();
    delete frameRateTimer;
}

//----------------------------------------------------------------------------
// Sets parameters of the viewer.
void
SoStudierstubeViewer::setStereoMode()
{
    //set window parameters
    displayMode->setStereoMode(this); 
    // set viewing mode (controlMode decides on viewing mode)
    if (controlMode->isViewing() != isViewing())
        setViewing(controlMode->isViewing());
    // override viewer settings according to control mode
    if (getCameraControlMode()->isViewing()) {
        if (!isPopupMenuEnabled())
            setPopupMenuEnabled(TRUE);
        setAutoClipping(TRUE);
    }
    else {
        if (isDecoration())
            setDecoration(FALSE);
        if (isPopupMenuEnabled())
            setPopupMenuEnabled(FALSE);
    }
}

//----------------------------------------------------------------------------
// Calls the setMonoMode() method of the used display mode object.
void
SoStudierstubeViewer::setMonoMode()
{
    displayMode->setMonoMode();
}

//----------------------------------------------------------------------------
// Sets the scene graph to be rendered by this viewer.
void
SoStudierstubeViewer::setSceneGraph(SoNode* newScene)
{
    if (!newScene) return; //ignore empty scenes
    root->replaceChild(3, newScene);
    SoGuiViewer::setSceneGraph(root);
}

//----------------------------------------------------------------------------
// Returns the scene graph rendered by this viewer.
SoNode *
SoStudierstubeViewer::getSceneGraph()
{
    return root->getChild(3);
}

//----------------------------------------------------------------------------
// Overides the inherited setHeadlight method
void
SoStudierstubeViewer::setHeadlight(SbBool on)
{
    SoStereoCameraKit *camKit = getStereoCameraKit();
    assert(camKit);

    SoGuiExaminerViewer::setHeadlight(on);
    // If the SoStudierstubeViewer uses the examiner-camera to render the
    // scene, don't set the custom headlight
    if ((camKit->getExaminerCamera()->getName() != "defaultExaminerCamera") &&
        getCameraControlMode()->isViewing())  
        return;

    //  Disable inherited headlight
    if (SoGuiExaminerViewer::getHeadlight())
        SoGuiExaminerViewer::getHeadlight()->on = FALSE;
    // and set custom headlight
    if (on) {
        if (!headlight) {
            // create SoStudierstubeViewer's custom headlight
            SoTransform *camXf = new SoTransform;
            SoRotation  *lorient = new SoRotation;
            headlight = new SoDirectionalLight;
            headlight->direction.setValue(1, -1, -10);
 
            SoTransformSeparator *lightroot = 
                (SoTransformSeparator *) root->getChild(1);
            lightroot->addChild(camXf);
            lightroot->addChild(lorient);
            lightroot->addChild(headlight);
            camXf->translation.connectFrom(&camKit->getTransform()->translation);
            camXf->rotation.connectFrom(&camKit->getTransform()->rotation);
                        
                        SoCalculator *vDir = new SoCalculator;
                    vDir->A.connectFrom(&camKit->getCameraLeft()->eyepointPosition);
                        vDir->B.connectFrom(&camKit->getCameraLeft()->position);
                    vDir->expression.setValue("oA = vec3f(0,0,-1); oB = B-A");
                        SoRotateFromTo *rot = new SoRotateFromTo;
                        rot->vectorFrom.connectFrom(&vDir->oA);
                        rot->vectorTo.connectFrom(&vDir->oB);
            lorient->rotation.connectFrom(&rot->rotateFromTo);
        }
        headlight->on = TRUE;
    }
    else
        if (headlight)
            headlight->on = FALSE;      
}


//----------------------------------------------------------------------------
// Returns whether the headlight is turned on or off.
SbBool 
SoStudierstubeViewer::isHeadlight() const
{
    if (headlight)
        return headlight->on.getValue();
    else
        return FALSE;
}

//----------------------------------------------------------------------------
// Returns the SoDirectionalLight-instance used as headlight.
SoDirectionalLight *
SoStudierstubeViewer::getHeadlight() const
{
    return headlight;
}


//----------------------------------------------------------------------------
// This method sets a window on top of all other windows and will keep it there
void
SoStudierstubeViewer::setWindowOnTop(SbBool on)
{
    if (on) {
#ifdef USE_SOQT
        this->getParentWidget()->reparent(this->getParentWidget()->parentWidget(), 
                                          Qt::WStyle_Customize| Qt::WStyle_StaysOnTop,
                                          this->getParentWidget()->pos(), TRUE);
#endif
        }
}


//----------------------------------------------------------------------------
// This method turns the decorations of a window on/off.
void
SoStudierstubeViewer::setWindowDecoration(SbBool on)
{
    // _attention_: extremely toolkit dependent stuff following...

    // checks if the decorations aren't already switched on or off
    if (this->isWindowDecoration() != on) {
        // switch window decorations on
        if (on) {
            
#ifdef USE_SOQT
                        this->getParentWidget()->reparent(this->getParentWidget()->parentWidget(), 
                                              Qt::WStyle_Customize|Qt::WStyle_NormalBorder,
                                              this->getParentWidget()->pos(), TRUE);
#elif USE_SOGTK
            gtk_widget_realize(GTK_WIDGET(this->getParentWidget()));
            gdk_window_set_decorations(GTK_WIDGET(this->getParentWidget())->window, GDK_DECOR_ALL);
#elif USE_SOXT
            Arg args[1];
            XtSetArg(args[0], XmNmwmDecorations, 1);
            XtSetValues(this->getParentWidget(), args, 1);
#elif USE_SOWIN
            SetActiveWindow(this->getParentWidget());
            SetWindowLong(this->getParentWidget(), GWL_STYLE,
                          WS_POPUP|WS_MAXIMIZE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE);
#endif
            
            this->isWindowDecorationActive = TRUE;
        }
        else {

#ifdef USE_SOQT
            this->getParentWidget()->reparent(this->getParentWidget()->parentWidget(), 
                                              Qt::WStyle_Customize|Qt::WStyle_NoBorder,
                                              this->getParentWidget()->pos());
#elif USE_SOGTK
            gtk_widget_realize(GTK_WIDGET(this->getParentWidget()));
            gdk_window_set_decorations(GTK_WIDGET(this->getParentWidget())->window, (GdkWMDecoration)0);
#elif USE_SOXT
            Arg args[1];
            XtSetArg(args[0], XmNmwmDecorations, 0);
            XtSetValues(this->getParentWidget(), args, 1);
#elif USE_SOWIN
            SetActiveWindow(this->getParentWidget());
            SetWindowLong(this->getParentWidget(), GWL_STYLE,
                          WS_POPUP|WS_MAXIMIZE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE);
#endif
            
            this->isWindowDecorationActive = FALSE;
        }
    }
}

//----------------------------------------------------------------------------
void 
SoStudierstubeViewer::setActualRedrawCB(void (*rcb)(void *, SbBool), void *data)
{
    redrawCB = rcb;
    redrawCBData = data;
}

//----------------------------------------------------------------------------
// This method sets the position and the size of a window on the desktop.
void
SoStudierstubeViewer::setWindowPosSize(int x, int y, int width, int height)
{

    char *ep = '\0';
    errno = 0;

    // string to int conversion the very safe OpenBSD way. --tamer
    char *envX = getenv("SOGUI_DEFAULT_X_SIZE");
    long sizeX = 0;

    if (envX) {
        sizeX = strtol(envX, &ep, 10);
        if (envX[0] == '\0' || *ep != '\0')
            cerr << "** SOGUI_DEFAULT_X_SIZE=" << envX << " is not a number, therefore ignored!" << endl;
        else if (errno == ERANGE && (sizeX == LONG_MAX || sizeX == LONG_MIN) || (sizeX > INT_MAX || sizeX < INT_MIN))
            cerr << "** SOGUI_DEFAULT_X_SIZE=" << envX << " is out of range, therefore ignored!" << endl;
        else {
            cout << "INFO: viewer widget environmental width override, width=(" << sizeX << ")" << endl;
            width = sizeX;
        }
    }

    char *envY = getenv("SOGUI_DEFAULT_Y_SIZE");
    long sizeY = 0;

    if (envY) {
        sizeY = strtol(envY, &ep, 10);
        if (envY[0] == '\0' || *ep != '\0')
            cerr << "** SOGUI_DEFAULT_Y_SIZE=" << envY << " is not a number, therefore ignored!" << endl;
        else if (errno == ERANGE && (sizeY == LONG_MAX || sizeY == LONG_MIN) || (sizeY > INT_MAX || sizeY < INT_MIN))
            cerr << "** SOGUI_DEFAULT_Y_SIZE=" << envY << " is out of range, therefore ignored!" << endl;
        else {
            cout << "INFO: viewer widget environmental height override, height=(" << sizeY << ")" << endl;
            height = sizeY;
        }
    }

#ifdef USE_SOQT
    (this->getParentWidget())->move(x, y);
#elif USE_SOGTK
    gtk_widget_realize(GTK_WIDGET(this->getParentWidget()));
    gdk_window_move(GTK_WIDGET(this->getParentWidget())->window, x, y);
#elif USE_SOXT
    XtMoveWidget(this->getParentWidget(), x, y);
#elif USE_SOWIN
    SetWindowPos(this->getParentWidget(), 
                 HWND_TOP,
                 x, y, width, height,
                 SWP_SHOWWINDOW);
#endif
    
    // the viewer instantiated with SoGui::init needs to be treated differently.
    // getShellWidget returns NULL in this case.
    if (!this->getShellWidget())
        SoGui::setWidgetSize(SoGui::getShellWidget(this->getWidget()), SbVec2s(width, height));
    else
        this->setSize(SbVec2s(width, height));
}

//----------------------------------------------------------------------------
// The actualRedraw method is overloaded for stereo rendering.
void 
SoStudierstubeViewer::actualRedraw()
{
    // FIXME: 
    // for some reason cannot do the initialization in SoDisplayKit anymore
    // therefore moved one time init here. Might be related to using SoWin ??
    // gerhard 29092004    
    if( initVideo == false )
    {
        activateVideoOverlay();
        initVideo = true;
    }

    if (redrawCB)
        redrawCB(redrawCBData, TRUE);

    // render left image - unconditional
    controlMode->selectCameraLeft();
    displayMode->selectBufferLeft();

    // we don't do this anymore. It has some conceptual problems. OpenTracker is 
    // not simply sampling devices but providing events. If they are not processed
    // and send to the scene graph, loss of important (button etc.) events can 
    // occur. Sampling and sending is happening only in the regular CB provided for that.
    // Stb3DEventGenerator::sample(TRUE);  // sample tracker AND update global fields =>
    // (pen, PIP & HMD positions) 
            
    if (isVideoActive) // check whether video background is active
        videoBackground->blitOverlay(STEREO_LEFT); // and blit background to left or mono image        

    // left: perform full actual redraw
    if (isStereoViewing())                 // if quad-buffering is used,     
        SoGuiRenderArea::actualRedraw();     // call the SoGuiRenderArea method                                            // to ommit waste renderpass
    else  
        SoGuiExaminerViewer::actualRedraw(); 

    // render right image - only if it is a real stereo mode
    if(displayMode->isStereoMode()) {
        controlMode->selectCameraRight();
        displayMode->selectBufferRight();
        if (isVideoActive) // check whether video background is active
            videoBackground->blitOverlay(STEREO_RIGHT); // and blit background to right image
        getSceneManager()->render(FALSE); //right: perform just screen update
    }

    if (redrawCB)
        redrawCB(redrawCBData, FALSE);

    frameRateTimer->timeThis(); // determine frame rate
}

//----------------------------------------------------------------------------
// Overridden to bypass SoGuiExaminerViewer button configuration.
void
SoStudierstubeViewer::createViewerButtons(GuiWidget parent, SbPList *buttonlist)
{
    SoGuiFullViewer::createViewerButtons(parent, buttonlist);
}

//----------------------------------------------------------------------------
// Returns used camera kit.
SoStereoCameraKit*
SoStudierstubeViewer::getStereoCameraKit()
{
    return controlMode->getStereoCameraKit();
}

//----------------------------------------------------------------------------
// Returns used display mode object.
SoDisplayMode* 
SoStudierstubeViewer::getDisplayMode()
{
    return displayMode;
}

//----------------------------------------------------------------------------
// Returns used camera control mode object.
SoCameraControlMode*
SoStudierstubeViewer::getCameraControlMode()
{
    return controlMode;
}

//----------------------------------------------------------------------------
// This static method stores a pointer to the given handler.
void
SoStudierstubeViewer::setExternalHandler(int (*handler)(const SoEvent *event, SoStudierstubeViewer * const viewer))
{
    externalHandler = handler;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// Overridden to control the processing of events by this class.
SbBool
SoStudierstubeViewer::processSoEvent(const SoEvent * const event)
{
    int handled=FALSE;
    const SoEvent * soEvent = event;

    if (!soEvent) return SoGuiExaminerViewer::processSoEvent(soEvent);

    if (SO_KEY_PRESS_EVENT(soEvent, ANY)) 
    {
        int keysym=((SoKeyboardEvent*)soEvent)->getKey();
        // if the camera control mode does not use the viewing mode
        // lock the ESCAPE key 
        if (!getCameraControlMode()->isViewing()) 
            if (keysym == SoKeyboardEvent::ESCAPE) 
                return FALSE;
        // add key commands
        switch(keysym)
        {
            case SoKeyboardEvent::X:
                if (isVideoActive) {
                    delete videoBackground;
                    isVideoActive = FALSE;
                }
                SoGui::exitMainLoop();
                handled=TRUE;
                break;
                // only in view mode: if key 'end' is pressed view all
            case SoKeyboardEvent::END:
                if (isViewing()) {
                    viewAll();
                    handled=TRUE;
                }
                break;
                // only in view mode: if key 'insert' is pressed save home position
            case SoKeyboardEvent::INSERT:
                if (isViewing()) {
                    saveHomePosition();    
                    handled=TRUE;
                }
                break;
            default:
                break;
        }
    }

    if (externalHandler && !handled)
        handled = externalHandler(soEvent, this);

    if (!handled) return SoGuiExaminerViewer::processSoEvent(soEvent);

    return FALSE;
}

//----------------------------------------------------------------------------
// Overridden to adapt the video background size
void
SoStudierstubeViewer::sizeChanged(const SbVec2s & size)
{
    if (isVideoActive)
        videoBackground->setOverlaySize(size[0], size[1]);
    
    // simply reset the stereo mode to adjust to screen size
    getDisplayMode()->setStereoMode(this);

    SoGuiExaminerViewer::sizeChanged(size);
}

// Activate rendering of a video image as background of the viewer
void
SoStudierstubeViewer::activateVideoOverlay()
{
    // initialize the video background
    SbVec2s size = getSize(); // current widget size

    // to indicate to actualRedraw if it should blit the background, we
    // query if the videoBackground got initialized and is ready to grab.
    // in case the SoVideoBackground::NONE type was specified isInitialized()
    // will return false. so no overhead is created.
    isVideoActive = videoBackground->initVideoBackground(size[0], size[1]);

    // do not clear the background, as we overwrite it anyway        
    if(isVideoActive) {
        setClearBeforeRender(FALSE);
    }
}


/* ===========================================================================
   End of SoStudierstubeViewer.cxx
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
