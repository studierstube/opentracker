// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       voStatus.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube MediDesk
//  CONTENT:    Demo application for volume rendering
//  VERSION:    1.3
// ===========================================================================
//  AUTHORS:    ww      Werner Wohlfahrter
// ===========================================================================
//  HISTORY:    2000 09 26  ww :) 
// ===========================================================================

#ifndef _voStatus_h_
#define _voStatus_h_

#include <vo/Geometry.h>
#include <vo/GeometryActions.h>
#include <vo/Appearance.h>
#include <vo/LookupTable.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <stbapi/tracker/SoTrakEngine.h>

class voStatus {

public:

   // Initializes this class
   void   initClass();

   // Constructor
   voStatus();

   // Destructor
   ~voStatus();
 
   //-----------------------------------------------------------------------------
   // set methods
   // volume data settings
   void   setVolume(voBrickSetCollection *volume);
   void   setTetraSet(voIndexedTetraSet *tetraSet);
   void   setLookupTable(voLookupTable *lookupTable);
   void   setVolumeSize(int x, int y, int z);
   void   setBrickSize(int x, int y, int z);
   void   setMaxBrickCount(int mbc);
   void   setRenderingMode(int rm);
   void   setInterpolationType(int it);
   void   setDataType(int dt);
   void   setBytesPerVoxel(int bpv);
   void   setDiskDataFormat(int ddf);
   void   setExternalDataFormat(int edf);
   void   setInternalDataFormat(int idf);
   void   setInterleavedArrayFormat(int iaf);
   void   setMaxSamplesNumber(int msn);

   // application status settings
   void   setUseLutStatus(bool useLutState);
   void   setMirrorStatus(bool mirrorState);
   void   setMipStatus(bool mipState);
   void   setAttenuateStatus(bool attenuateState);
   void   setDragStatus(bool dragState);
   void   setVOIStatus(bool voiState);
   void   setCPGetDataStatus(bool getDataState);
   void   setSliceStatus(bool s);
   void   setVolumeReadStatus(bool vr);
   void   setPipFlipStatus(bool pip);
   void   setClipPlaneOffsetStatus(bool cpoffset);
//jt
   void   setOrthoStatus(bool orthoState);

   // application data settings
   void   setClipPlane(SbPlane cp);
   void   setClipPlaneVecs(SbVec3f p0, SbVec3f p1, SbVec3f p2);
   void   setClipPlaneOffset(float offset);
   void   setHmdEngine(SoTrakEngine* hmdEngine);
   void   setPipEngine(SoTrakEngine* pipEngine);
   void   setPenEngine(SoTrakEngine* penEngine);

   //-----------------------------------------------------------------------------
   // get methods
   // get volume data
   voBrickSetCollection*  getVolume();
   voIndexedTetraSet*     getTetraSet();
   voLookupTable*         getLookupTable();
   voRenderingMode        getRenderingMode();
   voInterpolationType    getInterpolationType();
   voDataType             getDataType();
   voExternalFormatType   getDiskDataFormat();
   voExternalFormatType   getExternalDataFormat();
   voInternalFormatType   getInternalDataFormat();
   voInterleavedArrayType getInterleavedArrayFormat();
   int                    getMaxSamplesNumber();
   int                    getBytesPerVoxel();
   int                    getXVolumeSize();
   int                    getYVolumeSize();
   int                    getZVolumeSize();
   int                    getXBrickSize();
   int                    getYBrickSize();
   int                    getZBrickSize();
   int                    getMaxBrickCount();

   // get application status
   bool                   getUseLutStatus();
   bool                   getMirrorStatus();
   bool                   getMipStatus();
   bool                   getAttenuateStatus();
   bool                   getDragStatus();
   bool                   getVOIStatus();
   bool                   getCPGetDataStatus();
   bool                   getSliceStatus();
   bool                   getVolumeReadStatus();
   bool                   getPipFlipStatus();
   bool                   getClipPlaneOffsetStatus();
//jt
   bool                   getOrthoStatus();

