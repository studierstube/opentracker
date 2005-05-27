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
/** SoMonoDisplayMode.h contains the definition of class SoMonoDisplayMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna, Anton Fuhrmann
  *
  * $Id: SoLineSequentialDisplayMode.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SoLINESEQUENTIALDISPLAYMODE_H_
#define _SoLINESEQUENTIALDISPLAYMODE_H_

#include "SoDisplayMode.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** 
SoLineSequentialDisplayMode implements a line sequential stereo mode, 
i.e. a stereo pair is encoded into an image with the odd lines representing 
the left image, the even lines the right image. This mode is useful for 
instance for the Virtual I/O i-glasses head mounted display and various 
projectors.
The field useStencil specifys whether stencil-buffering (TRUE) or 
polygon-stippling (FALSE) shall be used to switch between left and right 
buffer.

@warning when useStencil is set to FALSE only scenes exclusively using 
         polygons will be rendered correctly, because polygon-stippling 
         will be used to switch between left and right buffer. 
         Any 2D-elements (e.g. SoText2D) will interfere with this 
         stereo-rendering method and hence produce erratic results.

@author Rainer Splechtna
@ingroup camera
*/
class STBAPI_API SoLineSequentialDisplayMode
:
    public SoDisplayMode
{
    SO_NODE_HEADER(SoLineSequentialDisplayMode);


public:
    /** This field specifys the method for switching between left and right 
        buffer: true = stencil, false = stipple pattern
    */
    SoSFBool useStencil;              

    /** Inventor class initialization */
    static void initClass();

    /** The constructor inits the field of the node */
    SoLineSequentialDisplayMode(      
        );
    /** Calls the inherited method from SoDisplayMode and inits
        some paramters for the stencil-buffering mode.

        @param aViewer viewer calling this method
    */
    virtual void setStereoMode(SoStudierstubeViewer* aViewer);
    /** Switches to left buffer (using stipple mask or stencil pattern)    
    */
    void selectBufferLeft(             
        );
    /** Switches to right buffer (using stipple mask or stencil pattern)    
    */
    void selectBufferRight(            
        );

private:
    ~SoLineSequentialDisplayMode(    
        );
    /** Paints (inits) even/odd mask for stencil */
    void paintStencilMask(             
        );
    /** Stencil parameters */
    short height, width; 
    /** True if stencil pattern is painted */
    SbBool stencilInitialized;  
    /** the bit mask used when stenciling is active */
    unsigned int stencilBitMask;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

//----------------------------------------------------------------------------
#endif//_SOLINESEQUENTIALDISPLAYMODE_H_

