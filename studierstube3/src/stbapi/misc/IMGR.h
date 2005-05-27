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
/** This file contains a facade class for the IMGR library.
  * It hides internals like extensive use of templates from the user.
  *
  * @author Benjamin A. Schmit
  *
  * $Id: IMGR.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _IMGR_H_
#define _IMGR_H_

#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** This structure contains the IMGR state variables.
  * The contents of the structure are not visible to other classes.
  * The structure itself is defined in the .cxx-File.
  */
struct STBAPI_API IMGR_var;


/** The IMGR class builds a facade for the IMGR library by Oliver Bimber.
  * It encapsulates the complex template and inline usage within the library
  * in a .cxx-File so that it will be easier to compile and link your own
  * applications to it.
  * 
  * The interface of the IMGR class is kept simple - Only functionality needed
  * by the Gesture Recognition Toolkit classes is visible. See the member
  * function descriptions below for further details.
  * 
  * Usage:
  * Create a new IMGR object and store it in a variable. This variable will
  * contain all the IMGR state.
  * Use libNew or libLoad to initialize the object.
  * Use gstNew and gstCont to store the samples of your gesture.
  * Use gstRecog or gstLearn to recognize a gesture or to train a gesture.
  * Use gstSave to store your library to a file.
  * @ingroup interaction
  */
class STBAPI_API IMGR
{
public:

    /// Constructor.
    IMGR();

    /// Destructor.
    ~IMGR();

    /** Resets the gesture library.
      * @warning In the current version of the IMGR library, this function
      *          is not implemented!
      */
    void libNew();  // WARNING: in this version of IMGR not implemented!!!

    /** Loads a gesture library file.
      * @param lib The gesture library file to be loaded.
      * @return True if everything went OK.
      */
    bool libLoad(const char* lib);

    /** Saves the current recognition data into a gesture library file.
      * @param lib The gesture library file to be written.
      * @return True if everything went OK.
      */
    bool libSave(const char* lib);

    /** Starts recognition of a new gesture.
      * @param x X coordinate of the starting point.
      * @param y Y coordinate of the starting point.
      * @param factor Virtual size factor to be used for this gesture.
      *               This can be used to adjust the recognition area size to
      *               the size of the original training data.
      *               See SoGestureKit::pressImplementation for details.
      */
    void gstNew(float x, float y, float factor);

    /** Continues a gesture with a new sample.
      * @param x X coordinate of the next sample point.
      * @param y Y coordinate of the next sample point.
      */
    void gstCont(float x, float y);

    /** Returns the number of samples used in the current gesture.
      * @return The number of samples.
      */
    int gstSamp();

    /** Recognizes the current gesture using the current gesture library.
      * @return The ID of the gesture in the library.
      */
    int gstRecog();
    
    /** Adds the current gesture to the current gesture library.
      * @param id The ID the gesture should be added with.
      */
    void gstLearn(int id);

private:

    /// Variables used by the IMGR library.
    IMGR_var* var;

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
