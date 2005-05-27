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
/* SoTransformCapture.h contains the definition of SoTransformCapture
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoTransformCapture.h 3426 2004-07-14 12:11:27Z tamer $
 * @file
 */
/* ======================================================================== */

#ifndef _SOTRANSFORMCAPTURE_H_
#define _SOTRANSFORMCAPTURE_H_

#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/actions/SoGLRenderAction.h>

// for the DLL export
#include "ARPuppet.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** SoTransformCapture is a node simply capturing the current accumulated transformation matrix

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class ARPUPPET_API SoTransformCapture : public SoNode 
{
    SO_NODE_HEADER(SoTransformCapture);
    
public:

    // only output fields
    SoSFVec3f translation;
    SoSFRotation rotation;
    SoSFVec3f scale;
    SoSFRotation scaleOrientation;

    static void initClass();

    SoTransformCapture();

protected:

    virtual void GLRender(SoGLRenderAction *action);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOTRANSFORMCAPTURE_H_
