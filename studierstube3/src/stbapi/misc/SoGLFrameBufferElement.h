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
 * header for frame buffer element
 *
 * @author Gerhard Reitmayr
 *
 * $Id: SoGLFrameBufferElement.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOGLFRAMEBUFFERELEMENT_H_
#define _SOGLFRAMEBUFFERELEMENT_H_

#include <Inventor/elements/SoElement.h>
#include <Inventor/elements/SoSubElement.h>

#include <stbapi/stbapi.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SoWinLeaveScope.h>
#endif // win

/**
 * This element influences the writing of pixels to the frame buffer. It 
 * uses to glColorMask to enable or disable writing to certain components.
 * @author Gerhard Reitmayr
 * @ingroup camera
 */
class STBAPI_API SoGLFrameBufferElement : public SoElement {
    typedef SoElement inherited;
    
    SO_ELEMENT_HEADER(SoGLFrameBufferElement);
public:
    static void initClass(void);
protected:
    virtual ~SoGLFrameBufferElement();
    
public:
    
    enum Components {
        NONE = 0,
        RED = 1,
        GREEN = 2,
        BLUE = 4,
        ALPHA = 8,
        ALL = 1 | 2 | 4 | 8,
    };
    
    static void set(SoState * state, const Components mask );
    virtual void init(SoState * state);
    virtual void push(SoState * state);
    virtual void pop(SoState * state, const SoElement * prevTopElement);
    
    virtual SbBool matches(const SoElement * element) const;
    virtual SoElement * copyMatchInfo(void) const;
    
private:
    Components mask;
    void updategl(void) const;    
};

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <SoWinEnterScope.h>
#endif

#endif // !_SOGLFRAMEBUFFERELEMENT_H_
