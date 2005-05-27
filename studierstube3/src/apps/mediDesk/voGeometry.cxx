// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       voGeometry.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube MediDesk
//  CONTENT:    Demo application for volume rendering
//  VERSION:    1.3
// ===========================================================================
//  AUTHORS:    ww      Werner Wohlfahrter
// ===========================================================================
//  HISTORY:    2000 09 26  ww :) 
// ===========================================================================


#include "voGeometry.h"
#include "voAppearance.h"
#include "voAppearanceElement.h"
#include "voStatus.h"
//#include "studierstube.h"

//jy
#include <Inventor/elements/SoModelMatrixElement.h>
//extern voStatus *geoStatus;
voStatus *geoStatus;
SoClipPlane       *clipPlane;

SO_NODE_SOURCE(voGeometry);


//-----------------------------------------------------------------------------
// This initializes the voGeometry class.

void voGeometry::initClass(voStatus  *s)
{
   //cout << "voGeometry::initClass running ..." << endl;

   // Initialize type id variables
   SO_NODE_INIT_CLASS(voGeometry, SoShape, "Shape");

   geoStatus = s;
}

//-----------------------------------------------------------------------------
// Constructor

voGeometry::voGeometry()
{
   //cout << "voGeometry::constructor running ..." << endl;

   // this is a quickly defined enum which
   // shortens the names (and removes the ::,
   // which for some reason screws Inventor files
   // up when reading) of the different vi
   // vertex types
   enum {
          DEFAULT     = voInterleavedArrayTypeScope::DEFAULT,
          V3F         = voInterleavedArrayTypeScope::V3F,
          T3F_V3F     = voInterleavedArrayTypeScope::T3F_V3F,
          C3F_V3F     = voInterleavedArrayTypeScope::C3F_V3F,
          C4F_V3F     = voInterleavedArrayTypeScope::C4F_V3F,
          T3F_C3F_V3F = voInterleavedArrayTypeScope::T3F_C3F_V3F,
          T3F_C4F_V3F = voInterleavedArrayTypeScope::T3F_C4F_V3F,
          USER_V3F    = voInterleavedArrayTypeScope::USER_V3F
   };

   SO_NODE_CONSTRUCTOR(voGeometry);
   SO_NODE_ADD_FIELD(coordIndex,(-1));
   SO_NODE_ADD_FIELD(samplingPeriodField,(1.0, 1.0, 1.0));
   SO_NODE_DEFINE_ENUM_VALUE(voInterleavedArrayType, DEFAULT);
   SO_NODE_DEFINE_ENUM_VALUE(voInterleavedArrayType, V3F);
   SO_NODE_DEFINE_ENUM_VALUE(voInterleavedArrayType, T3F_V3F);
   SO_NODE_DEFINE_ENUM_VALUE(voInterleavedArrayType, C3F_V3F);
   SO_NODE_DEFINE_ENUM_VALUE(voInterleavedArrayType, C4F_V3F);
   SO_NODE_DEFINE_ENUM_VALUE(voInterleavedArrayType, T3F_C3F_V3F);
   SO_NODE_DEFINE_ENUM_VALUE(voInterleavedArrayType, T3F_C4F_V3F);
   SO_NODE_DEFINE_ENUM_VALUE(voInterleavedArrayType, USER_V3F);
   SO_NODE_SET_SF_ENUM_TYPE(vertexFormat, voInterleavedArrayType);

   SO_NODE_ADD_FIELD(vertexFormat, (V3F));
   // C4F_V3F neccessary for slices!
   //SO_NODE_ADD_FIELD(vertexFormat, (C4F_V3F));

}

//-----------------------------------------------------------------------------
// Destructor

voGeometry::~voGeometry()
{
}

//-----------------------------------------------------------------------------
// Implements the SoGLRenderAction for the voGeometry node.

