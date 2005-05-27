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
/** SoWindowKit.h  - definition of class SoWindowKit (version 2).
  * SoWindowKit is the standard 3D container for graphical objects of
  * Studierstube applications.
  *
  * @author  Dieter Schmalstieg
  * @author  Gottfried Eibner
  *
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOWINDOWKIT_H_
#define _SOWINDOWKIT_H_

#include <Inventor/SbLinear.h>
#include <Inventor/SbColor.h>

#include <Inventor/nodekits/SoBaseKit.h>

#include <Inventor/nodes/SoSwitch.h>

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFBitMask.h>

#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/stbapi.h>
#include <stbapi/event3d/Base3D.h>

#include "EventAutomaton.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include <stbapi/workspace/appdef.h>

class SoWindowKit;
class SoSeparator;
class SoCallback;

/*
 * old and obsolete code
 * do not use the define and the WindowEventCB
 */
#define WM_EVENT 102
typedef SbBool WindowEventCB(void*, int, SoWindowKit*, int, So3DEvent*, SoPath*);

/** @typedef
 * Type for event callback functions, that are called whenever a 3D event was
 * handled by the windowkit node.
 * @param userData pointer to the user data registered with the callback
 * @param windowkit windowkit that handled the action
 * @param stationKit the StationKit calling the function
 * @param action the action traversing the scene graph
 */
typedef SbBool ApplicationEventCB(void *userData, SoWindowKit *windowkit,
                                           SoHandle3DEventAction *action);

