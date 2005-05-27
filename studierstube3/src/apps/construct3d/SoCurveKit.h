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


#ifndef _SOCURVEKIT_H_
#define _SOCURVEKIT_H_

#include "Object3dKit.h"
#include "SoPointKit.h"
#include "SoLineKit.h"
#include "SoPlaneKit.h"
#include "SoCubeKit.h"
#include "SoSphereKit.h"
#include "SoCylinderKit.h"
#include "SoIntersectKit.h"

#include "mmgr.hxx"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoCurveKit: public Object3dKit
{ 
  SO_KIT_HEADER(SoCurveKit);
  
  //set parts
  SO_KIT_CATALOG_ENTRY_HEADER(objectVertexProperty);
  SO_KIT_CATALOG_ENTRY_HEADER(object);
  SO_KIT_CATALOG_ENTRY_HEADER(objectVertexProperty_c);
  SO_KIT_CATALOG_ENTRY_HEADER(style_c);
  SO_KIT_CATALOG_ENTRY_HEADER(object_c);
  
public: 

  // the 2 objects this Curve operation uses (and it´s sensors)
  SoSFFloat lineThickness;// default = 3
  SoSFFloat lineThickness_thin; //default = 1
  std::vector<SoFieldSensor*> ObjectSensor;
  SoFieldSensor* PlaneSensor;
  std::vector<Object3dKit*> Points;
  SoSFInt32 NumofPoints;
  SoSFInt32 CurveOp;
  SoSFInt32 degree;
  SbLine* Line;
  SoMFName pointsName;
  SoSFName translationObjectName;
  SoSFName intersectObjectName;
  SoSFName LineName;
  SoSFName PlaneName;
  SoNode* intersectObject;
  SbBool SensorsCreated;

  EDGE* a_Curve;
  BODY* a_intCurve;
  BODY* a_intCurve_todraw;

  bool UpdateCurveObject;
  
  static void initClass(); 
  SoCurveKit(
    ); 
  ~SoCurveKit(
    );
//  SoCurveKit(
//    int number
//    ); 
  SoCurveKit(
    int number,
    std::vector<Object3dKit*> points,
    enumCurveOp type,         
    int numofPoints,
    int Degree,
    int userID
    );
  SoCurveKit(
    int number,
    std::vector<int> objectList,
    enumCurveOp type,
    int Degree,
    int userID
    );
  SoCurveKit(
    int number,
    BODY* intCurve,
    BODY* intCurve_todraw,
    SbName transObjName,
    SoNode* interObj,
    SbName intObjName,
    int userID
    );
  
  void addPoint(SoNode* firstPoint, SoNode* lastPoint, SoNode* middlePoint, enumCurveOp type);
  void addPoint(SoNode* endPoint, SoNode* newEndPoint, enumCurveOp type);
  bool rebuild(int deg);

  void deletePoint(SoNode* deletePoint, enumCurveOp type);

  
  void initSoCurveKit();
  
  float getDistance(SbVec3f penPosition);
  static void nameUpdate(void *data, SoSensor *sensor);

  virtual BODY* getBody();
  ENTITY* getEntity();
  SPAposition getstartPosition();

  // check if the given point is a point from the curve
  bool isPointonCurve(SPAposition pos);
  // finds tangent direction of this curve at the given point
  SPAunit_vector pointDirection(SPAposition pos);

protected:
  void setHighlightMaterial(
	  int userID
	);

  void UpdateAcisObject();
  // Builds the Curve
  void Build();
  void createCircledefinebyoneLineandtwoPoints();
  void createCircledefinebyoneLineandonePoints();
  void createEllipse();
  void createCircle();
  void createCurveBezier();
  void createCurvePointson();
  void createCurveIntersection();
  void Facet_Curve();
  void Facet_intCurve();
  void add_lines_between_Controlpoints();
  void CreateSensors();
  int Count_Edges(BODY* body);

  SPAposition GetcentrePoint(SPAposition Point1, SPAposition Point2, SPAposition Point3);
  SPAvector GetPlaneNormal(SPAposition Point1, SPAposition Point2, SPAposition Point3);

  virtual void OnChange();
  /** This will detach/attach the fieldSensor.
   * It is called at the end of the constructor (to attach).
   * and at the start/end of SoBaseKit::readInstance()
   * and on the new copy at the start/end of SoBaseKit::copy().
   * Returns the state of the node when this was called.
   */
  virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);
  virtual void Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum);
};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOCURVEKIT_H_
