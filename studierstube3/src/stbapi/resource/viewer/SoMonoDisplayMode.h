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
  * @author Dieter Schmalstieg, Rainer Splechtna
  *
  * $Id: SoMonoDisplayMode.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOMONODISPLAYMODE_H_
#define _SOMONODISPLAYMODE_H_

#include "SoDisplayMode.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoStudierstubeViewer.h"

/**
SoMonoDisplay mode is intended for non-stereo use of Studierstube 
applications. This is particularly useful for presenting monoscopic renderings 
of Studierstube on normal screens - for larger audiences, for documentation 
purposes (screen shots), and for development without need for stereo hardware.

@author Rainer Splechtna
@ingroup camera
*/
class STBAPI_API SoMonoDisplayMode
:
    public SoDisplayMode
{
SO_NODE_HEADER(SoMonoDisplayMode);

private:
    /** Destructor, does nothing */
    ~SoMonoDisplayMode(             
        );

public:
    /** Inventor class initialization */
    static void initClass();

    /** Constructor, inits the node */
    SoMonoDisplayMode(              
        );
    /** Enables viewer decoration and sets viewer characteristics:
        viewer->setDecoration(TRUE);
	    viewer->setPopupMenuEnabled(TRUE);
        viewer->setAutoClipping(FALSE);
        viewer->setTransparencyType(SoGLRenderAction::BLEND);

        @param aViewer viewer calling this method
    */
    void setStereoMode(SoStudierstubeViewer* aViewer);

    /** Returns FALSE, because this mode does not support stereo.
        @return FALSE
    */
    virtual SbBool isStereoMode(      
        );
    /** Dummy */
    virtual void setMonoMode(         
        );

};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

//----------------------------------------------------------------------------
#endif//_SOMONODISPLAYMODE_H_

