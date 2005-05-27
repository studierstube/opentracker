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
/** Class implementation for SoSeamKit
  *
  * @author Hermann Wurnig 
  *
  * $Id: SoSeamKit.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <Inventor/nodes/SoCube.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>

#include <stbapi/seams/SoSeamKit.h>
#include <stbapi/misc/SoAbortGroup.h>

int SoSeamKit::initClassAt = 0;

SO_KIT_SOURCE(SoSeamKit);

// default values of clip-plane and seam-polygon

const int defaultNumVertices = 4;
const float defaultPolygon[defaultNumVertices][3] = {
	{ -1.0, -1.0, 0.0 },
	{ 1.0, -1.0, 0.0 },
	{ 1.0, 1.0, 0.0 },
	{ -1.0, 1.0, 0.0 }
};


// depth of recursion
int SoSeamKit::stab;
int SoSeamKit::visitedStab;

int SoSeamKit::maxRecursionDepth;


SbBool SoSeamKit::visited[MAX_RECURSION_LEVEL];

// screen-boundingbox-data
float SoSeamKit::updateArea[MAX_RECURSION_LEVEL][4];

/////////////////////////////////////////////////////////////////////
// Name:		initClass
// Purpose:		initialisation of class SoSeamKit
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoSeamKit::initClass(void) {
	
	if (SoSeamKit::initClassAt == 1) return;

	SoSeamKit::initClassAt = 1;

	SoSeamKit::stab = 0;
	SoSeamKit::visitedStab = 0;
    SoSeamKit::maxRecursionDepth = 8;
    
	for (int i=0;i<SoSeamKit::maxRecursionDepth;i++)
		SoSeamKit::visited[i]=FALSE;

	SO_KIT_INIT_CLASS(SoSeamKit, SoBaseKit, "BaseKit");
}

void SoSeamKit::setDefaultOnNonWritingFields(void) {
	SoBaseKit::setDefaultOnNonWritingFields();
	topSeparator.setDefault(TRUE);
	GLCallback1.setDefault(TRUE);
	GLCallback2.setDefault(TRUE);
	top2Separator.setDefault(TRUE);
	seamSeparator.setDefault(TRUE);
	seamHints.setDefault(TRUE);
	seamPolygon.setDefault(TRUE);
	seamSeparator2.setDefault(TRUE);
	renderAbortGroup.setDefault(TRUE);
	firstSeparator.setDefault(TRUE);
	visibilityCallback.setDefault(TRUE);
}

/////////////////////////////////////////////////////////////////////
// Name:		SoSeamKit
// Purpose:		Constructor of class SoSeamKit. Default seam-geometry
//				clip-plane and sensors are set.
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
SoSeamKit::SoSeamKit() 
:
previousStencilRef(0), disableStencil(TRUE)
{
	SO_KIT_CONSTRUCTOR(SoSeamKit);

	SO_KIT_ADD_FIELD(overlay, (FALSE));
	SO_KIT_ADD_FIELD(leaveLightsOn, (TRUE));
	SO_KIT_ADD_FIELD(skipSeamWorld, (FALSE));
	SO_KIT_ADD_FIELD(checkVisibility, (TRUE));
	SO_KIT_ADD_FIELD(overlay, (FALSE));
	SO_KIT_ADD_FIELD(backgroundColor, (0.7f,0.7f,0.7f));

	SO_KIT_ADD_CATALOG_ENTRY(firstSeparator, SoSeparator, FALSE, this, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(visibilityCallback, SoCallback, FALSE, firstSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(renderAbortGroup, SoAbortGroup, FALSE, firstSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, FALSE, renderAbortGroup, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(top2Separator, SoSeparator, FALSE, topSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(seamSeparator, SoTransformSeparator, FALSE, top2Separator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(seamTransform, SoTransform, FALSE, seamSeparator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(seamGeometry, SoCoordinate3, FALSE, seamSeparator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(seamHints, SoShapeHints, FALSE, seamSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(GLCallback1, SoCallback, FALSE, seamSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(seamPolygon, SoFaceSet, FALSE, seamSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(GLCallback2, SoCallback, FALSE, seamSeparator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(secondaryTransform, SoTransform, TRUE, top2Separator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(secondaryWorld, SoSeparator, FALSE, top2Separator, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(stabUpdateCallback, SoCallback, FALSE, top2Separator, \x0, FALSE);
	SO_KIT_ADD_CATALOG_ENTRY(seamSeparator2, SoTransformSeparator, FALSE, topSeparator, \x0, TRUE);
	SO_KIT_INIT_INSTANCE();
	
	setupSensor();
	// setup callbacks
	((SoCallback *)GLCallback1.getValue())->setCallback(GLCB1,this);
	((SoCallback *)GLCallback2.getValue())->setCallback(GLCB2,this);
	((SoCallback *)visibilityCallback.getValue())->setCallback(visibilityCB,this);
	((SoCallback *)stabUpdateCallback.getValue())->setCallback(stabUpdateCB,this);
	// setup default values for seam polygon,
	// for SoCoordinate3 and SoFaceSet
	((SoCoordinate3 *)seamGeometry.getValue())->point.setValues(0, defaultNumVertices, defaultPolygon);
	((SoFaceSet *)seamPolygon.getValue())->numVertices.set1Value(0, defaultNumVertices);
	// setup default clipping plane and shape-hints
	((SoShapeHints *)seamHints.getValue())->vertexOrdering=SoShapeHints::COUNTERCLOCKWISE;
	((SoShapeHints *)seamHints.getValue())->shapeType=SoShapeHints::SOLID;
	setPart("seamSeparator2",(SoSeparator *)seamSeparator.getValue());

	((SoAbortGroup *)renderAbortGroup.getValue())->
		abortActions.connectFrom(&skipSeamWorld);

	backgroundColor.setValue(0.7f,0.7f,0.7f);
    setSearchingChildren(TRUE);
}

/////////////////////////////////////////////////////////////////////
// Name:		~SoSeamKit
// Purpose:		Destructor of class SoSeamKit. Removes sensors!
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
SoSeamKit::~SoSeamKit()
{
	if (geometrySensor) delete geometrySensor;
	if (colorSensor) delete colorSensor;
	if (seamSensor) delete seamSensor;
}
/////////////////////////////////////////////////////////////////////
// Name:		setNewCoordinates
// Purpose:		changes the seam-geometry to new values. It is 
//				possible to exchange the SoCoordinate-node as well
// Parameters:	number ... number of vertices
//				coords ... vertex-data (x0,y0,z0,x1,y1,z1,x2,...)
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoSeamKit::setNewCoordinates(int number, float *coords)
{
	for (int i=0;i<number ;i++)
		((SoCoordinate3 *)seamGeometry.getValue())->point.
			set1Value(i, coords[3*i], coords[3*i+1], coords[3*i+2]);

	((SoFaceSet *)seamPolygon.getValue())->numVertices.set1Value(0, defaultNumVertices);	
}

/////////////////////////////////////////////////////////////////////
// Name:		calculate
// Purpose:		changes the clip-plane's parameters to the current
//				values
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoSeamKit::calculate(void) {
	const SbVec3f *firstPoints = ((SoCoordinate3 *)seamGeometry.getValue())->point.getValues(0);
	SbPlane plane(*firstPoints, *(firstPoints+1), *(firstPoints+2));
	SbVec3f n = plane.getNormal();
	n.normalize();
	eqn[0] = -n[0];
	eqn[1] = -n[1];
	eqn[2] = -n[2];
	eqn[3] = plane.getDistanceFromOrigin();
	eqn[3] += 0.001;
}

/////////////////////////////////////////////////////////////////////
// Name:		setupSensor
// Purpose:		attaches sensors to coordinates to detect any changes
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoSeamKit::setupSensor(void) {
	geometrySensor = new SoFieldSensor(updateClipPlane, (void *)this);
	geometrySensor->attach(&((SoCoordinate3 *)seamGeometry.getValue())->point);

	colorSensor = new SoFieldSensor(updateColor, (void *)this);
	colorSensor->attach(&backgroundColor);

	seamSensor = new SoNodeSensor(updateClipPlane, (void *)this);
	seamSensor->attach(seamSeparator.getValue());
}

/////////////////////////////////////////////////////////////////////
// Name:		updateColor
// Purpose:		callback for changes of the color field
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoSeamKit::updateColor(void *data, SoSensor *sensor) {
	SoSeamKit *self = (SoSeamKit *)data;

	float r,g,b;
	
	((self->backgroundColor).getValue()).getValue(r,g,b);

	for (int i=0;i<SoSeamKit::maxRecursionDepth;i++)
	{
		self->backColor[i].setValue(r-(r*i)/SoSeamKit::maxRecursionDepth,
			g-(g*i)/SoSeamKit::maxRecursionDepth,b-(b*i)/SoSeamKit::maxRecursionDepth);
	}
}

/////////////////////////////////////////////////////////////////////
// Name:		setBGColor
// Purpose:		sets the background color of the seam
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoSeamKit::setBGColor(SbColor color)
{
	backgroundColor.setValue(color);
}


/////////////////////////////////////////////////////////////////////
// Name:		updateClipPlane
// Purpose:		callback for changes at seam-coordinates
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoSeamKit::updateClipPlane(void *data, SoSensor *sensor) {
	SoSeamKit *self = (SoSeamKit *)data;
	((SoFaceSet *)self->seamPolygon.getValue())->numVertices.set1Value(0, ((SoCoordinate3 *)self->seamGeometry.getValue())->point.getNum());
	self->calculate();

	if (sensor == (SoSensor *)self->seamSensor)
	{
		self->geometrySensor->detach();
		self->geometrySensor->attach(&((SoCoordinate3 *)self->seamGeometry.getValue())->point);
	}
}


/////////////////////////////////////////////////////////////////////
// Name:		performBoundingBoxTest
// Purpose:		checks if the seam-polygon is visible, stores the
//				screen bounding-box and stops traversal if necessary 
// Parameters:	action ... current SoGLRenderAction
//				seam ... pointer to seam-node
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
SbBool SoSeamKit::performBoundingBoxTest(SoAction *action, SoSeamKit *seam)
{
	// Calculate the min and max values on screen
	
	const SbViewVolume currentVolume = 
		SoViewVolumeElement::get(action->getState());

	const SbVec3f *coords = 
		((SoCoordinate3 * )seam->seamGeometry.getValue())->point.getValues(0);
	int count = ((SoCoordinate3 * )seam->seamGeometry.getValue())->point.getNum();
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

	for (int i=0; i < count; i++)
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

	if (SoSeamKit::stab > 0)
	{					// return FALSE when seams not overlapping

		if ((minx > SoSeamKit::updateArea[SoSeamKit::stab-1][2]) ||
		    (maxx < SoSeamKit::updateArea[SoSeamKit::stab-1][0]) ||	
		    (miny > SoSeamKit::updateArea[SoSeamKit::stab-1][3]) ||
		    (maxy < SoSeamKit::updateArea[SoSeamKit::stab-1][1])) return TRUE;		
	
		if (SoSeamKit::updateArea[SoSeamKit::stab-1][0] > minx) minx=SoSeamKit::updateArea[SoSeamKit::stab-1][0];
		if (SoSeamKit::updateArea[SoSeamKit::stab-1][2] < maxx) maxx=SoSeamKit::updateArea[SoSeamKit::stab-1][2];
		if (SoSeamKit::updateArea[SoSeamKit::stab-1][1] > miny) miny=SoSeamKit::updateArea[SoSeamKit::stab-1][1];
		if (SoSeamKit::updateArea[SoSeamKit::stab-1][3] < maxy) maxy=SoSeamKit::updateArea[SoSeamKit::stab-1][3];
	}

	if (((maxx-minx) < 0.01) || ((maxy-miny) < 0.01)) return TRUE;

	SoSeamKit::updateArea[SoSeamKit::stab][0]= minx;
	SoSeamKit::updateArea[SoSeamKit::stab][1]= miny;
	SoSeamKit::updateArea[SoSeamKit::stab][2]= maxx;
	SoSeamKit::updateArea[SoSeamKit::stab][3]= maxy;

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
void SoSeamKit::visibilityCB(void* data, SoAction* action)
{
    if (action->isOfType(SoGLRenderAction::getClassTypeId())) 
    {      
		SoSeamKit *seam = (SoSeamKit *) data;

		// RCS 20-12-2002
		if (glIsEnabled(GL_STENCIL_TEST))
			seam->disableStencil = FALSE;
		else if (SoSeamKit::stab == 0)
		{
			glClearStencil(0);
			glClear(GL_STENCIL_BUFFER_BIT);
		}

        // store current reference value used for stencil test
        glGetIntegerv(GL_STENCIL_REF,&seam->previousStencilRef);
        // The first (lowest) 3 bits are exclusively used and managed by the seam kit,
        // therefore the first 3 bits are masked out (only higher bits represent the 
        // "external" reference value).
        seam->previousStencilRef = seam->previousStencilRef & 0xFFF8;

		// GEORGES - LUEBKE

		seam->skipSeamWorld = performBoundingBoxTest(action, seam);

		if (seam->checkVisibility.getValue() == FALSE) seam->skipSeamWorld = FALSE;

			// limit recursion depth
		if (SoSeamKit::stab == SoSeamKit::maxRecursionDepth-1)
		{ 
			seam->skipSeamWorld=TRUE;
		}

	}
}


void SoSeamKit::stabUpdateCB(void* , SoAction * action)
{
    if (action->isOfType(SoGLRenderAction::getClassTypeId())) 
    {
		SoSeamKit::visitedStab--;

	}
/*
    if (action->isOfType(SoGLRenderAction::getClassTypeId())) 
		printf("Render-Action\n");
	else
    if (action->isOfType(SoGetBoundingBoxAction::getClassTypeId())) 
		printf("GetBoundingBox-Action\n");
	else
    if (action->isOfType(SoGetMatrixAction::getClassTypeId())) 
		printf("GetMatrix-Action\n");
	else
    if (action->isOfType(SoHandleEventAction::getClassTypeId())) 
		printf("HandleEvent-Action\n");
	else
    if (action->isOfType(SoPickAction::getClassTypeId())) 
		printf("Pick-Action\n");
	else
    if (action->isOfType(SoRayPickAction::getClassTypeId())) 
		printf("RayPick-Action\n");
	else
    if (action->isOfType(SoSearchAction::getClassTypeId())) 
		printf("Search-Action\n");
	else
    if (action->isOfType(SoWriteAction::getClassTypeId())) 
		printf("Write-Action\n");
	else
		printf("unknown Action\n");
*/

}


