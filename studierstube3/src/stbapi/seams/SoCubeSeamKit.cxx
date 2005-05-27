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
/** SoCubeKit - implementation
  *
  * @author Gerd Hesina, Hermann Wurnig
  *
  * $Id: SoCubeSeamKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#ifdef WIN32
  #include <windows.h>
#endif

#include <Inventor/nodes/SoCube.h>
#include <Inventor/actions/SoWriteAction.h>
#include <stbapi/seams/SoCubeSeamKit.h>
#include <stbapi/misc/SoAbortGroup.h>
#include <stbapi/util/ivio.h>
#include <GL/gl.h>
#include <cassert>

#define _USE_STENCIL_

SO_KIT_SOURCE(SoCubeSeamKit);

// depth of recursion
int SoCubeSeamKit::stab;

// screen-boundingbox-data
float SoCubeSeamKit::updateArea[10][4];

const char* invertedBoxIv =
"#VRML V1.0 ascii\n"
"Separator {\n"
"  Info { string \"VRML 1.0/2.0 exporter, Version 1, Beta 0\" }\n"
"  Info { string \"Date: Tue Mar 10 10:57:02 1998\" }\n"
"  ShapeHints {\n"
"    shapeType SOLID\n"
"    vertexOrdering COUNTERCLOCKWISE\n"
"    faceType CONVEX\n"
"  }\n"
"  DEF Box01 Separator {\n"
"    Translation { translation 0 0 -0.5 }\n"
"    Rotation { rotation -1 0 0 -1.57 }\n"
"    Material {\n"
"      ambientColor 0.0 0 0\n"
"      diffuseColor 0.1 0.13 0.16\n"
"      specularColor 0 0 0\n"
"      transparency 0\n"
"    }\n"
"    Texture2 {}\n"
"    Coordinate3 { point [\n"
"      -0.5 0 0.5,\n"
"      0.5 0 0.5,\n"
"      -0.5 0 -0.5,\n"
"      0.5 0 -0.5,\n"
"      -0.5 1 0.5,\n"
"      0.5 1 0.5,\n"
"      -0.5 1 -0.5,\n"
"      0.5 1 -0.5]\n"
"    }\n"
"    Normal { vector [\n"
"      1 0 0,\n"
"      -1 0 0,\n"
"      0 1 0,\n"
"      0 0 1,\n"
"      0 -1 0,\n"
"      0 0 -1,\n"
"    ] }\n"
"    NormalBinding { value PER_VERTEX_INDEXED }\n"
"    IndexedFaceSet { coordIndex [\n"
"      2, 0, 3, -1,\n" 
"      1, 3, 0, -1,\n" 
"      5, 4, 7, -1,\n" 
"      6, 7, 4, -1,\n" 
"      1, 0, 5, -1,\n" 
"      4, 5, 0, -1,\n" 
"      3, 1, 7, -1,\n" 
"      5, 7, 1, -1,\n" 
"      2, 3, 6, -1,\n" 
"      7, 6, 3, -1,\n" 
"      0, 2, 4, -1,\n"
"      6, 4, 2, -1]\n"
"    normalIndex [\n"
"      2, 2, 2, -1,\n"
"      2, 2, 2, -1,\n"
"      4, 4, 4, -1,\n"
"      4, 4, 4, -1,\n"
"      5, 5, 5, -1,\n"
"      5, 5, 5, -1,\n"
"      1, 1, 1, -1,\n"
"      1, 1, 1, -1,\n"
"      3, 3, 3, -1,\n"
"      3, 3, 3, -1,\n"
"      0, 0, 0, -1,\n"
"      0, 0, 0, -1,\n"
"    ]\n"
"    }\n"
"  }\n"
"}\n";


const char* transparentCubeIv =
"#Inventor V2.0 ascii\n"
"Separator {\n"
"  ShapeHints {\n"
"    shapeType SOLID\n"
"    vertexOrdering CLOCKWISE\n"
"    faceType CONVEX\n"
"  }\n"
"  DEF Box01 Separator {\n"
"    Translation { translation 0 0 -0.5 }\n"
"    Rotation { rotation -1 0 0 -1.57 }\n"
"    Material {\n"
"      ambientColor 0.21 0.244 0.325\n"
"      diffuseColor 0.21 0.244 0.325\n"
"      specularColor 0 0 0\n"
"      shininess 0.0\n"
"      transparency 0.7\n"
"    }\n"
"    Texture2 {}\n"
"    Coordinate3 { point [\n"
"      -0.5 0 0.5,\n"
"      0.5 0 0.5,\n"
"      -0.5 0 -0.5,\n"
"      0.5 0 -0.5,\n"
"      -0.5 1 0.5,\n"
"      0.5 1 0.5,\n"
"      -0.5 1 -0.5,\n"
"      0.5 1 -0.5]\n"
"    }\n"
"    Normal { vector [\n"
"      -1 0 0,\n"
"      1 0 0,\n"
"      0 -1 0,\n"
"      0 0 -1,\n"
"      0 1 0,\n"
"      0 0 1,\n"
"    ] }\n"
"    NormalBinding { value PER_VERTEX_INDEXED }\n"
"    IndexedFaceSet { coordIndex [\n"
"      2, 0, 3, -1, \n"
"      1, 3, 0, -1, \n"
"      5, 4, 7, -1, \n"
"      6, 7, 4, -1, \n"
"      1, 0, 5, -1, \n"
"      4, 5, 0, -1, \n"
"      3, 1, 7, -1, \n"
"      5, 7, 1, -1, \n"
"      2, 3, 6, -1, \n"
"      7, 6, 3, -1, \n"
"      0, 2, 4, -1, \n"
"      6, 4, 2, -1]\n"
"    normalIndex [\n"
"      2, 2, 2, -1,\n"
"      2, 2, 2, -1,\n"
"      4, 4, 4, -1,\n"
"      4, 4, 4, -1,\n"
"      5, 5, 5, -1,\n"
"      5, 5, 5, -1,\n"
"      1, 1, 1, -1,\n"
"      1, 1, 1, -1,\n"
"      3, 3, 3, -1,\n"
"      3, 3, 3, -1,\n"
"      0, 0, 0, -1,\n"
"      0, 0, 0, -1,\n"
"    ]\n"
"    }\n"
"  }\n"
"}\n";


// Returns a separator containing the scene contained in the iv-file name.
/*SoSeparator* readNode(const char* name)
{
    SoInput myInput;
    SoSeparator *ReadNodeSep = new SoSeparator;
    if (!myInput.openFile(name)) {
      printf("file not found!\n");
      return (0); 
    }
    ReadNodeSep = SoDB::readAll(&myInput);
    if (ReadNodeSep == NULL) return (0);
    myInput.closeFile();
    return ReadNodeSep;
}*/


/////////////////////////////////////////////////////////////////////
// Name:		initClass
// Purpose:		initialisation of class SoCubeSeamKit
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoCubeSeamKit::initClass(void) {
	SoCubeSeamKit::stab=0;
	SO_KIT_INIT_CLASS(SoCubeSeamKit, SoBaseKit, "BaseKit");
}

void SoCubeSeamKit::setDefaultOnNonWritingFields(void) {
	SoBaseKit::setDefaultOnNonWritingFields();
	topSeparator.setDefault(TRUE);
	GLCallback1.setDefault(TRUE);
	GLCallback2.setDefault(TRUE);
	top2Separator.setDefault(TRUE);
	seamSeparator.setDefault(TRUE);
	seamSeparator2.setDefault(TRUE);
	seamHints.setDefault(TRUE);
	renderAbortGroup.setDefault(TRUE);
	firstSeparator.setDefault(TRUE);
	visibilityCallback.setDefault(TRUE);
}

void clipOff(void *, SoAction *)
{
	glPushAttrib(GL_CLIP_PLANE0 |GL_CLIP_PLANE1 | GL_CLIP_PLANE2 |
				 GL_CLIP_PLANE3 | GL_CLIP_PLANE4 | GL_CLIP_PLANE5);
	glDisable(GL_CLIP_PLANE5);
	glDisable(GL_CLIP_PLANE4);
	glDisable(GL_CLIP_PLANE3);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE0);

}

void clipOn(void *, SoAction *)
{
	glPopAttrib();
}

