/* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
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
  * PROJECT: Studierstube
  * ======================================================================== */ 

 /* ======================================================================= */

/** 
  * Defines a node for performing position calibration of tracker stations.
  *
  * @author  Flo Ledermann <flo@subnet.at>
  * 
  * $Id: SoCalibrationKit.h 4057 2004-11-26 10:46:57Z reitmayr $
  * @file                                                                   
  */
 /* ======================================================================= */

#ifndef _SOCALIBRATIONKIT_H_
#define _SOCALIBRATIONKIT_H_

#include <stbapi/event3d/Base3D.h>

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <common/StaticTransformation.h>

using namespace ot;

#include "fingerfarben.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
Node for performing position calibration of tracker stations. If the station is
within the eventBounds of the Calibration kit and the first button of the
station is pressed, the offset between the stations position and the
CalibrationKit's current position is written to the specified OpenTracker node.
Note that you can put the CalibrationKit into a TrackedArtifactKit to
calculate the offset between two stations (e.g. using a reference marker
to calibrate a glove's "hot spot").

File Format / defaults:
@verbatim
    TYPE        NAME                DEFAULT         COMMENT

SoCalibrationKit {
    SoSFInt32   station             -1              # station number to calibrate
    SoSFString  otNodeName          ""              # DEF name of the OpenTracker transformation node the offest is written to
    SbTime      timeOut             2               # timeout of highlighting if station is within event bounds
    SoNode      geometry            Group {         # geometry that is shown all the time
                                        Material { diffuseColor 1 0 0 }
                                        Cube { width 0.01 height 0.01 depth 0.001 }
                                    }
    SoNode      highlightGeometry   Group {         # geometry that is shown if station is within bounds
                                        Material {
                                            diffuseColor 1 1 0
                                            transparency 0.8
                                        }
                                        Cube { width 0.2 height 0.2 depth 0.2 }
                                    }
    SoShape     eventBounds         Cube {          # bounds to capture events of station
                                        width 0.2 height 0.2 depth 0.2 
                                    }
}
@endverbatim

@ingroup fingerfarben
*/
class FINGERFARBEN_API SoCalibrationKit : public SoBaseKit, public Base3D 
{
    SO_KIT_HEADER(SoCalibrationKit);

     ///(hidden) root separator
    SO_KIT_CATALOG_ENTRY_HEADER(base);
    /// the actual user's content
    SO_KIT_CATALOG_ENTRY_HEADER(geometry);  
    SO_KIT_CATALOG_ENTRY_HEADER(highlightSwitch);  
    SO_KIT_CATALOG_ENTRY_HEADER(highlightGeometry);  
    SO_KIT_CATALOG_ENTRY_HEADER(boundsSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(nullDrawStyle);
    SO_KIT_CATALOG_ENTRY_HEADER(eventBounds);

    BASE3D_HEADER;

public: // member-variables

    SoSFInt32 station;
    SoSFString otNodeName;
    /**
     * timeout value for setting the content invisible in seconds. If the
     * timeOut is 0 then no timeout occurs. */
    SbTime timeOut;

public:// member-methods
				
    /// static method to initialize class for OpenInventor
    static void initClass();	

    /// constructor
    SoCalibrationKit();
	/// destructor			
    virtual ~SoCalibrationKit();

    /**
     * Checks whether the node is interested in a certain 3DEventAction.
     * @param h3a pointer to the 3DEventAction.
     * @return whether the event is significant to us.
     */
	virtual SbBool isInterested(SoHandle3DEventAction *h3a);

    /** 
     * Handles the 3D event. 
     * @param h3a pointer to the 3DEventAction.
     */
	virtual void handle3DEvent(SoHandle3DEventAction *h3a);

    /** 
     * This will detach/attach the connections to opentracker etc.
     * It is called at the end of the constructor (to attach).
     * and at the start/end of SoBaseKit::readInstance()
     * and on the new copy at the start/end of SoBaseKit::copy().
     * Returns the state of the node when this was called.
     */
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

protected: // member-variables

    /**
     * AlarmSensor to signal the time out. It triggers the
     * timerFired callback with a pointer to the SoStationKit. */
    SoAlarmSensor timer;

    /**
     * A pointer to the OpenTracker node that will contain the calibration
     * offset. */
    StaticTransformation* pOtOffset;
        
    /**
     * Called when the time out timer fires.
     * @param data pointer to the SoCalibrationKit (self)
     * @param sensor the sensor that called the callback function */
    static void timerFired( void *data, SoSensor *sensor );


			
};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOFINGERFARBENAPPKIT_H_
