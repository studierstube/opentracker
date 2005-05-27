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
/** SoDualChannelDisplayMode.h contains the definition of class 
  * SoDualChannelDisplayMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoDualChannelDisplayMode.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SODUALCHANNELDISPLAYMODE_H_
#define _SODUALCHANNELDISPLAYMODE_H_

#include "SoDisplayMode.h"

#include <Inventor/SbLinear.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
SoDualChannelDisplayMode implements a stereo mode using two viewports in one 
window, where each viewport represents a different output channel. The 
viewports are of equal size and, depending on the SplitMode, are placed in 
the left and right half (vertical split) or in the upper and lower half 
(horizontal split) of the window. The first viewport (left or upper half of 
the window) is used as the left buffer, the second viewport (right or lower 
half of the window) as the right buffer.
This mode is useful for large projection screens with stereo 
(e.g. barco doubleprojector with passive polarisation) in combination with 
workstations, which have multiple channel output 
(e.g. SGI Onyx with 8 channels). 
@author Rainer Splechtna
@ingroup camera
*/
class STBAPI_API SoDualChannelDisplayMode
:
    public SoDisplayMode
{
SO_NODE_HEADER(SoDualChannelDisplayMode);

public:
    /** Describes viewport split modes */
    enum SplitMode                    
    { 
        /** vertical viewport split */
        VERTICAL,                    
        /** horizontal viewport split */
        HORIZONTAL                   
    };

    /** Specifys viewport split mode. */
    SoSFEnum splitMode;              
    
    /** Inventor class initialization */
    static void initClass();

    /** The constructor inits the field of the node */
    SoDualChannelDisplayMode(      
        );

    /** Calls the inherited method from SoDisplayMode and calculates 
        the viewports used for the two channels.
        
        @param aViewer viewer calling this method
    */
    virtual void setStereoMode(
        SoStudierstubeViewer* aViewer);
    /** Switches to first viewport. */
    void selectBufferLeft(             
        );
    /** Switches to second viewport. */
    void selectBufferRight(            
        );

private:
    /** Destructor does nothing. */
    ~SoDualChannelDisplayMode(     
        );

    /** Stores the viewportRegion for the first channel */
    SbViewportRegion viewportChannelOne; 
    /** Stores the viewportRegion for the second channel */
    SbViewportRegion viewportChannelTwo;
    /** stores the last viewer size to test for changes in size */
    SbVec2s size;
    /** updates viewports based on size */
    void setupViewports(void);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

//----------------------------------------------------------------------------
#endif//_SODUALCHANNELDISPLAYMODE_H_

