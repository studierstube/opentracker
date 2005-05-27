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
//  PROJECT:    Studierstube
//  CONTENT:    Class definition for SoSeamKit
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    hw                      Hermann Wurnig (wurnig@cg.tuwien.ac.at
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:06:22  gh      last modification
// ===========================================================================

#ifndef _SOSEAMKIT_H_
#define _SOSEAMKIT_H_

#ifdef WIN32
  #include <windows.h>
#endif

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoGLUpdateAreaElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/fields/SoSFColor.h>
#include <GL/gl.h>
#include <cassert>
#include <stbapi/stbapi.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#define MAX_RECURSION_LEVEL 10

/**
 * Documented at
 * http://www.cg.tuwien.ac.at/groups/vr/studierstube/stbapi/SoSeamKit.html
 *  This class can be used to create seams between two worlds
 *  The second world will not be drawn when the seam-polygon
 *  is outside the view-frustum. Furthermore, the algorithm of
 *  Georges-Luebke is performed for recursive seams.
 *		When using HMDs, lights must not be turned off
 *		(leaveLightsOn = TRUE). 
 */



class STBAPI_API SoSeamKit : public SoBaseKit {
	SO_KIT_HEADER(SoSeamKit);

	SO_KIT_CATALOG_ENTRY_HEADER(firstSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(visibilityCallback);
	SO_KIT_CATALOG_ENTRY_HEADER(renderAbortGroup);
	SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(seamHints);
	SO_KIT_CATALOG_ENTRY_HEADER(top2Separator);
	SO_KIT_CATALOG_ENTRY_HEADER(GLCallback1);
	SO_KIT_CATALOG_ENTRY_HEADER(seamSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(seamGeometry);
	SO_KIT_CATALOG_ENTRY_HEADER(seamTransform);
	SO_KIT_CATALOG_ENTRY_HEADER(seamPolygon);
	SO_KIT_CATALOG_ENTRY_HEADER(GLCallback2);
	SO_KIT_CATALOG_ENTRY_HEADER(secondaryTransform);
	SO_KIT_CATALOG_ENTRY_HEADER(secondaryWorld);
	SO_KIT_CATALOG_ENTRY_HEADER(seamSeparator2);
	SO_KIT_CATALOG_ENTRY_HEADER(stabUpdateCallback);

public:
	SoSeamKit();
	static void initClass();
	SoSFBool overlay;			// TRUE -> both worlds are drawn
								// behind seam, FALSE -> only seam world
	SoSFBool leaveLightsOn;		// TRUE -> second world has same lights
								// as first one, FALSE -> second world
								// uses its own lights
	static int maxRecursionDepth;
								// maximum recursion depth of seams
	
	static int stab;
	static int visitedStab;
	static float updateArea[MAX_RECURSION_LEVEL][4];
	static SbBool visited[MAX_RECURSION_LEVEL];
								// used in callbacks

	void setNewCoordinates(int number, float *vertices);

	SoSFBool checkVisibility;	// TRUE -> if seam is not visible, the
								// second world will not be rendered
	SoSFColor	backgroundColor;

	void setBGColor(SbColor color);

	SbColor backColor[MAX_RECURSION_LEVEL];	
								// color of the seam's background

    static void clearStencilPlanes();
                                //reset stencil buffer before 1st seam use

protected:
	GLdouble eqn[4];			// equation of clip-plane

	SoSFBool skipSeamWorld;		// connected with abortRenderAction of
								// SoAbortRenderGroup in second world
	
	static int initClassAt;

	static void updateClipPlane(void *, SoSensor *);
	static void updateColor(void *, SoSensor *);
	virtual void setupSensor(void);
	virtual void setDefaultOnNonWritingFields();
	void calculate(void);
	~SoSeamKit();

private:

	static SbBool performBoundingBoxTest(SoAction*, SoSeamKit *);
	static void stabUpdateCB(void*, SoAction*);
  	static void GLCB1(void*, SoAction*);
	static void GLCB2(void*, SoAction*);
	static void visibilityCB(void*, SoAction*);
	SoFieldSensor *geometrySensor, *colorSensor;
	SoNodeSensor *seamSensor;
    // stores the stencilFunc reference value before entering the seam kit
    int previousStencilRef;
	SbBool disableStencil;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
