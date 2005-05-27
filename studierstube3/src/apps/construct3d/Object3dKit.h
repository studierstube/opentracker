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


#ifndef _OBJECT3DKIT_H_
#define _OBJECT3DKIT_H_

#include <assert.h>
#include <stbapi/interaction/SoDragKit.h>

#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoFaceSet.h>

#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/engines/SoConcatenate.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/engines/SoCompose.h>
#include <Inventor/sensors/SoNodeSensor.h>

#include "CnDconstants.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#define WNT

#include <iostream>
#include <string>
#include <sstream>


#include <math.h>

//acis include
#include "cstrapi.hxx"
#include "acis.hxx" 
#include "curdef.hxx"   
#include "elldef.hxx"
//#include "intdef.hxx"
#include "acis_options.hxx"
#include "kernapi.hxx"
#include "api.hxx" 
#include "entity.hxx"
#include "debug.hxx"
#include "point.hxx"
#include "ellipse.hxx"
#include "intcurve.hxx"
#include "plane.hxx"
#include "transfrm.hxx"
#include "fileinfo.hxx"
#include "lists.hxx"
#include "body.hxx"
#include "edge.hxx" 
#include "alltop.hxx"
#include "intcucu.hxx"
#include "geom_utl.hxx" 
#include "acistype.hxx"
#include "vector_utils.hxx" 
#include "unitvec.hxx"
#include "transf.hxx"
#include "vector.hxx"
#include "box.hxx"
#include "position.hxx"
#include "af_api.hxx"
#include "ppmeshmg.hxx"
#include "refine.hxx"
#include "af_enum.hxx"
#include "at_bool.hxx"
#include "boolapi.hxx" 
#include "intrapi.hxx"
#include "cstrapi.hxx"
#include "ablapi.hxx"  
#include "blendapi.hxx"
#include "lop_api.hxx"
#include "bnd_crv.hxx"
#include "param.hxx"
#include "coverapi.hxx"
#include "sweepapi.hxx"
#include "swp_opts.hxx"
//end acis include


class Object3dKit;

typedef void Object3dKitCB(void*, Object3dKit*);


