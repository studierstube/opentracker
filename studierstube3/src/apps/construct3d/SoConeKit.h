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


#ifndef _SOCONEKIT_H_
#define _SOCONEKIT_H_

#include "Object3dKit.h"
#include "SoPointKit.h"
#include "SoLineKit.h"
#include "SoPlaneKit.h"
#include "SoCubeKit.h"
#include "SoSphereKit.h"
#include "SoCylinderKit.h"
#include <Inventor/nodes/SoCone.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoConeKit: public Object3dKit
{ 
  SO_KIT_HEADER(SoConeKit);
  
  //set parts
//  SO_KIT_CATALOG_ENTRY_HEADER(object);
  
public: 
  SoFieldSensor *objectOneSensor;
  SoFieldSensor *objectTwoSensor;
  SoFieldSensor *objectThreeSensor;
    
  SoSFName ObjectOneName;
  SoSFName ObjectTwoName;
  SoSFName ObjectThreeName;

  SoSFFloat coneRadius, coneHeight;
  SoSFVec3f coneCenter;
  SoMFRotation coneRotation;
  
  SoCone* cone;
  
  static void initClass(); 
  SoConeKit(
    ); 
  ~SoConeKit(
    );
/*  SoConeKit(
    int number
    ); 
  // only one constructor so far => all values mCoord, aCoord, pCoord,
  // height and bottomRadius are always defined
  SoConeKit(
    int number,
    SoNode* pointOne,
    SoNode* pointTwo,
    SoNode* pointThree
    );
*/  SoConeKit(
    int number,
    std::vector<int> objectList,
    int userID
    );
  void initSoConeKit(
    );
  float getDistance(
    SbVec3f penPosition
    );
  static void nameUpdate(
    void *data,
    SoSensor *sensor
    );

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

#endif//_SOCONEKIT_H_
