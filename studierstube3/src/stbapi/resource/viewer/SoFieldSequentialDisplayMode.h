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
/** SoFieldSequentialDisplayMode.h contains the definition of class 
  * SoFieldSequentialDisplayMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoFieldSequentialDisplayMode.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOFIELDSEQUENTIALDISPLAYMODE_H_
#define _SOFIELDSEQUENTIALDISPLAYMODE_H_

#include "SoDisplayMode.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
SoFieldSequentialDisplayMode implements a field sequential stereo mode useful 
for instance for LCD shutter glasses or the Sony Glasstron HMD. Normally this 
mode uses quadruple buffers for rendering. If the hardware does not support 
quad-buffering (e.g. O2) there is the possibility to use an alternative 
method called splitscreen rendering (STR_TOP), where the complete screen is 
split horizontally and the frame for the left eye is rendered in the upper 
half of the screen and the frame for the right eye is rendered in the lower 
half of the screen. These two buffers are stretched vertically to the full 
screen height when displayed. Therefore, the pixels are not square anymore.
@author Rainer Splechtna
@ingroup camera
*/
class STBAPI_API SoFieldSequentialDisplayMode
:
    public SoDisplayMode
{
SO_NODE_HEADER(SoFieldSequentialDisplayMode);

public:
    /** Describes stereo harware modes */
    enum StereoMode                    
    { 
        /** full OpenGL quadruple buffers */
        QUAD_BUFFER,                   
        /** O2-style STR_TOP screen split */
        SPLIT_STEREO                   
    };
    
    /** Stereo mode that shall be used */
    SoSFEnum stereoMode;

    /** Inventor class initialization */
    static void initClass();

    /** The constructor inits the fields of the node */
    SoFieldSequentialDisplayMode(     
        );
    /** Calls the inherited method from SoDisplayMode and sets
        a visual supporting quad-buffering for the viewer.

        @param aViewer viewer calling this method
    */
    virtual void setStereoMode(SoStudierstubeViewer* aViewer);
    /** QUAD_BUFFER mode: switches to left back buffer using the method 
                          glDrawBuffer
        SPLIT_STEREO mode: switches to STEREO_BUFFER_LEFT using the method
                           XSGISetStereoBuffer 
    */
    virtual void selectBufferLeft(     
        );
    /** QUAD_BUFFER mode: switches to right back buffer using the method 
                          glDrawBuffer
        SPLIT_STEREO mode: switches to STEREO_BUFFER_RIGHT using the method 
                           XSGISetStereoBuffer 
    */
    virtual void selectBufferRight(    
        );

private:
    /** Destructor does nothing. */
    ~SoFieldSequentialDisplayMode( 
        );
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

//----------------------------------------------------------------------------
#endif//_SOFIELDSEQUENTIALDISPLAYMODE_H_

