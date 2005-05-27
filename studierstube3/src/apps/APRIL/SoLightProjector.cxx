 /* ========================================================================
  * Copyright (C) 2000,2001  Vienna University of Technology
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

#ifdef WIN32
#include <SoWinEnterScope.h>
#include <windows.h>
#endif

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoTextureMatrixElement.h>
#include <Inventor/elements/SoGLTextureMatrixElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoSwitchElement.h>
#include <Inventor/elements/SoGLLazyElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#ifdef __COIN__
#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/elements/SoTransparencyElement.h>
#include <Inventor/elements/SoProjectionMatrixElement.h>
#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <stbapi/misc/SoGLStencilBufferElement.h>
#include <stbapi/misc/SoGLFrameBufferElement.h>
#endif
#include <stbapi/context/SoContextElement.h>

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoLightProjector.h"

SO_NODE_SOURCE(SoLightProjector);

void
SoLightProjector::initClass()
{
    SO_NODE_INIT_CLASS(SoLightProjector, SoNode, "Node");
    SO_ENABLE(SoGLRenderAction, SoGLLazyElement);
    SO_ENABLE(SoCallbackAction, SoLazyElement);
}

SoLightProjector::SoLightProjector()
{
   SO_NODE_CONSTRUCTOR(SoLightProjector);

   SO_NODE_ADD_FIELD(worldProxy, (new SoGroup()));
   SO_NODE_ADD_FIELD(content, (new SoGroup()));
   SO_NODE_ADD_FIELD(lightColor, (1.0,1.0,1.0));
   SO_NODE_ADD_FIELD(lightPosition, (0.0,0.0,0.0));

   children = new SoChildList(this);

   children->append(worldProxy.getValue());
   children->append(content.getValue());

	worldProxySensor.setFunction( SoLightProjector::fieldChangedCB );
	worldProxySensor.setData( this );
    worldProxySensor.attach(&worldProxy);
	contentSensor.setFunction( SoLightProjector::fieldChangedCB );
	contentSensor.setData( this );
    contentSensor.attach(&content);
}

SoLightProjector::~SoLightProjector()
{
}

void
SoLightProjector::GLRender(SoGLRenderAction *action)
{
    SoState* state = action->getState();
	SoCacheElement::invalidate(state);

#ifdef __COIN__

    
    glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
    
    state->push();

    int num = lightPosition.getNum();

    // camera used for texture rendering
    SbViewVolume * vvol = (SbViewVolume*) malloc(num * sizeof(SbViewVolume));

	//set up viewport for texture rendering
	/// @todo FIXME: determine maximum texture size
    SbViewportRegion oldVP = SoViewportRegionElement::get(state);

    SbViewportRegion vp;
    vp.setViewportPixels(0,0, 512, 512);

    SoGetBoundingBoxAction bba(vp);
    bba.apply(worldProxy.getValue());
    SbVec3f center = bba.getCenter();
    float span = (bba.getBoundingBox().getMax() - bba.getBoundingBox().getMin()).length();
    float dist;

    SoViewportRegionElement::set(state, vp);

	// get new texture objects to use for our purposes
	GLuint * texName = (GLuint*) malloc(num * sizeof(GLuint));
	glGenTextures(num, texName);

    GLint currentBuffer[1];
    glGetIntegerv(GL_DRAW_BUFFER, currentBuffer);
    glReadBuffer(currentBuffer[0]);

    // set up stencil buffer. we are only using 1 bit.
    SoGLStencilBufferElement::setMask(state,1);

    //for every light source, render light texture into texture memory:
    int i;
    for (i=0; i<num; i++) {

        dist = (lightPosition[i] - center).length();
        vvol[i].frustum(-span/16, span/16, -span/16, span/16, dist/8, dist*2);
        vvol[i].rotateCamera(SbRotation(SbVec3f(0,0,1),lightPosition[i]));
        vvol[i].translateCamera(lightPosition[i]);

        SbMatrix affine, proj;
        vvol[i].getMatrices(affine, proj);
        SoViewVolumeElement::set(state, this, vvol[i]);
        SoProjectionMatrixElement::set(state, this, proj);
        SoViewingMatrixElement::set(state, this, affine);

	    //clear our buffer
        SoGLFrameBufferElement::set(state, SoGLFrameBufferElement::ALL);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    // render real content into depth buffer only -> correct occlusion of virtual content
        SoGLFrameBufferElement::set(state, SoGLFrameBufferElement::NONE);
	    children->traverse(action,0);   // render world

	    // render virtual content into stencil buffer -> masking out shadowed regions
	    glClearStencil(1);							// set stencil buffer to all ones
	    glClear(GL_STENCIL_BUFFER_BIT);
        SoGLStencilBufferElement::setEnable(state, true);

        SoGLStencilBufferElement::setFunc(state, SoGLStencilBufferElement::ALWAYS, 0, 1);
	    // set to zeroes where virtual content is rendered
        SoGLStencilBufferElement::setOps(state, SoGLStencilBufferElement::KEEP, SoGLStencilBufferElement::KEEP, SoGLStencilBufferElement::REPLACE);

        //SoGLLazyElement::sendAllMaterial(state);
        /*
        float trans[1];
        trans[0] = 0.0f;
        SoTransparencyElement::set(state, this, 1, trans); 
        */
	    children->traverse(action,1);  // render virtual content

	    // render illumination for real content into framebuffer
        SoGLFrameBufferElement::set(state, SoGLFrameBufferElement::ALL);
	    // render plain color
        SoLightModelElement::set(state, this, SoLightModelElement::BASE_COLOR);
        SoDiffuseColorElement::set(state, this, 1, &(lightColor.getValue()));

        SoGLStencilBufferElement::setFunc(state, SoGLStencilBufferElement::EQUAL, 1, 1);
	    // enable stenciling out masked region
        SoGLStencilBufferElement::setOps(state, SoGLStencilBufferElement::KEEP, SoGLStencilBufferElement::KEEP, SoGLStencilBufferElement::KEEP);
	    
        glDisable(GL_DEPTH_TEST);

	    children->traverse(action,0);    // render the real content into framebuffer

        glEnable(GL_DEPTH_TEST);
        SoGLStencilBufferElement::setEnable(state, false);

	    // transfer frame buffer into texture memory
	    glBindTexture(GL_TEXTURE_2D, texName[i]);

	    glCopyTexImage2D(GL_TEXTURE_2D,		// target
					     0,					// level
					     GL_RGB,			// format
					     0,0,				// x,y
					     512,512,			// width, height
					     0);				// border

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
	
	//set projection matrix to projectors model

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// set viewport to fullscreen
    SoViewportRegionElement::set(state, oldVP);

    SoLazyElement::enableBlending(state, GL_ONE, GL_ZERO);  

    for (i=1; i<num; i++) {
	    // set up projective texturing
        SoGLLazyElement::getInstance(state)->send(state, SoLazyElement::GLIMAGE_MASK);
	    glBindTexture(GL_TEXTURE_2D, texName[i]);
        //SoLazyElement::setGLImageId(state, texName[i], true);

	    GLfloat planeS[] =  {1.0, 0.0, 0.0, 0.0};  
        GLfloat planeT[] =  {0.0, 1.0, 0.0, 0.0};  
        GLfloat planeR[] =  {0.0, 0.0, 1.0, 0.0};  
        GLfloat planeQ[] =  {0.0, 0.0, 0.0, 1.0};
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  

	    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	    glTexGenfv(GL_S, GL_EYE_PLANE, planeS);
	    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	    glTexGenfv(GL_T, GL_EYE_PLANE, planeT);
	    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	    glTexGenfv(GL_R, GL_EYE_PLANE, planeR);
	    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        glTexGenfv(GL_Q, GL_EYE_PLANE, planeQ);

	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	    // set texture matrix to viewers'
        SoGLTextureMatrixElement::makeIdentity(state, this);
        SoGLTextureMatrixElement::translateBy(state, this, SbVec3f(0.5, 0.5, 0.0));
        SoGLTextureMatrixElement::scaleBy(state, this, SbVec3f(0.5, 0.5, 1.0));
        SoGLTextureMatrixElement::mult(state, this, vvol[i].getMatrix());
	    
	    // enable texture mapping
	    glEnable(GL_TEXTURE_2D);
	    glEnable(GL_TEXTURE_GEN_S);
	    glEnable(GL_TEXTURE_GEN_T);
	    glEnable(GL_TEXTURE_GEN_R);
	    glEnable(GL_TEXTURE_GEN_Q);

        // render real content into framebuffer using projective texture
	    children->traverse(action,0);

        //switch blending function for remaining children
        if (i==0) SoLazyElement::enableBlending(state, GL_ZERO, GL_SRC_COLOR);
    }

	glDeleteTextures(num, texName);

    free(texName);
    free(vvol);

	// restore original state
    SoLazyElement::disableBlending(state);

    glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);
    
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_LIGHTING);						// reset
    
    state->pop();


#else

	// save current matrices to stack

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

    GLint vpdim[4];
    glGetIntegerv(GL_VIEWPORT, vpdim);

	/// set up optimal frustum for on-axis camera
    SbViewportRegion vp; 
    SoGetBoundingBoxAction bba(vp);
    bba.apply(worldProxy.getValue());
    SbVec3f center = bba.getCenter();
    float span = (bba.getBoundingBox().getMax() - bba.getBoundingBox().getMin()).length();
    float dist = (lightPosition[0] - center).length();

	glLoadIdentity();
	glFrustum(span/3.9, -span/3.9, -span/3.9, span/3.9, dist/2, 3.0);

	// get a new trexture object to use for our purposes
    int num = lightPosition.getNum();
	GLuint * texName = (GLuint*) malloc(num * sizeof(GLuint));
	glGenTextures(num, texName);

	//set up viewport for texture rendering
	/// @todo FIXME: determine maximum texture size
	glViewport(0,0,512,512);
    GLint currentBuffer[1];
    glGetIntegerv(GL_DRAW_BUFFER, currentBuffer);
    glReadBuffer(currentBuffer[0]);

    //for every light source, render light texture into texture memory:
    int i;
    for (i=0; i<num; i++) {

	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();
	    gluLookAt(lightPosition[i][0],lightPosition[i][1],lightPosition[i][2], center[0],center[1],center[2], 0,1,0);

	    //clear our buffer
	    glColorMask(1,1,1,1);
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    // render real content into depth buffer -> correct occlusion of virtual content

	    glColorMask(0,0,0,0);
	    children->traverse(action,0);   // render world

	    // render virtual content into stencil buffer -> masking out shadowed regions

	    glClearStencil(1);							// set stencil buffer to all ones
	    glClear(GL_STENCIL_BUFFER_BIT);

	    glEnable(GL_STENCIL_TEST);
	    glStencilMask(1);

	    glStencilFunc(GL_ALWAYS, 0, 1);
	    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE );	// set to zeroes where virtual content is rendered

	    children->traverse(action,1);  // render virtual content

	    // render illumination for real content into framebuffer

	    glColorMask(1,1,1,1);
	    glDisable(GL_LIGHTING);						// render plain color

	    glColor3fv(lightColor.getValue().getValue());

	    glStencilFunc(GL_EQUAL, 1, 1);
	    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);		// enable stenciling out masked region

	    children->traverse(action,0);    // render the real content into framebuffer

	    glDisable(GL_STENCIL_TEST);

	    // transfer frame buffer into texture memory

	    glBindTexture(GL_TEXTURE_2D, texName[i]);

	    glCopyTexImage2D(GL_TEXTURE_2D,		// target
					     0,					// level
					     GL_RGB,			// format
					     0,0,				// x,y
					     512,512,			// width, height
					     0);				// border

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
	
	//set projection matrix to projectors model

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	// set viewport to fullscreen
    glViewport(vpdim[0], vpdim[1], vpdim[2], vpdim[3]); 

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ZERO);

    for (i=0; i<num; i++) {
	    // set up projective texturing
	    glBindTexture(GL_TEXTURE_2D, texName[i]);

	    GLfloat planeS[] =  {1.0, 0.0, 0.0, 0.0};  
        GLfloat planeT[] =  {0.0, 1.0, 0.0, 0.0};  
        GLfloat planeR[] =  {0.0, 0.0, 1.0, 0.0};  
        GLfloat planeQ[] =  {0.0, 0.0, 0.0, 1.0};
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  

	    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	    glTexGenfv(GL_S, GL_EYE_PLANE, planeS);
	    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	    glTexGenfv(GL_T, GL_EYE_PLANE, planeT);
	    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	    glTexGenfv(GL_R, GL_EYE_PLANE, planeR);
	    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        glTexGenfv(GL_Q, GL_EYE_PLANE, planeQ);

	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	    // set texture matrix to viewers'

	    glMatrixMode(GL_TEXTURE);
	    glLoadIdentity();
	    
	    glTranslatef(0.5, 0.5, 0.0);
	    glScalef(0.5, 0.5, 1.0);

	    glFrustum(span/3.9, -span/3.9, -span/3.9, span/3.9, dist/2, 3.0);
	    gluLookAt(lightPosition[i][0],lightPosition[i][1],lightPosition[i][2], center[0],center[1],center[2], 0,1,0);
	    
	    // enable texture mapping

	    glEnable(GL_TEXTURE_2D);
	    glEnable(GL_TEXTURE_GEN_S);
	    glEnable(GL_TEXTURE_GEN_T);
	    glEnable(GL_TEXTURE_GEN_R);
	    glEnable(GL_TEXTURE_GEN_Q);

        // render real content into framebuffer using projective texture
	    glMatrixMode(GL_MODELVIEW);
	    children->traverse(action,0);
        //switch blending function for remaining children
        if (i==0) glBlendFunc(GL_ZERO, GL_SRC_COLOR);
    }

	// restore original state

    glDisable(GL_BLEND);

    glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);
	glDisable(GL_TEXTURE_GEN_Q);

	glDeleteTextures(num, texName);

	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_LIGHTING);						// reset
#endif

	return;
}

void
SoLightProjector::callback(SoCallbackAction *action)
{
	content.getValue()->callback(action);
}

void
SoLightProjector::doAction(SoAction *action)
{
	content.getValue()->doAction(action);


}


SoChildList* SoLightProjector::getChildren() const {
	return children;
}

void SoLightProjector::fieldChangedCB(void* data, SoSensor* which) {
	((SoLightProjector*)data)->getChildren()->set(0, ((SoLightProjector*)data)->worldProxy.getValue());
	((SoLightProjector*)data)->getChildren()->set(1, ((SoLightProjector*)data)->content.getValue());
}

