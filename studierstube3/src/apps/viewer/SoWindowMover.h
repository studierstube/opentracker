/* ========================================================================
 * Copyright (C) 2000,2001,2002  Vienna University of Technology
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: viewer
 * ======================================================================== */
/**
 * header file for SoWindowMover, a widget like node that grabs and moves 
 * windows around.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoWindowMover.h 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOWINDOWMOVER_H_
#define _SOWINDOWMOVER_H_

#include "viewer.h"
#include <stbapi/tracker/SoStationKit.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>

class SoWindowKit;

/** This class implements a simple 3D user interface to move windows. It
 * is associated with a station that sends 3D events and locks on windows
 * to move these according to the station. There are several assumptions
 * to keep in mind when working with this node.
 * @li it uses only one station and assumes this is configured correctly 
 *     (stationPosition and stationOrientation fields must be set to default
 *      values, or offsets for snap behaviour)
 * @li it assumes the station data and the window location to be in the same
 *     (global) coordinate system. If you put the SoWindowMover node under
 *     some transformations, these will not be taken into account !
 *
 * Generally it operates in two modes, active or passive. This is specified
 * by the boolean field active. If it is active and no window is attached, it
 * checks for any windows close enough to lock onto. It will select the closest
 * that is closer than the value of field threshold. This window is then
 * set in the window field. The field threshold specifies the maximal distance 
 * between the window center and the current mover position for the mover to lock on.
 *
 * If it is passive, it will
 * only use a window that is set explicitly in the window field.
 * If a timeout is specified an occurs, it will reset the window to NULL, if
 * the SoWindowMover is in active mode.
 *
 * There are different behaviours regarding how the window is moved. They are
 * selected using the behaviour field. Currently only the following two are implemented :
 * @li @c PICK the offset between the window origin and the station data is
 *    computed and stored for further station updates. This allows the window to 
 *    attach at its current location and provide seamless movement. This behaviour
 *    overwrites any offsets specified in stationPosition and stationOrientation.
 * @li @c SNAP the window is immediatly snapped to the current station location.
 *    This behaviour uses offsets specified in stationPosition and stationOrientation
 *    to snap the window relative to the station data.
 *
 * You can put any geometry in content and it will behave just like a typical 
 * SoStationKit (useful for debugging and visualising markers etc.). The following example
 * iv code also allows automatic startup of an application, and snapping to the 
 * SoWindowMover :
 * @verbatim
DEF SPRAY SoWindowMover {
    stations [ 5 ]
	stationPosition [ 0 0 0 ]
	stationOrientation [ 0 0 1 0 ]		
	timeOut 0
	
	behaviour   PICK
	active      TRUE
	threshold   0.05        

	content SoSeparator {        
	    SoLinkApp {
		    fileName         "spray_stb.dll"
			workDir          "../apps/spray/"
			keepCurrent      KEEP
            position         = USE SPRAY.position
			trigger			 = USE SPRAY.position
		}	 
    }
}
  @endverbatim
 * 
 *  File syntax:
 *@verbatim
  SoWindowMover {
    behaviour   PICK
    active      FALSE
    threshold   0.05
    application  ""
    window      0 

    # from SoStationKit
    stations []
    content SoGroup {}
  }
  @endverbatim
 *
 * @author Gerhard Reitmayr
 */
class VIEWER_API SoWindowMover : public SoStationKit  
{

    SO_KIT_HEADER(SoWindowMover);

public:
    ///static class initialization
    static void initClass();

	SoWindowMover();

	virtual ~SoWindowMover();

    enum 
    {
        PICK      = 0, ///< the window is smoothly moved with the marker position
        SNAP      = 1  ///< the window snaps to the real marker position
    };
    
    /** specifies how the mover behaves, when it picks up a new window. */
    SoSFEnum behaviour;

    /** if this is true, then the mover picks a window, if it is empty.
     * Otherwise behaves rather passive and waits for it to be set. */
    SoSFBool active;

    /** maximal distance between window center and station for the mover
     * to lock on. */
    SoSFFloat threshold;

    SoSFName application;

    SoSFInt32 window;

protected:
    
    void moveWindow( SoWindowKit * window );

    static void eventCB( void * data, int station, SoStationKit * kit, SoHandle3DEventAction * h3a );

    static void timeCB( void * data, SoStationKit * kit );
};

#endif // _SOWINDOWMOVER_H_