   // get application data
   SbPlane                getClipPlane();
   void                   getClipPlaneVecs(SbVec3f &p0, SbVec3f &p1, SbVec3f &p2);
   float                  getClipPlaneOffset();
   voIndexedFaceSet ***   createPolygonSetArray();
   SoTrakEngine *         getHmdEngine();
   SoTrakEngine *         getPipEngine();
   SoTrakEngine *         getPenEngine();

   // public data
   voVertexData           *allVertexData;
   voIndexedFaceSet       ***aPolygonSetArray;
   voIndexedTetraSet      *aTetraSet;

private:

   //-----------------------------------------------------------------------------
   // volume data variables
   voBrickSetCollection   *aVolume;
   //voIndexedTetraSet      *aTetraSet;
   voLookupTable          *aLookupTable;
   //voVertexData           *allVertexData;
   //voIndexedFaceSet       ***aPolygonSetArray;
   int                    renderingMode;
   int                    interpolationType;
   int                    dataType;
   int                    bytesPerVoxel;
   int                    diskDataFormat;
   int                    externalDataFormat;
   int                    internalDataFormat;
   int                    interleavedArrayFormat;
   int                    maxSamplesNumber;
   int                    xVolumeSize;
   int                    yVolumeSize;
   int                    zVolumeSize;
   int                    xBrickSize;
   int                    yBrickSize;
   int                    zBrickSize;
   int                    maxBrickCount;

   // application status variables
   bool                   useLutStatus;
   bool                   mirrorStatus;
   bool                   mipStatus;
   bool                   attenuateStatus;
   bool                   dragStatus;
   bool                   voiStatus;
   bool                   clipPlaneGetDataStatus;
   bool                   sliceStatus;
   bool                   clipSliceStatus;
   bool                   volumeReadStatus;
   bool                   pipFlipStatus;
   bool                   clipPlaneOffsetStatus;
//jt
   bool                   orthoStatus;

   // application data variables
   SbPlane                freezeClipPlane;
   SbVec3f                clipPlaneP0; 
   SbVec3f                clipPlaneP1; 
   SbVec3f                clipPlaneP2; 
   float                  clipPlaneOffset;
   double                 eqn[4];
   SoTrakEngine           *hmdEngine;
   SoTrakEngine           *pipEngine;
   SoTrakEngine           *penEngine;


};


//-----------------------------------------------------------------------------
// getVolume - get volume pointer
inline voBrickSetCollection* 
voStatus::getVolume(void) {
   return aVolume;
}

//-----------------------------------------------------------------------------
// getTetraSet - get tetra set pointer
inline voIndexedTetraSet*    
voStatus::getTetraSet(void) {
   return aTetraSet;
}

//-----------------------------------------------------------------------------
// getLookupTable - get lookup table pointer
inline voLookupTable*        
voStatus::getLookupTable(void) {
   return aLookupTable;
}

//-----------------------------------------------------------------------------
// getRenderingMode - get rendering mode
// enum renderingMode {
//      DEFAULT,
//      MONOCHROME,
//      COLOR
// }
inline voRenderingMode
voStatus::getRenderingMode(void) {
   if (renderingMode == 0) {
      return voRenderingModeScope::DEFAULT;
   } else if (renderingMode == 1) {
      return voRenderingModeScope::MONOCHROME;
   } else if (renderingMode == 2) {
      return voRenderingModeScope::COLOR;
   }
   return voRenderingModeScope::DEFAULT;
}

//-----------------------------------------------------------------------------
// getInterpolationType - get interpolation type
// enum voIntrpolationType {
//      DEFAULT,
//      _2D,
//      _3D
// }
inline voInterpolationType
voStatus::getInterpolationType(void) {
   if (interpolationType == 0) {
      return voInterpolationTypeScope::DEFAULT;
   } else if (interpolationType == 1) {
      return voInterpolationTypeScope::_2D;
   } else if (interpolationType == 2) {
      return voInterpolationTypeScope::_3D;
   }
   return voInterpolationTypeScope::DEFAULT;
}

