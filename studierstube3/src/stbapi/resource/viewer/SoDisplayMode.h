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
/** SoDisplayMode.h contains the definition of class SoDisplayMode
  *
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoDisplayMode.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SODISPLAYMODE_H_
#define _SODISPLAYMODE_H_

#include <Inventor/nodes/SoSubNode.h>

#include "SoStudierstubeViewer.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** 
SoDisplayMode is the abstract interface class to managing a (stereo) display. 
It encapsulates behavior for switching to and from stereo mode and selecting 
stereo buffers for drawing. All display modes have to be derived form this 
abstract base class. The methods selectBufferLeft() and selectBufferRight() 
contain the core functionality of a SoDisplayMode-class and have to be 
overridden by the subclass,

@author Rainer Splechtna
@ingroup camera
*/
class STBAPI_API SoDisplayMode : public SoNode
{
SO_NODE_HEADER(SoDisplayMode);

protected:
    /** Abstract constructor. */
    SoDisplayMode(                    
        );
    /** Abstract destructor. */
    virtual ~SoDisplayMode(           
        ) {} 

public:
    /** Inventor class initialization */
    static void initClass();

    /** Sets viewer characteristics, which fit for most display modes: 
        aViewer->setBorder(FALSE); 
        aViewer->setAutoClipping(FALSE); 
        aViewer->setTransparencyType(SoGLRenderAction::DELAYED_BLEND); 
        aViewer->setHeadlight(FALSE); 
        aViewer->setStereoOffset(0.0); 
    
        @param aViewer viewer calling this method
    */
    virtual void setStereoMode(SoStudierstubeViewer* aViewer);
    /** This method is only a dummy at the moment, which does nothing. 
    */
    virtual void setMonoMode(        
        );
    /** Switches to left stereo buffer.
        By default these method does nothing.
    */
    virtual void selectBufferLeft(    
        ) {}
    /** Switches to right stereo buffer.
        By default these method does nothing.
    */
    virtual void selectBufferRight(    
        ) {}
    /** Returns true if mode supports stereo.
        @return TRUE by default   
    */
    virtual SbBool isStereoMode(       
        );                             

protected:
    /** Stores a pointer to the viewer, which uses this instance of
        SoDispalyMode.
    */
    SoStudierstubeViewer* viewer;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

//----------------------------------------------------------------------------
#endif//_SODISPLAYMODE_H_

