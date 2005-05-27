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
/* Cal3DAnimator.h contains the Cal3DAnimator component
 *
 * @author Istvan Barakonyi
 *
 * $Id: Cal3DAnimator.h 3653 2004-09-03 15:44:26Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _CAL3DANIMATOR_H_
#define _CAL3DANIMATOR_H_

#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFRotation.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoFieldSensor.h>

// Cal3D includes
#include <cal3d/quaternion.h>
#include <cal3d/skeleton.h>

// own classes
#include "../../characters/cal3DPuppet/SoCal3DPuppeteer.h"

// for the DLL export
#include "cal3dposer.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** Cal3DAnimator ...

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class CAL3DPOSER_API Cal3DAnimator : public SoBaseKit
{
    SO_KIT_HEADER(Cal3DAnimator);

    SO_KIT_CATALOG_ENTRY_HEADER(root);
    SO_KIT_CATALOG_ENTRY_HEADER(puppeteer);

public:

    enum AxisConstraint { X=1, Y=2, Z=4, XY=X&Y, XZ=X&Z, YZ=Y&Z, XYZ=X&Y&Z };
    enum CursorMode { BONE=0, BODY, SCALE };

    Cal3DAnimator();

    /// Display/interaction option fields
    SoSFBool enablePosing;
    SoSFInt32 cursorMode; //  for values see enum CursorMode (the reason for not using bit flags is the compatibility with PUC)
    SoSFBool autoDisplayBB; // overrides showBoneBB and showBodyBB
    SoSFBool showBoneBB;
    SoSFBool showBodyBB;

    /// Bone/body manipulation fields
    SoSFVec3f cursorPosition;
    SoSFBool cursorButton;    
    SoSFVec3f insideBBTolerance;    
    SoSFInt32 pickedBone;
    SoSFInt32 axisConstraint; //  for values see enum AxisConstraint (the reason for not using bit flags is the compatibility with PUC)
    SoSFBool keepAspectRatio;
    SoSFFloat rotationSensitivity;
    SoSFFloat scaleSensitivity;

    /// Keyframing fields
    SoSFInt32 time;

SoINTERNAL public:

    /// Inventor class initialization
    static void initClass();

protected:

    /// Storing pointer to puppeteer's Cal3DPuppet for quick access
    SoCal3DPuppet *puppet;

    SbVec3f oldCursorPosition;
    SbVec3f oldBodyPosition;
    SbRotation oldBodyOrientation;
    SbVec3f oldScale;
    SbBool isDragging;
    CalQuaternion oldBoneRotation;

    /// Shortcut to Cal3D model's skeleton
    CalSkeleton *skeleton;

    /// Storing the current bone rotation to be applied
    CalQuaternion boneRotation;

    /// Field sensor
    SoFieldSensor *cursorPositionSensor;
    SoFieldSensor *cursorButtonSensor;
    SoFieldSensor *pickedBoneSensor;
    SoFieldSensor *timeSensor;
    
    virtual ~Cal3DAnimator();

    /// Checks whether the given point is inside the given bounding box
    bool isPointInAABB(SbVec3f point,SbVec3f bbMin,SbVec3f bbMax);

    /// Callback function for direct bone manipulation
    static void setPickedBoneRotation(void *data,SoCal3DPuppet *cal3dPuppet);

    /// Setting up field connections
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// Static callback for field sensors
    static void cursorPositionCB(void *data,SoSensor *sensor);
    static void cursorButtonCB(void *data,SoSensor *sensor);
    static void pickedBoneCB(void *data,SoSensor *sensor);
    static void timeCB(void *data,SoSensor *sensor);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _CAL3DANIMATOR_H_
