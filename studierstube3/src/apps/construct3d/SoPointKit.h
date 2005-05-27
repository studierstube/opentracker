/* =======================================================================
* (C) 2001 Vienna University of Technology
* =======================================================================
* PROJECT: Studierstube
* ======================================================================= */
/** SoPointKit.h containts the implementation of SoPointKit.
* SoPointKit is the point class of Construct3d.
* All points constructed with Construct3d are SoPointKits.
*
* @see contextcnd2 Construct3d - Geometric Construction in n Dimensions 
*
* @author  Hannes Kaufmann
*
* $Id: SoPointKit.h 3426 2004-07-14 12:11:27Z tamer $
* @file                                                                   */
/* ======================================================================= */


#ifndef _SOPOINTKIT_H_
#define _SOPOINTKIT_H_

#include "Object3dKit.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

/** SoPointKit - Point Kit for Construct3d.
* All points are generated with this Nodekit. 
* Normal Points, as well as intersection points are calculated and defined.
*
* @author Hannes Kaufmann
*/

class SoPointKit: public Object3dKit
{ 
  SO_KIT_HEADER(SoPointKit);
  
  //set parts
  /// type of geometric representation of a point (I use a cube)
//  SO_KIT_CATALOG_ENTRY_HEADER(object);
  
  /** material and transparency of surrounding geometry.
  * A surrounding transparent box is used to enable easier
  * dragging with SoDragKit
  */
//  SO_KIT_CATALOG_ENTRY_HEADER(material2);
  
  /// type of surrounding object (I use a cube)
//  SO_KIT_CATALOG_ENTRY_HEADER(transparentobject);
  
  //    SO_KIT_CATALOG_ENTRY_HEADER(drawstyle);
  //    SO_KIT_CATALOG_ENTRY_HEADER(frameobject);
public: 
  /// size of the point (we use the constant pSize - in CnDConstants - with 7mm)
  SoSFFloat pointSize;
  
  /// actual coordinates of point in 3-space
  SoSFVec3f pCoord;
  
  /** reference objects:
  * If 2 other Objects (in Construct3d other objects are always Object3dKits)
  * are used to generate and calculate this point (e.g. 2 intersection lines)
  * then the names of these objects are stored in p and q
  */
  SoSFName p,q;

  SoSFName ObjectName;
  SoSFVec3f PointPosition;
  SoSFVec3f StartPosition; //for redo and undo point movement
  SbBool    StartFlag; //for redo and undo point movement
  SbBool    MoveFlag; //for redo and undo point movement
  SbBool    centreofLine; //set to true if point is centre of Line

  SbBool    addMovetoRedoUndoList; //for undolist, if the point is add from intersectkit, no addcommandtoundoredolist during move
  
  SoFieldSensor *objectOneSensor;
  SoFieldSensor *objectTwoSensor;
  //SoFieldSensor *objectchangingSensor; //Sensor on the translation Nute of the Point
  
  //Acis SPAposition
  SPAposition a_Point;
  
  /// initialisation method
  static void initClass(); 
  /// default Constructor
  SoPointKit(
    ); 
  /// Destructor
  ~SoPointKit(
    );
    /** Constructor (never used yet)
    * @param number Number of Object (all generated ObjectKits are 
    *               consecutively numbered)
  */
//  SoPointKit(
//    int number
//    ); 
    /** Constructor with point coordinates
    * @param number Number of Object (all generated ObjectKits are 
    *               consecutively numbered)
    * @param t coordinates of point
  */
  SoPointKit(
    int number,
    SbVec3f t,
    int userID
    );
  SoPointKit(
    int number, 
    std::vector<int> objectList, 
    int userID
    );
  // Point fix on Object
//  SoPointKit(
//    int number,
//    SbVec3f t,
//    SoNode* object
//    );
  SoPointKit(
    int number, 
    SbVec3f t, 
    SbName objectName,
    int userID
    );
  SoPointKit(
    int number,
    std::vector<int> objectList,
    SbVec3f evenPos,
    int userID
    );
  void initSoPointKit(
    );
  float getDistance(
    SbVec3f penSPAposition
    );
//  void select(
//    SoSelection* selectionRoot
//    );
//  void deselect(
//    SoSelection* selectionRoot
//    );

  static void nameUpdate(
    void *data,
    SoSensor *sensor
    );

  SoMFName* NamesofDependentObjects(SbName ObjectType); //return a list of names 

  void Update(SbVec3f coord);
  void Update(float x, float y, float z);

protected:
  void setHighlightMaterial(
	  int userID
	);

  void UpdateAcisObject();

  void CreateSensors();

  void CalcentreofLine();

  //static void updateObject(void *data, SoSensor *sensor);
  virtual void OnMove();
  virtual void OnRelease();
  virtual void OnChange();
  virtual void OnPress();

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

#endif//_SOPOINTKIT_H_
