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
 * source file for SoScreenCapture
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoScreenCapture.cxx 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#include <assert.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SoScreenCapture.h"

#ifdef USE_SOQT
#include <qimage.h>
#endif

SO_NODE_SOURCE(SoScreenCapture);

void SoScreenCapture::initClass()
{
    SO_NODE_INIT_CLASS(SoScreenCapture, SoNode, "Node");
}

SoScreenCapture::SoScreenCapture()
{
    SO_NODE_CONSTRUCTOR(SoScreenCapture);
    
    SO_NODE_ADD_FIELD( trigger, ());
    SO_NODE_ADD_FIELD( screen, (SbVec2s(0,0),0,NULL));
    SO_NODE_ADD_FIELD( baseName, (""));
    SO_NODE_ADD_FIELD( count, (0));
    SO_NODE_ADD_FIELD( writeImage, (TRUE));
    
    triggerSensor.setData( this );
    triggerSensor.setFunction( SoScreenCapture::triggerFired );
    triggerSensor.attach( &trigger );
    imageSensor.setData( this );
    imageSensor.setFunction( SoScreenCapture::imageChanged );
    imageSensor.attach( &screen );    
}

SoScreenCapture::~SoScreenCapture()
{
    triggerSensor.detach();
    imageSensor.detach();
}

void SoScreenCapture::triggerFired( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    SoScreenCapture * self = (SoScreenCapture *)data;

    // capture code !
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );
    if( viewport == NULL )
        return;
    SbVec2s size; 
    int nc;
    unsigned char * imagedata = (unsigned char *)self->screen.getValue( size, nc );
    // do we have to initialize the image ?
    if( size[0] != viewport[2] || size[1] != viewport[3] || nc != 3 )
    {
        size.setValue( viewport[2], viewport[3] );
        imagedata = new unsigned char[ viewport[2]*viewport[3]*4 ];
        
        glReadPixels( 0, 0, viewport[2], viewport[3], GL_RGB, GL_BYTE, imagedata );
        self->screen.setValue( size, 3, imagedata );
        delete imagedata;
    }
    else // or can we reuse it ?
    {
        imagedata = self->screen.startEditing( size, nc );
        glReadPixels( 0, 0, viewport[2], viewport[3], GL_RGB, GL_BYTE, imagedata );
        self->screen.finishEditing();
    }
}

void SoScreenCapture::imageChanged( void * data, SoSensor * sensor )
{
    assert( data != NULL );
    SoScreenCapture * self = (SoScreenCapture *)data;
        
    if( self->writeImage.getValue() == FALSE )
    {
        return;
    }    
    
    SbString filename = self->baseName.getValue();
    filename += SbString( self->count.getValue());
    filename += ".png";
    self->count.setValue( self->count.getValue() + 1 );
    
    SbVec2s size;
    int nc;
    const unsigned char * imagedata = self->screen.getValue( size, nc );

    if( size[0] == 0 || size[1] == 0 || nc != 3 )
    {
        return;
    }

#ifdef USE_SOQT
    QImage image( size[0], size[1], 32 );
    int i,j;
    for( i = 0; i < size[0]; i ++ )
    {
        for( j = 0; j < size[1]; j++ )
        {
            image.setPixel( i,size[1]-j-1, qRgb(imagedata[j*size[0]*3+i*3], imagedata[j*size[0]*3+i*3+1], imagedata[j*size[0]*3+i*3+2] ));
        }
    }
    image.save( filename.getString(), "PNG" );
#endif
}
