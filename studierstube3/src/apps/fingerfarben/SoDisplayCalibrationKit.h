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
  * Defines a node class to perform position and orientation calibration of the virtual camera.
  *
  * @author  Flo Ledermann <flo@subnet.at>
  *
  * $Id: SoDisplayCalibrationKit.h 4057 2004-11-26 10:46:57Z reitmayr $
  * @file
  */
 /* ======================================================================= */

#ifndef _SODISPLAYCALIBRATIONKIT_H_
#define _SODISPLAYCALIBRATIONKIT_H_

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
Node for performing position and orientation calibration of the virtual camera.
This node displays a reference geometry in a constant position in front of the
user, as soon as the specified station fires events. The user should then align the
actual real-world geometry connected to the station (e.g. an ARToolkit marker) with
the virtual reference geometry. When the two gometries (real and virtual) are aligned,
the button is clicked and the stations pose is used to calculate a new camera pose to
match the alignment.
Note that, for control purposes, also an overlayed geometry is shown at the stations
position. For calibrating, the REAL geometry should be matched with the constant-offset
reference geometry. After calibration, the registration of the overlay geometry with
the station should be improved.

File Format / defaults:
@verbatim
    TYPE        NAME                DEFAULT         COMMENT

SoDisplayCalibrationKit {
    SoSFInt32   station             -1              # station number to use for calibration
    SoSFString  otNodeName          ""              # DEF name of the OpenTracker transformation node the offest is written to
                                                    # (should be the transformation of the cameras pose)
    SoSFTime      timeOut             2               # timeout of highlighting from last event received from station

    SoTranslation calibTranslation  Translation {   # (constant) position of the reference geometry. should be in front of the user
                                        translation 0 0 0
                                    }

    SoNode      calibGeometry       Separator {     # reference geometry. The default is designed for use with a 50mm ARToolkit marker.
                                        Material {
                                            diffuseColor 1 1 0
                                            transparency 0.5
                                        }
                                        Cube { width 0.05 height 0.05 depth 0.001 }
                                        Material {
                                            diffuseColor 1 1 0
                                            transparency 0.0
                                        }
                                        Separator {
                                            Translation { translation -0.024 0.024 0.0 }
                                            Cube { width 0.002 height 0.002 depth 0.002 }
                                        }
                                        Rotation { rotation 0 0 1  1.5707963 }
                                        Separator {
                                            Translation { translation -0.024 0.024 0.0 }
                                            Cube { width 0.002 height 0.002 depth 0.002 }
                                        }
                                        Rotation { rotation 0 0 1  1.5707963 }
                                        Separator {
                                            Translation { translation -0.024 0.024 0.0 }
                                            Cube { width 0.002 height 0.002 depth 0.002 }
                                        }
                                        Rotation { rotation 0 0 1  1.5707963 }
                                        Separator {
                                            Translation { translation -0.024 0.024 0.0 }
                                            Cube { width 0.002 height 0.002 depth 0.002 }
                                        }
                                    }
    SoNode      stationGeometry     Group {         # geometry that is overlayed on the station
                                        Material {
                                            diffuseColor 1 0 0
                                            transparency 0.7
                                        }
                                        Separator {
                                            Translation { translation 0.0 0.03 0.0 }
                                            Cone {
                                                bottomRadius 0.003
                                                height 0.015
                                            }
                                        }
                                        Separator {
                                            Translation { translation 0.0 0.024 0.0 }
                                            Cube { width 0.05 height 0.002 depth 0.002 }
                                        }
                                        Rotation { rotation 0 0 1  1.5707963 }
                                        Separator {
                                            Translation { translation 0.0 0.024 0.0 }
                                            Cube { width 0.05 height 0.002 depth 0.002 }
                                        }
                                        Rotation { rotation 0 0 1  1.5707963 }
                                        Separator {
                                            Translation { translation 0.0 0.024 0.0 }
                                            Cube { width 0.05 height 0.002 depth 0.002 }
                                        }
                                        Rotation { rotation 0 0 1  1.5707963 }
                                        Separator {
                                            Translation { translation 0.0 0.024 0.0 }
                                            Cube { width 0.05 height 0.002 depth 0.002 }
                                        }
                                    }
}
@endverbatim

@ingroup fingerfarben
*/
class FINGERFARBEN_API SoDisplayCalibrationKit : public SoBaseKit, public Base3D 
{
    SO_KIT_HEADER(SoDisplayCalibrationKit);

    /// the actual user's content
    SO_KIT_CATALOG_ENTRY_HEADER(baseSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(geomGroup);
    SO_KIT_CATALOG_ENTRY_HEADER(dummyGroup);
    SO_KIT_CATALOG_ENTRY_HEADER(calibSeparator);  
    SO_KIT_CATALOG_ENTRY_HEADER(calibXf);  
    SO_KIT_CATALOG_ENTRY_HEADER(calibTranslation);  
    SO_KIT_CATALOG_ENTRY_HEADER(calibGeometry);  
    SO_KIT_CATALOG_ENTRY_HEADER(stationSeparator);  
    SO_KIT_CATALOG_ENTRY_HEADER(stationMover);  
    SO_KIT_CATALOG_ENTRY_HEADER(stationGeometry);  

    BASE3D_HEADER;

public: // member-variables

    /**
     * The station number to use as calibration reference */
    SoSFInt32 station;

    /**
     * The name of the OpenTracker node to be adjusted.
     * (should be the camera's pose transformation) */
    SoSFString otNodeName;

    /**
     * timeout value for setting the content invisible in seconds. If the
     * timeOut is 0 then no timeout occurs. */
    SoSFTime timeOut;


public:// member-methods
				
    /// static method to initialize class for OpenInventor
    static void initClass();	

    /// constructor
    SoDisplayCalibrationKit();
	/// destructor			
    virtual ~SoDisplayCalibrationKit();

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
    A pointer to the OpenTracker node. */
    StaticTransformation* pOtOffset;

    SbRotation lastRot;
    SbVec3f lastPos;

    /**
    Active flag, determines if a click should trigger calibration. TRUE if station
    fired last event within timeout. */
    SbBool active;

    /**
     * called when the time out timer fires.
     * @param data pointer to the SoCalibrationKit (self)
     * @param sensor the sensor that called the callback function */
    static void timerFired( void *data, SoSensor *sensor );
			
};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SODISPLAYCALIBRATIONKIT_H_
