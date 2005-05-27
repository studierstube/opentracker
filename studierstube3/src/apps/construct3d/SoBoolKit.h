// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Cnd.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube Workspace
//  CONTENT:    Geometric Construction in n Dimensions
//  VERSION:    1.3
// ===========================================================================
//  AUTHOR:    rs      Reinhard Steiner
// ===========================================================================
//  HISTORY:   Time of development: January 2002 - 
//
// ===========================================================================


#ifndef _SOBOOLKIT_H_
#define _SOBOOLKIT_H_

#include "Object3dKit.h"
#include "SoPointKit.h"
#include "SoLineKit.h"
#include "SoPlaneKit.h"
#include "SoCubeKit.h"

#include "mmgr.hxx"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoBoolKit: public Object3dKit
{ 
  SO_KIT_HEADER(SoBoolKit);
  
  //set parts
  SO_KIT_CATALOG_ENTRY_HEADER(objectVertexProperty);
  SO_KIT_CATALOG_ENTRY_HEADER(objectShapeHints);
  SO_KIT_CATALOG_ENTRY_HEADER(objectFaceSet);
  
public:   

  // Boolean operation type, this object represents
  SoSFInt32 m_BoolOp;

  // Save with part of the slice is the boolkit
  SoSFInt32 slicingPart;
  
  // the 2 objects this boolean operation uses (and it´s sensors)
  SoSFName m_Object1Name, m_Object2Name;
  SoFieldSensor *objectOneSensor;
  SoFieldSensor *objectTwoSensor;

  // helper variable for performance
  bool m_UpdateBoolObject;

  
  static void initClass(); 
  SoBoolKit(
    ); 
  ~SoBoolKit(
    );
  SoBoolKit(
    int number,
    std::vector<int> objectList,
    enumBoolOp boolop,
    int userID
    );
  SoBoolKit(
    int number, 
    SbName ObjectOneName,
    BODY* BodyOne, 
    BODY* BodyTwo,
    int slicePart, 
    std::vector<int> objectList,
    int userID
    );


  void initSoBoolKit();
  
  float getDistance(SbVec3f penPosition);
  static void nameUpdate(void *data, SoSensor *sensor);

  virtual void Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum);

protected:
  void setHighlightMaterial(
	  int userID
	);

  virtual void OnChange();
  virtual void OnMove();
  virtual void OnRelease();

  void CreateSensors();
  virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);
  
  // Builds the shape, and triangulates it
  void Build(float tolerance);
  void Build(BODY* bodyOne, BODY* bodyTwo, float tolerance);

  // return number of triangles, size of p_VertexArray is number of triangles * 3
  int Triangulate(BODY* p_Shape, float tolerance);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOBOOLKIT_H_