void voGeometry::GLRender(SoGLRenderAction *action)
{
   //cout << "voGeometry::GLRender running ..." << endl;

   voIndexedTetraSet *tmpTetraSet;

   // Access the state from the action
   SoState  *state = action->getState();

   const SoCoordinateElement *coord = SoCoordinateElement::getInstance(state);
   const SoTextureCoordinateElement * texCoord = SoTextureCoordinateElement::getInstance(state);

   // remember if lighting is enabled; equivalently, use glPushAttrib(3G)
   const SoLightModelElement::Model lighting = SoLightModelElement::get(state);

   if( lighting != SoLightModelElement::BASE_COLOR )  //  PHONG or BASE_COLOR
      glDisable(GL_LIGHTING);

   int newCache = 0;

   // get values per vertex from vertex type
   // and create TetraSet while traversing the first time!
   if ((geoStatus->getTetraSet() == NULL) || 
       (interleavedArrayFormat != (voInterleavedArrayType) vertexFormat.getValue())) {
     
      interleavedArrayFormat = (voInterleavedArrayType) vertexFormat.getValue();

      switch (interleavedArrayFormat) {
      case voInterleavedArrayTypeScope::V3F:
         //cout << "interleavedArrayFormat <V3F>" << endl;
         valuesPerVertex = 3;
         break;
      case voInterleavedArrayTypeScope::T3F_V3F:
         //cout << "interleavedArrayFormat <T3F_V3F>" << endl;
         valuesPerVertex = 6;
         break;
      case voInterleavedArrayTypeScope::C3F_V3F:
         //cout << "interleavedArrayFormat <C3F_V3F>" << endl;
         valuesPerVertex = 6;
         break;
      case voInterleavedArrayTypeScope::C4F_V3F:
         //cout << "interleavedArrayFormat <C4F_V3F>" << endl;
         valuesPerVertex = 7;
         break;
      case voInterleavedArrayTypeScope::T3F_C3F_V3F:
         //cout << "interleavedArrayFormat <T3F_C3F_V3F>" << endl;
         valuesPerVertex = 9;
         break;
      case voInterleavedArrayTypeScope::T3F_C4F_V3F:
         //cout << "interleavedArrayFormat <T3F_C4F_V3F>" << endl;
         valuesPerVertex = 10;
         break;
      default:
         //cout << "interleavedArrayFormat <DEFAULT>" << endl;
         if( interleavedArrayFormat != voInterleavedArrayTypeScope::V3F ) {
            voError::setErrorNumber(voErrorTypeScope::UNSUPPORTED);
            voError::setErrorString("voGeometry::GLRender(): Unsuported interleavedArrayFormat");
            voError::callErrorHandler();
            return;
         }
      }
      geoStatus->setInterleavedArrayFormat((int)interleavedArrayFormat);
     
      tmpTetraSet = new voIndexedTetraSet(coord->getNum(),valuesPerVertex,coordIndex.getNum());
      geoStatus->setTetraSet(tmpTetraSet);
      assert(geoStatus->getTetraSet());

      newCache = 1;

   }

   const SoLazyElement *material = SoLazyElement::getInstance(state);

   // get the vertices
   int j1;
   for(j1=0;j1<coord->getNum();j1++)
   {
      SbVec3f point;
      SbVec4f texPoint;
      SbColor diffuse;
      float transparency;

      tmpTetraSet = geoStatus->getTetraSet();
      float *ptr = (*tmpTetraSet->getVertexData())[j1];

      // read texture coord, in vertexes have them
      if (interleavedArrayFormat == voInterleavedArrayTypeScope::T3F_V3F || 
          interleavedArrayFormat == voInterleavedArrayTypeScope::T3F_C3F_V3F ||
          interleavedArrayFormat == voInterleavedArrayTypeScope::T3F_C4F_V3F) {
         texPoint = texCoord->get4(j1).getValue();
         *ptr++ = texPoint[0];
         *ptr++ = texPoint[1];
         *ptr++ = texPoint[2];
      }


      // read diffuse color for verts, if they have color
      if (interleavedArrayFormat == voInterleavedArrayTypeScope::C3F_V3F ||
	  interleavedArrayFormat == voInterleavedArrayTypeScope::C4F_V3F || 
	  interleavedArrayFormat == voInterleavedArrayTypeScope::T3F_C3F_V3F ||
	  interleavedArrayFormat == voInterleavedArrayTypeScope::T3F_C4F_V3F) {
         diffuse = material->getDiffuse(state, j1);
         *ptr++ = diffuse[0];
         *ptr++ = diffuse[1];
         *ptr++ = diffuse[2];

      }

      // read alpha component of color, if vertex has alpha
      // component
      if (interleavedArrayFormat == voInterleavedArrayTypeScope::C4F_V3F ||
	  interleavedArrayFormat == voInterleavedArrayTypeScope::T3F_C4F_V3F) {
         transparency = material->getTransparency(state, j1);
         *ptr++ = 1 - transparency;
      }

      // read vertex position
      point = coord->get3(j1).getValue();
      *ptr++ = point[0];
      *ptr++ = point[1];
      *ptr++ = point[2];


   } // for(j1

   tmpTetraSet->getVertexData()->setCount(coord->getNum());
   //geoStatus->aTetraSet->getVertexData()->setCount(coord->getNum());

   // done getting vertex data

   // get all index values from Inventor state
   // NOTE: privide copy-free interface
   for(j1=0;j1<coordIndex.getNum();j1++)
      (*tmpTetraSet->getIndices())[j1] = coordIndex[j1];
   tmpTetraSet->getIndices()->setCount(coordIndex.getNum());
   //geoStatus->aTetraSet->getIndices()->setCount(coordIndex.getNum());

   // get current voBrickSetCollection from state
   voAppearance *appearance = voAppearanceElement::get(action->getState());
   voBrickSetCollection *aVolume = appearance->getVolume();
   int maxSamplesNumber = geoStatus->getMaxSamplesNumber() * 2.0;

   // get other fields
   for (j1 = 0; j1 < 3; j1++) {
      samplingPeriod[j1] = samplingPeriodField.getValue()[j1];
   }

   // get current material from state
   SbColor diffuse = material->getDiffuse(state,0);
   glColor3f(diffuse[0],diffuse[1],diffuse[2]);

   voSamplingMode samplingMode = voSamplingModeScope::BEST_PERFORMANCE;

   //maxSamplesNumber = 256;

   glEnable(GL_DEPTH_TEST);

   // create cache for drawing, if neccessary
   static voDrawActionCache *aCache = NULL;
   if(newCache ) {
      aCache = new voDrawActionCache(
                     tmpTetraSet, 
                     aVolume,
                     maxSamplesNumber,
                     samplingMode,
                     tmpTetraSet->getVertexData()->getValuesPerVertex(),
                     250000);
      aCache->setThreshold(2);
   }

   // --------
   // CLIPPING
   // 
   SbVec3f eye;
   SbVec3f padNormal;
   SbVec3f eyeVec;
   SbVec3f xyza(0.0,0.0,0.0);
   SbVec3f xyzb(0.0,0.0,1.0);
   SbVec3f xyzc(-1.0,0.0,0.0);
   SbVec3f xyz0;
   SbVec3f xyz1;
   SbVec3f xyz2;
   SbVec3f e1;
   SbVec3f e2;
   SbVec3f n;

   SoTrakEngine* trakSource = geoStatus->getHmdEngine();
   SoTransform * trans = new SoTransform();
   trans->translation.connectFrom(&trakSource->translation);
   trans->rotation.connectFrom(&trakSource->rotation);
   SbRotation eyeR = trans->rotation.getValue();
   SbVec3f    eyeT = trans->translation.getValue();
   trans->translation.disconnect();
   trans->rotation.disconnect();

   eyeVec = eyeT;
   eyeVec.normalize();

   trakSource = geoStatus->getPipEngine();
   trans->translation.connectFrom(&trakSource->translation);
   trans->rotation.connectFrom(&trakSource->rotation);
   SbRotation pipR = trans->rotation.getValue();
   SbVec3f    pipT = trans->translation.getValue();
   trans->translation.disconnect();
   trans->rotation.disconnect();

   // multiply pip rotation matrix and pad points and store it in xyz{0,1,2}
   pipR.multVec(xyza,xyz0);
   pipR.multVec(xyzb,xyz1);
   pipR.multVec(xyzc,xyz2);
   // add piptransformation to every pad point
   xyz0+=pipT;
   xyz1+=pipT;
   xyz2+=pipT;
 
   // calculate plane normal
   e1 = xyz1-xyz0;
   e2 = xyz2-xyz0;
   n  = e1.cross(e2);
   n.normalize();
 
   if (eyeVec.dot(n) > 0) {                 // pad is in cutting position

      AGETNODE(sliceContent,SoSeparator,"SLICE_SEP");
      
      if (geoStatus->getAttenuateStatus() && (sliceContent->getNumChildren() > 0)) {
	 //printf ("+++ slice available and pad upside down !!\n");

         // switch pip to 2
	 AGETNODE(sw,SoSwitch,"PIP_VISIBLE_SWITCH");
	 sw->whichChild = 2;

         //AGETNODE(pipSlice,SoSeparator,"PIP_SLICE");
         //cout << "slice num <" << pipSlice->getNumChildren() << ">" << endl;

	 geoStatus->setPipFlipStatus(true);

      } else {

	 //printf ("+++ pad is in cutting position !!\n");
//jt
      SbPlane oClipPlane = SbPlane(xyz0, xyz2, xyz1);
      if (geoStatus->getOrthoStatus())
         {
            // Orthogonal slicing mode. Orient the clipping plane
            // so that it is parallel to the sides of the volume
            // bounding box
         SbMatrix oMat = SoModelMatrixElement::get(state);
         // Note: we cannot use SbRotation(SbMatrix) to get the rotation
            // component of the model matrix because it will also contain
            // the scale value in that case.
         SbVec3f vTmpTrans, vTmpScale;
         SbRotation oModelRot, oTmpSo;
            oMat.getTransform(vTmpTrans, oModelRot, vTmpScale, oTmpSo);
         // Transform base vectors of volume coordinate system
         SbVec3f vVcsX = SbVec3f(1.0, 0.0, 0.0);
         SbVec3f vVcsY = SbVec3f(0.0, 1.0, 0.0);
         SbVec3f vVcsZ = SbVec3f(0.0, 0.0, 1.0);
         SbVec3f vVcsXt, vVcsYt, vVcsZt;
         oModelRot.multVec(vVcsX, vVcsXt);
         oModelRot.multVec(vVcsY, vVcsYt);
         oModelRot.multVec(vVcsZ, vVcsZt);
         SbVec3f vPlaneNormal = oClipPlane.getNormal();
         float nDistance = oClipPlane.getDistanceFromOrigin();
         float nDotX = vVcsXt.dot(vPlaneNormal);
         float nDotY = vVcsYt.dot(vPlaneNormal);
         float nDotZ = vVcsZt.dot(vPlaneNormal);
         float nMax;
         SbVec3f vNewClipNormal;
            SbVec3f vNewClipX;
            SbVec3f vNewClipY;
            // Determine the most closest side for the clipping plane
         if (fabs(nDotX) > fabs(nDotY))
         {
             if (fabs(nDotX) > fabs(nDotZ))
             {
                     nMax = nDotX;
                     vNewClipNormal = vVcsXt;
                     vNewClipX = vVcsYt;
                     vNewClipY = vVcsZt;
             }
             else
             {
                     nMax = nDotZ;
                     vNewClipNormal = vVcsZt;
                     vNewClipX = vVcsXt;
                     vNewClipY = vVcsYt;
             }
         }
         else
         {
             if (fabs(nDotY) > fabs(nDotZ))
             {
                     nMax = nDotY;
                     vNewClipNormal = vVcsYt;
                     vNewClipX = vVcsZt;
                     vNewClipY = vVcsXt;
             }
             else
             {
                     nMax = nDotZ;
                     vNewClipNormal = vVcsZt;
                     vNewClipX = vVcsXt;
                     vNewClipY = vVcsYt;
             }
         }
         if (nMax < 0)
            {
             vNewClipNormal = -vNewClipNormal;
                SbVec3f vTmp;
                vTmp = vNewClipX;
             vNewClipX = vNewClipY;
             vNewClipY = vTmp;
            }
         // Compute new offset for clipping plane
         float nAlpha = vNewClipNormal.dot(vPlaneNormal);
         float nNewClipDistance = nAlpha * nDistance;
         oClipPlane = SbPlane(vNewClipNormal, nNewClipDistance);
         xyz0 = vNewClipNormal * nNewClipDistance;
            xyz1 = xyz0 + vNewClipY;
            xyz2 = xyz0 + vNewClipX;
         }

	 //SbPlane *planeEqn = new SbPlane(xyz0, xyz2, xyz1);
	 //planeEqn->offset(geoStatus->getClipPlaneOffset());

	 AGETNODE(clipPlane,SoClipPlane,"CLIP_PLANE");
	 clipPlane->plane.setValue(oClipPlane);
	 clipPlane->on = TRUE;

	 AGETNODE(sw,SoSwitch,"PIP_VISIBLE_SWITCH");
	 sw->whichChild = 1;

	 geoStatus->setPipFlipStatus(true);
	 geoStatus->setClipPlane(oClipPlane);
	 geoStatus->setClipPlaneVecs(xyz0, xyz1, xyz2);
      }

   } else {                              // pad is NOT in cutting position

      //printf ("+++ pad is NOT in cutting position !!\n");

      AVGETNODE(clipPlane,SoClipPlane,"CLIP_PLANE");
      clipPlane->on = FALSE;

      AGETNODE(sw,SoSwitch,"PIP_VISIBLE_SWITCH");
      sw->whichChild = 0;

      geoStatus->setPipFlipStatus(false);
   }


   // --------
   // DRAW


   if (geoStatus->getDragStatus()) {

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glColor4f(0.0,0.0,1.0,1.0);
      voGeometryActions::draw(aVolume->getCurrentBrickSet());
      glColor4f(1.0,1.0,1.0,1.0);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   }

/*
   // ATTANUATE
   if (geoStatus->getAttenuateStatus()) {
      glColor4f(0.0,0.0,1.0,1.0);

   // NORMAL MODE
   } else {
      glColor4f(1.0,1.0,1.0,1.0);
      //glDisable(GL_BLEND);
   }
*/
   glColor4f(1.0,1.0,1.0,1.0);

   // MIP
   if (geoStatus->getMipStatus()) {
      glDisable(GL_BLEND);
      glEnable(GL_BLEND);
      glBlendEquationEXT(GL_MAX_EXT);
   

   // OVERBLENDING
   } else  {
      glDisable(GL_BLEND);
      glEnable(GL_BLEND);
      glBlendEquationEXT(GL_FUNC_ADD_EXT);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   }
  
   voGeometryActions::drawUtility(
                    tmpTetraSet,
                    interleavedArrayFormat,
                    aVolume,
                    aCache,
                    voSamplingSpaceScope::OBJECT,
                    samplingPeriod,
                    voDecorationFlagsScope::NONE);

                    // NONE   => draw no outlines
                    // BRICKS => draw brick outlines
                    // TETRAS => draw volume outlines
                    // ALL    => draw volume and brick outlines


   glDisable(GL_BLEND);

   // restore lighting if necessary; alternatively use glPopAttrib(3G) here
   if( lighting != SoLightModelElement::BASE_COLOR )
      glEnable(GL_LIGHTING);
}


