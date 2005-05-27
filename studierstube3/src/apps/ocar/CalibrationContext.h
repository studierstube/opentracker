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
* header for calibration application context
*
* @author Gerhard Reitmayr
*
* $Id: CalibrationContext.h 3426 2004-07-14 12:11:27Z tamer $
* @file                                                                   */
/* ======================================================================= */

#ifndef _CALIBRATIONCONTEXT_H_
#define _CALIBRATIONCONTEXT_H_

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/SbTime.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodekits/SoBaseKit.h>

#include "ocar.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**	
 * This class allows calibration of the users global position and orientation.
 * @author Gerhard Reitmayr
 * @ingroup ocar
 */
class OCAR_API CalibrationContext : public SoBaseKit  
{
    
    SO_KIT_HEADER(CalibrationContext);
    SO_KIT_CATALOG_ENTRY_HEADER(visible);
    SO_KIT_CATALOG_ENTRY_HEADER(target);
    
public:
    static void initClass(void);

	CalibrationContext();

    /** @name Configuration data fields
     *  hold configuration data that is not expected to change at runtime. 
     * In a future distributed model with database servers, these could be 
     * the interfaces to update the clients.
     */
    /*@{*/
	SoSFVec3f calibrationPosition;

	SoSFVec3f calibrationDirection;

    SoSFFloat scale;
    /*@}*/
        
    /** @name Input data fields
     * These will only be read by the
     * navigation application and trigger updates in the state.
     */
    /*@{*/

    SoSFVec3f userPosition;

    SoSFRotation userOrientation;

    SoSFVec3f pointer;

    SoSFBool pointerButton;
    
    SoSFBool absoluteButton;

    SoSFBool orientationButton;

    SoSFBool positionButton;
    /*@}*/
        
    /** @name Output data fields
     * These will only be written to by the
     * navigation application. Writing to these fields does not do anything.
     */
    /*@{*/

    SoSFVec3f positionOffset;

    SoSFRotation orientationY;

    SoSFRotation orientationX;

#undef ABSOLUTE
    enum StateEnum { NONE = 0, ABSOLUTE, POSITION, ORIENTATION, CAMERA };

    SoSFEnum state;
    /*@}*/

protected:

    virtual ~CalibrationContext();
    
    /** This will detach/attach the fieldSensor.
     * It is called at the end of the constructor (to attach).
     * and at the start/end of SoBaseKit::readInstance()
     * and on the new copy at the start/end of SoBaseKit::copy().
     * Returns the state of the node when this was called.
     */
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    void doCalibration( void );
    void doAbsoluteCalibration( void );

    SoFieldSensor absoluteButtonSensor;
    SoFieldSensor orientationButtonSensor;
    SoFieldSensor positionButtonSensor;
    SoFieldSensor pointerSensor;
    SoFieldSensor pointerButtonSensor;

    static void buttonChanged( void * data, SoSensor * sensor );
    static void pointerChanged( void * data, SoSensor * sensor ); 

    SbVec3f startOffset;
    SbVec3f startPointer;
	SbRotation startY;
    SbRotation startX;
    SbTime timeout;
};

#endif // !defined(_CALIBRATIONCONTEXT_H_)