/////////////////////////////////////////////////////////////////////
// Name:		GLCB1
// Purpose:		callback for doing special GL-calls before rendering
//				the seam-polygon
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoSeamKit::GLCB1(void* data, SoAction* action)
{
    if (action->isOfType(SoGLRenderAction::getClassTypeId())) 
    {
		SoSeamKit *seam = (SoSeamKit *) data;

		if (SoSeamKit::visited[SoSeamKit::visitedStab]==TRUE)
		{
			glDisable(GL_CLIP_PLANE5);
			glPopAttrib();

			glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);		// Stencil Bits decrementieren				
			glColorMask(FALSE,FALSE,FALSE,FALSE);		// und dabei die Pixel im RGB Buffer nicht aendern	

			if (seam->overlay.getValue() == TRUE)
				glDisable(GL_DEPTH_TEST);				// Z-Buffer Test wieder einschalten			

			SoSeamKit::visited[SoSeamKit::visitedStab]=FALSE;

		}
		else
		{
			SoSeamKit::visited[SoSeamKit::visitedStab]=TRUE;
		
	        // Make my custom GL calls
			glColorMask(FALSE,FALSE,FALSE,FALSE);		// und dabei die Pixel im RGB Buffer nicht aendern	
			glEnable(GL_STENCIL_TEST);					// Stencil Test einschalten (damit die Bits geaendert werden 			
            glStencilFunc(GL_EQUAL,seam->previousStencilRef | SoSeamKit::stab,0xFFFF);	// nur in der bisherigen Maske zeichnen, wo die StencilBits == stab sind, der SEAM-Rekursionstiefe 
			glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);		// nur wenn der Z-Buffer-Test erfolgreich ist und die StencilBits stimmen, wird die Maske erhoeht 

			if (seam->overlay.getValue() == TRUE)
				glDisable(GL_DEPTH_TEST);				// Z-Buffer Test wieder einschalten			
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
void SoSeamKit::GLCB2(void* data, SoAction* action)
{
    if (action->isOfType(SoGLRenderAction::getClassTypeId())) 
    {
		SoSeamKit *seam = (SoSeamKit *) data;

		if (SoSeamKit::visited[SoSeamKit::visitedStab] == FALSE)
		{
	        // Make my custom GL calls
			glColorMask(TRUE,TRUE,TRUE,TRUE);			// RGB Buffer wieder dazuschalten			

			if (seam->overlay.getValue() == TRUE)
					glEnable(GL_DEPTH_TEST);			// Z-Buffer Test wieder einschalten			

			glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);		// Stencil Bits nicht mehr aendern, da ja in der primaeren Welt weitergezeichnet wird 
			glDepthFunc(GL_LEQUAL);

			SoSeamKit::stab--;							// wir kehren aus dem SEAM zurueck			
 			
			glStencilFunc(GL_EQUAL, seam->previousStencilRef | SoSeamKit::stab, 0xFFFF);
 			 
			if(SoSeamKit::stab == 0 &&  seam->disableStencil)
 		        glDisable(GL_STENCIL_TEST);			
		}
		else
		{
	        // Make my custom GL calls
			SoSeamKit::stab++;							// ab jetzt sind wir ein SEAM tiefer in der SEAM Rekursion 
			SoSeamKit::visitedStab++;							// ab jetzt sind wir ein SEAM tiefer in der SEAM Rekursion 

			if (seam->overlay.getValue() == TRUE)
				glEnable(GL_DEPTH_TEST);				// Z-Buffer Test wieder einschalten			

			if (seam->leaveLightsOn.getValue() == TRUE)
			{
				glPushAttrib(GL_CLIP_PLANE5);
				glDisable(GL_CLIP_PLANE5);
			}
			else
			{
				glPushAttrib(GL_CLIP_PLANE5 | GL_LIGHT0 | GL_LIGHT1 | GL_LIGHT2 | GL_LIGHT3 | 
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
			}

			glColorMask(TRUE,TRUE,TRUE,TRUE);			// und dabei die Pixel im RGB Buffer nicht aendern			
            glStencilFunc(GL_EQUAL,seam->previousStencilRef | SoSeamKit::stab,0xFFFF);	// nur dort zeichnen, wo man das SEAM sieht (Bits==stab) 
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

			float hColor[3]={0,0,0};
			seam->backColor[SoSeamKit::stab-1].getValue(hColor[0],hColor[1],hColor[2]);

			glColor3fv(hColor); // Hintergrund des Seams		

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

			glEnable(GL_CLIP_PLANE5);
			glClipPlane(GL_CLIP_PLANE5, seam->eqn);

			// switch off textures. The state will be restored at the top-separator
			// it is therefore not necessary to switch textures on again

			SoGLTextureEnabledElement::set(action->getState(),FALSE);

		}
        // Invalidate the state so that a cache is not made
        SoCacheElement::invalidate(action->getState());

    }
}

/////////////////////////////////////////////////////////////////////
// Name:		clearStencilPlanes
// Purpose:		reset stencil planes before 1st use of a seam
// Parameters:	---
// Returnvalue: ---
///////////////////////////////////////////////////////////////////// 
void SoSeamKit::clearStencilPlanes()
{ 
	glDisable(GL_STENCIL_TEST);     //switch off stencil test
    glClearStencil(0);              //and clear stencil buffer
    glClear(GL_STENCIL_BUFFER_BIT); //switch on Stencil Test 
}
