// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Cnd.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube Workspace
//  CONTENT:    Geometric Construction in n Dimensions
//  VERSION:    1.3
// ===========================================================================
//  AUTHOR:    kf Klaus Feiler
// ===========================================================================
//  HISTORY:   Time of development: Dezember 2002 - 
//
// ===========================================================================


#ifndef _SOSURFACEKIT_H_
#define _SOSURFACEKIT_H_

#include "Object3dKit.h"
#include "SoPointKit.h"
#include "SoLineKit.h"
#include "SoPlaneKit.h"
#include "SoCubeKit.h"
#include "SoSphereKit.h"
#include "SoCylinderKit.h"

#include "mmgr.hxx"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoSurfaceKit: public Object3dKit
{ 
  SO_KIT_HEADER(SoSurfaceKit);
  
  //set parts
  SO_KIT_CATALOG_ENTRY_HEADER(objectVertexProperty);
  SO_KIT_CATALOG_ENTRY_HEADER(objectShapeHints);
  SO_KIT_CATALOG_ENTRY_HEADER(objectFaceSet);
  //SO_KIT_CATALOG_ENTRY_HEADER(object);
  
public: 

  // the 2 objects this Curve operation uses (and it´s sensors)
  SoSFFloat lineThickness;// default = 3
  std::vector<SoFieldSensor*> ObjectSensor;
  std::vector<Object3dKit*> Points;
  SoFieldSensor *sweepObjectSensor;
  SoFieldSensor *sweepAxisSensor;
  SoSFInt32 NumofPointsX;
  SoSFInt32 NumofPointsY;
  SoSFInt32 SurfaceOp;
  SoMFName pointsName;
  //save the object names for sweepsurface
  SoSFName sweepObjectName;
  SoSFName sweepAxisName;

  FACE* a_Surface;
  
  static void initClass(); 
  SoSurfaceKit(
    ); 
  ~SoSurfaceKit(
    );
/*SoSurfaceKit(
    int number
    ); 
  SoSurfaceKit(
    SoSFInt32* objSum,
    std::vector<Object3dKit*> points,
    SoSFInt32 numberX,
    SoSFInt32 numberY,
    enumSurfaceOp type,
    SoSFInt32 degreeX, 
    SoSFInt32 degreeY,
    SoSelection* selectionRoot 
    );
*/  
  SoSurfaceKit(
    SoSFInt32* objSum,
    std::vector<int> objectList,
    enumSurfaceOp type,
    SoSFInt32 numberX,
    SoSFInt32 numberY,
    SoSFInt32 degreeX, 
    SoSFInt32 degreeY,
    SoSelection* selectionRoot ,
    int userID
    );
  //constructor for sweepsurface
  SoSurfaceKit(
    SoSFInt32* objSum,
    std::vector<int> objectList,
    int userID
    );

  
  void initSoSurfaceKit();
  
  float getDistance(SbVec3f penPosition);
  static void nameUpdate(void *data, SoSensor *sensor);
  virtual BODY* getBody();
  FACE* getFace();
  
  virtual void Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum);

protected:
  void setHighlightMaterial(
	  int userID
	);

  bool updateSurfaceObject;
  bool buildfirst;
  SoSFInt32* ObjSum;
  SoSFInt32 degree_X;
  SoSFInt32 degree_Y;
  SoSelection* SelectionRoot;
  SbBool SensorsCreated;

  void UpdateAcisObject();
  // Builds the Surface
  void Build(float tolerance);
  void BuildSweepSurface(float tolerance);
  void OnRelease();
  void OnMove();
  void CreateSensors();
  int Triangulate(FACE* p_Surface, float tolerance);
  int Triangulate(BODY* p_Shape, float tolerance);

  void getPoints(SoSFInt32* objSum, SoSelection* selectionRoot, std::vector<int> objectList);

  int getKnots(SoSFInt32 degree, SoSFInt32 numofPoints, double* knots);

  virtual void OnChange();
  /** This will detach/attach the fieldSensor.
   * It is called at the end of the constructor (to attach).
   * and at the start/end of SoBaseKit::readInstance()
   * and on the new copy at the start/end of SoBaseKit::copy().
   * Returns the state of the node when this was called.
   */
  virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);
};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOSURFACEKIT_H_
