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


#ifndef _SOCYLINDERKIT_H_
#define _SOCYLINDERKIT_H_

#include "Object3dKit.h"
#include "SoPointKit.h"
#include "SoLineKit.h"
#include "SoPlaneKit.h"
#include "SoCubeKit.h"
#include "SoSphereKit.h"
#include <Inventor/nodes/SoCylinder.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoCylinderKit: public Object3dKit
{ 
  SO_KIT_HEADER(SoCylinderKit);
  
  //set parts
//  SO_KIT_CATALOG_ENTRY_HEADER(object);
  SO_KIT_CATALOG_ENTRY_HEADER(cylinder);
  SO_KIT_CATALOG_ENTRY_HEADER(sh);
  
public: 
  SoSFFloat cylinderRadius, cylinderHeight;
  SoSFVec3f cylinderCenter;
  SoMFRotation cylinderRotation;

  SoFieldSensor *objectOneSensor;
  SoFieldSensor *objectTwoSensor;
  SoFieldSensor *objectThreeSensor;
  SoFieldSensor *objectFourSensor;
  SoFieldSensor *objectFiveSensor;
    
  SoSFName ObjectOneName;
  SoSFName ObjectTwoName;
  SoSFName ObjectThreeName;
  SoSFName LineOneName;
  SoSFName LineTwoName;
  
//  SoCylinder* cylinder; 
  
  static void initClass(); 
  SoCylinderKit(
    ); 
  ~SoCylinderKit(
    );
/*  SoCylinderKit(
    int number
    ); 
  SoCylinderKit(
    int number,
    SoNode* pointOne,
    SoNode* pointTwo,
    SoNode* pointThree
    );
  SoCylinderKit(
    int number,
    SoNode* objOne,
    SoNode* objTwo
    );
*/  SoCylinderKit(
    int number,
    std::vector<int> objectList,
    int userID
    );
  void initSoCylinderKit(
    );
  float getDistance(
    SbVec3f penPosition
    );
  static void nameUpdate(
    void *data,
    SoSensor *sensor
    );

//  void SoCylinderKitCreate(int number, SoNode* pointOne, SoNode* pointTwo, SoNode* pointThree);

protected:  
  void setHighlightMaterial(
	  int userID
	);

  virtual void OnChange();
  
  void CreateSensors();

  void UpdateAcisObject();
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

#endif//_SOCYLINDERKIT_H_
