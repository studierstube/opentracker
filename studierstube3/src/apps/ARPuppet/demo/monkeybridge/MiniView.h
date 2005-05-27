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
/* MiniView.h contains the MiniView component
 *
 * @author Istvan Barakonyi
 *
 * $Id: MiniView.h 4209 2005-05-19 09:06:39Z bara $
 * @file
 */
/* ======================================================================== */

#ifndef _MINIVIEW_H_
#define _MINIVIEW_H_

#include <Inventor/fields/SoSFVec2f.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFColor.h>
#include <Inventor/fields/SoMFColor.h>

#include <Inventor/nodekits/SoBaseKit.h>

#include <Inventor/sensors/SoFieldSensor.h>

// for the DLL export
#include "monkeybridge.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** MiniView is a nodekit generating a miniature abstract view of the playfield

@author Istvan Barakonyi
@ingroup ARPuppet
*/
class MONKEYBRIDGE_API MiniView : public SoBaseKit
{
    SO_KIT_HEADER(MiniView);

    SO_KIT_CATALOG_ENTRY_HEADER(tileRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(coordinateMesh);
    SO_KIT_CATALOG_ENTRY_HEADER(materialBindingMesh);
    SO_KIT_CATALOG_ENTRY_HEADER(materialMesh);   
    SO_KIT_CATALOG_ENTRY_HEADER(mesh);
    SO_KIT_CATALOG_ENTRY_HEADER(playerRoot);
    SO_KIT_CATALOG_ENTRY_HEADER(materialBindingPoint);
    SO_KIT_CATALOG_ENTRY_HEADER(materialPoint);
    SO_KIT_CATALOG_ENTRY_HEADER(drawStylePoint);
    SO_KIT_CATALOG_ENTRY_HEADER(coordinatePoint);
    SO_KIT_CATALOG_ENTRY_HEADER(pointSet);
    SO_KIT_CATALOG_ENTRY_HEADER(pathRoot);

public:

    MiniView();

    /// Miniview size-related fields
    /*@{*/

    SoSFVec2f maxCoordinate;
    SoSFFloat tileSize;
    SoSFFloat globalScale;

    /*@}*/

    /// Defining colors and overall transparency for miniview fields and player representation
    /*@{*/

    SoSFColor defaultTileColor;
    SoMFColor userTileColor;
    SoSFColor fixTileColor;
    SoMFColor positionColor;
    SoMFColor pathColor;
    SoSFFloat transparency;
    
    /*@}*/

    /// Information about player movement
    /*@{*/

    SoMFVec2f occupationInfo;
    SoMFVec2f currentPosition;
    SoMFVec2f nextPosition; // TODO: implemetn functionality

    /*@}*/

SoINTERNAL public:

    /// Inventor class initialization
    static void initClass();

protected:

    /// Field sensor
    SoFieldSensor *maxCoordinateSensor;
    SoFieldSensor *tileSizeSensor;
    SoFieldSensor *occupationInfoSensor;
    SoFieldSensor *currentPositionSensor;
    SoFieldSensor *nextPositionSensor;
    
    virtual ~MiniView();

    /// Setting up field connections
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    /// Static callback for field sensors
    static void buildGridCB(void *data,SoSensor *sensor);
    static void occupationInfoCB(void *data,SoSensor *sensor);
    static void userPositionCB(void *data,SoSensor *sensor);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _MINIVIEW_H_
