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

#include "GridKit.h"

SO_KIT_SOURCE(GridKit);

void GridKit::initClass() 
{ 
    SO_KIT_INIT_CLASS(GridKit, SoBaseKit, "BaseKit"); 
} 

GridKit::GridKit() 
{ 
	initGrid();
}

void 
GridKit::setPosition(SbVec3f penPosition)
{
    if (active.getValue()) // if grid turned on
	    position.setValue(penPosition);
}

void
GridKit::initGrid()
{    
	SO_KIT_CONSTRUCTOR(GridKit);

	// including the SoSwitch for switching the grid on and off did not work
    // no possibility could be found to integrate the SoSwitch in the scene
    // graph.It crashed every time when trying to access parts of the switchGrid

	//-----------------------name-----|-type-------|nuldef| par |rite|public
    SO_KIT_ADD_CATALOG_ENTRY(gridGeo, SoSeparator,  FALSE, this, \x0, TRUE);
//    SO_KIT_ADD_CATALOG_ENTRY(switchGrid, SoSwitch,  FALSE, this,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(gridMaterial,SoMaterial,FALSE, gridGeo, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(drawstyle,SoDrawStyle, FALSE, gridGeo,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(xCoord, SoCoordinate3, FALSE, gridGeo,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(xAxis, SoLineSet, FALSE, gridGeo, \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(yCoord, SoCoordinate3, FALSE,gridGeo,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(yAxis, SoLineSet, FALSE, gridGeo,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(zCoord, SoCoordinate3, FALSE,gridGeo,\x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(zAxis, SoLineSet, FALSE, gridGeo,\x0, TRUE);

    //fields
    SO_NODE_ADD_FIELD(color, (1.0f, 1.0f, 1.0f));
    SO_NODE_ADD_FIELD(linethickness, (1.5f));
    SO_NODE_ADD_FIELD(xStart, (0.0f, 0.0f, 0.0f));
    SO_NODE_ADD_FIELD(xEnd, (0.0f, 0.0f, 0.0f));
    SO_NODE_ADD_FIELD(yStart, (0.0f, 0.0f, 0.0f));
    SO_NODE_ADD_FIELD(yEnd, (0.0f, 0.0f, 0.0f));
    SO_NODE_ADD_FIELD(zStart, (0.0f, 0.0f, 0.0f));
    SO_NODE_ADD_FIELD(zEnd, (0.0f, 0.0f, 0.0f));
    SO_NODE_ADD_FIELD(position, (0.0f, 0.0f, 0.0f));
    SO_NODE_ADD_FIELD(visibleGrid, (SO_SWITCH_ALL));
    SO_NODE_ADD_FIELD(active, (TRUE));

    // Initialize this SoPointKit
	SO_KIT_INIT_INSTANCE();
	setSearchingChildren(TRUE);

//	SoSwitch* sw = (SoSwitch*) getPart("switchGrid", false);
//    sw->whichChild.connectFrom(&visibleGrid);
//	SoSeparator* gr = (SoSeparator*) getPart("gridGeo", false);
//    printf("%d \n",gr->getNumChildren());

    color.setValue(gridColor);
    linethickness.setValue(gridLineWidth);
    position.setValue(0.f, 0.f, 0.f);
    visibleGrid.setValue(SO_SWITCH_ALL);

    SoDecomposeVec3f* deComp = new SoDecomposeVec3f();
    deComp->vector.connectFrom(&position);

    SoComposeVec3f* xComp = new SoComposeVec3f();
	xComp->x = 0;
	xComp->y.connectFrom(&(deComp->y));
    xComp->z.connectFrom(&(deComp->z));
    xStart.connectFrom(&(xComp->vector));
    xEnd.connectFrom(&position);

    SoComposeVec3f* yComp = new SoComposeVec3f();
	yComp->x.connectFrom(&(deComp->x));
	yComp->y = 0;
    yComp->z.connectFrom(&(deComp->z));
    yStart.connectFrom(&(yComp->vector));
    yEnd.connectFrom(&position);

    SoComposeVec3f* zComp = new SoComposeVec3f();
	zComp->x.connectFrom(&(deComp->x));
	zComp->y.connectFrom(&(deComp->y));
    zComp->z = 0;
    zStart.connectFrom(&(zComp->vector));
    zEnd.connectFrom(&position);

    SoMaterial* mat = (SoMaterial*) getPart("gridMaterial", false);
    mat->emissiveColor.connectFrom(&color);
    SoDrawStyle* ds = (SoDrawStyle*) getPart("drawstyle",false); 
    ds->lineWidth.connectFrom(&linethickness);

    //create Coordinate nodes
    SoConcatenate* xconc = new SoConcatenate(SoMFVec3f::getClassTypeId());
	xconc->input[0]->connectFrom(&(xStart));
	xconc->input[1]->connectFrom(&(xEnd));
    SoCoordinate3* xcoordinate = (SoCoordinate3*) getPart("xCoord", false);
	xcoordinate->point.connectFrom(xconc->output);

    SoLineSet* xLine = (SoLineSet*) getPart("xAxis", false);
    xLine->numVertices.setValue(2);
    xLine->startIndex.setValue(0);

    SoConcatenate* yconc = new SoConcatenate(SoMFVec3f::getClassTypeId());
	yconc->input[0]->connectFrom(&(yStart));
	yconc->input[1]->connectFrom(&(yEnd));
    SoCoordinate3* ycoordinate = (SoCoordinate3*) getPart("yCoord", false);
	ycoordinate->point.connectFrom(yconc->output);

    SoLineSet* yLine = (SoLineSet*) getPart("yAxis", false);
    yLine->numVertices.setValue(2);
    yLine->startIndex.setValue(0);

    SoConcatenate* zconc = new SoConcatenate(SoMFVec3f::getClassTypeId());
	zconc->input[0]->connectFrom(&(zStart));
	zconc->input[1]->connectFrom(&(zEnd));
    SoCoordinate3* zcoordinate = (SoCoordinate3*) getPart("zCoord", false);
	zcoordinate->point.connectFrom(zconc->output);

    SoLineSet* zLine = (SoLineSet*) getPart("zAxis", false);
    zLine->numVertices.setValue(2);
    zLine->startIndex.setValue(0);
}


// Destructor
GridKit::~GridKit()
{
}

void 
GridKit::turnOff()
{
    
    position.setValue(0.f, 0.f, 0.f);
    active.setValue(FALSE);
//	visibleGrid.setValue(SO_SWITCH_NONE);
}

void 
GridKit::turnOn()
{
    active.setValue(TRUE);
//    visibleGrid.setValue(SO_SWITCH_ALL);
}
