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
/** Implementation of class So3DSeparator
  *
  * @author Andreas Zajic, Markus Krutz, Gerd Hesina
  *
  * $Id: So3DSeparator.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/actions/SoGetBoundingBoxAction.h>

#include <stbapi/interaction/So3DSeparator.h>

#include <stbapi/event3d/So3DEvent.h>
#include <stbapi/event3d/SoHandle3DEventAction.h>

SO_NODE_SOURCE(So3DSeparator);

BASE3D_SOURCE(So3DSeparator);

void So3DSeparator::initClass() {

	SO_NODE_INIT_CLASS(So3DSeparator, SoSeparator, "Separator");
    
	BASE3D_INIT_CLASS(So3DSeparator);
}

So3DSeparator::So3DSeparator() {

	SO_NODE_CONSTRUCTOR(So3DSeparator);

    SO_NODE_ADD_FIELD( boundingBoxCheck, (TRUE));

	BASE3D_CONSTRUCTOR;
}

So3DSeparator::~So3DSeparator() {
}

SbBool So3DSeparator::isInterested( SoHandle3DEventAction* h3a )
{
    if( boundingBoxCheck.getValue())
        return Base3D::isInterested( h3a );
    return TRUE;
}