//-----------------------------------------------------------------------------
// generatePrimitives - creates point primitives
// for all tetras in node

void voGeometry::generatePrimitives(SoAction * action)
{

   //cout << "voGeometry::generatePrimitives running ..." << endl;

   int i;
   SoPrimitiveVertex pv;
   SoPointDetail det;

   SoState  *state = action->getState();
   
   // get coordinate
   const SoCoordinateElement *coord = SoCoordinateElement::getInstance(state);
   for (i=0; i< coordIndex.getNum(); i++) {

      // for each coordinate, create a primitive vertex
      // with an appropriate detail, and invoke callbacks

      pv.setPoint(coord->get3(coordIndex[i]));
      det.setCoordinateIndex(coordIndex[i]);
      pv.setDetail(&det);
      invokePointCallbacks(action, &pv);
   }
}


//-----------------------------------------------------------------------------
// createPointDetail - creates a point detail for given
// primitive vertex

SoDetail * voGeometry::createPointDetail(SoRayPickAction * ,
				    const SoPrimitiveVertex * pv,
				    SoPickedPoint * ) {
   //cout << "voGeometry::createPointDetail running ..." << endl;

   SoPointDetail * det = new SoPointDetail;
   *det = *(SoPointDetail *)pv->getDetail();
   return (SoDetail *)det;
}


