// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       voStatus.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube MediDesk
//  CONTENT:    Demo application for volume rendering
//  VERSION:    1.3
// ===========================================================================
//  AUTHORS:    ww      Werner Wohlfahrter
// ===========================================================================
//  HISTORY:    2000 09 26  ww :) 
// ===========================================================================

#include "voStatus.h"

//-----------------------------------------------------------------------------
// initClass

void voStatus::initClass()
{
   // volume data
   aVolume = NULL;
   aTetraSet = NULL; 
   aLookupTable = NULL;
   xVolumeSize = 0;
   yVolumeSize = 0;
   zVolumeSize = 0;
   xBrickSize = 0;
   yBrickSize = 0;
   zBrickSize = 0;
   maxBrickCount = 0;
   renderingMode = 0;
   interpolationType = 0;
   dataType = 0;
   bytesPerVoxel = 1;

   // application status
   mirrorStatus = false;
   useLutStatus = false;
   mipStatus = false;
   attenuateStatus = false;
   dragStatus = false;
   voiStatus = false;
   sliceStatus = false;
   clipSliceStatus = false;
   volumeReadStatus = false;
   pipFlipStatus = false;
   clipPlaneOffsetStatus = false;
//jt
   orthoStatus = false;

   // application data
   //clipPlaneOffset = -0.5*ONE_INCH;
   clipPlaneOffset = 0.3;
   hmdEngine = NULL;
   pipEngine = NULL;
   penEngine = NULL;
}


//-----------------------------------------------------------------------------
// Constructor

voStatus::voStatus()
{
   initClass();
}


//-----------------------------------------------------------------------------
// Destructor
voStatus::~voStatus()
{
}


// set methods

//-----------------------------------------------------------------------------
// setVolume
void   voStatus::setVolume(voBrickSetCollection *volume) {
   aVolume = volume;
}

//-----------------------------------------------------------------------------
// setTetraSet
void   voStatus::setTetraSet(voIndexedTetraSet *tetraSet) {
   aTetraSet = tetraSet;
}

//-----------------------------------------------------------------------------
// setLookupTable
void   voStatus::setLookupTable(voLookupTable *lookupTable) {
   aLookupTable = lookupTable;
}

//-----------------------------------------------------------------------------
// setVolumeSize
void   voStatus::setVolumeSize(int x, int y, int z) {
   xVolumeSize = x;
   yVolumeSize = y;
   zVolumeSize = z;
}

//-----------------------------------------------------------------------------
// setBrickSize
void   voStatus::setBrickSize(int x, int y, int z) {
   xBrickSize = x;
   yBrickSize = y;
   zBrickSize = z;
}

//-----------------------------------------------------------------------------
// setMaxBrickount
void   voStatus::setMaxBrickCount(int mbc) {
   maxBrickCount = mbc;
}

//-----------------------------------------------------------------------------
// setRenderingMode
// CAUTION : this method stores the enumeration as an int value!
void   voStatus::setRenderingMode(int rm) {
   renderingMode = rm;
}

//-----------------------------------------------------------------------------
// setInterpolationType
// CAUTION : this method stores the enumeration as an int value!
void   voStatus::setInterpolationType(int it) {
   interpolationType = it;
}

//-----------------------------------------------------------------------------
// setDataType - stores the voxels datatype
// CAUTION : this method stores the enumeration as an int value!
void   voStatus::setDataType(int dt) {
   dataType = dt;
}

//-----------------------------------------------------------------------------
// setBytesPerVoxel  
void   voStatus::setBytesPerVoxel(int bpv) {
   bytesPerVoxel = bpv;
}

//-----------------------------------------------------------------------------
// setDiskDataFormat
// CAUTION : this method stores the enumeration as an int value!
void   voStatus::setDiskDataFormat(int ddf) {
   diskDataFormat = ddf;
}

//-----------------------------------------------------------------------------
// setExternalDataFormat
// CAUTION : this method stores the enumeration as an int value!
void   voStatus::setExternalDataFormat(int edf) {
   externalDataFormat = edf;
}

//-----------------------------------------------------------------------------
// setInternalDataFormat
// CAUTION : this method stores the enumeration as an int value!
void   voStatus::setInternalDataFormat(int idf) {
   internalDataFormat = idf;
}

//-----------------------------------------------------------------------------
// setInterleavedArrayFormat
// CAUTION : this method stores the enumeration as an int value!
void   voStatus::setInterleavedArrayFormat(int iaf) {
   interleavedArrayFormat = iaf;
   //cout << "set: interleavedArrayFormat <" << interleavedArrayFormat << ">" << endl;
}

//-----------------------------------------------------------------------------
// setMaxSamplesNumber
void   voStatus::setMaxSamplesNumber(int msn) {
   maxSamplesNumber = msn;
}

//-----------------------------------------------------------------------------
// setUseLutStatus - set use lookup table mode
void   voStatus::setUseLutStatus(bool useLutState) {
   useLutStatus = useLutState;
}

//-----------------------------------------------------------------------------
// setMirrorStatus - set mirror mode
void   voStatus::setMirrorStatus(bool mirrorState) {
   mirrorStatus = mirrorState;
}

//-----------------------------------------------------------------------------
// setMipStatus - set using maximum intensity projection
void   voStatus::setMipStatus(bool mipState) {
   mipStatus = mipState;
}