//-----------------------------------------------------------------------------
// getDataType - get voxel's data type 
// enum dataType {
//    DEFAULT,
//    UNSIGNED_BYTE,
//    BYTE,
//    UNSIGNED_BYTE_3_3_2_EXT,
//    UNSIGNED_SHORT,
//    SHORT,
//    UNSIGNED_SHORT_4_4_4_4_EXT,
//    UNSIGNED_SHORT_5_5_5_1_EXT,
//    UNSIGNED_INT,
//    INT,
//    UNSIGNED_INT_8_8_8_8_EXT,
//    UNSIGNED_INT_10_10_10_2_EXT,
//    FLOAT  
// }; 
inline voDataType
voStatus::getDataType(void) {
   if (dataType == 0) {
      return voDataTypeScope::DEFAULT;
   } else if (dataType == 1) {
      return voDataTypeScope::UNSIGNED_BYTE;
   } else if (dataType == 2) {
      return voDataTypeScope::BYTE;
   } else if (dataType == 3) {
      return voDataTypeScope::UNSIGNED_BYTE_3_3_2_EXT;
   } else if (dataType == 4) {
      return voDataTypeScope::UNSIGNED_SHORT;
   } else if (dataType == 5) {
      return voDataTypeScope::SHORT;
   } else if (dataType == 6) {
      return voDataTypeScope::UNSIGNED_SHORT_4_4_4_4_EXT;
   } else if (dataType == 7) {
      return voDataTypeScope::UNSIGNED_SHORT_5_5_5_1_EXT;
   } else if (dataType == 8) {
      return voDataTypeScope::UNSIGNED_INT;
   } else if (dataType == 9) {
      return voDataTypeScope::INT;
   } else if (dataType == 10) {
      return voDataTypeScope::UNSIGNED_INT_8_8_8_8_EXT;
   } else if (dataType == 11) {
      return voDataTypeScope::UNSIGNED_INT_10_10_10_2_EXT;
   } else if (dataType == 12) {
      return voDataTypeScope::FLOAT;
   } 
   return voDataTypeScope::DEFAULT;
}

//-----------------------------------------------------------------------------
// getBytesPerVoxel 
inline int
voStatus::getBytesPerVoxel(void) {
   return bytesPerVoxel;
}

//-----------------------------------------------------------------------------
// getDiskDataFormat - get disk data format
// enum voExternalFormatType {  
//    DEFAULT,
//    INTENSITY,
//    LUMINANCE_ALPHA,
//    LUMINANCE,
//    RGBA,
//    ABGR_EXT  
// }; 
inline voExternalFormatType
voStatus::getDiskDataFormat(void) {
   if (diskDataFormat == 0) {
      return voExternalFormatTypeScope::DEFAULT;
   } else if (diskDataFormat == 1) {
      return voExternalFormatTypeScope::INTENSITY;
   } else if (diskDataFormat == 2) {
      return voExternalFormatTypeScope::LUMINANCE_ALPHA;
   } else if (diskDataFormat == 3) {
      return voExternalFormatTypeScope::LUMINANCE;
   } else if (diskDataFormat == 4) {
      return voExternalFormatTypeScope::RGBA;
   } else if (diskDataFormat == 5) {
      return voExternalFormatTypeScope::ABGR_EXT;
   }
   return voExternalFormatTypeScope::DEFAULT;
}

//-----------------------------------------------------------------------------
// getExternalDataFormat - get external data format
// enum voExternalFormatType {
//    DEFAULT,
//    INTENSITY,
//    LUMINANCE_ALPHA,
//    LUMINANCE,
//    RGBA,
//    ABGR_EXT
// };
inline voExternalFormatType
voStatus::getExternalDataFormat(void) {
   if (externalDataFormat == 0) {
      return voExternalFormatTypeScope::DEFAULT;
   } else if (externalDataFormat == 1) {
      return voExternalFormatTypeScope::INTENSITY;
   } else if (externalDataFormat == 2) {
      return voExternalFormatTypeScope::LUMINANCE_ALPHA;
   } else if (externalDataFormat == 3) {
      return voExternalFormatTypeScope::LUMINANCE;
   } else if (externalDataFormat == 4) {
      return voExternalFormatTypeScope::RGBA;
   } else if (externalDataFormat == 5) {
      return voExternalFormatTypeScope::ABGR_EXT;
   }
   return voExternalFormatTypeScope::DEFAULT;
}

