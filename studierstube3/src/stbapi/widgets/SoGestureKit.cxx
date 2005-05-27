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
  * $Id: SoGestureKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include "SoGestureKit.h"
#include <stbapi/misc/GRT_defs.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

SO_KIT_SOURCE(SoGestureKit);


/// Macros used for initialization.

void SoGestureKit::constSoGestureKit()
{
    SO_KIT_CONSTRUCTOR(SoGestureKit);
    // no new parts or fields added
    SO_KIT_INIT_INSTANCE();
}


/// This initializes the SoGestureKit.

void SoGestureKit::initSoGestureKit(float myXSize, float myYSize,
    float myHeight)
{
    gestures= "";
    processing= false;
    gestureCallback= NULL;
    gestureData= NULL;
    rcgDot= false;
    imgr.libNew();
}


/// Extension mechanism for subclassing - gesture recognition.

void SoGestureKit::gestureImplementation(int gesture)
{
    // intentionally unused - override to extend
}


/// Extension mechanism for subclassing - gesture start.

void SoGestureKit::startAtCenter(bool center)
{
    // intentionally unused - override to extend
}


/// Extension of the SoSketchKit

void SoGestureKit::pressImplementation()
{
    // start processing:
    processing= true;

    // For optimal recognition, the SoGestureKit should always have (about) the
    // same size: We keep the area constant, the x/y relation should also
    // remain the same.
    float factor= (float) 
        sqrt(_grt_area*xSize.getValue()/ySize.getValue())/ySize.getValue();

    // initialization of IMGR variables and creation of first sample:
    SbVec2f vec= posOnSurface();
    imgr.gstNew(vec[0], vec[1], factor);

    // used for extension:
    startAtCenter((-xSize.getValue()/6 < vec[0]) &&
        (vec[0] < xSize.getValue()/6));
}


/// Extension of the SoSketchKit.

void SoGestureKit::moveImplementation()
{
    // check if still processing:
    if (processing)
    {
        // add a new sample:
        SbVec2f vec= posOnSurface();
        imgr.gstCont(vec[0], vec[1]);
    }
}


/// Extension of the SoSketchKit.

void SoGestureKit::releaseImplementation()
{
    // is everything set up OK?
    if (processing && (gestures != ""))
    {
        // do we have the dot?
        rcgDot= (imgr.gstSamp() == 1);

        // recognize the gesture:
        int gesture= imgr.gstRecog();

        // execute callbacks:
        gestureImplementation(gesture);  // internal callback
        if (gestureCallback != NULL)
            gestureCallback(gestureData, gesture, this);
    }

    // done:
    processing= false;
}


/// Static class initialization.

void SoGestureKit::initClass()
{
    SoSketchKit::initClass();
    SO_KIT_INIT_CLASS(SoGestureKit, SoSketchKit, "SoSketchKit");
}


/// Default Constructor.

SoGestureKit::SoGestureKit()
{
    constSoGestureKit();
    initSoGestureKit(xSize.getValue(), ySize.getValue(), height.getValue());
}


/// Parameterized constructor.

SoGestureKit::SoGestureKit(float myXSize, float myYSize, float myHeight)
    : SoSketchKit(myXSize, myYSize, myHeight)
{
    constSoGestureKit();
    initSoGestureKit(myXSize, myYSize, myHeight);
}


/// Destructor.

SoGestureKit::~SoGestureKit() {}


/// Sets a callback on gesture recognition.

void SoGestureKit::setGestureCallback(
    SoGestureKitCB* cb, void* userData= NULL)
{
    gestureCallback= cb;
    gestureData= userData;
}


/// Removes a callback on gesture recognition.

void SoGestureKit::removeGestureCallback()
{
    gestureCallback= NULL;
    gestureData= NULL;
}


/// Sets the gesture library for the SoGestureKit.

void SoGestureKit::setGestures(SbString filename)
{
    if (imgr.libLoad(filename.getString())) gestures= filename;
    else gestures= "";
}


/// Returns the gesture library currently used in the SoGestureKit.

SbString SoGestureKit::getGestures()
{
    return SbString(gestures);
}
