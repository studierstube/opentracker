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
* source for calibration application context
*
* @author Gerhard Reitmayr
*
* $Id: CalibrationContext.cxx 3426 2004-07-14 12:11:27Z tamer $
* @file                                                                   */
/* ======================================================================= */

#include <Inventor/nodes/SoSwitch.h>

#include <stbapi/misc/SoFactorRotation.h>

#include "CalibrationContext.h"

const double TIMEOUT_INTERVAL = 1.0;

#include <cassert>

SO_KIT_SOURCE(CalibrationContext);

void CalibrationContext::initClass(void)
{
    SO_KIT_INIT_CLASS(CalibrationContext, SoBaseKit, "BaseKit" );
}

CalibrationContext::CalibrationContext()
{
    SO_KIT_CONSTRUCTOR(CalibrationContext);

    SO_KIT_ADD_CATALOG_ENTRY(visible, SoSwitch, FALSE, this, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(target, SoNode, TRUE, visible, \x0, TRUE );

    isBuiltIn = TRUE;
    
    SO_KIT_INIT_INSTANCE();

	SO_KIT_ADD_FIELD(calibrationPosition, (0,0,0));
    SO_KIT_ADD_FIELD(calibrationDirection, (0,0,0));
	SO_KIT_ADD_FIELD(userPosition, (0,0,0));
    SO_KIT_ADD_FIELD(userPosition, (0,0,0));
    SO_KIT_ADD_FIELD(userOrientation, (SbVec3f(0, 1, 0),0));
    SO_KIT_ADD_FIELD(pointer,(0,0,0));
    SO_KIT_ADD_FIELD(pointerButton, (FALSE));
    SO_KIT_ADD_FIELD(absoluteButton, (FALSE));
    SO_KIT_ADD_FIELD(positionButton, (FALSE));
    SO_KIT_ADD_FIELD(orientationButton, (FALSE));
    SO_KIT_ADD_FIELD(positionOffset, (0,0,0));
    SO_KIT_ADD_FIELD(orientationY, (SbVec3f(0, 1, 0),0));
    SO_KIT_ADD_FIELD(orientationX, (SbVec3f(0, 1, 0),0));    
    SO_KIT_ADD_FIELD(state, (NONE));
    SO_KIT_ADD_FIELD(scale,(10.0));

    SO_KIT_DEFINE_ENUM_VALUE(StateEnum, NONE);   
    SO_KIT_DEFINE_ENUM_VALUE(StateEnum, ABSOLUTE);
    SO_KIT_DEFINE_ENUM_VALUE(StateEnum, POSITION);
    SO_KIT_DEFINE_ENUM_VALUE(StateEnum, ORIENTATION);
    SO_KIT_DEFINE_ENUM_VALUE(StateEnum, CAMERA);
    SO_KIT_SET_SF_ENUM_TYPE( state, StateEnum );

    absoluteButtonSensor.setFunction( CalibrationContext::buttonChanged);
    absoluteButtonSensor.setData( this );
    positionButtonSensor.setFunction( CalibrationContext::buttonChanged);
    positionButtonSensor.setData( this );
    orientationButtonSensor.setFunction( CalibrationContext::buttonChanged);
    orientationButtonSensor.setData( this );
    pointerSensor.setFunction( CalibrationContext::pointerChanged);
    pointerSensor.setData( this );
    pointerButtonSensor.setFunction( CalibrationContext::pointerChanged);
    pointerButtonSensor.setData( this );

    setUpConnections( TRUE, TRUE );
}

CalibrationContext::~CalibrationContext()
{
    
}

SbBool CalibrationContext::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    // todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onOff, doItAlways);

        absoluteButtonSensor.attach(&absoluteButton);
        positionButtonSensor.attach(&positionButton);
        orientationButtonSensor.attach(&orientationButton);
        pointerSensor.attach(&pointer);
        pointerButtonSensor.attach(&pointerButton);

        state.setValue( NONE );
    }
    else {
        // We disconnect BEFORE base class.
        absoluteButtonSensor.detach();
        positionButtonSensor.detach();
        orientationButtonSensor.detach();
        pointerSensor.detach();
        pointerButtonSensor.detach();
        
        SoBaseKit::setUpConnections(onOff, doItAlways);        
    }
    return !(connectionsSetUp = onOff);
}

void CalibrationContext::buttonChanged( void * data, SoSensor * sensor )
{
    assert(data != NULL);
    CalibrationContext * self = (CalibrationContext *)data;
    if( self->state.getValue() != NONE )
        return;
    if( sensor == & self->absoluteButtonSensor && 
        self->absoluteButton.getValue() == FALSE &&
        self->timeout < SbTime::getTimeOfDay())
    {
        SO_GET_ANY_PART(self,"visible",SoSwitch)->whichChild.setValue(-3);
        self->timeout = SbTime::getTimeOfDay() + TIMEOUT_INTERVAL;
        self->state.setValue( CalibrationContext::ABSOLUTE );
    }
    else if( sensor == & self->positionButtonSensor && 
             self->positionButton.getValue() == FALSE &&
             self->timeout < SbTime::getTimeOfDay() )
    {
        self->startOffset = self->positionOffset.getValue();
        self->startPointer = self->pointer.getValue();
        self->timeout = SbTime::getTimeOfDay() + TIMEOUT_INTERVAL;
        self->state.setValue( CalibrationContext::POSITION );        
    } 
    else if( sensor == & self->orientationButtonSensor && 
             self->orientationButton.getValue() == FALSE &&
             self->timeout < SbTime::getTimeOfDay() )
    {
        self->startY = self->orientationY.getValue();
        self->startX = self->orientationX.getValue();
        self->startPointer = self->pointer.getValue();
        self->timeout = SbTime::getTimeOfDay() + TIMEOUT_INTERVAL;        
        self->state.setValue( CalibrationContext::ORIENTATION );        
    }
    // printf("CalibrationContext::buttonChanged state %i\n", self->state.getValue());
}

void CalibrationContext::pointerChanged( void * data, SoSensor * sensor )
{
    assert( data != NULL);
    CalibrationContext * self = (CalibrationContext *)data;
    if( self->state.getValue() == CalibrationContext::NONE || 
        self->state.getValue() == CalibrationContext::CAMERA )
        return;
    if( sensor == & self->pointerSensor )              // 2D pointer position change
    {
        // calculate update based on state value
        self->doCalibration();
    } 
    else if( self->pointerButton.getValue() == TRUE && 
        self->timeout < SbTime::getTimeOfDay() ) // pointer button released after timeout
    {
        if( self->state.getValue() == CalibrationContext::ABSOLUTE )
        {
            self->doAbsoluteCalibration();
        }
        self->state.setValue( CalibrationContext::NONE );
        self->timeout = SbTime::getTimeOfDay() + TIMEOUT_INTERVAL;        
    }
    // printf("CalibrationContext::pointerChanged state %i\n", self->state.getValue());    
}

void CalibrationContext::doCalibration( void )
{
    switch( state.getValue())
    {
    case POSITION:
        {
            SbVec3f offset, result;
            offset[0] = (pointer.getValue()[0] - startPointer[0]) * scale.getValue() * -1.0f;
            offset[1] = 0.0f;
            offset[2] = (pointer.getValue()[1] - startPointer[1]) * scale.getValue() * -1.0f;
            userOrientation.getValue().multVec( offset, result );
            result[1] = 0.0f;
            positionOffset.setValue( result + startOffset );
        }
        break;
    case ORIENTATION:
        {
            // compose from a rotation around y and a rotation around x
            SbRotation yRot(SbVec3f(0,1,0), (pointer.getValue()[0] - startPointer[0])*(float)M_PI);
            SbRotation xRot(SbVec3f(1,0,0), (pointer.getValue()[1] - startPointer[1])*(float)M_PI_2);
            orientationY.setValue( yRot * startY );
            orientationX.setValue( xRot * startX );
		}
        break;
    case ABSOLUTE:
        // don't do anything here
        break;        
    default: 
        assert( FALSE && "undefined switch case in CalibrationContext::doCalibration !");
    }
}

void CalibrationContext::doAbsoluteCalibration( void )
{
    // computing position offset
    SbVec3f offset = calibrationPosition.getValue() - userPosition.getValue();
    positionOffset.setValue( offset );
    
    // computing direction offset
    SbVec3f dir = calibrationDirection.getValue() - calibrationPosition.getValue();
    SbVec3f zm(0,0,-1);

    // calibration direction and its factorization
    SbRotation calibRot( zm, dir ), calibY, calibRem;
    SoFactorRotation::computeFactorization( calibRot, calibY, calibRem, 
                                            SoFactorRotation::Y, SoFactorRotation::PRE );

    // user direction and its factorization
    SbRotation userRot = userOrientation.getValue(), userY, userRem;
    SoFactorRotation::computeFactorization( userRot, userY, userRem,
        SoFactorRotation::Y, SoFactorRotation::PRE );
    
    // computing the two factors of the correction
    SbRotation resultY = calibY * userY.invert();
    SbRotation resultRem = calibRem * userRem.invert();

    orientationY.setValue( resultY );
    orientationX.setValue( resultRem );
}
