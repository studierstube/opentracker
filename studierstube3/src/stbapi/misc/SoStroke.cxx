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
/**
 * implementation of SoStroke kit
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoStroke.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <cassert>

#include <Inventor/engines/SoComputeBoundingBox.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>

#include <stbapi/workspace/StbWorkspace.h>

#include "SoStroke.h"

SO_KIT_SOURCE(SoStroke);

void SoStroke::initClass()
{
    SO_KIT_INIT_CLASS(SoStroke, SoBaseKit, "BaseKit");
}

SoStroke::SoStroke()
{
    SO_KIT_CONSTRUCTOR(SoStroke);
    
    isBuiltIn = TRUE;
    
    SO_KIT_ADD_CATALOG_ENTRY(root, SoSeparator, FALSE, this, \x0, FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(transform, SoTransform, FALSE, root, \x0, FALSE );
    SO_KIT_ADD_CATALOG_ENTRY(geometry, SoGroup, TRUE, root, \x0, TRUE );
    
    SO_KIT_INIT_INSTANCE();
    
    SO_KIT_ADD_FIELD( point, (SbVec3f(0,0,0)));
    SO_KIT_DEFINE_ENUM_VALUE( Scale, UNIFORM );
    SO_KIT_DEFINE_ENUM_VALUE( Scale, DIRECTION );
    SO_KIT_SET_SF_ENUM_TYPE( scale, Scale );
    SO_KIT_ADD_FIELD( scale, (UNIFORM));
    SO_KIT_ADD_FIELD( min, (SbVec3f(0,0,0)));
    SO_KIT_ADD_FIELD( max, (SbVec3f(0,0,0)));
    SO_KIT_ADD_FIELD( center, (SbVec3f(0,0,0)));
    
    point.deleteValues(0, -1);

    bboxSensor.setFunction( SoStroke::parameterChanged );
    bboxSensor.setData( this );
    pointSensor.setFunction( SoStroke::parameterChanged );
    pointSensor.setData( this );
    scaleSensor.setFunction( SoStroke::parameterChanged );
    scaleSensor.setData( this );

    computeBB = new SoComputeBoundingBox;
    computeBB->ref();
    
    setUpConnections(TRUE, TRUE);
}

SoStroke::~SoStroke()
{
    setUpConnections( FALSE, TRUE );
    computeBB->unref();
}

SbBool SoStroke::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    // todo take master / slave status into account
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    if (onOff) {
        // We connect AFTER base class.
        SoBaseKit::setUpConnections(onOff, doItAlways);

        computeBB->node.connectFrom( &geometry );
        min.connectFrom( &computeBB->min );
        max.connectFrom( &computeBB->max );
        center.connectFrom( &computeBB->boxCenter );
        bboxSensor.attach(&min);
        pointSensor.attach(&point);
        scaleSensor.attach(&scale);
        SoStroke::parameterChanged( this, NULL );
    }
    else {
        // We disconnect BEFORE base class.                
        bboxSensor.detach();
        pointSensor.detach();
        scaleSensor.detach();
        min.disconnect();
        max.disconnect();
        center.disconnect();        
        computeBB->node.disconnect();

        SoBaseKit::setUpConnections(onOff, doItAlways);        
    }
    return !(connectionsSetUp = onOff);
}

void SoStroke::parameterChanged( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    SoStroke * self = (SoStroke *)data;

    if( self->point.getNum() < 2 )
        return;

    SbVec3f translation(0,0,0);
    SbRotation rotation=SbRotation::identity();
    SbVec3f scale(0, 0, 0);

    if (self->point[1] != self->point[0]){
        // compute scale
        scale = self->point[1] - self->point[0];
        scale[0] = scale.length() / ( self->max.getValue()[0] - self->min.getValue()[0] );
        if( self->scale.getValue() == SoStroke::UNIFORM )
        {
            scale[1] = scale[0];
            scale[2] = scale[0];
        }
        else
        {
            scale[1] = 1;
            scale[2] = 1;
        }

        // compute translation
        translation = (self->point[1] + self->point[0])/2 - self->center.getValue();

        // compute rotation
        SbVec3f x( 1, 0,0 ), y( 0, 1, 0), corr, dest;
        SbVec3f stroke = self->point[1] - self->point[0];
        stroke.normalize();
        rotation = SbRotation( x, stroke );
        if( stroke.cross(y).length() == 0 ) // if the stroke is parallel to the y axis
            corr = x;                       // correct to x axis
        else
            corr = y;                       // otherwise correct to y axis
        rotation.multVec( corr, dest );
        corr = corr - stroke * corr.dot( stroke );
        corr.normalize();
        float angle = acos( dest.dot( corr ));
        if (angle<0) angle=3.1415926f ;
        SbRotation correction = SbRotation( stroke, -angle);
        rotation = rotation * correction;
    }

    // write back !
    SoTransform * transform = SO_GET_ANY_PART(self, "transform", SoTransform );
    assert( transform != NULL );
    GETDIVOBJECTNODE( self );
    if( theDivObject )
        theDivObject->disableNode( transform );
    transform->scaleFactor.setValue( scale );
    transform->translation.setValue( translation );
    transform->rotation.setValue( rotation );
    if( theDivObject )
        theDivObject->enableNode( transform );
}
