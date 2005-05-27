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
/* ChoreographerKit.h contains the definition of class Choreographer
 *
 * @author Istvan Barakonyi
 *
 * $Id: Choreographer.h 4127 2005-01-20 14:03:28Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _CHOREOGRAPHER_H_
#define _CHOREOGRAPHER_H_

/// Inventor includes
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFBool.h>
#include <Inventor/fields/SoMFVec3f.h>

// STL classes
#include <string>

// for the DLL export
#include "arlego.h"

// own classes
#include "Character.h"
#include "../../characters/legoPuppet/SoLegoPuppeteer.h"
#include "../../characters/SoChoreographerKit.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** Choreographer is derived SoChoreographerKit and contains the arlego-specific
    choreographer commands 

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class ARLEGO_API Choreographer : public SoChoreographerKit
{
    SO_KIT_HEADER(Choreographer);

    SO_KIT_CATALOG_ENTRY_HEADER(bbSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(bbOff);
    SO_KIT_CATALOG_ENTRY_HEADER(bbOn);
    SO_KIT_CATALOG_ENTRY_HEADER(bbMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(bbDrawStyle);
    SO_KIT_CATALOG_ENTRY_HEADER(bbTransformBase);
    SO_KIT_CATALOG_ENTRY_HEADER(bbTransformOffset);
    SO_KIT_CATALOG_ENTRY_HEADER(bbCube);

public:

    /// Character (LEGO minifig) position event constants
    enum Event { NONE=0, COMMAND, FINISHED_CMD, NEAR_RCX, NEAR_DISPLAY };

    enum BoundingVolumeID { RCX=0, LOCALMONITOR };

    /// Constructor
    Choreographer();

    /// Destructor
    ~Choreographer();

    /* This field sends events about the current position of the LEGO minifig character. These events are defined in the Event constant enum.
       This field is connected to the <em>minifigEvent</em> field of the director.
     */
    SoSFInt32 minifigEvent;

    /// Connected to bounding volumes outputting whether configured tracked objects are inside (TRUE) or outside (FALSE) them
    SoMFBool insideBoundingVolume;

    /// Indicates whether character should use 1 or 2 hands to carry the tile ( = 0 --> single-handed, = 1 --> two-handed )
    SoSFInt32 handedness;

    /// Offset to be added to the minifig hotspot translation values
    SoSFVec3f hsOffset;

    /// Center of the BB of all model parts
    SoMFVec3f modelPartCenter;

    /// BB vertices (min and max) of all model parts
    SoMFVec3f modelPartBoundingBox;

    /// Turns bounding box on/off around the next model part to be placed
    SoSFBool showNextModelPartBB;

    /// Path vertices of the path the minifig has to walk while placing the next model part
    SoMFVec3f minifigPath;

    /// Constant prefix strings for the id value of the minifig and the RCX puppeteers
    static const SbString minifigIDString;
	static const SbString rcxIDString;
    static const SbString mptpIDString;

SoINTERNAL public:

    static void initClass();

protected:

    /// Quick pointers to important puppeteers
    Character *minifig;
    SoLegoPuppeteer *rcx;
    SoPuppeteerKit *mptp;

    /// Variable to store the originally set station number of the minifig
    int originalMinifigStation;

    // Storing the last minifig position on the current model construction's bounding volume borders
    SbVec3f lastBVPos;

    /// Field sensors
    SoFieldSensor *insideBoundingVolumeSensor;

    /// Static callback functions for field sensors
    static void insideBoundingVolumeCB(void *data,SoSensor *sensor);

    /// Sets model part centers and bounding boxes
    void setupModelPartVisualization();

    /// Setting up field connections etc.
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// Choreographer command implementation
    virtual bool implementCommand();

    /// Application-specific command choreography
    virtual bool choreograph();

    /// Returns path for the minifig around LEGO robot
    void getPathString(float x,float y,float z,std::string &dirStr,char *path);

    int getGotoTime(SbVec3f source,SbVec3f target);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif //_CHOREOGRAPHER_H_

