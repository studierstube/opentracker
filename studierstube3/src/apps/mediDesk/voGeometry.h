// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       voGeometry.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube MediDesk
//  CONTENT:    Demo application for volume rendering
//  VERSION:    1.3
// ===========================================================================
//  AUTHORS:    ww      Werner Wohlfahrter
// ===========================================================================
//  HISTORY:    2000 09 26  ww :) 
// ===========================================================================

#ifndef _GEOMETRY_
#define _GEOMETRY_

#include <iostream.h>
#include <GL/gl.h>
#include <Inventor/SbBox.h>
#include <Inventor/SoPath.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/nodes/SoSpotLight.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoShape.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/Xt/viewers/SoXtViewer.h>
#include <Inventor/misc/SoState.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/misc/ivmacros.h>
#include <vo/Enums.h>
#include <vo/Appearance.h>
#include <vo/Geometry.h>
#include <vo/GeometryActions.h>
#include "voStatus.h"

class voGeometry : public SoShape {

   SO_NODE_HEADER(voGeometry);

public:

   // Initializes this class
   static void       initClass(voStatus  *s);

   // Constructor
   voGeometry();
 
   // get bounding box
   void getBBox(SbVec3f &min, SbVec3f &max);

public:
   SoMFInt32          coordIndex;
   SoSFVec3f          samplingPeriodField;
   SoSFEnum           vertexFormat;

protected:

   virtual void       GLRender(SoGLRenderAction *action);
   virtual void       generatePrimitives(SoAction *action);
   virtual void       computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center);
   virtual SoDetail * createPointDetail(SoRayPickAction *action,
			  const SoPrimitiveVertex *v, SoPickedPoint *pp);
private:
   float              samplingPeriod[3];
   int                valuesPerVertex;
   voInterleavedArrayType interleavedArrayFormat;

   double             peqn[4];
   double             head;
   float              panelOffset;
   double             cpeqn[4];

   // transient data
   //voVertexData       *allVertexData;
   voIndexedFaceSet   ***polygonSet;

   // Destructor
   virtual            ~voGeometry();

};
#endif // _GEOMETRY_
