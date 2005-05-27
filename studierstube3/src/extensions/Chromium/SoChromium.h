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
 * PROJECT: stbapi
 * ======================================================================== */
/** header file for SoChromium extension node
 *
 *
 * @author Matthias Grumet, Peter Rautek, Gerhard Reitmayr
 *
 * $Id: SoChromium.h 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef  _SO_CHROMIUM_
#define  _SO_CHROMIUM_

#include "chromiumext.h"
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFBool.h>

/**
 * This node renders a frame of Chromium OpenGL stream commands for 
 * every render action. It acts as a server in a Chromium stream network.
 * It manipulates the view and model matrices, so that the geometry
 * of the remove application will integrate with the OpenInventor
 * scene graph. It also allows to disable certain OpenGL commands
 * in the stream to allow tweaking of the representation.
 *
 * @author Matthias Grumet, Peter Rautek, Gerhard Reitmayr
 * @ingroup chromium
 */
class CHROMIUM_API SoChromium : public SoNode {
    
    SO_NODE_HEADER(SoChromium);
        
public:
    
	/**  The address and port of the Chromium mothership. The
	 * default value is 'localhost'. */
    SoSFString mothership;
	/// allow depth tests
	SoSFBool depthTest;
	/// allow alpha tests
	SoSFBool alphaTest;
	/// allow stencil tests
	SoSFBool stencilTest;
	/// allow blending
	SoSFBool blend;
	/// allow culling
	SoSFBool cullFace;
	/// allow line smoothing
	SoSFBool lineSmooth;
	/// allow fog
	SoSFBool fog;

    // Constructor
    SoChromium();	

SoINTERNAL public:

	/** initialization method for OpenInventor type system */
    static void		initClass();

protected:

	/** the GLRenderAction callback. It stores the current GL state
	 * and then passes the control to the Chromium server code to 
	 * render a single frame. Afterwards it restores the current GL
	 * state to avoid influences on OpenInventor 
	 */
    virtual void GLRender( SoGLRenderAction * action);    

    /** 
     * the chromium server state for this node
     */
    CRServer * server;

    virtual ~SoChromium();    
};

#endif /* _SO_CHROMIUM_ */