//-----------------------------------------------------------------------------
// getInternalDataFormat - get internal data format
// enum voInternalFormatType {  
//   DEFAULT,
//   INTENSITY8_EXT,
//   LUMINANCE8_EXT,
//   LUMINANCE8_ALPHA8_EXT,
//   RGBA8_EXT,
//   RGB8_EXT,
//   RGBA4_EXT,
//   QUAD_LUMINANCE8_SGIS,
//   DUAL_LUMINANCE_ALPHA8_SGIS  
// }; 
inline voInternalFormatType
voStatus::getInternalDataFormat(void) {
   if (internalDataFormat == 0) {
      return voInternalFormatTypeScope::DEFAULT;
   } else if (internalDataFormat == 1) {
      return voInternalFormatTypeScope::INTENSITY8_EXT;
   } else if (internalDataFormat == 2) {
      return voInternalFormatTypeScope::LUMINANCE8_EXT;
   } else if (internalDataFormat == 3) {
      return voInternalFormatTypeScope::LUMINANCE8_ALPHA8_EXT;
   } else if (internalDataFormat == 4) {
      return voInternalFormatTypeScope::RGBA8_EXT;
   } else if (internalDataFormat == 5) {
      return voInternalFormatTypeScope::RGB8_EXT;
   } else if (internalDataFormat == 6) {
      return voInternalFormatTypeScope::RGBA4_EXT;
   } else if (internalDataFormat == 7) {
      return voInternalFormatTypeScope::QUAD_LUMINANCE8_SGIS;
   } else if (internalDataFormat == 8) {
      return voInternalFormatTypeScope::DUAL_LUMINANCE_ALPHA8_SGIS;
   }
   return voInternalFormatTypeScope::DEFAULT;
}

//-----------------------------------------------------------------------------
// getInterleavedArrayFormat
// enum voInterleavedArrayTypeScope {
//   V3F,
//   T3F_V3F,
//   C3F_V3F,
//   C4F_V3F,
//   T3F_C3F_V3F,
//   T3F_C4F_V3F,
// };
inline voInterleavedArrayType
voStatus::getInterleavedArrayFormat(void) {
   cout << "get: interleavedArrayFormat <" << interleavedArrayFormat << ">" << endl;
   if (interleavedArrayFormat == 0) {
      cout << "interleavedArrayFormat <V3F>" << endl;
      return voInterleavedArrayTypeScope::V3F;
   } else if (interleavedArrayFormat == 1) {
      cout << "interleavedArrayFormat <T3F_V3F>" << endl;
      return voInterleavedArrayTypeScope::T3F_V3F;
   } else if (interleavedArrayFormat == 2) {
      cout << "interleavedArrayFormat <C3F_V3F>" << endl;
      return voInterleavedArrayTypeScope::C3F_V3F;
   } else if (interleavedArrayFormat == 3) {
      cout << "interleavedArrayFormat <C4F_V3F>" << endl;
      return voInterleavedArrayTypeScope::C4F_V3F;
   } else if (interleavedArrayFormat == 4) {
      cout << "interleavedArrayFormat <T3F_C3F_V3F>" << endl;
      return voInterleavedArrayTypeScope::T3F_C3F_V3F;
   } else if (interleavedArrayFormat == 5) {
      cout << "interleavedArrayFormat <T3F_C4F_V3F>" << endl;
      return voInterleavedArrayTypeScope::T3F_C4F_V3F;
   } 
   cout << "interleavedArrayFormat <DEFAULT>" << endl;
   return voInterleavedArrayTypeScope::DEFAULT;
}

//-----------------------------------------------------------------------------
// getMaxSamplesNumber
inline int
voStatus::getMaxSamplesNumber(void) {
   return maxSamplesNumber;
}

//-----------------------------------------------------------------------------
// getXVolumeSize - get volume size in x
inline int                   
voStatus::getXVolumeSize(void) {
   return xVolumeSize;
}

//-----------------------------------------------------------------------------
// getYVolumeSize - get volume size in y
inline int                   
voStatus::getYVolumeSize(void) {
   return yVolumeSize;
}

//-----------------------------------------------------------------------------
// getZVolumeSize - get volume size in z
inline int                   
voStatus::getZVolumeSize(void) {
   return zVolumeSize;
}

