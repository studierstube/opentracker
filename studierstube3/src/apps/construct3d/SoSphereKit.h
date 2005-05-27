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


#ifndef _SOSPHEREKIT_H_
#define _SOSPHEREKIT_H_

#include "Object3dKit.h"
#include "SoPointKit.h"
#include "SoLineKit.h"
#include "SoPlaneKit.h"
#include "SoCubeKit.h"
#include <Inventor/nodes/SoSphere.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoSphereKit: public Object3dKit
{ 
  SO_KIT_HEADER(SoSphereKit);
  
  //set parts
//  SO_KIT_CATALOG_ENTRY_HEADER(shapehint);  
  SO_KIT_CATALOG_ENTRY_HEADER(sphere);
  
public: 
  SoSFFloat radius;
  SoSFVec3f sphereCenter;
  
  SoFieldSensor *objectOneSensor;
  SoFieldSensor *objectTwoSensor;
  SoFieldSensor *objectThreeSensor;
  
  SoSFName ObjectOneName;
  SoSFName ObjectTwoName;
  SoSFName ObjectThreeName;
  
  //SoSphere* sphere;

  static void initClass(); 
  SoSphereKit(
    ); 
  ~SoSphereKit(
    );
/*  SoSphereKit(
    int number
    ); 
  SoSphereKit(
    int number,
    SoNode* pointOne,
    SoNode* pointTwo
    );
  SoSphereKit(
    int number,
    SoNode* pointOne,
    SoNode* pointTwo,
    SoNode* pointThree
    );
*/  SoSphereKit(
    int number,
    std::vector<int> objectList,
    int userID
    );
  void initSoSphereKit(
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

#endif//_SOSPHEREKIT_H_
