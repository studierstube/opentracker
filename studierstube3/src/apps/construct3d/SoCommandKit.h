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


#ifndef _SOCOMMANDKIT_H_
#define _SOCOMMANDKIT_H_

#include "Object3dKit.h"
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFTime.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoCommandKit: public SoBaseKit
{ 
  SO_KIT_HEADER(SoCommandKit);
  
public: 
  
  static void initClass(); 
  SoCommandKit(
    ); 
  ~SoCommandKit(
    );
  SoCommandKit(
    SbName command, 
    SbName objectName, 
    SbName objectType, 
    int degreeold,
    int degree,
    int commandPos
    );
  SoCommandKit(
    SbName command, 
    SbName objectName, 
    float sliderValueNew,
    float sliderValueOld,
    int commandPos
    );
  SoCommandKit(
    SbName command, 
    SbName objectName, 
    SbName objectType, 
    SoMFName* objects,
    int userID,
    int commandPos
    );
  SoCommandKit(
    SbName command, 
    SbName objectName, 
    SbName objectType, 
    SbVec3f eventPos,
    SoMFName* objects,
    int userID,
    int commandPos
    );
  SoCommandKit(
    SbName command, 
    SbVec3f position,
    int commandPos
    );
  SoCommandKit(
    SbName command, 
    SbName objectName,
    int commandPos
    );
  SoCommandKit(
    SbName command, 
    SbName objectName,
    int userID,
    int commandPos
    );
  SoCommandKit(
    SbName command, 
    int aktiveLayerNew,
    int aktiveLayerOld,
    int commandPos
    );
  SoCommandKit(
    SbName command, 
    SoMFName* objects,
    int aktiveLayerNew,
    int aktiveLayerOld,
    int commandPos
    );
  SoCommandKit(
    SbName command, 
    SbName objectName, 
    SbVec3f startPosition, 
    SbVec3f endPosition,
    int commandPos
    );
  SoCommandKit(
    SbName command, 
    const char* filePath,
    int appID,
    int commandPos
    );
  // for transformation commands (translate, rotate, ...)
  SoCommandKit(
    SbName command, 
    SoMFName* objects, 
    int userID, 
    int commandPos
  );

  void initSoCommandKit(
    );

  void setCommandLine(
    SbName command, 
    SbName objectName, 
    SbName objectType, 
    std::vector<Object3dKit*> objects, 
    int numofObjects,
    int userID
    );
  void setCommandLine(
    SbName command, 
    SbName objectName, 
    SbName objectType, 
    SoMFName* objects,
    int userID
    );

  void PrintoutNode();


  SbName getObjectName();
  Object3dKit* getObject();
  SbName getCommand();
  SbName getObjectType();
  SbName getObjectNames(int num);
  int getNumofObjectNames();
  SbVec3f getPosition();
  SbVec3f getStartPosition();
  SbVec3f getEndPosition();
  SbName getinvolvedObjectName(int num);
  int getDegree();
  int getDegreeOld();
  float getSliderValueOld();
  float getSliderValueNew();
  int getUserID();
  int getAktiveLayerNew();
  int getAktiveLayerOld();
  SbName getFilePath();
  int getAppID();
  void setAppID(int appID);

  void Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum);
  void SelectInvolvedObjects(SoSelection* SelectionRoot);
  void DeleteSelectedObjectsNames();
  void RemoveAllCallbacks();

protected:
  
  SoSFName command;
  SoSFInt32 commandPos;
  SoSFName objectName;
  SoSFName objectType;
  SoSFName filePath;
  SoMFName selectedObjectNames;
  SoSFVec3f position;
  SoSFVec3f startPosition;
  SoSFVec3f endPosition;
  SoSFInt32 degree;
  SoSFInt32 degreeold;
  SoSFFloat sliderValueNew;
  SoSFFloat sliderValueOld;
  SoSFInt32 userID;
  SoSFInt32 aktiveLayerNew;
  SoSFInt32 aktiveLayerOld;
  SoSFInt32 appID;
  SoSFTime time;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOCOMMANDKIT_H_
