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
/** SoAnaglyphDisplayMode.h contains the definition of class
  * SoAnaglyphDisplayMode
  *
  * @author Rainer Splechtna
  *
  * $Id: SoAnaglyphDisplayMode.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOANAGLYPHDISPLAYMODE_H_
#define _SOANAGLYPHDISPLAYMODE_H_

#include <Inventor/fields/SoSFColor.h>

#include "SoDisplayMode.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** 
SoAnaglyphDisplayMode implements an anaglyph (red/green, red/blue, ...)
stereo viewing mode. 
To view the stereo effect, use red/green (default) stereo glasses with 
the red lens over the left eye. 
For use with other stereo glasses (e.g. red/blue) the color of the 
left and/or right eye lens must be set accordingly in the field
leftLensColor and/or rightLensColor (e.g. rightLensColor = (0,0,1)).

@author Rainer Splechtna
@ingroup camera
*/
class STBAPI_API SoAnaglyphDisplayMode
:
    public SoDisplayMode
{
    SO_NODE_HEADER(SoAnaglyphDisplayMode);


public:
    /** Inventor class initialization */
    static void initClass();

    /** The constructor inits the field of the node */
    SoAnaglyphDisplayMode(      
        );
    /** Calls the inherited method from SoDisplayMode.

        @param aViewer viewer calling this method
    */
    virtual void setStereoMode(SoStudierstubeViewer* aViewer);
    /** Switches to left buffer (using color mask for left lens)    
    */
    void selectBufferLeft(             
        );
    /** Switches to right buffer (using color mask for right lens)    
    */
    void selectBufferRight(            
        );
    /** Specifys the color of the left eye lens (RGB).
        Default value is (1,0,0) = RED
        @warning all RGB-values < 1 will be interpreted as 0
                 (hence only 0 and 1 are useful values)
    */      
    SoSFColor leftLensColor;
    /** Specifys the color of the right eye lens (RGB).        
        Default value is (0,1,0) = GREEN
        @warning all RGB-values < 1 will be interpreted as 0
                 (hence only 0 and 1 are useful values)                 
    */      
    SoSFColor rightLensColor;

private:
    ~SoAnaglyphDisplayMode(    
        );
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

//----------------------------------------------------------------------------
#endif//_SOANAGLYPHDISPLAYMODE_H_

