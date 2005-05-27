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
/** This file contains default values for the Gesture Recognition Toolkit.
  * If you want your own default color, size, or gesture definitions, this
  * is the place to change them.
  * Note that by changing anything here, applications that rely on these
  * defaults may have problems.
  *
  * @author Benjamin A. Schmit
  *
  * $Id: GRT_defs.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _GRT_DEFS_H_
#define _GRT_DEFS_H_

#include <Inventor/SbColor.h>
#include <Inventor/SbLinear.h>

#ifdef WIN32
#pragma warning (disable:4309)
#endif

/* Define GRT_FENCE if you want the sketch to be 3-dimensional.
 * The height of the sketch is 1/2 the height of the SoSketchKit.
 */
// #define GRT_FENCE


/** Size of the default SoSketchKit.
  * This changes the size the SoSketchKit, SoGestureKit, and SoAlphabetKit
  * initially have when the constructor is called without arguments.
  */
static const SbVec3f _grt_size= SbVec3f(16.0f, 12.0f, 1.0f);


/** Default threshold value.
  * Movement is only detected when the distance from the last movement
  * detection to here is less than this.
  */
static const float _grt_threshold= 0.25f;


/// Default surface color.
static const SbColor _grt_surface=   SbColor(0.7f , 0.65f, 0.7f );

/// Default highlight color.
static const SbColor _grt_highlight= SbColor(1.0f , 1.0f , 0.0f );

/// Default sketch color.
static const SbColor _grt_sketch=    SbColor(0.0f , 0.0f , 0.0f );

/// Default text color (SoAlphabetKit).
static const SbColor _grt_text=      SbColor(0.25f, 0.25f, 0.25f);

/// Default zone separator color (SoAlphabetKit).
/// @todo Is this still needed?
static const SbColor _grt_zone=      SbColor(0.0f , 0.0f , 0.0f );


/** The logical size of the SoGestureKit.
  * The SoGestureKit should always have about the same (logical) size.
  * See SoGestureKit::pressImplementation for further details.
  */
static const float _grt_area= 7500.0f;

/** extended characters, not available on all machines.
  * codes less than 0x80 should be available on all systems.
  * codes replaced by ' ' are not supported on a system and filtered out.
  */

#ifdef WIN32
#define DEF_EXT
static const char _grt_ext[24]=
    {
        0xb7, 0x91, 0x92, 0xab, 0xbb, 0xb0, 0x2b, 0x2d, 0xd7, 0xf7,
        0x3d, 0xd8, 0xa7, 0xb5,  ' ', 0xdf, 0xbf, 0xa1, 0xa9,  ' ',
        0xae, 0xa2, 0xa5, 0xa3
    };
#endif

#ifdef mips
#define DEF_EXT
static const char _grt_ext[24]=
    {
        0xb7, 0xb4, 0x27, 0xab, 0xbb, 0xb0, 0x2b, 0x2d, 0xd7, 0xf7,
        0x3d, 0xd8, 0xa7, 0xb5,  ' ', 0xdf, 0xbf, 0xa1, 0xa9,  ' ',
        0xae, 0xa2, 0xa5, 0xa3
    };
#endif


/** Special characters.
  * These should be the same on all systems.
  */
static const char _grt_spc[33]=
    {
        '.', ',','\'', '?', '-', '!', '/', '(', ')', '$',
        '@', '#', '%', '^', '&', '*', '<', '>', '_', '+',
        '=', '|','\\', '{', '}', '[', ']', '~', '`', ';',
        ':', '"','\t'
    };


#endif