/////////////////////////////////////////////////////////////////////
// Name:		~SoCubeSeamKit
// Purpose:		Constructor of class SoCubeSeamKit. Default seam-geometry
//				clip-plane and sensors are set.
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
SoCubeSeamKit::SoCubeSeamKit() {
	SO_KIT_CONSTRUCTOR(SoCubeSeamKit);

	SO_KIT_ADD_FIELD(overlay, (FALSE));
	SO_KIT_ADD_FIELD(leaveLightsOn, (TRUE));
	SO_KIT_ADD_FIELD(skipSeamWorld, (FALSE));
	SO_KIT_ADD_FIELD(backgroundColor, (0.4f,0.4f,0.4f));
	SO_KIT_ADD_FIELD(checkVisibility, (TRUE));

	SO_KIT_ADD_CATALOG_ENTRY(firstSeparator, SoSeparator, FALSE, this, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(visibilityCallback, SoCallback, FALSE, firstSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(renderAbortGroup, SoAbortGroup, FALSE, firstSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, FALSE, renderAbortGroup, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(top2Separator, SoSeparator, FALSE, topSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(seamSeparator, SoTransformSeparator, FALSE, top2Separator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(seamTransform, SoTransform, FALSE, seamSeparator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(GLCallback1, SoCallback, FALSE, seamSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(seamHints, SoShapeHints, FALSE, seamSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(seamCube, SoCube, FALSE, seamSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(GLCallback2, SoCallback, FALSE, seamSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(secondaryTransform, SoTransform, TRUE, top2Separator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(secondaryBBoxAbort, SoAbortGroup, FALSE, top2Separator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(secondaryWorld, SoSeparator, FALSE, secondaryBBoxAbort, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(seamSurroundingSeparator, SoTransformSeparator, FALSE, topSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(seamSurroundingTransform, SoTransform, FALSE, seamSurroundingSeparator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(seamSurroundingGeometry, SoSeparator, FALSE, seamSurroundingSeparator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(seamSeparator2, SoTransformSeparator, FALSE, topSeparator, \x0, TRUE);
	SO_KIT_INIT_INSTANCE();
	
	setupSensor();
	// setup callbacks

	((SoCallback *)GLCallback1.getValue())->setCallback(GLCB1,this);
	((SoCallback *)GLCallback2.getValue())->setCallback(GLCB2,this);
	((SoCallback *)visibilityCallback.getValue())->setCallback(visibilityCB,this);
	// setup default values for seam polygon,
	// for SoCoordinate3 and SoFaceSet
	((SoCube *)seamCube.getValue())->width=1;
	((SoCube *)seamCube.getValue())->height=1;
	((SoCube *)seamCube.getValue())->depth=1;

	setPart("seamSurroundingTransform",(SoTransform *)seamTransform.getValue());

	SoSeparator *surSep = new SoSeparator;

	SoCallback *surroundingCB1 = new SoCallback;
	surroundingCB1->setCallback(clipOff, this);
	surSep->addChild(surroundingCB1);

    SoNode* n = readMemory(invertedBoxIv);
	assert(n);
	//SoWriteAction a; a.apply(n);
	surSep->addChild(n);
	setPart("seamSurroundingGeometry",surSep);
	
	n = readMemory(transparentCubeIv);
	assert(n);
    //a.apply(n);
	surSep->addChild(n);

	SoCallback *surroundingCB2 = new SoCallback;
	surroundingCB1->setCallback(clipOn, this);
	surSep->addChild(surroundingCB2);
	

	((SoShapeHints *)seamHints.getValue())->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	((SoShapeHints *)seamHints.getValue())->shapeType=SoShapeHints::SOLID;

	setPart("seamSeparator2",(SoSeparator *)seamSeparator.getValue());

	((SoAbortGroup *)renderAbortGroup.getValue())->
		abortActions.connectFrom(&skipSeamWorld);

    ((SoAbortGroup *)secondaryBBoxAbort.getValue())->abortActions.setValue( SoAbortGroup::BBOX );
    ((SoAbortGroup *)secondaryBBoxAbort.getValue())->abortActions.setDefault( TRUE );

	visited=FALSE;
    setSearchingChildren(TRUE);
}

/////////////////////////////////////////////////////////////////////
// Name:		calculate
// Purpose:		changes the clip-plane's parameters to the current
//				values
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoCubeSeamKit::calculate(void) {
	
	float x,y,z;
	x=((SoCube *) seamCube.getValue())->width.getValue();
	y=((SoCube *) seamCube.getValue())->height.getValue();
	z=((SoCube *) seamCube.getValue())->depth.getValue();

	x/=2;y/=2;z/=2;

	SbPlane plane(SbVec3f(x,y,-z),SbVec3f(x,y,z),SbVec3f(x,-y,z));
	SbVec3f n = plane.getNormal();
	n.normalize();
	eqn[0][0] = -n[0];
	eqn[0][1] = -n[1];
	eqn[0][2] = -n[2];
	eqn[0][3] = plane.getDistanceFromOrigin();
//	eqn[0][3] -= 0.001;

	SbPlane plane2(SbVec3f(-x,-y,z),SbVec3f(x,y,z),SbVec3f(-x,y,z));
	n = plane2.getNormal();
	n.normalize();
	eqn[1][0] = -n[0];
	eqn[1][1] = -n[1];
	eqn[1][2] = -n[2];
	eqn[1][3] = plane2.getDistanceFromOrigin();
//	eqn[1][3] -= 0.001;

	SbPlane plane3(SbVec3f(-x,y,z),SbVec3f(-x,y,-z),SbVec3f(-x,-y,-z));
	n = plane3.getNormal();
	n.normalize();
	eqn[2][0] = -n[0];
	eqn[2][1] = -n[1];
	eqn[2][2] = -n[2];
	eqn[2][3] = plane3.getDistanceFromOrigin();
//	eqn[2][3] -= 0.001;

	SbPlane plane4(SbVec3f(-x,-y,-z),SbVec3f(-x,y,-z),SbVec3f(x,y,-z));
	n = plane4.getNormal();
	n.normalize();
	eqn[3][0] = -n[0];
	eqn[3][1] = -n[1];
	eqn[3][2] = -n[2];
	eqn[3][3] = plane4.getDistanceFromOrigin();
//	eqn[3][3] -= 0.001;

	SbPlane plane5(SbVec3f(-x,y,z),SbVec3f(x,y,z),SbVec3f(x,y,-z));
	n = plane5.getNormal();
	n.normalize();
	eqn[4][0] = -n[0];
	eqn[4][1] = -n[1];
	eqn[4][2] = -n[2];
	eqn[4][3] = plane5.getDistanceFromOrigin();
//	eqn[4][3] -= 0.001;

	SbPlane plane6(SbVec3f(x,-y,z),SbVec3f(-x,-y,z),SbVec3f(-x,-y,-z));
	n = plane6.getNormal();
	n.normalize();
	eqn[5][0] = -n[0];
	eqn[5][1] = -n[1];
	eqn[5][2] = -n[2];
	eqn[5][3] = plane6.getDistanceFromOrigin();
//	eqn[5][3] -= 0.001;

}

/////////////////////////////////////////////////////////////////////
// Name:		setupSensor
// Purpose:		attaches sensors to coordinates to detect any changes
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoCubeSeamKit::setupSensor(void) {
	seamSensor = new SoNodeSensor(updateClipPlane, (void *)this);
	seamSensor->attach(seamCube.getValue());

	transformSensor = new SoNodeSensor(updateTransform, (void *)this);
	transformSensor->attach(seamSeparator.getValue());
}

/////////////////////////////////////////////////////////////////////
// Name:		updateClipPlane
// Purpose:		callback for changes at seam-coordinates
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoCubeSeamKit::updateClipPlane(void *data, SoSensor *) {

	SoCubeSeamKit *self = (SoCubeSeamKit *)data;
	self->calculate();
	
}


/////////////////////////////////////////////////////////////////////
// Name:		updateTransform
// Purpose:		callback for changes at seam-coordinates
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoCubeSeamKit::updateTransform(void *data, SoSensor *) {

	SoCubeSeamKit *self = (SoCubeSeamKit *)data;
	self->setPart("seamSurroundingTransform",(SoTransform *)self->seamTransform.getValue());
	
		
}


/////////////////////////////////////////////////////////////////////
// Name:		performBoundingBoxTest
// Purpose:		checks if the seam-polygon is visible, stores the
//				screen bounding-box and stops traversal if necessary 
// Parameters:	action ... current SoGLRenderAction
//				seam ... pointer to seam-node
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
SbBool SoCubeSeamKit::performBoundingBoxTest(SoAction *action, SoCubeSeamKit *seam)
{
	// Calculate the min and max values on screen
	
	const SbViewVolume currentVolume = 
		SoViewVolumeElement::get(action->getState());

	SbVec3f coords[8];

	float x,y,z;
	x=((SoCube *) seam->seamCube.getValue())->width.getValue();
	y=((SoCube *) seam->seamCube.getValue())->height.getValue();
	z=((SoCube *) seam->seamCube.getValue())->depth.getValue();
	x/=2;
	y/=2;
	z/=2;

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
	seamPath->append(seam->top2Separator.getValue());
	seamPath->append(seam->seamSeparator.getValue());
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

	if (SoCubeSeamKit::stab > 0)
	{					// return FALSE when seams not overlapping

		if ((minx > SoCubeSeamKit::updateArea[SoCubeSeamKit::stab-1][2]) ||
		    (maxx < SoCubeSeamKit::updateArea[SoCubeSeamKit::stab-1][0]) ||	
		    (miny > SoCubeSeamKit::updateArea[SoCubeSeamKit::stab-1][3]) ||
		    (maxy < SoCubeSeamKit::updateArea[SoCubeSeamKit::stab-1][1])) return TRUE;		
	
		if (SoCubeSeamKit::updateArea[SoCubeSeamKit::stab-1][0] > minx) minx=SoCubeSeamKit::updateArea[SoCubeSeamKit::stab-1][0];
		if (SoCubeSeamKit::updateArea[SoCubeSeamKit::stab-1][2] < maxx) maxx=SoCubeSeamKit::updateArea[SoCubeSeamKit::stab-1][2];
		if (SoCubeSeamKit::updateArea[SoCubeSeamKit::stab-1][1] > miny) miny=SoCubeSeamKit::updateArea[SoCubeSeamKit::stab-1][1];
		if (SoCubeSeamKit::updateArea[SoCubeSeamKit::stab-1][3] < maxy) maxy=SoCubeSeamKit::updateArea[SoCubeSeamKit::stab-1][3];
	}

	if (((maxx-minx) < 0.01) || ((maxy-miny) < 0.01)) return TRUE;

	SoCubeSeamKit::updateArea[SoCubeSeamKit::stab][0]= minx;
	SoCubeSeamKit::updateArea[SoCubeSeamKit::stab][1]= miny;
	SoCubeSeamKit::updateArea[SoCubeSeamKit::stab][2]= maxx;
	SoCubeSeamKit::updateArea[SoCubeSeamKit::stab][3]= maxy;

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
void SoCubeSeamKit::visibilityCB(void* data, SoAction* action)
{
    if (action->isOfType(SoGLRenderAction::getClassTypeId())) 
    {
		SoCubeSeamKit *seam = (SoCubeSeamKit *) data;

			// GEORGES - LUEBKE

		seam->skipSeamWorld = performBoundingBoxTest(action, seam);
		if (seam->checkVisibility.getValue() == FALSE) seam->skipSeamWorld = FALSE;

			// limit recursion depth to 6
		if (SoCubeSeamKit::stab == 7) seam->skipSeamWorld=TRUE;
	}
}

/////////////////////////////////////////////////////////////////////
// Name:		GLCB1
// Purpose:		callback for doing special GL-calls before rendering
//				the seam-polygon
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoCubeSeamKit::GLCB1(void* data, SoAction* action)
{
    if (action->isOfType(SoGLRenderAction::getClassTypeId())) 
    {
		SoCubeSeamKit *seam = (SoCubeSeamKit *) data;

		if (seam->visited==TRUE)
		{

			glDisable(GL_CLIP_PLANE5);
			glDisable(GL_CLIP_PLANE4);
			glDisable(GL_CLIP_PLANE3);
			glDisable(GL_CLIP_PLANE2);
			glDisable(GL_CLIP_PLANE1);
			glDisable(GL_CLIP_PLANE0);
			glPopAttrib();

			glColorMask(FALSE,FALSE,FALSE,FALSE);		// und dabei die Pixel im RGB Buffer nicht aendern	

#ifdef _USE_STENCIL_
			glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);		// Stencil Bits decrementieren				

			if (seam->overlay.getValue() == TRUE)
				glDisable(GL_DEPTH_TEST);				// Z-Buffer Test wieder einschalten			
#else
			glDisable(GL_DEPTH_TEST);				// Z-Buffer Test wieder einschalten			
#endif
			seam->visited=FALSE;

		}
		else
		{
			seam->visited=TRUE;
		
	        // Make my custom GL calls
			glColorMask(FALSE,FALSE,FALSE,FALSE);		// und dabei die Pixel im RGB Buffer nicht aendern	
#ifdef _USE_STENCIL_
			glEnable(GL_STENCIL_TEST);					// Stencil Test einschalten (damit die Bits geaendert werden 
			glStencilFunc(GL_EQUAL,SoCubeSeamKit::stab,0xFFFFFFFF);	// nur in der bisherigen Maske zeichnen, wo die StencilBits == stab sind, der SEAM-Rekursionstiefe 
			glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);		// nur wenn der Z-Buffer-Test erfolgreich ist und die StencilBits stimmen, wird die Maske erhoeht 

			if (seam->overlay.getValue() == TRUE)
				glDisable(GL_DEPTH_TEST);				// Z-Buffer Test wieder einschalten			
#else
			glDisable(GL_DEPTH_TEST);				// Z-Buffer Test wieder einschalten			
#endif
		}
        // Invalidate the state so that a cache is not made
        SoCacheElement::invalidate(action->getState());
    }
}


/////////////////////////////////////////////////////////////////////
// Name:		GLCB2
// Purpose:		callback for doing special GL-calls after rendering
//				the seam-polygon
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoCubeSeamKit::GLCB2(void* data, SoAction* action)
{
    if (action->isOfType(SoGLRenderAction::getClassTypeId())) 
    {
		SoCubeSeamKit *seam = (SoCubeSeamKit *) data;

		if (seam->visited == FALSE)
		{
	        // Make my custom GL calls
			glColorMask(TRUE,TRUE,TRUE,TRUE);			// RGB Buffer wieder dazuschalten			

#ifdef _USE_STENCIL_

			if (seam->overlay.getValue() == TRUE)
					glEnable(GL_DEPTH_TEST);			// Z-Buffer Test wieder einschalten			

			glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);		// Stencil Bits nicht mehr aendern, da ja in der primaeren Welt weitergezeichnet wird 
			glDepthFunc(GL_LEQUAL);
#else
					glEnable(GL_DEPTH_TEST);			// Z-Buffer Test wieder einschalten			
#endif
			SoCubeSeamKit::stab--;							// wir kehren aus dem SEAM zurueck			

#ifdef _USE_STENCIL_
			if(SoCubeSeamKit::stab>0) 
			{
	            glStencilFunc(GL_EQUAL,SoCubeSeamKit::stab,0xFFFFFFFF);	  // entweder die StencilMaske weiterbeachten (noch in einer sekundaeren Welt) 
			} 
			else 
			{
		        glDisable(GL_STENCIL_TEST);				// oder nicht mehr (primaer Welt)			
				glClearStencil(0);
				glClear(GL_STENCIL_BUFFER_BIT);			// Stencil Test einschalten (damit die Bits geaendert werden
			}
#endif
		}
		else
		{
	        // Make my custom GL calls
			SoCubeSeamKit::stab++;							// ab jetzt sind wir ein SEAM tiefer in der SEAM Rekursion 

#ifndef _USE_STENCIL_
			glEnable(GL_DEPTH_TEST);				// Z-Buffer Test wieder einschalten			
#endif
			if (seam->overlay.getValue() == TRUE)
				glEnable(GL_DEPTH_TEST);				// Z-Buffer Test wieder einschalten			

			if (seam->leaveLightsOn.getValue() == TRUE)
			{
				glPushAttrib(GL_CLIP_PLANE5);
				glDisable(GL_CLIP_PLANE5);
			}
			else
			{
				glPushAttrib(GL_CLIP_PLANE0 |GL_CLIP_PLANE1 | GL_CLIP_PLANE2 |
						 GL_CLIP_PLANE3 | GL_CLIP_PLANE4 | GL_CLIP_PLANE5 |
						 GL_LIGHT0 | GL_LIGHT1 | GL_LIGHT2 | GL_LIGHT3 | 
						 GL_LIGHT4 | GL_LIGHT5 | GL_LIGHT6 | GL_LIGHT7);
				glDisable(GL_LIGHT0);
				glDisable(GL_LIGHT1);
				glDisable(GL_LIGHT2);
				glDisable(GL_LIGHT3);
				glDisable(GL_LIGHT4);
				glDisable(GL_LIGHT5);
				glDisable(GL_LIGHT6);
				glDisable(GL_LIGHT7);
				glDisable(GL_CLIP_PLANE5);
				glDisable(GL_CLIP_PLANE4);
				glDisable(GL_CLIP_PLANE3);
				glDisable(GL_CLIP_PLANE2);
				glDisable(GL_CLIP_PLANE1);
				glDisable(GL_CLIP_PLANE0);
			}
#ifdef _USE_STENCIL_

//			glColorMask(TRUE,TRUE,TRUE,TRUE);			// und dabei die Pixel im RGB Buffer nicht aendern	

			glStencilFunc(GL_EQUAL,SoCubeSeamKit::stab,0xFFFFFFFF);	// nur dort zeichnen, wo man das SEAM sieht (Bits==stab) 
			glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);		// und dabei die StencilBits nicht veraendern		
			// clear z-Buffer to far					// wir loeschen den Z-Buffer und den RGB Buffer durch	
			glPushMatrix();								// Zeichnen eines Polygons ueber den ganzen Bildschirm	
			glLoadIdentity();							// dafuer heben wir die momentane Projektion auf und	
			glMatrixMode(GL_PROJECTION);				// stellen eine Normalprojektion ein, denn dann wissen	
			glPushMatrix();								// wissen wir die Koordinaten dieses Polygons		
			glLoadIdentity();							// eigentlich  wuerde es genuegen, ein Polygon von der	
			glMatrixMode(GL_MODELVIEW);					// Groesse der BBox des Seams zu zeichnen, und da wir	
			glOrtho(-1,1,-1,1,-1,1);					// die schon ausgerechnet haben, tun wir das auch	

			if (seam->overlay.getValue() == FALSE)
				glDepthFunc(GL_ALWAYS);					// Kein Z-Buffer Test, sonst wird nix ueberschrieben	
			glDisable(GL_LIGHTING);						// Keine Beleuchtung, einfaerbig SEAM loechen		

			float bgColor[3]={0,0,0};
			seam->backgroundColor.getValue().getValue(bgColor[0],bgColor[1],bgColor[2]);

			glColor3fv(bgColor); // Hintergrund des Seams		

			glBegin(GL_QUADS);							// Viereck drueberzeichnen - der StencilTest sorgt fuer 
			glVertex3f(-1,-1,-1);				        // die richtigen Pixel				      

			glVertex3f(+1,-1,-1);
			glVertex3f(+1,+1,-1);
			glVertex3f(-1,+1,-1);
			glEnd();

			glEnable(GL_LIGHTING);						// Beleuchtung wieder einschalten			

			glPopMatrix();								// und Projektion wieder herstellen			
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);

			glDepthFunc(GL_LEQUAL);						// Z-Buffer Test wieder einschalten			
#endif
			glColorMask(TRUE,TRUE,TRUE,TRUE);			// und dabei die Pixel im RGB Buffer nicht aendern	
			glEnable(GL_CLIP_PLANE0);
			glClipPlane(GL_CLIP_PLANE0, seam->eqn[0]);
			glEnable(GL_CLIP_PLANE1);
			glClipPlane(GL_CLIP_PLANE1, seam->eqn[1]);
			glEnable(GL_CLIP_PLANE2);
			glClipPlane(GL_CLIP_PLANE2, seam->eqn[2]);
			glEnable(GL_CLIP_PLANE3);
			glClipPlane(GL_CLIP_PLANE3, seam->eqn[3]);
			glEnable(GL_CLIP_PLANE4);
			glClipPlane(GL_CLIP_PLANE4, seam->eqn[4]);
			glEnable(GL_CLIP_PLANE5);
			glClipPlane(GL_CLIP_PLANE5, seam->eqn[5]);

			// switch off textures. The state will be restored at the top-separator
			// it is therefore not necessary to switch textures on again

			SoGLTextureEnabledElement::set(action->getState(),FALSE);

		}
        // Invalidate the state so that a cache is not made
        SoCacheElement::invalidate(action->getState());

    }
}

/////////////////////////////////////////////////////////////////////
// Name:		~SoCubeSeamKit
// Purpose:		Destructor of class SoCubeSeamKit
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
SoCubeSeamKit::~SoCubeSeamKit() {
}