//-----------------------------------------------------------------------------
// setAttenuateStatus - set using attenuate blending
void   voStatus::setAttenuateStatus(bool attenuateState) {
   attenuateStatus = attenuateState;
}

//-----------------------------------------------------------------------------
// setDragStatus - set volume dragging status
void   voStatus::setDragStatus(bool dragState) {
   dragStatus = dragState;
}

//-----------------------------------------------------------------------------
// setVOIStatus - set volume of interest status
void   voStatus::setVOIStatus(bool voiState) {
   voiStatus = voiState;
}

//-----------------------------------------------------------------------------
// setVolumeReadStatus - set volume read status
void   voStatus::setVolumeReadStatus(bool vr) {
   volumeReadStatus = vr;
}

//-----------------------------------------------------------------------------
// setPipFlipStatus - true if pip is turned upside down
void   voStatus::setPipFlipStatus(bool pip) {
   pipFlipStatus = pip;
}
   
//-----------------------------------------------------------------------------
// setClipPlaneOffsetStatus - true if clipplane offset is used.
void   voStatus::setClipPlaneOffsetStatus(bool cpoffset) {
   clipPlaneOffsetStatus = cpoffset;
}
//jt
/----------------------------------------------------------------------------
// setOrthoStatus - set orthogonal clipping plane mode
void   voStatus::setOrthoStatus(bool orthoState) {
   orthoStatus = orthoState;
}

//-----------------------------------------------------------------------------
// setCPGetDataStatus - set clipping plane get data status
void   voStatus::setCPGetDataStatus(bool getDataState) {
   //cout << "voStatus::setCPGetDataStatus ..." << endl;
   clipPlaneGetDataStatus = getDataState;
}

//-----------------------------------------------------------------------------
// setSliceStatus - set slice texture status
void   voStatus::setSliceStatus(bool s) {
   sliceStatus = s;
}

//-----------------------------------------------------------------------------
// setClipPlane - store clipping plane 
void   voStatus::setClipPlane(SbPlane cp) {
   freezeClipPlane = cp;
}

//-----------------------------------------------------------------------------
// setClipPlaneVecs
void   voStatus::setClipPlaneVecs(SbVec3f p0, SbVec3f p1, SbVec3f p2) {
   clipPlaneP0 = p0;
   clipPlaneP1 = p1;
   clipPlaneP2 = p2;
}

//-----------------------------------------------------------------------------
// setClipPlaneOffset - store clipping plane offset
void   voStatus::setClipPlaneOffset(float offset) {
   clipPlaneOffset = offset;
}

//-----------------------------------------------------------------------------
// setHmdEngine
void   voStatus::setHmdEngine(SoTrakEngine *currentHmdEngine) {
   hmdEngine = currentHmdEngine;
}

//-----------------------------------------------------------------------------
// setPipEngine
void   voStatus::setPipEngine(SoTrakEngine *currentPipEngine) {
   pipEngine = currentPipEngine;
}

//-----------------------------------------------------------------------------
// setPenEngine
void   voStatus::setPenEngine(SoTrakEngine *currentPenEngine) {
   penEngine = currentPenEngine;
}


//-----------------------------------------------------------------------------
// create polygon set array
// Allocate storage for transient polygons.  In a general case, one needs
// a FaceSet for each brick and each sampling plane.  Note, that while
// each FaceSet is allocated separately, they all share a single vertex
// array in order to minimize fragmentation.  This scheme is general enough
// to allow polygonization of multiple mis-aligned volumes without much
// sorting.  Note however, that is does not use space very efficiently.
// In particular, when using 2D textures only aPolygonSetArray[j][k], where
// j/samplingPeriod < k < (j+1)/samplingPeriod needs to be allocated (e.g.,
// while sampling every 0.5 voxel, each brick needs to have only 2 FaceSets
// allocated).

voIndexedFaceSet ***  voStatus::createPolygonSetArray() {

   voInterleavedArrayType interleavedArrayFormat = getInterleavedArrayFormat();
   // interleavedArrayForamt must include T3F to be able to store textures!
   // V3F = vertex 3 floats (vx, vy, vz)
   // T3F = texture 3 floats
   if (interleavedArrayFormat == voInterleavedArrayTypeScope::V3F) {
      interleavedArrayFormat = voInterleavedArrayTypeScope::T3F_V3F;
      //cout << "createPSA: interleavedArrayFormat <T3F_V3F>" << endl;
   }
   int valuesPerVtx = getValuesPerVertex(interleavedArrayFormat);

   if( aTetraSet ) {
      valuesPerVtx = aTetraSet->getVertexData()->getValuesPerVertex();
   }

   // Holds all intermediate PER_VERTEX information.
   allVertexData = new voVertexData(100000, valuesPerVtx);

   aPolygonSetArray = new voIndexedFaceSet**[maxBrickCount];
   for (int j1 = 0; j1 < maxBrickCount; j1++) {
       aPolygonSetArray[j1] = new voIndexedFaceSet*[maxSamplesNumber];
       for (int j2 = 0; j2 < maxSamplesNumber; j2++)
          // do not pre-allocate index buffers to save space
          aPolygonSetArray[j1][j2] = new voIndexedFaceSet(allVertexData, 0);
   }

   return aPolygonSetArray;
}
