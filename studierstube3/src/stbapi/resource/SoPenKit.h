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
/** SoPenKit.h contains the definition of class SoPenKit
  *
  * @author Rainer Splechtna
  *
  * $Id: SoPenKit.h 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   
  */
 /* ======================================================================== */

#ifndef _SOPENKIT_H_
#define _SOPENKIT_H_

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/fields/SoSFEnum.h>

#include "SoTrackedArtifactKit.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

//----------------------------------------------------------------------------

/** SoPenKit is a nodekit representing a pen used for interaction in the
studierstube environment.

This nodekit inherits from SoTrackedArtifact and adds some fields for storing
certain properties of the pen.
@author Rainer Splechtna
@ingroup interaction
@ingroup core
 */
class STBAPI_API SoPenKit : public SoTrackedArtifactKit
{
    SO_KIT_HEADER(SoPenKit);

    /** This field is used for storing a temporary geometry. It is only used
        by the StbWinManager for storing a dragged geometry.
      */
    SO_KIT_CATALOG_ENTRY_HEADER(dragGeometry);   

public:

    /** This enum defines values for the pen's 3D window focussing policy 
      */
    enum FocusPolicy {
        /** just point to window to focus it */
        POINT_TO_FOCUS = 0, 
        /** press primary button when within window to focus it */
        CLICK_TO_FOCUS,
        /** press primary button when within window to get master privileges */
        CLICK_TO_MASTER        
    };

    /** This field is used for storing, which button shall be used as primary
        button
      */
    SoSFInt32 primaryButton;
    /** This field is used for storing, which button shall be used as secondary
        button
      */
    SoSFInt32 secondaryButton;
    /** This field is used for storing, which 3D window focussing policy shall
        be used
      */
    SoSFEnum focusPolicy;

    /** The constructor inits all fields
      */
    SoPenKit(
        );
    /** The destructor does nothing */
    ~SoPenKit(
        );
    /** Inventor class initialization */
    static void initClass(             
        );
    /** Enables the pen to trigger 3D events in the given graph ("root")
        @param root the graph within which the pen tiggers 3D events
      */
    void setEventRoot(SoNode *root); 
    /** Attaches the geometry of the current Drag&Drop to the pen.
        @param dragGeometry graph to store in field dragGeometry
      */
    void setDragGeometry(SoNode *dragGeometry); 
    /** Use this method to clear the dragGeometry field */
    void removeDragGeometry(); 
private:


};

//----------------------------------------------------------------------------
#endif//_SOPENKIT_H_

