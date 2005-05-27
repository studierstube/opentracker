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
/** Class implementation for SoClipCube   
  *
  * @author  Gerd Hesina, Hermann Wurnig 
  *
  * $Id: SoClipCube.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoCube.h>

#include <stbapi/seams/SoClipCube.h>
#include <stbapi/misc/SoAbortGroup.h>

SO_KIT_SOURCE(SoClipCube);

// depth of recursion
int SoClipCube::stab;

// screen-boundingbox-data
float SoClipCube::updateArea[10][4];


/////////////////////////////////////////////////////////////////////
// Name:		initClass
// Purpose:		initialisation of class SoClipCube
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoClipCube::initClass(void) {
	SoClipCube::stab=0;
	SO_KIT_INIT_CLASS(SoClipCube, SoBaseKit, "BaseKit");
}


/////////////////////////////////////////////////////////////////////
// Name:		~SoClipCube
// Purpose:		Constructor of class SoClipCube. Default seam-geometry
//				clip-plane and sensors are set.
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
SoClipCube::SoClipCube() {
	SO_KIT_CONSTRUCTOR(SoClipCube);

	SO_KIT_ADD_FIELD(skipSeamWorld, (FALSE));

	SO_KIT_ADD_CATALOG_ENTRY(firstSeparator, SoSeparator, FALSE, this, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(visibilityCallback, SoCallback, FALSE, firstSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(renderAbortGroup, SoAbortGroup, FALSE, firstSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoTransformSeparator, FALSE, renderAbortGroup, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(seamTransform, SoTransform, FALSE, topSeparator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(seamGeometry, SoSeparator, FALSE, topSeparator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(clipPlane1, SoClipPlane, FALSE, topSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(clipPlane2, SoClipPlane, FALSE, topSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(clipPlane3, SoClipPlane, FALSE, topSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(clipPlane4, SoClipPlane, FALSE, topSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(clipPlane5, SoClipPlane, FALSE, topSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(clipPlane6, SoClipPlane, FALSE, topSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(secondaryTransform, SoTransform, TRUE, renderAbortGroup, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(secondaryWorld, SoSeparator, FALSE, renderAbortGroup, \x0, TRUE);
	SO_KIT_INIT_INSTANCE();

	calculate();
	((SoCallback *)visibilityCallback.getValue())->setCallback(visibilityCB,this);
	
	((SoAbortGroup*)renderAbortGroup.getValue())->
		abortActions.connectFrom(&skipSeamWorld);
    setSearchingChildren(TRUE);
}

/////////////////////////////////////////////////////////////////////
// Name:		calculate
// Purpose:		changes the clip-plane's parameters to the current
//				values
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoClipCube::calculate(void) {
	
	float x,y,z;
	x=1;
	y=1;
	z=1;

	x/=2;y/=2;z/=2;

	SbPlane plane(SbVec3f(x,y,z),SbVec3f(x,y,-z),SbVec3f(x,-y,z));
	((SoClipPlane *) clipPlane1.getValue())->plane = plane;	
	((SoClipPlane *) clipPlane1.getValue())->on = TRUE;	


	SbPlane plane2(SbVec3f(x,y,z),SbVec3f(-x,-y,z),SbVec3f(-x,y,z));
	((SoClipPlane *) clipPlane2.getValue())->plane = plane2;	
	((SoClipPlane *) clipPlane2.getValue())->on = TRUE;	

	SbPlane plane3(SbVec3f(-x,y,-z),SbVec3f(-x,y,z),SbVec3f(-x,-y,-z));
	((SoClipPlane *) clipPlane3.getValue())->plane = plane3;	
	((SoClipPlane *) clipPlane3.getValue())->on = TRUE;	

	SbPlane plane4(SbVec3f(-x,y,-z),SbVec3f(-x,-y,-z),SbVec3f(x,y,-z));
	((SoClipPlane *) clipPlane4.getValue())->plane = plane4;	
	((SoClipPlane *) clipPlane4.getValue())->on = TRUE;	

	SbPlane plane5(SbVec3f(x,y,z),SbVec3f(-x,y,z),SbVec3f(x,y,-z));
	((SoClipPlane *) clipPlane5.getValue())->plane = plane5;	
	((SoClipPlane *) clipPlane5.getValue())->on = TRUE;	

	SbPlane plane6(SbVec3f(-x,-y,z), SbVec3f(x,-y,z),SbVec3f(-x,-y,-z));
	((SoClipPlane *) clipPlane6.getValue())->plane = plane6;	
	((SoClipPlane *) clipPlane6.getValue())->on = TRUE;	

}


/////////////////////////////////////////////////////////////////////
// Name:		performBoundingBoxTest
// Purpose:		checks if the seam-polygon is visible, stores the
//				screen bounding-box and stops traversal if necessary 
// Parameters:	action ... current SoGLRenderAction
//				seam ... pointer to seam-node
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
SbBool SoClipCube::performBoundingBoxTest(SoAction *action, SoClipCube *seam)
{
	// Calculate the min and max values on screen
	
	const SbViewVolume currentVolume = 
		SoViewVolumeElement::get(action->getState());

	SbVec3f coords[8];

	float x,y,z;
	x=0.5;
	y=0.5;
	z=0.5;

	coords[0].setValue(x,y,z);
	coords[1].setValue(x,y,-z);
	coords[2].setValue(x,-y,z);
	coords[3].setValue(x,-y,-z);
	coords[4].setValue(-x,y,z);
	coords[5].setValue(-x,y,-z);
	coords[6].setValue(-x,-y,z);
	coords[7].setValue(-x,-y,-z);
	
	SbVec3f transformedPoint, screenPoint;

	float minx=1, maxx=0, miny=1, maxy=0;

	SbBool allBefore=TRUE, allBehind=TRUE;	

	float nearDist = currentVolume.getNearDist();
	float farDist = nearDist + currentVolume.getDepth();
	SbPlane nearPlane = currentVolume.getPlane(nearDist);
	SbPlane farPlane = currentVolume.getPlane(farDist);

	const SoPath *rootPath = action->getCurPath();
	SoPath *seamPath = ((SoFullPath *) rootPath)->copy(0,((SoFullPath *)rootPath)->getLength()-2);
	seamPath->ref();
	seamPath->append(seam->firstSeparator.getValue());
	seamPath->append(seam->renderAbortGroup.getValue());
	seamPath->append(seam->topSeparator.getValue());
	seamPath->append(seam->seamTransform.getValue());

	static SbViewportRegion vp;
	static SoGetMatrixAction *gma=new SoGetMatrixAction(vp);

	gma->apply((SoFullPath *)seamPath);
	SbMatrix currentMatrix=gma->getMatrix();

	seamPath->unref();

	for (int i=0; i < 8; i++)
	{
		// get coordinaes in world-space 
		currentMatrix.multVecMatrix(coords[i], transformedPoint); 

		currentVolume.projectToScreen(transformedPoint,screenPoint);

		float f1,f2,f3;
		screenPoint.getValue(f1,f2,f3);

		if (f1<0) minx=0;
		else if (f1>1) maxx=1;
		else
		{
			if (f1 < minx) minx=f1;
			if (f1 > maxx) maxx=f1;
		}	

		if (f2<0) miny=0;
		else if (f2>1) maxy=1;
		else
		{
			if (f2 < miny) miny=f2;
			if (f2 > maxy) maxy=f2;
		}	


		if (nearPlane.isInHalfSpace(transformedPoint) == FALSE) 
		{			// after clipping-plane
			allBefore = FALSE;
		}
		if (farPlane.isInHalfSpace(transformedPoint) == TRUE) 
		{			// before far-clipping-plane
			allBehind = FALSE;
		}
		
	}
	if ((allBehind == TRUE) || (allBefore == TRUE)) return TRUE;

	if (SoClipCube::stab > 0)
	{					// return FALSE when seams not overlapping

		if ((minx > SoClipCube::updateArea[SoClipCube::stab-1][2]) ||
		    (maxx < SoClipCube::updateArea[SoClipCube::stab-1][0]) ||	
		    (miny > SoClipCube::updateArea[SoClipCube::stab-1][3]) ||
		    (maxy < SoClipCube::updateArea[SoClipCube::stab-1][1])) return TRUE;		
	
		if (SoClipCube::updateArea[SoClipCube::stab-1][0] > minx) minx=SoClipCube::updateArea[SoClipCube::stab-1][0];
		if (SoClipCube::updateArea[SoClipCube::stab-1][2] < maxx) maxx=SoClipCube::updateArea[SoClipCube::stab-1][2];
		if (SoClipCube::updateArea[SoClipCube::stab-1][1] > miny) miny=SoClipCube::updateArea[SoClipCube::stab-1][1];
		if (SoClipCube::updateArea[SoClipCube::stab-1][3] < maxy) maxy=SoClipCube::updateArea[SoClipCube::stab-1][3];
	}

	if (((maxx-minx) < 0.01) || ((maxy-miny) < 0.01)) return TRUE;

	SoClipCube::updateArea[SoClipCube::stab][0]= minx;
	SoClipCube::updateArea[SoClipCube::stab][1]= miny;
	SoClipCube::updateArea[SoClipCube::stab][2]= maxx;
	SoClipCube::updateArea[SoClipCube::stab][3]= maxy;

	//this is the area we have to draw. Now it would be good if we could
	// resize the update-area !!! ;)

	return FALSE;
}

/////////////////////////////////////////////////////////////////////
// Name:		visibilityCB
// Purpose:		determines if seam-polygon is visible and sets the 
//				field skipSeamWorld.
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoClipCube::visibilityCB(void* data, SoAction* action)
{
    if (action->isOfType(SoGLRenderAction::getClassTypeId())) 
    {
		SoClipCube *seam = (SoClipCube *) data;

			// GEORGES - LUEBKE

		seam->skipSeamWorld = performBoundingBoxTest(action, seam);
		if (seam->checkVisibility.getValue() == FALSE) seam->skipSeamWorld = FALSE;

			// limit recursion depth to 6
		if (SoClipCube::stab == 7) seam->skipSeamWorld=TRUE;
	}
}

/////////////////////////////////////////////////////////////////////
// Name:		~SoClipCube
// Purpose:		Destructor of class SoClipCube
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
SoClipCube::~SoClipCube() {
}
