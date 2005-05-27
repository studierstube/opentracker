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
 * PROJECT: ocar
 * ======================================================================== */
/**
 * header for distributed user interfaces
 *
 * @author Gerhard Reitmayr
 *
 * $Id: TourGroup.h 3801 2004-09-30 14:02:02Z reitmayr $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _TOURGROUP_H_
#define _TOURGROUP_H_

#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoMFNode.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/fields/SoMFTime.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/nodes/SoSubNode.h>

#include "ocar.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**	
 * This class manages the user's representation and so in the OCAR setup.
 * @author Gerhard Reitmayr
 * @ingroup ocar
 */
class OCAR_API TourGroup : public SoNode
{
    SO_NODE_HEADER(TourGroup);

public:
    static void initClass(void);

	TourGroup();

public:

    /** @name Configuration data fields
     *  hold configuration data that is not expected to change at runtime. 
     * In a future distributed model with database servers, these could be 
     * the interfaces to update the clients.
     */
    /*@{*/
	SoSFString name;
        
    SoSFNode navigator;

    SoSFTime timeOut;
    /*@}*/
        
    /** @name Input data fields
     * These will only be read by the application and trigger updates in the state.
     */
    /*@{*/

    // should not be distributed, or written out !
    SoMFString neighbor;
    
    // positions of neighbors
    SoMFInt32 neighborStation;
    
    // last message times for timeouts
    SoMFTime neighborStamp;
    
    // last closest waypoint
    SoMFString neighborWaypoint;
    
    // current neighbor destination
    SoMFString neighborDestination;

	SoMFString activeNeighbor;

    SoSFVec3f activePosition;

    SoSFInt32 activeStation;

	enum Mode { OFF = 0, FOLLOW, GUIDE, MEET };
	SoSFInt32 mode;

    // should not be distributed, or written out !
    SoSFString listboxFeedback;

    SoMFString receive;

    SoSFInt32 userStation;

    SoSFVec3f userPosition;

    SoSFString userWaypoint;    

    SoSFString userDestination;

    SoMFString currentPath;

    /*@}*/
        
    /** @name Output data fields
     * These will only be written to by the application. Writing to these fields does not do anything.
     */
    /*@{*/

    SoSFBool waitingForPath;

    SoMFString send;
	/*@}*/

protected:
    virtual ~TourGroup();
    
    
	SoFieldSensor activeNeighborSensor;
    SoFieldSensor modeSensor;
    SoFieldSensor pathSensor;
    SoFieldSensor listboxFeedbackSensor;
    SoFieldSensor receiveSensor;
    SoTimerSensor pingTimer;

	static void activeNeighborChanged( void * data, SoSensor * sensor );
    static void modeChanged( void * data, SoSensor * sensor );
    static void pathChanged( void * data, SoSensor * sensor );
    static void listboxFeedbackChanged( void * data, SoSensor * sensor );
    static void receiveCB( void * data, SoSensor * sensor );
    static void pingCB( void * data, SoSensor * sensor );

    void update(void);

    void receiveData(void);

    /// send msg out by setting and touching the send field
    void sendData( std::vector<SbString> & msg );
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // !defined(_TOURGROUP_H_)