//-----------------------------------------------------------------------------
// getXBrickSize - get brick size in x
inline int
voStatus::getXBrickSize(void) {
   return xBrickSize;
}

//-----------------------------------------------------------------------------
// getYBrickSize - get brick size in y
inline int
voStatus::getYBrickSize(void) {
   return yBrickSize;
}

//-----------------------------------------------------------------------------
// getZBrickSize - get brick size in z
inline int
voStatus::getZBrickSize(void) {
   return zBrickSize;
}

//-----------------------------------------------------------------------------
// getMaxBrickCount - get max brick number
inline int
voStatus::getMaxBrickCount(void) {
   return maxBrickCount;
}

//-----------------------------------------------------------------------------
// getUseLutStatus - get current use lookup table status
inline bool
voStatus::getUseLutStatus(void) {
   return useLutStatus;
}

//-----------------------------------------------------------------------------
// getMirrorStatus - get current mirror status
inline bool                  
voStatus::getMirrorStatus(void) {
   return mirrorStatus;
}

//-----------------------------------------------------------------------------
// getMipStatus - get current mip status
inline bool                  
voStatus::getMipStatus(void) {
   return mipStatus;
}

//-----------------------------------------------------------------------------
// getAttenuateStatus - get current Attenuate status
inline bool                  
voStatus::getAttenuateStatus(void) {
   return attenuateStatus;
}

//-----------------------------------------------------------------------------
// getDragStatus - get current volume drag status
inline bool
voStatus::getDragStatus(void) {
   return dragStatus;
}

//-----------------------------------------------------------------------------
// getVOIStatus - get current volume of interest status
inline bool
voStatus::getVOIStatus(void) {
   return voiStatus;
}

//-----------------------------------------------------------------------------
// getCPGetDataStatus - get clippingPlaneGetDataStatus
inline bool
voStatus::getCPGetDataStatus(void) {
   return clipPlaneGetDataStatus;
}

//-----------------------------------------------------------------------------
// getSliceStatus - get slice texture status
inline bool
voStatus::getSliceStatus(void) {
   return sliceStatus;
}

//-----------------------------------------------------------------------------
// getVolumeReadStatus - get volume read status
inline bool
voStatus::getVolumeReadStatus(void) {
   return volumeReadStatus;
}

//-----------------------------------------------------------------------------
// getPipFlipStatus - true if pip is turned over (used for clipping!)
inline bool
voStatus::getPipFlipStatus(void) {
   return pipFlipStatus;
}


//-----------------------------------------------------------------------------
// getClipPlaneOffsetStatus - true if clipplane uses an offset
inline bool
voStatus::getClipPlaneOffsetStatus(void) {
   return clipPlaneOffsetStatus;
}
//jt
/----------------------------------------------------------------------------
// getOrthoStatus - true if orthogonal clipplanes are activated
inline bool
voStatus::getOrthoStatus(void) {
   return orthoStatus;
}

//-----------------------------------------------------------------------------
// getClipPlane - get current clippingPlane
inline SbPlane
voStatus::getClipPlane() {
   return freezeClipPlane;
}

//-----------------------------------------------------------------------------
// getClipPlaneVecs
inline void
voStatus::getClipPlaneVecs(SbVec3f &p0, SbVec3f &p1, SbVec3f &p2) {
   p0 = clipPlaneP0;
   p1 = clipPlaneP1;
   p2 = clipPlaneP2;
}

//-----------------------------------------------------------------------------
// getClipPlaneOffset - get current clippingPlaneOffset
inline float
voStatus::getClipPlaneOffset() {
   return clipPlaneOffset;
}

//-----------------------------------------------------------------------------
// getHmdEngine - get pointer to hmdEngine
inline SoTrakEngine*    
voStatus::getHmdEngine() {
   return hmdEngine;
}

//-----------------------------------------------------------------------------
// getPipEngine - get pointer to pipEngine
inline SoTrakEngine*    
voStatus::getPipEngine() {
   return pipEngine;
}

//-----------------------------------------------------------------------------
// getPenEngine - get pointer to penEngine
inline SoTrakEngine*    
voStatus::getPenEngine() {
   return penEngine;
}


#endif // _voStatus_h_

