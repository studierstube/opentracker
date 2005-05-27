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
/** source file for SoChromium extension node
 *
 * @author Matthias Grumet, Peter Rautek, Gerhard Reitmayr
 *
 * $Id: SoChromium.cxx 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

extern "C" 
{
#include "crserver/server_stb.h"
}

#include "SoChromium.h"
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoCacheElement.h>
#include <assert.h>

SO_NODE_SOURCE(SoChromium);

void SoChromium::initClass()
{
    SO_NODE_INIT_CLASS(SoChromium, SoNode,"Node");
}

SoChromium::SoChromium()
{   
	SO_NODE_CONSTRUCTOR(SoChromium);

	SO_NODE_ADD_FIELD(mothership, ("localhost"));
	SO_NODE_ADD_FIELD(depthTest, (true));
	SO_NODE_ADD_FIELD(alphaTest, (true));
	SO_NODE_ADD_FIELD(stencilTest, (true));
	SO_NODE_ADD_FIELD(blend, (false));
	SO_NODE_ADD_FIELD(cullFace, (true));
	SO_NODE_ADD_FIELD(lineSmooth, (false));
	SO_NODE_ADD_FIELD(fog, (false));

	server = NULL;
}

void SoChromium::GLRender( SoGLRenderAction * action)
{
    // this node will not work well with caches :)
    SoCacheElement::invalidate(action->getState());

	if(NULL == server)
	{
		server = initChromium((char*)mothership.getValue().getString());
        assert(server);
	}

    ChromiumState * state = getChromiumState(server);
	state->depthTest = (GLboolean)depthTest.getValue();
    state->alphaTest = (GLboolean)alphaTest.getValue();
    state->stencilTest = (GLboolean)stencilTest.getValue();
    state->fog = (GLboolean)fog.getValue();
    state->lineSmooth = (GLboolean)lineSmooth.getValue();
    state->blend = (GLboolean)blend.getValue();

    glPushAttrib( GL_ALL_ATTRIB_BITS );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();

    crServerSerializeRemoteStreams();

    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
    glPopAttrib();
}
	
SoChromium::~SoChromium(){
	
}
