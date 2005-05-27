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
/* SoViewingFrustum.h contains the definition of SoViewingFrustum,
 * a nodekit representing and visualizing a viewing frustum
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoViewingFrustum.h 3426 2004-07-14 12:11:27Z tamer $
 * @file
 */
/* ======================================================================= */

#ifndef _SOVIEWINGFRUSTUM_H_
#define _SOVIEWINGFRUSTUM_H_

// for the DLL export
#include "ARPuppet.h"

#include <Inventor/SbLinear.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/actions/SoGLRenderAction.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** SoViewingFrustum is a nodekit representing the eyesight of an ARPuppet. It is visualized as a viewing frustum 
    and includes useful routines like testing whether the bounding box (AABB) of an object or a given point is intersecting it. 
    Thus visibility checks become possible for the ARPuppets i.e. it "sees" an object or not.

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class ARPUPPET_API SoViewingFrustum : public SoBaseKit
{
    SO_KIT_HEADER(SoViewingFrustum);

    SO_KIT_CATALOG_ENTRY_HEADER(root);
    SO_KIT_CATALOG_ENTRY_HEADER(offsetTransform);
    SO_KIT_CATALOG_ENTRY_HEADER(coords);
    SO_KIT_CATALOG_ENTRY_HEADER(indexedFaceSet);

public:

    /// Position offset
    SoSFVec3f positionOffset;

    /// Orientation offset
    SoSFRotation orientationOffset;

    /// Aspect ratio = x/y
    SoSFFloat aspectRatio;

    /// Distance of the near clipping plane from the viewpoint
    SoSFFloat nearDistance;

    /// Distance of the far clipping plane from the viewpoint
    SoSFFloat farDistance;

    /// Vertical angle of the pyramid. widthAngle = heightAngle * aspectRatio
    SoSFFloat heightAngle;

    SoViewingFrustum();
    
    /// Check intersection with axis-aligned bounding box defined by its minimum and maximum points
    bool intersectBB(const SbVec3f &min,const SbVec3f &max);

    /// Check intersection with point
    bool intersectPoint(const SbVec3f &p);

SoINTERNAL public:
    static void initClass();

protected:

    virtual ~SoViewingFrustum();

    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);
    virtual void GLRender(SoGLRenderAction *action);

    /// Storing frustum vertices
    SbVec3f *point;

    /// Storing vertex indices of faces
    int faceIndex[5*6];

    /// Storing the vertex indices of the edges
    int edgeIndex[12][2];

    int bbPlaneIndex[6][3];

    /// Storing the planes of the faces
    SbPlane plane[6];

    /// Storing the edges of the frustum
    SbLine edge[12];

    /// Current transformation accumulated in scene graph above the viewing frustum
    SbMatrix matrix;

    /// @name Field sensors
    /*@{*/

    SoFieldSensor *aspectRatioSensor;
    SoFieldSensor *nearDistanceSensor;
    SoFieldSensor *farDistanceSensor;
    SoFieldSensor *heightAngleSensor;

    /*@}*/

    // sensor callbacks
    /*@{*/

    static void setGeometry(void *data,SoSensor *sensor);
    
    /*@}*/
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif //_SOVIEWINGFRUSTUM_H_
