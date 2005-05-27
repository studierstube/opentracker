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
 * PROJECT: stbapi
 * ======================================================================== */
/**
 * header for frame buffer node
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoFrameBuffer.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */ 

#ifndef _SOFRAMEBUFFER_H_
#define _SOFRAMEBUFFER_H_

#include <Inventor/fields/SoSFBitMask.h>
#include <Inventor/nodes/SoSubNode.h>
#include <stbapi/misc/SoGLFrameBufferElement.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SoWinLeaveScope.h>
#endif // win

/**
 * This node sets the color mask to enable or disable writing to the frame buffer
 * on a component basis. Set the bitmask field mask to define to which components
 * color values will be written.
 *
 * File format syntax / defaults:
@verbatim
SoFrameBuffer {
    SoSFBitMask mask ALL # NONE | RED | GREEN | BLUE | ALPHA | ALL
}
@endverbatim
 *
 * @author Gerhard Reitmayr
 * @ingroup camera
 */
class STBAPI_API SoFrameBuffer : public SoNode {
    
    SO_NODE_HEADER(SoFrameBuffer);
    
public:
    static void initClass(void);
    SoFrameBuffer(void);
    
    enum Components {
        NONE = SoGLFrameBufferElement::NONE,
            RED = SoGLFrameBufferElement::RED,
            GREEN = SoGLFrameBufferElement::GREEN,
            BLUE = SoGLFrameBufferElement::BLUE,
            ALPHA = SoGLFrameBufferElement::ALPHA,
            ALL = SoGLFrameBufferElement::ALL,
    };
    
    SoSFBitMask mask;
    
    virtual void GLRender(SoGLRenderAction * action);
    
protected:
    virtual ~SoFrameBuffer();
};

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SoWinEnterScope.h>
#endif // win

#endif // !_SOFRAMEBUFFER_H_