/**
 * SoWindowKit - 3D Window class for Studierstube.
 * SoWindowKit is the standard 3D container for graphical objects of
 * Studierstube applications.
 *
 *  File format :
 *  @verbatim
  SoWindowKit {
      position     0 0 0
      orientation  0 0 1 0
      center       0 0 0
      thickness    0.025
      size         1 1 1
      state        NORMAL
      style        DEFAULT
      focusPolicy; CLICK_TO_FOCUS
      appearance;  BACK_WALL_SOLID | FRONT_WALL
      title        ""
      clientVolume    Separator {
          ...
      }
  }@endverbatim
 *
 * kit structure of SoWindowKit:
 * @verbatim
 * Separator
 * "root"
 * |
 * +-----------+
 * |           |
 * Transform   Switch
 * "transform" "visibleSwitch"
 *             |
 *             +----------------+---------------+---------------+
 *             |                |               |               |
 *             Switch           Switch          Switch          Switch
 *             "borderSwitch"   "frameSwitch"   "clipSwitch"    "clientSwitch"
 *             |                |               |               |
 *             IdxTriStripSet   Separator       +---------+-... **)
 *             "borderGeometry" "frameSeparator"|         |
 *             |                |               ClipPlane ClipPlane ...
 *             *)               |               "clipX0"  "clipX1"  ...
 *                              |
 *     +--------------+---------+-------+---------------+
 *     |              |                 |               |
 *     LightModel     ShapeHints        Switch          Switch
 *     "frameNoLight" "frameShapeHints" "frameFrontSw"  "frameBackSw"
 *                                      |               |
 *                                      |               |
 *                     +----------------+               +--------------+
 *                     |                |               |              |
 *             DrawStyle        IndexedFaceSet   DrawStyle      IndexedFaceSet
 *             "frontDrawStyle" "frontGeometry" "backDrawStyle" "backGeometry"
 *                                      |                              |
 *                                      *)                             *)
 *    **) |
 *        +--------------+
 *        |              |
 *        Translation    Separator
 *        "clientCenter" "clientVolume"
 *
 *
 *     *) SoVertexProperty
 *          (not a part!)
 * @endverbatim
 *
<h2> SoWindowKit state tables </h2>

This figures show the possible state changes with needed eventss
and constraints and triggered Output when the state change is
possible. A / is marking the impossible state changes in the tables <br> <br>

<table>
    <tr> <td> States</td> <td></td> <td></td></tr>
    <tr> <td> outside </td><td></td><td> outside of any window volume </td></tr>
    <tr> <td> TV </td><td></td><td> Transform Volume </td></tr>
    <tr> <td> VV </td><td></td><td> ViewingVolume == ClientVolume </td></tr>
    <tr> <td> BV </td><td></td><td> BorderVolume (move, rotate) </td></tr>
    <tr> <td> CV </td><td></td><td> CornerVolume </td></tr>
    <tr> <td> BVsel </td><td></td><td> BorderVolume selected </td></tr>
    <tr> <td> CVsel </td><td></td><td> CornerVolume selected </td></tr>
</table>

<br><b> Events forcing a state change </b><br>
  <table>
   <tr><td> from/to  </td><td> outside </td><td> VV </td><td> TV </td><td> BV </td><td> CV </td><td> BVsel </td><td> CVsel </td></tr>
   <tr><td> outside </td><td> b,<s>b</s>,m,bm </td><td> m,bm </td><td> m,bm </td><td> m,bm </td><td> m,bm </td><td> m </td><td> m </td></tr>
   <tr><td> VV      </td><td> m </td><td> b,<s>b</s>,m,bm </td><td> m </td><td> m </td><td> m </td><td> / </td><td> / </td></tr>
   <tr><td> TV      </td><td> m,bm </td><td> m,bm </td><td> b,<s>b</s>,m,bm </td><td> m,bm </td><td> m,bm </td><td> / </td><td> / </td></tr>
   <tr><td> BV      </td><td> m </td><td> m </td><td> m </td><td> m,<s>b</s></td><td> m </td><td> b </td><td> / </td></tr>
   <tr><td> CV      </td><td> m </td><td> m </td><td> m </td><td> m </td><td> m,<s>b</s></td><td> / </td><td> b </td></tr>
   <tr><td> BVsel   </td><td> / </td><td> / </td><td> / </td><td><s>b</s></td><td> / </td><td> bm </td><td> / </td></tr>
   <tr><td> CVsel   </td><td> / </td><td> / </td><td> / </td><td> / </td><td><s>b</s></td><td> / </td><td> bm </td></tr>
  </table>
  <table>
   <tr><td> events: </td><td>
   <tr><td> b </td><td> button pressed </td></tr>
   <tr><td> <s>b</s> </td><td> button released </td></tr>
   <tr><td> m  </td><td> move with button released </td></tr>
   <tr><td> bm </td><td> move with  button pressed </td></tr>
   <tr><td>  / </td><td> state change not allowed </td></tr>
  </table>

<br><b> Outputs generated by state change </b><br>
  <table>
   <tr><td> from/to  </td><td> outside </td><td> VV </td><td> TV </td><td> BV </td><td> CV </td><td> BVsel </td><td> CVsel </td></tr>
   <tr><td> outside  </td><td> - </td><td> G </td><td> - </td><td> G </td><td> G </td><td> / </td><td> / </td></tr>
   <tr><td> VV       </td><td> <s>G</s> </td><td> - </td><td> - </td><td> <s>G</s>,G </td><td> <s>G</s>,G </td><td> / </td><td> / </td></tr>
   <tr><td> TV       </td><td> - </td><td> <s>G</s>,G </td><td> - </td><td> G </td><td> G </td><td> / </td><td> / </td></tr>
   <tr><td> BV       </td><td> <s>G</s> </td><td> <s>G</s>,G </td><td> <s>G</s> </td><td> - </td><td> <s>G</s>,G </td><td> DH </td><td> / </td></tr>
   <tr><td> CV       </td><td> <s>G</s> </td><td> <s>G</s>,G </td><td> <s>G</s> </td><td> <s>G</s>,G,<s>H</s>,H </td><td> - </td><td> / </td><td> DH </td></tr>
   <tr><td> BVsel    </td><td> / </td><td> / </td><td> / </td><td> <s>G</s>,G,<s>H</s>,H </td><td> / </td><td> - </td><td> / </td></tr>
   <tr><td> CVsel    </td><td> / </td><td> / </td><td> / </td><td> / </td><td> <s>G</s>,G,<s>H</s>,H </td><td> / </td><td> - </td></tr>
  </table>
  <table>
   <tr><td> outputs: </td><td>
   <tr><td> G </td><td> set grabber </td></tr>
   <tr><td> <s>G</s> </td><td> release grabber </td></tr>
   <tr><td> H  </td><td> use highlight </td></tr>
   <tr><td> DH </td><td> use drag highlight </td></tr>
   <tr><td> S  </td><td> set handled </td></tr>
   <tr><td>  - </td><td> no output generated </td></tr>
   <tr><td>  / </td><td> state change not allowed </td></tr>
  </table>


<br><b> Constraints allowing state change </b><br>
  <table>
   <tr><td> from/to  </td><td> outside </td><td> VV </td><td> TV </td><td> BV </td><td> CV </td><td> BVsel </td><td> CVsel </td></tr>
   <tr><td> outside  </td><td> - </td><td> - </td><td> - </td><td> - </td><td> - </td><td> / </td><td> / </td></tr>
   <tr><td> VV       </td><td> - </td><td> in(VV) </td><td> - </td><td> - </td><td> - </td><td> / </td><td> / </td></tr>
   <tr><td> TV       </td><td> - </td><td> - </td><td> in(TV) </td><td> - </td><td> - </td><td> / </td><td> / </td></tr>
   <tr><td> BV       </td><td> - </td><td> - </td><td> - </td><td> in(BV) </td><td> - </td><td> in(BV) </td><td> / </td></tr>
   <tr><td> CV       </td><td> - </td><td> - </td><td> - </td><td> - </td><td> in(CV) </td><td> / </td><td> in(CV) </td></tr>
   <tr><td> BVsel    </td><td> / </td><td> / </td><td> / </td><td> in(BV) </td><td> / </td><td> in(BV) </td><td> / </td></tr>
   <tr><td> CVsel    </td><td> / </td><td> / </td><td> / </td><td> / </td><td> in(CV) </td><td> / </td><td> in(CV) </td></tr>
  </table>
  <table>
   <tr><td> constraints: </td><td>
   <tr><td> in(VV) </td><td> pen in viewing/client volume </td></tr>
   <tr><td> in(TV) </td><td> pen in transform volume </td></tr>
   <tr><td> in(BV) </td><td> pen in border volume </td></tr>
   <tr><td> in(VC) </td><td> pen in corner volume </td></tr>
   <tr><td>  - </td><td> state change not restricted </td></tr>
   <tr><td>  / </td><td> state change not allowed </td></tr>
  </table>

 * @author  Dieter Schmalstieg
 * @author  Gottfried Eibner
 * @ingroup workspace
 * @ingroup core
 */

class STBAPI_API SoWindowKit
:
    public SoBaseKit,
    public Base3D
{
    SO_KIT_HEADER(SoWindowKit);
    BASE3D_HEADER;

      /// top separator, transform and switch node for the window
	SO_KIT_CATALOG_ENTRY_HEADER(renderCallback); // SoCallback   
    SO_KIT_CATALOG_ENTRY_HEADER(root);           // SoSeparator
    SO_KIT_CATALOG_ENTRY_HEADER(transform);      // SoTransform
    SO_KIT_CATALOG_ENTRY_HEADER(visibleSwitch);  // SoSwitch
      /// border components
    SO_KIT_CATALOG_ENTRY_HEADER(borderSwitch);   // SoSwitch
    SO_KIT_CATALOG_ENTRY_HEADER(borderGeometry); // SoIndexedTriangleStripSet
      /// frame components
    SO_KIT_CATALOG_ENTRY_HEADER(frameSwitch);    // SoSwitch
    SO_KIT_CATALOG_ENTRY_HEADER(frameSeparator); // SoSeparator
    SO_KIT_CATALOG_ENTRY_HEADER(frameNoLight);   // SoLightModel
    SO_KIT_CATALOG_ENTRY_HEADER(frameShapeHints);// SoShapeHint
    SO_KIT_CATALOG_ENTRY_HEADER(frameFrontSw);   // SoSwitch
    SO_KIT_CATALOG_ENTRY_HEADER(frontDrawStyle); // SoDrawStyle
    SO_KIT_CATALOG_ENTRY_HEADER(frontGeometry);  // SoIndexedFaceSet
    SO_KIT_CATALOG_ENTRY_HEADER(frameBackSw);    // SoSwitch
    SO_KIT_CATALOG_ENTRY_HEADER(backDrawStyle);  // SoDrawStyle
    SO_KIT_CATALOG_ENTRY_HEADER(backGeometry);   // SoIndexedFaceSet
        /// clipping plane components
    SO_KIT_CATALOG_ENTRY_HEADER(clipSwitch);     // SoSwitch
    SO_KIT_CATALOG_ENTRY_HEADER(clipPlaneX0);    // SoClipPlane
    SO_KIT_CATALOG_ENTRY_HEADER(clipPlaneX1);    // SoClipPlane
    SO_KIT_CATALOG_ENTRY_HEADER(clipPlaneY0);    // SoClipPlane
    SO_KIT_CATALOG_ENTRY_HEADER(clipPlaneY1);    // SoClipPlane
    SO_KIT_CATALOG_ENTRY_HEADER(clipPlaneZ0);    // SoClipPlane
    SO_KIT_CATALOG_ENTRY_HEADER(clipPlaneZ1);    // SoClipPlane
        /// client area components
    SO_KIT_CATALOG_ENTRY_HEADER(clientSwitch);   // SoSwitch
    SO_KIT_CATALOG_ENTRY_HEADER(clientCenter);   // SoTranslation
    SO_KIT_CATALOG_ENTRY_HEADER(clientVolume);   // SoSeparator

public:
//--enums
    enum State {
        NORMAL    = 0, ///< normal window
        MINIMIZED = 1, ///< minimized window (invisible, just icon on pip)
        MAXIMIZED = 2  ///< maximized window (no border, frame or clip)
    };

    enum Style {
        BORDER                = 0x0001, ///< switch frame lines in-/visible
        CLIENTVOLUMEVISIBLE   = 0x0002, ///< switch client vol. in-/visible
        TITLE                 = 0x0004, ///< switch off title (not implemented)
        ROTATEABLE            = 0x0008, ///< user can translate window
        TRANSLATEABLE         = 0x0010, ///< user can rotate window
        MOVEABLE              = 0x0018, ///< user can translate+rotate window
        RESIZEABLE            = 0x0020, ///< user can resize window
        CLIPPING              = 0x0040, ///< clipping on
        VISIBLE               = 0x0080, ///< window visible

        DEFAULT = BORDER | CLIENTVOLUMEVISIBLE | TITLE |
                  MOVEABLE | RESIZEABLE | CLIPPING | VISIBLE
    };

    /** FocusPolicy
     *  values for focus policy of windows
     *  @li CLICK_TO_FOCUS user have to click into a window to get focus
     *  @li MOVE_TO_FOCUS user have to move the pen into a window to get focus
     */
    enum FocusPolicy {
        CLICK_TO_FOCUS,
        MOVE_TO_FOCUS
    };

    /** AppearanceDetail
     *  values for appearance details of windows
     *  the appearance will be set with the setappearance method, the values
     *  can be combined using the bitwise or.
     *  @li the default appearance is back walls are drawn wireframe and
     *  front walls are invisible
     *  DEFAULT_WALL                shows back walls wireframed
     *  @li backwalls can be drawn either solid or wireframe for focused/
     *  unfocused window
     *  BACK_WALL_SOLID_FOCUSED       focused window draws back walls solid
     *  BACK_WALL_SOLID_UNFOCUSED   unfocused window draws back walls solid
     *  BACK_WALL_SOLID             everytime draw back walls solid
     *  @li with frontwalls visible the front walls will be drawn wireframe
     *  either for focused/unfocused window or both
     *  FRONT_WALL_FOCUSED            focused window draws front walls visible
     *  FRONT_WALL_UNFOCUSED        unfocused window draws front walls visible
     *  FRONT_WALL_VISIBLE          front walls always visible
     */
    enum AppearanceDetail {
        DEFAULT_WALL              = 0x00,
        /// back walls can be drawn either solid or wireframe for focused/
        /// unfocused window
        BACK_WALL_SOLID_FOCUSED   = 0x01,///focused window back walls solid
        BACK_WALL_SOLID_UNFOCUSED = 0x02,///unfocused window back walls solid
        BACK_WALL_SOLID           =      ///everytime draw back walls solid
            BACK_WALL_SOLID_FOCUSED | BACK_WALL_SOLID_UNFOCUSED,
        ///  with front walls visible the front walls will be drawn wireframe
        ///  either for focused/unfocused window or both
        FRONT_WALL_FOCUSED     = 0x04, ///focused window front walls visible
        FRONT_WALL_UNFOCUSED   = 0x08, ///unfocused window front walls visible
        FRONT_WALL             =       ///front walls always visible
            FRONT_WALL_FOCUSED | FRONT_WALL_UNFOCUSED
    };

//--fields:
    SoSFVec3f    position;      ///< position of window
    SoSFRotation orientation;   ///< orientation of window
    SoSFVec3f    center;        ///< center point of client volume
                                ///  (default 0.5,0.5,0.5) range 0..1
    SoSFFloat    thickness;     ///< thickness of window borders
    SoSFVec3f    size;          ///< size of window
    SoSFVec3f    minSize;       ///< minimum size of window
    SoSFEnum     state;         ///< state of window (max,min,norm)
    SoSFBitMask  style;         ///< behavior and visibility of window
    SoSFString   title;         ///< title bar (not supported)

    SoSFBitMask  focusPolicy;   /// focus policy of window (@see FocusPolicy)
    SoSFBitMask  appearance;    /// window's appearance (@see AppearanceDetail)

/// default constructor
    SoWindowKit();

/// destructor
    virtual ~SoWindowKit();

/// static initialization function - call before creating first instance!
    static void initClass();

/** evaluate window's interest in given 3D event
  * @param h3a - event action containing 3d event in question
  * @return true if window is interested */
    virtual SbBool isInterested(SoHandle3DEventAction* h3a);

/** process 3d event
  * @param h3a - event action containing 3d event to be processed */
    virtual void handle3DEvent(SoHandle3DEventAction* h3a);

/** set application callback routine and data pointer
  * @param callback - application's method to call when an event should be
  *     passed to the application
  * @param data - data that will be passed to the application's callback
  *     routine */
    void setApplicationCallback(WindowEventCB *callback, void *data);

/** get separator of client volume */
    SoSeparator *getClientVolume(void);

protected:

   /** This will detach/attach the fieldSensor.
     * It is called at the end of the constructor (to attach).
     * and at the start/end of SoBaseKit::readInstance()
     * and on the new copy at the start/end of SoBaseKit::copy().
     * Returns the state of the node when this was called.
     */
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

private:
    SoFieldSensor* positionSensor;    ///< sensor for position changes
    SoFieldSensor* orientationSensor; ///< sensor for orientation changes
    SoFieldSensor* centerSensor;      ///< sensor for orientation changes
    SoFieldSensor* thicknessSensor;   ///< sensor for thickness changes
    SoFieldSensor* sizeSensor;        ///< sensor for size changes
    SoFieldSensor* stateSensor;       ///< sensor for state changes
    SoFieldSensor* styleSensor;       ///< sensor for style changes
    SoFieldSensor* focusSensor;       ///< sensor for focus changes

/** callback for position changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to positionSensor */
    static void positionCB(void* data, SoSensor* sensor);

/** callback for orientation changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to orientationSensor */
    static void orientationCB(void* data, SoSensor* sensor);

/** callback for client volume center changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to centerSensor */
    static void centerCB(void* data, SoSensor* sensor);

/** callback for size and thickness changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to sizeSensor or thicknessSensor */
    static void sizeCB(void* data, SoSensor* sensor);

/** callback for state changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to stateSensor */
    static void stateCB(void* data, SoSensor* sensor);

/** callback for style changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to styleSensor */
    static void styleCB(void* data, SoSensor* sensor);

/** callback for focus changes
  * @param data - pointer to SoWindowKit instance
  * @param sensor - pointer to focusSensor */
    static void focusCB(void* data, SoSensor* sensor);

/** set back and front walls depengind whether the window is (un-)focused.
  * @param focused - true if window is focused */
    void setAppearance(bool focused);

/** process state change
  * @param h3a - event action containing 3d event to be processed
  * @return new state value */
    int checkNewState(SoHandle3DEventAction* h3a);

/** process color change for individual user colors
  * @param data - user data for callback (windowkit)
  * @param action - action that triggered callback */
    static void userColorChange(void *data, SoAction *action);

/** set frame component color to user's color or inactive if none user given
  * @param user - user's id */
    void setFrameColor(int user);

/** set highlight colors
  * @param line   - color of lines
  * @param border - color of borders
  * @param corner - color of corners */
    void setColor(unsigned int line, unsigned int border, unsigned int corner);

    int careState;              /// last state with important action
    bool bordersOff;            /// true if size and move borders invisible

    int grabberSet;              //true if grabber is set

    // values to be stored when a dragging action is started with a button
    // press event
    SbVec3f    startSize;           //size of window
    SbVec3f    startPosition;       //position of window (local)
    SbRotation startOrientation;    //orientation of window (local)
    SbVec3f    startTranslationPenG;//pen translation (global)
    SbRotation startRotationPenG;   //pen rotation (global)
    SbMatrix   globalToLocal;       //global to local transformation
    SbVec3f    pickOffset;          //offset to pen's origin (local)

/** isFocused change focus of window, if state has changed and no other
  * window is in focus (exception: maximized windows always loose focus)
  * @param h3a      - action
  * @param input    - input state vector (may be changed)
  * @return true    - if window is focused */
    bool isFocused(SoHandle3DEventAction* h3a, int *input);

/** callCallback call application callback if pen is in client area
  * @param h3a - current event action */
    void callCallback(SoHandle3DEventAction *h3a);

/** hideBorder hide borders
  * @param volume - volume pen is in may switch borders off
  *        (e.g. outside and client volume are shown without borders) */
    void hideBorder(int volume);

/** handleGrabber handle grabber set/release events
  * @param h3a   - event to handle grabber state
  * @param state - grabber's new state */
    void handleGrabber(SoHandle3DEventAction* h3a, int state);

/** changeHighlight highlight the specified volume
  * @param highlight - kind of highlighting
  * @param volume    - volume to highlight */
    void changeHighlight(int highlight, int volume);

/** handle event input (pen's movement and buttons)
  * @param type   - type of event (press, release, motion)
  * @param volume - current volume pen is in relative to window
  *                 (indside, outside, border,..)
  * @param action - 3D action
  * @param event  - 3D event */
    void handleEventInput(int type, int volume,
        SoHandle3DEventAction *action, So3DEvent *event);

/** write back new position and orientation
  * @param translation - new position vector
  * @param rotation    - new rotation vector
  * @param pickoff     - new pickOffset vector */
    void writeBack(const SbVec3f &trans, const SbRotation &rot,
        const SbVec3f &pickoff);

/** update translation and rotation (pen's movement to window's pos and rot)
  * @param transIn  - translation in
  * @param rotIn    - rotation in */
    void update(const SbVec3f &transIn, const SbRotation &rotIn);

    static AutomatonTransition transitions[];
    EventAutomaton eventAutomaton;

/** normalize window
  * turn visibility, frame, border, clipping, client volume switch on,
  * if requested by style */
    inline void normalize(void);

/** maximize window
  * turn frame, border, clipping switch off
  * turn visibility, client volume switch on, if requested by style */
    inline void maximize(void);

/** minimize window
  * turn visibility switch on */
    inline void minimize(void);

/** check if style flag is set
  * @param style - styles (bitset) to be checked
  * @return true - if at least one style bit is set */
    bool hasStyle(int style)
    { return (this->style.getValue() & style) != 0; }

/** set switch node to view all its children */
    void setSwitch(SoSFNode &node)
    { ((SoSwitch *)node.getValue())->whichChild = SO_SWITCH_ALL;  }

/** set switch node to view no child */
    void clearSwitch(SoSFNode &node)
    { ((SoSwitch *)node.getValue())->whichChild = SO_SWITCH_NONE; }

/** get switch node value */
    int getSwitchValue(SoSFNode &node)
    { return ((SoSwitch*)node.getValue())->whichChild.getValue(); }

    /// attributes which are change by the window manager
    SoMFInt32 focus;    ///< individual focus per user
                        ///< if an user(ID) is in the list then this window
                        ///< is focused by the user

/** add the given user to focus list
  * @param userId - user's id */
    void addUser(int userId);

/** remove the given user from focus list
  * @param userId - user's id */
    void removeUser(int userId);

/** get user's color
  * @param userId - user's id */
    const SbColor &getUserColor(int userId);

    // Attributes taken from StbWinManager windowEntry_ struct
    // There is no longer any need for an extra structure to store these
    // attributes.
    // The old windowEntry_ struct already represents an one to one relation
    // of these attributes and the windowKit
    int applicationID;  ///< application's id which is attached to this window
    WindowEventCB *callback;  ///< application callback method pointer
    void *userData;           ///< data passed to callback method
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif


