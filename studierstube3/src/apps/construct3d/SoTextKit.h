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


#ifndef _SOTEXTKIT_H_
#define _SOTEXTKIT_H_

#include "Object3dKit.h"
#include <Inventor/nodes/SoText3.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoTextKit: public Object3dKit
{ 
  SO_KIT_HEADER(SoTextKit);
  
  //set parts
  SO_KIT_CATALOG_ENTRY_HEADER(font);
  SO_KIT_CATALOG_ENTRY_HEADER(billboard);
  //SO_KIT_CATALOG_ENTRY_HEADER(object);
public: 
  SoSFFloat textSize, distance;
  SoSFVec3f textCoord; 
//  SoSFName p,q;
  SoFieldSensor *objectOneSensor;
  SoFieldSensor *objectTwoSensor;
  std::vector<SoFieldSensor*> ObjectSensor;
  SoSFInt32 NumofPoints;
  SoMFName pointsName;
  SoSFName LineOneName, LineTwoName;
  
  SoText3* text;
  
  static void initClass(); 
  SoTextKit(
    ); 
  ~SoTextKit(
    );
  //    SoTextKit(
  //        int number
  //        ); 
  SoTextKit(
    int number,
    SbVec3f t
    );
//  SoTextKit(
//    int number,
//    SoNode* objectOne, 
//    SoNode* objectTwo
//    );
  SoTextKit(
    int number,
    std::vector<int> objectList,
    int userID
    );
  void initSoTextKit(
    );
  virtual float getDistance(
    SbVec3f penPosition
    );
  static void nameUpdate(
    void *data,
    SoSensor *sensor
    );
//  void setNameSensors( );
  void createSensors(
    );
//  void calcText(
//    SoNode* objectOne,
//    SoNode* objectTwo
//    );
  void Build(
    );

protected:
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);
    virtual void OnChange();
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOTEXTKIT_H_
