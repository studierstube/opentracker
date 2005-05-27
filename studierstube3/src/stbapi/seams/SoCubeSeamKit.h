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
//  CONTENT:    Class definition for SoCubeSeamKit
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    hw      Hermann Wurnig (wurnig@cg.tuwien.ac.at
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:06:22  gh      last modification
// ===========================================================================

#ifndef _SOCUBESEAMKIT_H_
#define _SOCUBESEAMKIT_H_

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
#include <Inventor/nodes/SoScale.h>
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
#include <stbapi/stbapi.h>
#include <GL/gl.h>
#include <cassert>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

////////////////////////////////////////////////////////////////////
//
//  Class: SoCubeSeamKit
//
//  This class can be used to create seams between two worlds
//  The second world will not be drawn when the seam-polygon
//  is outside the view-frustum. Furthermore, the algorithm of
//  Georges-Luebke is performed for recursive seams.
//
//		When using HMDs, lights must not be turned off
//		(leaveLightsOn = TRUE). 
//  
////////////////////////////////////////////////////////////////////

class STBAPI_API SoCubeSeamKit : public SoBaseKit {
	SO_KIT_HEADER(SoCubeSeamKit);

	SO_KIT_CATALOG_ENTRY_HEADER(firstSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(visibilityCallback);
	SO_KIT_CATALOG_ENTRY_HEADER(renderAbortGroup);
	SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(top2Separator);
	SO_KIT_CATALOG_ENTRY_HEADER(GLCallback1);
	SO_KIT_CATALOG_ENTRY_HEADER(seamSurroundingSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(seamSurroundingTransform);
	SO_KIT_CATALOG_ENTRY_HEADER(seamSurroundingGeometry);
	SO_KIT_CATALOG_ENTRY_HEADER(seamSeparator);
	SO_KIT_CATALOG_ENTRY_HEADER(seamTransform);
	SO_KIT_CATALOG_ENTRY_HEADER(seamHints);
	SO_KIT_CATALOG_ENTRY_HEADER(seamCube);
	SO_KIT_CATALOG_ENTRY_HEADER(GLCallback2);
	SO_KIT_CATALOG_ENTRY_HEADER(secondaryTransform);
	SO_KIT_CATALOG_ENTRY_HEADER(secondaryBBoxAbort);
	SO_KIT_CATALOG_ENTRY_HEADER(secondaryWorld);
	SO_KIT_CATALOG_ENTRY_HEADER(seamSeparator2);

public:
	SoCubeSeamKit();
	static void initClass();
	SoSFBool overlay;			// TRUE -> both worlds are drawn
								// behind seam, FALSE -> only seam world
	SoSFBool leaveLightsOn;		// TRUE -> second world has same lights
								// as first one, FALSE -> second world
								// uses its own lights
	static int stab;
	static float updateArea[10][4];
	void setNewCoordinates(int number, float *vertices);

	SoSFBool checkVisibility;	// TRUE -> if seam is not visible, the
								// second world will not be rendered
	SoSFColor backgroundColor;	// color of the seam"S background

protected:
	GLdouble eqn[6][4];			// equation of clip-plane

	SoSFBool skipSeamWorld;		// connected with abortRenderAction of
								// SoAbortRenderGroup in second world
	SbBool visited;				// used in callbacks
	
	static void updateClipPlane(void *, SoSensor *);
	static void updateTransform(void *, SoSensor *);
	virtual void setupSensor(void);
	virtual void setDefaultOnNonWritingFields();
	void calculate(void);
	~SoCubeSeamKit();

private:
	static SbBool performBoundingBoxTest(SoAction*, SoCubeSeamKit *);
	static void GLCB1(void*, SoAction*);
	static void GLCB2(void*, SoAction*);
	static void visibilityCB(void*, SoAction*);
	SoNodeSensor *seamSensor, *transformSensor;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif
