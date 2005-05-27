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
/* SoTransformCapture.cxx contains the implementation of SoTransformCapture
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoTransformCapture.cxx 3426 2004-07-14 12:11:27Z tamer $
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/elements/SoModelMatrixElement.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoTransformCapture.h"

SO_NODE_SOURCE(SoTransformCapture);

void SoTransformCapture::initClass()
{
   SO_NODE_INIT_CLASS(SoTransformCapture, SoNode, "Node");
}

SoTransformCapture::SoTransformCapture()
{
    SO_NODE_CONSTRUCTOR(SoTransformCapture);
    SO_NODE_ADD_FIELD(translation,  (0.0f,0.0f,0.0f));
    SO_NODE_ADD_FIELD(rotation, (SbRotation::identity()));
    SO_NODE_ADD_FIELD(scale, (1.0f,10.0f,1.0f));
    SO_NODE_ADD_FIELD(scaleOrientation, (SbRotation::identity()));
}

void SoTransformCapture::GLRender(SoGLRenderAction *action)
{
    SbMatrix matrix=SoModelMatrixElement::get(action->getState());

    SbVec3f t,s,c;
    SbRotation r,so;
    matrix.getTransform(t,r,s,so);

    translation.setValue(t);
    rotation.setValue(r);
    scale.setValue(s);
    scaleOrientation.setValue(so);
}
