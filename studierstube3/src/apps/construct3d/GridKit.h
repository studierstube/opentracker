// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Cnd.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube Workspace
//  CONTENT:    Geometric Construction in n Dimensions
//  VERSION:    1.3
// ===========================================================================
//  AUTHOR:    hk      Hannes Kaufmann
// ===========================================================================
//  HISTORY:   Time of development: Dezember 1999 - 
//
// ===========================================================================


#ifndef _GRIDKIT_H_
#define _GRIDKIT_H_

#include <assert.h>
#include <Inventor/SbLinear.h>
#include <Inventor/nodekits/SoBaseKit.h>

#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/engines/SoConcatenate.h>
#include <Inventor/engines/SoCompose.h>

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <stbapi/misc/ivmacros.h>

#include "CnDconstants.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class GridKit: public SoBaseKit 
{ 
    SO_KIT_HEADER(GridKit);

    //set parts
    SO_KIT_CATALOG_ENTRY_HEADER(gridGeo);
//    SO_KIT_CATALOG_ENTRY_HEADER(switchGrid);
    SO_KIT_CATALOG_ENTRY_HEADER(gridMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(drawstyle);
    SO_KIT_CATALOG_ENTRY_HEADER(xCoord);
    SO_KIT_CATALOG_ENTRY_HEADER(xAxis);
    SO_KIT_CATALOG_ENTRY_HEADER(yCoord);
    SO_KIT_CATALOG_ENTRY_HEADER(yAxis);
    SO_KIT_CATALOG_ENTRY_HEADER(zCoord);
    SO_KIT_CATALOG_ENTRY_HEADER(zAxis);

public: 
    //default color (1,1,1);
    SoSFVec3f color; 
    SoSFFloat linethickness;
    SoSFVec3f xStart, xEnd, yStart, yEnd, zStart, zEnd, position;
    // if visible== SO_SWITCH_ALL, then object is visible, 
    // if visible== SO_SWITCH_NONE then invisible
    SoSFInt32 visibleGrid;
    SoSFBool active; // if grid is turned on or off

    static void initClass(); 
    GridKit(
        ); 
    ~GridKit(
        );
	void initGrid(
		);
	void setPosition(
        SbVec3f penPosition
        );
    void turnOff(
        );
    void turnOn(
        );
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_GRIDKIT_H_
