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


#ifndef _SOINTERSECTKIT_H_
#define _SOINTERSECTKIT_H_

#include "Object3dKit.h"

#include "mmgr.hxx"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class SoIntersectKit: public Object3dKit
{ 
  SO_KIT_HEADER(SoIntersectKit);

  //set parts
  //SO_KIT_CATALOG_ENTRY_HEADER(objectVertexProperty);
  //SO_KIT_CATALOG_ENTRY_HEADER(objectShapeHints);
  //SO_KIT_CATALOG_ENTRY_HEADER(objectFaceSet);

public: 

  // for object object intersection. save two curve, a_Shape is  exact, a_Shape_todraw is bigger 
  BODY* a_Shape_todraw;

  //for slicing save two body
  BODY* sliceResultOne;
  BODY* sliceResultTwo;

  // Intersection type
  SoSFInt32 intersectionType;
  
  // the 2 objects this intersection operation uses (and it´s sensors)
  SoSFName Object1Name, Object2Name;
  SoFieldSensor *objectOneSensor, *objectTwoSensor;

  // save the names of result objects
  SoMFName objectNames;
  SoSFInt32 numofObjectNames;

  //save the coords of the line from plane_plane intersection
  SoSFVec3f pCoord, qCoord;
  
  bool UpdateIntersectionObject;
  
  static void initClass(); 
  SoIntersectKit(
    ); 
  ~SoIntersectKit(
    );

  SoIntersectKit(
    int number
    );
  SoIntersectKit(
	  std::vector<int> objectList, 
	  SoSelection* selectionRoot, 
	  SoSFInt32* objSum, 
	  int userID
	  );
/*  SoIntersectKit(
    SoNode* object1, 
    SoNode* object2, 
    enumIntersectionType intType,
    int number);
  SoIntersectKit(
    SoNode* object1,
    SoNode* object2,
    SoSelection* selectionRoot,
    SoSFInt32* objSum,
    enumIntersectionType intType
    );
  SoIntersectKit(
    std::vector<int> objectList,
    SoSelection* selectionRoot,
    SoSFInt32* objSum
    ); 
  void SetIntersectKit(
    SoNode* object1, 
    SoNode* object2, 
    SoSelection* selectionRoot, 
    SoSFInt32* objSum, 
    enumIntersectionType intType
    );
*/
  void SetIntersectKit(
    std::vector<int> objectList,
    SoSelection* selectionRoot, 
    SoSFInt32* objSum,
    int userID
    );
  //use for slicing body with a plane
  void SetIntersectKitSlice(
    std::vector<int> objectList, 
    SoSelection* selectionRoot, 
    SoSFInt32* objSum, 
    int userID
    );
  void initSoIntersectKit();
  float getDistance(SbVec3f penPosition);
  void select(SoSelection* selectionRoot) {}; 
  void deselect(SoSelection* selectionRoot) {};
  // Builds the Intersection
  void Build();

  virtual void Undo(
    SoSelection* SelectionRoot, 
    SoSFInt32* ObjSum
    );
  //select all objects in the dependenobjectlist
  virtual void selectAllDependentObjects(
    SoSelection* selectionRoot
    );  
  //deselect all objects in the dependenobjectlist for undo the section
  virtual void deselectAllDependentObjects(
    SoSelection* selectionRoot
    ); 
  //return 1 if all dependent object selected
  virtual int areAllDependentObjectsSelected(
    SoSelection* selectionRoot
    );

  //return the name of involved object, if the type is like the given type 
  //we need this function in curve kit, to draw a circle with two points.
  SbName getinvolvedObjectName(
    SbName ObjectType
    );

private:
  SoSelection* selectionRoot;
  SoSFInt32* objSum;
  SbName thisobjectName;
  SbBool SensorsCreated;

  static void nameUpdate(void *data, SoSensor *sensor);
  void CreateSensors();

  EDGE* getallEdges(BODY* body); //return all edges of a body

  //intersections
  void Object_Object_int(SbName Obj1Name, SbName Obj2Name);
  void BODY_BODY_int(BODY* bodyOne, BODY* bodyTwo);
  void EGDE_FACE_int(EDGE* Objectedge, FACE* Objectface);
  void EGDE_FACE_int(BODY* Objectbody, FACE* Objectface);

  void PLANE_PLANE_int(PLANE* plane_One, PLANE* plane_Two);
  void EDGE_EDGE_int(EDGE* edge_One, EDGE* edge_Two);
  void BODY_PLANE_int(SbName object1Name, SbName object2Name);

  void SlicingBODY(SbName Obj1Name, SbName Obj2Name);

  void get_edges (ENTITY_LIST& edge_list, BODY *bod);
  void get_edges(ENTITY_LIST& edge_list, LOOP *lp);

  SbName Get_ObjectName(char* name, int number);

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

#endif//_SOINTERSECTKIT_H_
