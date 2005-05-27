// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       Cnd.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube Workspace
//  CONTENT:    Geometric Construction in n Dimensions
//  VERSION:    1.3
// ===========================================================================
//  AUTHOR:    hk      Hannes Kaufmann
// ===========================================================================
//  HISTORY:   Time of development: Dezember 1999 - 
//
// ===========================================================================


#ifndef _SOLINEKIT_H_
#define _SOLINEKIT_H_

#include "Object3dKit.h"
#include "SoPointKit.h"
#include "SoPlaneKit.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoLineKit: public Object3dKit
{ 
  SO_KIT_HEADER(SoLineKit);
  BASE3D_HEADER;
  
  //set parts
  SO_KIT_CATALOG_ENTRY_HEADER(coord);
  SO_KIT_CATALOG_ENTRY_HEADER(object);
  
public: 
  SoSFFloat lineThickness;// default = 3
  SoSFBool normal;    // TRUE if line is normal to some object
  SoSFName p,q,intersectionPointName;
  // we use a connection directly to the midpoint in case of a normal to a plane
  SoSFVec3f pCoord, qCoord, midpoint;  
  SoFieldSensor *objectOneSensor;
  SoFieldSensor *objectTwoSensor;
  SoFieldSensor *objectThreeSensor;
  SoFieldSensor *objectFourSensor;
  SPAposition position_One;
  SPAposition position_Two;

  //acis edge
  EDGE *a_Line;
  
  static void initClass(); 
  SoLineKit(
    ); 
  ~SoLineKit(
    );
  SoLineKit(
    int number,
    int userID
    ); 
  SoLineKit(
    int number, 
    int userID, 
    SbName intersectObjectName
    ); 
/*
  SoLineKit(
    int number, 
    std::vector<int> objectList, 
    int userID
    );
*/
/*  SoLineKit(
    int number,
    SoNode* objectOne,
    SoNode* objectTwo,
    int userID
    );
*/  SoLineKit(
    int number,
    std::vector<int> objectList,
    int userID,
    SoSFInt32* objSum,
	SbBool angleBisector
    );
/*  void SetLineKit(
    std::vector<int> objectList,
    SoSFInt32* objSum
    );
*/
  void initSoLineKit(
    );
  float getDistance(
    SbVec3f penPosition
    );
  static void nameUpdate(
    void *data,
    SoSensor *sensor
    );
  void CreateSensors(
    );
  void Update(SPAposition pos1, SPAposition pos2);

  void Build();
protected:
  // save the name of the tangent point of object one and two
  SoSFName TangentPointOne, TangentPointTwo, intersectObjectName;
  SoSFInt32 objSum;

  void setHighlightMaterial(
	  int userID
	);
  virtual void OnChange();

  /** This will detach/attach the fieldSensor.
   * It is called at the end of the constructor (to attach).
   * and at the start/end of SoBaseKit::readInstance()
   * and on the new copy at the start/end of SoBaseKit::copy().
   * Returns the state of the node when this was called.
   */
  virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

  int checkObjects(std::vector<int> objectList);
  virtual void Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum);

  void calculateTangent();
  void calculateNormalLine();
  void calculateNormaltoObject();
  void calculateParallelLine();
  void calculateAngleBisector();

   //return true if all points of object one and two in a plane
  bool PointsinaPlane(SbName objectOne, SbName objectTwo);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOLINEKIT_H_
