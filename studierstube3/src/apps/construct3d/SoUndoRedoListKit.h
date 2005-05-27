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


#ifndef _SOUNDOREDOLISTKIT_H_
#define _SOUNDOREDOLISTKIT_H_

#include "Object3dKit.h"
#include "SoCommandKit.h"
#include <Inventor/nodekits/SoNodeKit.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoMFNode.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoUndoRedoListKit: public SoBaseKit
{ 
  SO_KIT_HEADER(SoUndoRedoListKit);
  
public: 
  
  static void initClass(); 
  SoUndoRedoListKit(
    ); 
  ~SoUndoRedoListKit(
    );
  void initSoUndoRedoListKit(
    );
  void initVariables();

  void addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType, SoMFName* objects, int userID);
  void addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType, SbVec3f eventPos, SoMFName* objects, int userID);
  void addCommandtoUndoReduList(SbName command, SbVec3f position);
  void addCommandtoUndoReduList(SbName command, SbName objectName);
  void addCommandtoUndoReduList(SbName command, SbName objectName, int userID);
  void addCommandtoUndoReduList(SbName command, int activeLayerNew, int activeLayerOld);
  void addCommandtoUndoReduList(SbName command, SoMFName* objects, int activeLayerNew, int activeLayerOld);
  void addCommandtoUndoReduList(SbName command, SbName objectName, SbVec3f startPosition, SbVec3f endPosition);
  void addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType, int degreeold, int degree);
  void addCommandtoUndoReduList(SbName command, SbName objectName, float sliderValueNew, float sliderValueOld);
  void addCommandtoUndoReduList(SbName command, const char* filePath, int appID);
  void addCommandtoUndoReduList(SbName command, SoMFName* objects, int userID);
  
  void printoutUndoRedoList();
  void printoutNode(); //print out the node at list position

  int getListPos();
  void setListPos(int pos);
  void incListPos();
  void decListPos();
  void setNumofItems();
  void setNumofItems(int numofItems);
  Object3dKit* getObject();
  Object3dKit* getObject(int pos);
  SbName getCommand();
  SoCommandKit* getCommandLine();
  SoCommandKit* getCommandLine(int num);
  int getNumofItems();
  int countNumofItems();

  //delete all items in the undoredolist after list position
  void cleanList();

protected:
  
  SoMFNode undo_redo_List; //save all commands for undo and redo
  SoSFInt32 num_of_Items;
  SoSFInt32 undo_redo_List_pos;
  SoSFTime startTime;
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOUNDOREDOLISTKIT_H_
