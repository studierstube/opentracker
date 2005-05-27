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
//  PROJECT:    Studierstube
//  CONTENT:    Class definition of So3DSeparator
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    az      Andreas Zajic
//              mk      Markus Krutz
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:02:56  gh      last modification
// ===========================================================================

#ifndef _SO3DSEPARATOR_H_
#define _SO3DSEPARATOR_H_

#include <Inventor/nodes/SoSeparator.h>
#include <stbapi/event3d/Base3D.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/**
 * 3d separator blocks traversal for certain 3d events. Documented at
 * http://www.studierstube.org/stbapi/So3DSeparator.html
 * This node is a regular separator node, but is also is derived from the 
 * Base3D class and is therefore 3D event aware. By setting its station and 
 * event mask to specific values, only selected events are passed to the 
 * node's children. In particular, choosing a value of BM_NO_EVENT as a value
 * for eventMask, means that the SoHandle3DEventAction will not traverse the
 * node's children. Use this for complex subgraphs that do not contain any 3D
 * aware nodes, to speed up processing. 
 * One can also use it to filter events based on a bounding box check of its children.
 * If boundingBoxCheck is TRUE (the default value), it will only hand down events that
 * are within the bounding box of its children. If FALSE it will hand down all events.
 * @ingroup event
 */
class STBAPI_API So3DSeparator
:
    public SoSeparator, public Base3D 
{

	SO_NODE_HEADER(So3DSeparator);
	BASE3D_HEADER;

public:

	static void initClass(  //static class initialization
        );
	So3DSeparator(          //constructor
        );

    virtual SbBool isInterested( SoHandle3DEventAction* h3a );

    /// flag to define whether to filter events based on a bounding box check.
    SoSFBool boundingBoxCheck;

private:
	virtual ~So3DSeparator( //destructor
        );

}; //So3DSeparator


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SO3DSEPARATOR_H_
