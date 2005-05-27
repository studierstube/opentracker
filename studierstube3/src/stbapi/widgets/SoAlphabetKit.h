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
/** This file contains the SoAlphabetKit.
  * See the class description for details.
  *
  * @author Benjamin A. Schmit
  *
  * $Id: SoAlphabetKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _SOALPHABETKIT_H_  // include only once
#define _SOALPHABETKIT_H_

#include "SoGestureKit.h"

#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFString.h>


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** Use this class to enter texts in Graffiti-like manner into the
  * studierstube.
  *
  * This class is a subclass of the SoGestureKit, to which it adds the
  * Graffiti alphabet. Characters, numbers, and some special characters are
  * supported.
  *
  * The SoAlphabetKit also provides an automatically filled history list, and
  * the possibility to complete a given string against a list of predefined
  * strings as well as the history list.
  * Completion is done by entering the <TAB> special character, and a text
  * callback (and the history update) is done on entering <CR>.
  *
  * The SoAlphabetKit knows several modes that correspond to different gesture
  * libraries. Selection of a library depends on where you start a gesture,
  * what was the last gesture, and the variables of misc/GRT_defs.h (location
  * of the gesture files).

  State information:

The main states are implemented as described in "Pad&Pen Text-Input using
Graffiti" by Oliver Bimber (Fraunhofer IDG Rostock).

State information is kept in the "mode" variable and queried by getMode():
0 - lower alpha (center), numeric (edges), control
1 - upper alpha, control
2 - special
3 - extended

The completion mechanism keeps its state in the variables "completionCount"
(this is the number of the entry last used for completion, or -1 if completion
was not the last action), and "completionChar" (which is the character at
which the text was appended).

  * @ingroup interaction
  */
class STBAPI_API SoAlphabetKit : public SoGestureKit
{

    SO_KIT_HEADER(SoAlphabetKit);

public:

    /** This is the callback function type.
      * @param userData This passes data to your callback function.
      *        The data usually contains the calling object.
      * @param alphabetKit The SoAlphabetKit that executed the callback.
      */
    typedef void SoAlphabetKitCB(void* userData, SoAlphabetKit* alphabetKit);

    /// Contains the string currently in the SoAlphabetKit.
    SoSFString text;

    /// Color of the text string.
    SoSFColor textColor;

    /// Toggles display of the text string.
    SoSFEnum textVisible;

    /// Strings considered for completion.
    SoMFString completion;

    /// The history list, filled automatically.
    SoMFString history;

    /// Toggles completion against history list.
    SoSFBool historyEnabled;

private:

    /// These are the additional parts of the SoAlphabetKit.
    SO_KIT_CATALOG_ENTRY_HEADER(surfaceTexture);
    SO_KIT_CATALOG_ENTRY_HEADER(textSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(textSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(textMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(textTransform);
    SO_KIT_CATALOG_ENTRY_HEADER(text3);

    /// This engine is used for switching functionality on or off.
    SoVisibleSwitchEngine textEng;

    /// The function called on string entry.
    SoAlphabetKitCB* alphabetCallback;

    /// Data passed on string entry.
    void* alphabetData;

    /// The state of the SoAlphabetKit.
    char mode;

    /// The last string used within the internal completion mechanism.
    int completionCount;

    /// The character at which the last internal completion began.
    int completionChar;

    /** Adds a string to the history.
      * Used internally.
      * @param string The string to be added.
      */
    void memorize(SbString string);

    /** Invokes string completion.
      * Used internally.
      */
    void complete();

protected:

    /** Macros used for initialization.
      * Used internally.
      */
    void constSoAlphabetKit();

    /** This initializes the SoAlphabetKit.
      * Used internally.
      */
    void initSoAlphabetKit(float myXSize, float myYSize, float myHeight);

    /// Extension mechanism for subclassing - string entry.
    virtual void alphabetImplementation();

    /** Extension of the SoGestureKit.
      * @warning Do not override this function!
      */
    virtual void gestureImplementation(int gesture);

    /** Extension of the SoGestureKit.
      * @warning Do not override this function!
      */
    virtual void startAtCenter(bool center);

public:

    /** Static class initialization.
      * Call this before instantiating an SoAlphabetKit.
      */
    static void initClass();

    /// Default Constructor.
    SoAlphabetKit();

    /** Parameterized Constructor.
      * @param myXSize X size of the surface.
      * @param myYSize Y size of the surface.
      * @param myHeight Height of the surface.
      */
    SoAlphabetKit(float myXSize, float myYSize, float myHeight);

    /// Destructor.
    ~SoAlphabetKit();

    /** Sets a callback on string entry.
      * @param cb The callback to be set.
      * @param userData Data to be passed on to the callback.
      */
    void setAlphabetCallback(SoAlphabetKitCB* cb, void* userData);

    /// Removes a callback on string entry.
    void removeAlphabetCallback();

    /** Returns the current state of the SoAlphabetKit.
      * @return The state of the SoAlphabetKit.
      */
    char getMode() const;

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