class Object3dKit: public SoDragKit
//class Object3dKit: public SoBaseKit, public Base3D
{ 
  SO_KIT_HEADER(Object3dKit);
  
  //set parts
  //    SO_KIT_CATALOG_ENTRY_HEADER(material);
  SO_KIT_CATALOG_ENTRY_HEADER(switchNode);
  SO_KIT_CATALOG_ENTRY_HEADER(style);
  SO_KIT_CATALOG_ENTRY_HEADER(nearestSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(objectGroup);
  SO_KIT_CATALOG_ENTRY_HEADER(nearestGroup); 
  SO_KIT_CATALOG_ENTRY_HEADER(nearestMaterial); 
  SO_KIT_CATALOG_ENTRY_HEADER(nearestDrawStyle); 
  
public: 
   	enum ColorStateType {
		WORK      = 0,
		HIGHLIGHT = 1,
		DRAG      = 2,
		SELECT    = 3,
		LAYER_INACTIVE_WORK       = 4,
		LAYER_INACTIVE_HIGHLIGHT  = 5,
		LAYER_INACTIVE_DRAG       = 6,
		LAYER_INACTIVE_SELECT     = 7
	};

  //default color (1,1,1);
  //SoSFVec3f color; 
  SoSFInt32 colorState;
  SoSFInt32 childNumber, layer;
  SoSFName name;
  // if visible== SO_SWITCH_ALL, then object is visible, 
  // if visible== SO_SWITCH_NONE then invisible
  SoSFInt32 visible, showLayer;
//  SoSFFloat transparencyValue;
  SoSFBool highlighted;
  SoSFBool dragBoxIntrusion; // TRUE if the Pen gets into the dragging area of the Object
  SoSFInt32 eventInStation;
  SoFieldSensor* highlightedSensor;
  SoFieldSensor* dragBoxIntrusionSensor;
  SoFieldSensor* colorChangeSensor;
  SoSFBool objectdeleted;
  //save the userID form the user who created the object
  SoSFInt32 userID; 
  SoMaterial *W, *S, *D, *N, *L;
  
  SoCalculator* calc;
  
  SoSFInt32 ChangeObject; //inc if object is changing
  // open cascade objects
  //  TopoDS_Shape m_Shape;     // should be inherited
  
  std::vector<Object3dKitCB*> changeCallback;
  std::vector<void*> changeCallbackData;
  
  outcome a_return; //save the returnvalue from modeller
  outcome s_return; //save the returnvalue from create shape
  BODY* a_Shape; //save the Acis Shape
  
  // a comment on dependency:
  // dependency between objects in c3d is mutual, but oriented
  // to clarify object relationship robert recommends the following denotation:
  // CHILDREN are created from PARENT objects (as oposed to the denotation in hannes' diss, page 48)
  //   example: a sphere (child) is created from two points: mid point and point on sphere (parents)
  // children are DEPENDENT from parents 
  //   (i.e. child is recalculated when parent object is moved)
  // parents are COUNTER-DEPENDENT from children 
  //   (i.e. when translation operation on child is executed, parents are moved by this operand and child is recalculated)

  // list of names of dependent objects, i.e. this objects children in robert's denotation
  // in short: childList
  SoMFName dependentObjectsName;
  // list of names of counter-dependent objects, i.e. this objects parents in robert's denotation
  // in short: parentList
  SoMFName counterDependentObjectsName;

  // if we would use the translationOn property derived from SoDragKit to detect 
  // availability for transformation commands (translate, rotate, ...), objects would be 
  // unintentionally draggable, so we use a new boolean which is TRUE only if this 
  // objects was made by an intersection:
  SoSFBool isLocked;

  // keep a link to this object's copy during copy process
  Object3dKit* copyObject;

  static void initClass(); 
  Object3dKit(
    ); 
  ~Object3dKit(
    );
//  Object3dKit(
//    int number
//    ); 
  void initObject3d(
    );
  void setNodeName(
    char id[1], 
    int number
    );
  virtual void setTransparency(
    float transparency
    );
  virtual float getDistance(
    SbVec3f penSPAposition
    );
  virtual void select(
    SoSelection* selectionRoot
    );
  virtual void deselect(
    SoSelection* selectionRoot
    );
  virtual void setHighlightMaterial(
	  int userID
	);
  virtual void deleteObject(
    );
  virtual void undodelete(
    );
  virtual SbBool isInterested(
    SoHandle3DEventAction *h3a
    );
  void handle3DEvent(                 //deal with 3d event
    SoHandle3DEventAction* h3a      //action owning 3d event
    );
  
  // Callbacks
  void connectChild(             // Connect childs with Change, Move, Release and Press Callbacks
    Object3dKit *DependentObject3dKit
    );
  void disconnectChild(             // Disconnects child with Change, Move, Release and Press Callbacks
    Object3dKit *DependentObject3dKit
    );

  // Callbacks
  void setAllCallbacks(
    Object3dKit *DependentDragKit
    );
  void removeAllCallbacks(
    Object3dKit *DependentDragKit
    );
  
  void setChangeCallback(
    Object3dKitCB* cb,
    void* userData = NULL
    );
  void RemoveAllCallbacks(
    );
  void removeChangeCallback(
    Object3dKitCB* cb, 
    void* userData
    );
  void triggerChange(
    );
  static void staticTriggerPressCallback(
    void* userData,
    SoDragKit* dragKit
    )
  { 
      ((SoDragKit*)userData)->triggerPressCallback();
  }
  static void staticTriggerReleaseCallback(
    void* userData,
    SoDragKit* dragKit
    )
  { 
	  ((SoDragKit*)userData)->triggerReleaseCallback(); 
  }
  static void staticTriggerMoveCallback(
    void* userData,
    SoDragKit* dragKit
    )
  { 
	  ((SoDragKit*)userData)->triggerMoveCallback(); 
  }
  
  //add objects to list of dependent Objects
  void addDependentObjects(
    std::vector<int> objectList, int number
    ); 
  //add a object name to list of dependent Objects
  void addDependentObjectsName(
    SbName ObjectName
    ); 
  //add a object name to list of counter-dependent Objects
  void addCounterDependentObjectsName(
    SbName ObjectName
    ); 
  // removes all dependent objects names out of all other objects
  void removeAllDependentObjectsName(
    );
  //get all names of dependents objects
  SoMFName getAllDependentObjectsName(
    );  
  // get one name of dependent object
  SbName getDependentObjectName(
    int i
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
  //returns true if given object is dependent on this object
  bool isDependentObjectsName(
    SbName ObjectName
    ); 
  //return a list of names 
  SoMFName* NamesofDependentObjects(
    SbName ObjectType
    ); 
  //compare two list of name and returns the first equal object
  SbName ComparetwoObjectsNameList(
    SoMFName* list1, 
    SoMFName* list2
    ); 
  void printDependentObjectsName(
    SoMFName list
    );
  void printDependentObjectsName();
  void deleteDependentObjectsName(
    SbName objectName
    );
  int getAllPointsFromAncestors(std::vector<int>*, SbBool*);
  int getAllAncestors(std::vector<int>*, SbBool*);
  
  virtual void OnChange() {};    // overload this method to sense change
  
  static void staticTriggerChangeCallback(
    void* userData,
    Object3dKit* object3dKit
    )
  { 
	  ((Object3dKit*)userData)->triggerChange(); 
  }
  
  int Line_Plane_Intersect(SPAposition point_on_Plane, SPAposition point_on_Line1, SPAposition point_on_Line2, SPAposition &intersect_Point, double &distance_Point_Line);
  
  virtual void Undo(SoSelection* SelectionRoot, SoSFInt32* ObjSum);
  virtual void Undo(SoSelection* SelectionRoot, int NodeNumber);
  
  void deselectObjects(
    std::vector<int> objectList, 
    int number
    );
  SoMFName* getObjectList();
  void createObjectList(
    std::vector<int> objectList,
    int number
    );
  void setvisible(
    );
  void setinvisible(
    );

  virtual BODY* getBody();

protected:
  SoMFName objectList;
  SbBool triggerAllowed;

  void CreateSensors(
    );
  static void highlightUpdate(
    void *data, 
    SoSensor *sensor
    );
  static void dragableUpdate(
    void *data, 
    SoSensor *sensor
    );
  static void colorChange(
      void *data, 
      SoSensor *sensor
      );

  bool isObject(int num);
  bool isObject(SbName name);
  
  Object3dKit* getObject(int n);

  Object3dKit* getObject(SbName name);
  
  /** This will detach/attach the fieldSensors.
  * It is called at the end of the constructor (to attach).
  * and at the start/end of SoBaseKit::readInstance()
  * and on the new copy at the start/end of SoBaseKit::copy().
  * Returns the state of the node when this was called.
  */
  virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

  SoSFVec3f convertPosition(SPAposition Position);
  SPAposition convertSoSFVec3f(const SbVec3f & vec); 

  //return the normal of a plane def by three points
  SPAunit_vector getNormal(SPAposition posOne, SPAposition posTwo, SPAposition posThree);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_OBJECT3DKIT_H_