//-----------------------------------------------------------------------------
// Computes the bounding box and center of a pyramid.
void voGeometry::getBBox(SbVec3f &min, SbVec3f &max)
{


}



//-----------------------------------------------------------------------------
// Computes the bounding box and center of a pyramid.

void voGeometry::computeBBox(SoAction *action, SbBox3f &box, SbVec3f &center)
{
   //cout << "voGeometry::computeBBox running ..." << endl;

   // Access the state from the action
   SoState  *state = action->getState();

   const SoCoordinateElement *ce = SoCoordinateElement::getInstance(state);

   int vertNum = ce->getNum();
 
   SbVec3f min, max;

   center.setValue(0,0,0);

   // compute BBox only on the vertices that are indexed
   min = max = ce->get3( coordIndex[0] );

   float minx, miny, minz;
   float maxx, maxy, maxz;

   min.getValue(minx,miny,minz);
   max.getValue(maxx,maxy,maxz);


   int coordIndexCount = coordIndex.getNum();

   for(int k1=0;k1<coordIndexCount;k1++)  // Kludge; fix in BBox too
   {
      float x, y, z;
      center += ce->get3(coordIndex[k1]).getValue();
      ce->get3(coordIndex[k1]).getValue(x,y,z);
      if( minx > x ) minx = x;
      if( miny > y ) miny = y;
      if( minz > z ) minz = z;

      if( maxx < x ) maxx = x;
      if( maxy < y ) maxy = y;
      if( maxz < z ) maxz = z;
   }

   min.setValue(minx,miny,minz);
   max.setValue(maxx,maxy,maxz);

   box.setBounds(min, max);

   center /= vertNum;

}

