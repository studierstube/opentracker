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


#ifndef _SOCUBEKIT_H_
#define _SOCUBEKIT_H_

#include "Object3dKit.h"
#include "SoPointKit.h"
#include "SoLineKit.h"
#include "SoPlaneKit.h"
#include <Inventor/nodes/SoCube.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoCubeKit: public Object3dKit
{ 
  SO_KIT_HEADER(SoCubeKit);
  
  //set parts
  SO_KIT_CATALOG_ENTRY_HEADER(texture);
  SO_KIT_CATALOG_ENTRY_HEADER(object);
  
public: 

  SoFieldSensor *objectOneSensor;
  SoFieldSensor *objectTwoSensor;
        
  SoSFName ObjectOneName;
  SoSFName ObjectTwoName;

  SoSFFloat boxWidth, boxHeight, boxDepth;
  SoSFVec3f boxCenter;
  
  static void initClass(); 
  SoCubeKit(
    ); 
  ~SoCubeKit(
    );
/*  SoCubeKit(
    int number
    ); 
  SoCubeKit(
    int number,
    SoNode* pointOne,
    SoNode* pointTwo
    );
*/  SoCubeKit(
    int number,
    std::vector<int> objectList,
    int userID
    );
  void initSoCubeKit(
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

#endif//_SOCUBEKIT_H_
