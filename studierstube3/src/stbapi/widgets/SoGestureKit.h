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
/** This file contains the SoGestureKit.
  * See the class description for details.
  *
  * @author Benjamin A. Schmit
  *
  * $Id: SoGestureKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOGESTUREKIT_H_  // include only once
#define _SOGESTUREKIT_H_

#include "SoSketchKit.h"
#include <Inventor/SbString.h>
#include <stbapi/misc/IMGR.h>


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** Use this class to recognize 2d gestures in the studierstube.
  * This class uses the IMGR library written by Oliver Bimber.
  * It can be made to compile without this library
  * (comment out the "#define GRT_IMGR" in misc/GRT_defs.h).
  * Future use of other gesture recognition libraries is also possible through
  * the facade class defined in IMGR.h.
  * 
  * This class provides an interface for (flat) gesture recognition inside the
  * Studierstube environment. It is based on the SoSketchKit, to which it adds
  * a callback that is executed whenever a gesture is recognized.
  * @ingroup interaction
  */
class STBAPI_API SoGestureKit : public SoSketchKit
{

    SO_KIT_HEADER(SoGestureKit);

public:

    /** This is the callback function type.
      * @param userData This passes data to your callback function.
      *        The data usually contains the calling object.
      * @param gesture The ID of the gesture recognized before the callback.
      * @param gestureKit The SoGestureKit that executed the callback.
      */
    typedef void SoGestureKitCB(
        void* userData, int gesture, SoGestureKit* gestureKit);

private:

    /// The function called on gesture recognition.
    SoGestureKitCB* gestureCallback;

    /// Data passed on gesture recognition.
    void* gestureData;                // data for the callback

    /// The IMGR facade object.
    IMGR imgr;

    /// Name of the gesture library in use.
    SbString gestures;

    /// True if the SoGestureKit is currently recording a gesture.
    bool processing;

protected:

    /** Macros used for initialization.
      * Used internally.
      */
    void constSoGestureKit();

    /** This initializes the SoGestureKit.
      * Used internally.
      */
    void initSoGestureKit(float myXSize, float myYSize, float myHeight);

    /** Extension mechanism for subclassing - gesture recognition.
      * @param gesture The ID of the gesture recognized before the callback.
      */
    virtual void gestureImplementation(int gesture);

    /** Extension mechanism for subclassing - gesture start.
      * Used by the SoAlphabetKit's library selection algorithm.
      * @param center True if the gesture started at the center of the
      *               SoGestureKit.
      */
    virtual void startAtCenter(bool center);

    /** Extension of the SoSketchKit.
      * @warning Do not override this function!
      */
    virtual void pressImplementation();

    /** Extension of the SoSketchKit.
      * @warning Do not override this function!
      */
    virtual void moveImplementation();

    /** Extension of the SoSketchKit.
      * @warning Do not override this function!
      */
    virtual void releaseImplementation();

    /** True if the last gesture was a dot (1 sample).
      * Used by the SoAlphabetKit's library selection algorithm.
      */
    bool rcgDot;

public:

    /** Static class initialization.
      * Call this before instantiating an SoGestureKit.
      */
    static void initClass();

    /// Default Constructor.
    SoGestureKit();

    /** Parameterized constructor.
      * @param myXSize X size of the surface.
      * @param myYSize Y size of the surface.
      * @param myHeight Height of the surface.
      */
    SoGestureKit(float myXSize, float myYSize, float myHeight);

    /// Destructor.
    ~SoGestureKit();

    /** Sets a callback on gesture recognition.
      * @param cb The callback to be set.
      * @param userData Data to be passed on to the callback.
      */
    void setGestureCallback(SoGestureKitCB* cb, void* userData);

    /// Removes a callback on gesture recognition.
    void removeGestureCallback();

    /** Sets the gesture library for the SoGestureKit.
      * @param filename The file to load the gesture library from.
      */
    void setGestures(SbString filename);

    /** Returns the gesture library currently used in the SoGestureKit.
      * @return The file name of the gesture library in use.
      */
    SbString getGestures();        // gets the gesture library in use

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
