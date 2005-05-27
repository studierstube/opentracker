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
/* SoBoundingVolume.h contains the SoBoundingVolume component
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoBoundingVolume.h 3852 2004-10-07 20:49:59Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _SOBOUNDINGVOLUME_H_
#define _SOBOUNDINGVOLUME_H_

#include <Inventor/SbLinear.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoMFEnum.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/sensors/SoFieldSensor.h>

// for the DLL export
#include "arpuppet.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** 
@author Istvan Barakonyi
@ingroup ARPuppet
*/
class ARPUPPET_API SoBoundingVolume : public SoBaseKit
{
    SO_KIT_HEADER(SoBoundingVolume);

    SO_KIT_CATALOG_ENTRY_HEADER(root);
    SO_KIT_CATALOG_ENTRY_HEADER(materialSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(materialPointIn);
    SO_KIT_CATALOG_ENTRY_HEADER(materialPointOut);
    SO_KIT_CATALOG_ENTRY_HEADER(drawStyle);
    SO_KIT_CATALOG_ENTRY_HEADER(volumeSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(sphereRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(ifsRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(sphere);
    SO_KIT_CATALOG_ENTRY_HEADER(coords);
    SO_KIT_CATALOG_ENTRY_HEADER(indexedFaceSet);

public:

    SoBoundingVolume();
    ~SoBoundingVolume();

SoINTERNAL public:

    /// Enum values for bounding volume types
    enum VolumeType { NONE=0, POINT, SPHERE, RECTANGLE, BOX };

    /// Enum values for bounding volume handling mode --> they WON'T be used as SoSFEnum values!!!
    enum Mode { DRAWN, DRAWING, EXTERNAL };

    /// Initialize class
    static void initClass();

    /// Indicates whether dynamic bounding volume drawing is currently active or not. If not active, discard cursor events.
    SoSFEnum mode;

    /// If triggered, resets the most recently drawn bounding volume and start a new one
    SoSFTrigger reset;

    /// The current position of the BB drawer cursor.
    SoSFVec3f cursorPosition;

    /// The current button events of the BB drawer cursor.
    SoSFBool cursorButton;

    /// Output field only, it contains the 8 vertices of the defined bounding volume
    SoMFVec3f boundingVolume;

    /// Number of control points to define the bounding volume.
    SoSFEnum volumeType;

    /// Vertices of input bounding volumes to be tested whether they are inside or outside the bounding volume
    SoMFVec3f boundingVolumeTest;

    /// Types of input bounding volumes to be tested whether they are inside or outside the bounding volume
    SoMFEnum boundingVolumeTestType;

    /// Test results indicating whether the input bounding volumes are inside or outside our bounding volume
    SoMFBool boundingVolumeInside;

    /// Turns bounding volume visualization on/off
    SoSFBool visualization;

    /// Turns event generation (e.g. point-bounding volume intersection) on/off
    SoSFBool eventGeneration;

protected:

    /// Number of points to define the bounding volume
    int controlPointNum;

    /// Counter to store button clicks to indicate which BB control point we are setting
    int clickCounter;

    /// Variable storing the face indices for the indexed faceset
    int *faceIndex;

    /// Storing the planes of the bounding volume (if applicable)
    SbPlane *plane;

    /// center point of the bounding volume
    SbVec3f centerPoint;

    /// Field sensor
    SoFieldSensor *resetSensor;
    SoFieldSensor *cursorButtonSensor;
    SoFieldSensor *cursorPositionSensor;
    SoFieldSensor *volumeTypeSensor;
    SoFieldSensor *boundingVolumeTestSensor;
    SoFieldSensor *visualizationSensor;

    /// Setting up field connections
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// Static field sensor callback functions
    static void resetCB(void *data,SoSensor *sensor);
    static void cursorButtonCB(void *data,SoSensor *sensor);
    static void cursorPositionCB(void *data,SoSensor *sensor);
    static void volumeTypeCB(void *data,SoSensor *sensor);
    static void boundingVolumeTestCB(void *data,SoSensor *sensor);
    static void visualizationCB(void *data,SoSensor *sensor);

    /// Delete pointers
    void cleanup();

    /// Dump vertices resulted from the bounding volume drawing
    void dumpResult();

    /// Returns the number of vertices in the given bounding volume type
    int getVertexNum(int type);

    /// Calculate parameters that are useful when testing intersection
    void calculateParams();

    /// Calculate intersection with specified bounding volume
    bool intersectBV(int volumeTestType,int vertexCounter,int vertexNum);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOBOUNDINGVOLUME_H_
