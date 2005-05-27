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
  * $Id: SoSketchKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOSKETCHKIT_H_
#define _SOSKETCHKIT_H_

/** State Information:

The SoSketchKit knows a number of states which are listed below:
+-------+----------+------------------+--------------------------------------+
| State | Name     | Flags            | Description                          |
+-------+----------+------------------+--------------------------------------+
|     0 | inactive | !inside, !active | mouse outside SoSketchKit, no action |
|     1 | sleeping | inside, !active  | mouse inside, no button pressed      |
|     2 | drawing  | inside, active   | mouse inside, button 1 down, drawing |
|     3 | waiting  | !inside, active  | mouse outside, but still drawing     |
+-------+----------+------------------+--------------------------------------+
*/

/** Event Information:

State sequences and their triggering actions are listed below:
If nothing is set, it means no transition, should not happen.
The sign '+' means grab events, '-' means release.
+-------------------------+----+----+----+----+
| \/ Action   old State > | 0  | 1  | 2  | 3  |
+-------------------------+----+----+----+----+
| mouse move in           | 1+ |    |    | 2  |
| mouse move out          |    | 0- | 3  |    |
| button 1 press          |    | 2  |    |    |
| button 1 release        |    |    | 1  | 0- |
+-------------------------+----+----+----+----+
*/

#include <Inventor/SbLinear.h>
#include <Inventor/SoPath.h>
#include <Inventor/actions/SoAction.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoSubKit.h>
#include <stbapi/event3d/Base3D.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>
#include <stbapi/misc/SoVisibleSwitchEngine.h>


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** This is the SoSketchKit.
  * It is a widget that can be used for a 3D user interface.
  * 
  * The SoSketchKit can be used for drawing or for entering a sequence of
  * points.
  * There are callbacks for the start of a line, adding a new point to a line,
  * and finishing a line.
  *
  * The SoSketchKit can also be used as a base class for other classes that
  * operate on drawings on flat surfaces.
  * One of these classes is the SoGestureKit, which is able to recognize
  * gestures out of the sketches.
  *
  * The SoSketchKit has various parts (see below), some of them can be
  * switched off in order to implement further functionality.
  * It is e.g. possible to switch off display of the sketch and use some other
  * widget for this.
 * @ingroup oldwidgets
  */
class STBAPI_API SoSketchKit : public SoBaseKit, public Base3D
{

    SO_KIT_HEADER(SoSketchKit);
    BASE3D_HEADER;

public:

    /** This is the callback function type.
      * @param userData This passes data to your callback function.
      *        The data usually contains the calling object.
      * @param sketchKit The SoSketchKit that executed the callback.
      */
    typedef void SoSketchKitCB(void* userData, SoSketchKit* sketchKit);

    /// See SoVisibleSwitchEngine for this.
    enum Visible { HIDDEN= 0, VISIBLE };

    /// Contains the points of the sketch.
    SoMFVec3f point;

    /// Contains the line endpoints of the sketch.
    SoMFInt32 numVertices;

    /// The size of the SoSketchKit.
    SoSFFloat xSize, ySize, height;

    /// Threshold used for movement detection on the surface.
    SoSFFloat threshold;

    /// Colors of the SoSketchKit.
    SoSFColor surfaceColor, highlightColor, sketchColor;

    /// Show or hide parts of the SoSketchKit.
    SoSFEnum surfaceVisible, sketchVisible, cursorVisible;

    /// True if the pen is inside the SoSketchKit.
    SoSFBool inside;

    /// If false, a drawing is deleted when the next one is started.
    SoSFBool multiple;

private:

    /// These are the parts of the SoSketchKit.
    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(surfaceSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(surfaceSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(surfaceMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(surface);
    SO_KIT_CATALOG_ENTRY_HEADER(sketchSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(sketchMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(sketchCoordinates);
    SO_KIT_CATALOG_ENTRY_HEADER(sketch);
    SO_KIT_CATALOG_ENTRY_HEADER(cursorSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(cursorSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(cursorTranslation);
    SO_KIT_CATALOG_ENTRY_HEADER(cursor);

    /// These engines are used for switching functionality on or off.
    SoVisibleSwitchEngine surfaceEng, sketchEng, cursorEng;

    /// The function called on key press.
    SoSketchKitCB* pressCallback;

    /// The function called on movement on the SoSketchKit's surface.
    SoSketchKitCB* moveCallback;

    /// The function called on key release.
    SoSketchKitCB* releaseCallback;

    /// Data passed on key press.
    void* pressData;

    /// Data passed on movement.
    void* moveData;

    /// Data passed on key release.
    void* releaseData;

    /// True if someone currently draws a sketch.
    bool active;

    /// Reference to the root of the scene graph - needed by other functions.
    SoPath* root;

protected:

    /** Macros used for initialization.
      * Used internally.
      */
    void constSoSketchKit();

    /** This initializes the SoSketchKit.
      * Used internally.
      */
    void initSoSketchKit(float myXSize, float myYSize, float myHeight);

    /// Extension mechanism for subclassing - key press.
    virtual void pressImplementation();

    /// Extension mechanism for subclassing - movement.
    virtual void moveImplementation();

    /// Extension mechanism for subclassing - key release.
    virtual void releaseImplementation();

    /** This returns the local-to-global matrix.
      * @return The matrix.
      */
    SbMatrix getTransformationMatrix();

    /** This starts or continues a line.
      * @param pt The point to be drawn.
      * @param newLine True if a new line should be started.
      */
    void drawPoint(SbVec3f pt, bool newLine);

public:

    /** Static class initialization.
      * Call this before instantiating an SoSketchKit.
      */
    static void initClass();

    /** Default Constructor.
      */
    SoSketchKit();

    /** Parameterized constructor.
      * @param myXSize X size of the surface.
      * @param myYSize Y size of the surface.
      * @param myHeight Height of the surface.
      */
    SoSketchKit(float myXSize, float myYSize, float myHeight);

    /** Destructor.
      */
    ~SoSketchKit();           // destructor

    /** This function clears any drawings.
      */
    void clear();

    /** Intersects a drawing with the surface of the SoSketchKit.
      * Currently, it just flattens the drawing to the surface.
      * @param drawing The drawing to be flattened.
      * @return The flattened drawing.
      */
    SoMFVec3f intersect(SoMFVec3f drawing);

    /** The pen's position on the surface.
      * @return Position in local coordinates.
      */
    SbVec2f posOnSurface();

    /** A point's position in surface coordinates.
      * @param position The point in global coordinates.
      * @return Position of the point in local coordinates.
      */
    SbVec2f posOnSurface(SbVec3f position);

    /** The pen's position in space.
      * @return Position in global coordinates.
      */
    SbVec3f posInWorld();

    /** A surface point's position in world coordinates.
      * @param position The point in local coordinates.
      * @return Position of the point in global coordinates.
      */
    SbVec3f posInWorld(SbVec2f position);

    /** Is a given point on the surface?
      * @param position The point in local coordinates.
      * @return True if inside.
      */
    bool isInside(SbVec2f position);

    /** Is a given point inside the SoSketchKit?
      * @param position The point in global coordinates.
      * @return True if inside.
      */
    bool isInside(SbVec3f position);

    /** Sets a callback on key press (start line).
      * @param cb The callback to be set.
      * @param userData Data to be passed on to the callback.
      */
    void setPressCallback(SoSketchKitCB* cb, void* userData);

    /** Sets a callback on movement (continue line).
      * @param cb The callback to be set.
      * @param userData Data to be passed on to the callback.
      */
    void setMoveCallback(SoSketchKitCB* cb, void* userData);

    /** Sets a callback on key release (finish line).
      * @param cb The callback to be set.
      * @param userData Data to be passed on to the callback.
      */
    void setReleaseCallback(SoSketchKitCB* cb, void* userData);

    /// Removes a callback on key press (start line).
    void removePressCallback();

    /// Removes a callback on movement (continue line).
    void removeMoveCallback();

    /// Removes a callback on key release (finish line).
    void removeReleaseCallback();

    /** This does 3D event handling.
      * @param h3a The action containing the event.
      */
    void handle3DEvent(SoHandle3DEventAction* h3a);

    /** Tells if we are interested in an event.
      * @param h3a The action containing the event.
      */
    SbBool isInterested(SoHandle3DEventAction* h3a);

    /** This does general event handling.
      * @param action The action containing the event.
      */
    void doAction(SoAction* action);

//------------------------- added by hpi

	 enum { INACTIVE=0, SLEEPING, DRAWING, WAITING } m_State;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
