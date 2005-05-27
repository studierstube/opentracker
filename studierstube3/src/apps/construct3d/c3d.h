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


#ifndef _SOCNDKIT_H_
#define _SOCNDKIT_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <Inventor/fields/SoSFUInt32.h>
#include <Inventor/fields/SoMFBool.h>

#include <Inventor/sensors/SoNodeSensor.h>

#include <stbapi/event3d/Base3D.h>
#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/widgets/SoPushButton.h>
#include <stbapi/widgets/SoToggleButton.h>
#include <stbapi/widgets/SoRadioButton.h>
#include <stbapi/widgets/SoSpeechSlider.h>
#include <stbapi/widgets/SoIncrementalSlider.h>

#ifdef USE_SOQT_LIB
#include <Inventor/Qt/SoQt.h>
#endif
#include "construct3d.h"

#include "Object3dKit.h"
#include "GridKit.h"
#include "SoCommandKit.h"


#ifdef WIN32
#pragma warning(disable:4786)   // avoid warnings about too long stl names
#include <SoWinLeaveScope.h>
#endif

#include <vector>

class CONSTRUCT3D_API SoCnDKit
:
public SoContextKit, public Base3D
{
  SO_KIT_HEADER(SoCnDKit);
  BASE3D_HEADER;


  /* Parts for horizontal Projection */
  SO_KIT_CATALOG_ENTRY_HEADER(horProSwi);
  SO_KIT_CATALOG_ENTRY_HEADER(horProSep);
  SO_KIT_CATALOG_ENTRY_HEADER(horProCam);
  SO_KIT_CATALOG_ENTRY_HEADER(horProCom);
  SO_KIT_CATALOG_ENTRY_HEADER(horProEnv);
  SO_KIT_CATALOG_ENTRY_HEADER(horProLig1);
  SO_KIT_CATALOG_ENTRY_HEADER(horProLig2);
  SO_KIT_CATALOG_ENTRY_HEADER(horProLig3);
  SO_KIT_CATALOG_ENTRY_HEADER(horProLig4);
  SO_KIT_CATALOG_ENTRY_HEADER(horProLig5);
  SO_KIT_CATALOG_ENTRY_HEADER(horProLig6);
  SO_KIT_CATALOG_ENTRY_HEADER(horProSel);

  /* Parts for vertical Projection */
  SO_KIT_CATALOG_ENTRY_HEADER(vertProSwi);
  SO_KIT_CATALOG_ENTRY_HEADER(vertProSep);
  SO_KIT_CATALOG_ENTRY_HEADER(vertProCam);
  SO_KIT_CATALOG_ENTRY_HEADER(vertProCom);
  SO_KIT_CATALOG_ENTRY_HEADER(vertProEnv);
  SO_KIT_CATALOG_ENTRY_HEADER(vertProLig1);
  SO_KIT_CATALOG_ENTRY_HEADER(vertProLig2);
  SO_KIT_CATALOG_ENTRY_HEADER(vertProLig3);
  SO_KIT_CATALOG_ENTRY_HEADER(vertProLig4);
  SO_KIT_CATALOG_ENTRY_HEADER(vertProLig5);
  SO_KIT_CATALOG_ENTRY_HEADER(vertProLig6);
  SO_KIT_CATALOG_ENTRY_HEADER(vertProSel);

  /* Parts for left Projection */
  SO_KIT_CATALOG_ENTRY_HEADER(leftProSwi);
  SO_KIT_CATALOG_ENTRY_HEADER(leftProSep);
  SO_KIT_CATALOG_ENTRY_HEADER(leftProCam);
  SO_KIT_CATALOG_ENTRY_HEADER(leftProCom);
  SO_KIT_CATALOG_ENTRY_HEADER(leftProEnv);
  SO_KIT_CATALOG_ENTRY_HEADER(leftProLig1);
  SO_KIT_CATALOG_ENTRY_HEADER(leftProLig2);
  SO_KIT_CATALOG_ENTRY_HEADER(leftProLig3);
  SO_KIT_CATALOG_ENTRY_HEADER(leftProLig4);
  SO_KIT_CATALOG_ENTRY_HEADER(leftProLig5);
  SO_KIT_CATALOG_ENTRY_HEADER(leftProLig6);
  SO_KIT_CATALOG_ENTRY_HEADER(leftProSel);

  /* Parts for right Projection */
  SO_KIT_CATALOG_ENTRY_HEADER(rightProSwi);
  SO_KIT_CATALOG_ENTRY_HEADER(rightProSep);
  SO_KIT_CATALOG_ENTRY_HEADER(rightProCam);
  SO_KIT_CATALOG_ENTRY_HEADER(rightProCom);
  SO_KIT_CATALOG_ENTRY_HEADER(rightProEnv);
  SO_KIT_CATALOG_ENTRY_HEADER(rightProLig1);
  SO_KIT_CATALOG_ENTRY_HEADER(rightProLig2);
  SO_KIT_CATALOG_ENTRY_HEADER(rightProLig3);
  SO_KIT_CATALOG_ENTRY_HEADER(rightProLig4);
  SO_KIT_CATALOG_ENTRY_HEADER(rightProLig5);
  SO_KIT_CATALOG_ENTRY_HEADER(rightProLig6);
  SO_KIT_CATALOG_ENTRY_HEADER(rightProSel);

public:
  // SoSelection* selectionRoot;  // no "global" pointer - does not work with distribution
  // objSum = Number of constructed Elements
  SoSFInt32 objSum;

  static void initClass(             //static class initialization
    );
  SoCnDKit(
    );
  SoContextKit* factory(             //override this to create new context
    );                             //of a specific type
  ~SoCnDKit(
    );
  Object3dKit* O(
    int n
    );
  
  static void pointButtonCB(void* userData, SoButtonKit* button);
  static void pushButtonCB(void* userData, SoButtonKit* button);

  static void horizontalProjectionPCB(void* userData, SoButtonKit* button);
  static void horizontalProjectionRCB(void* userData, SoButtonKit* button);

  static void verticalProjectionPCB(void* userData, SoButtonKit* button);
  static void verticalProjectionRCB(void* userData, SoButtonKit* button);

  static void leftProjectionPCB(void* userData, SoButtonKit* button);
  static void leftProjectionRCB(void* userData, SoButtonKit* button);

  static void rightProjectionPCB(void* userData, SoButtonKit* button);
  static void rightProjectionRCB(void* userData, SoButtonKit* button);
  
//  static void layerButtonCB(void* userData, SoButtonKit* button);
//  static void layerActiveButtonCB(void* userData, SoButtonKit* button);
//  static void propLayerButtonCB(void* userData, SoButtonKit* button);
  
//  static void transSliderSCB(void* userData, SoDraggerKit* slider);
//  static void transSliderMCB(void* userData, SoDraggerKit* slider);
//  static void transSliderFCB(void* userData, SoDraggerKit* slider);

  static void gridSliderSCB(void* userData, SoDraggerKit* slider);
  static void gridSliderMCB(void* userData, SoDraggerKit* slider);
  static void gridSliderFCB(void* userData, SoDraggerKit* slider);
  
  static void degSliderSCB(void* userData, SoDraggerKit* slider);
  static void degSliderMCB(void* userData, SoDraggerKit* slider);
  static void degSliderFCB(void* userData, SoDraggerKit* slider);
  
  static void degSliderYSCB(void* userData, SoDraggerKit* slider);
  static void degSliderYMCB(void* userData, SoDraggerKit* slider);
  static void degSliderYFCB(void* userData, SoDraggerKit* slider);
  
  static void surfaceXSliderSCB(void* userData, SoDraggerKit* slider);
  static void surfaceXSliderMCB(void* userData, SoDraggerKit* slider);
  static void surfaceXSliderFCB(void* userData, SoDraggerKit* slider);

  static void surfaceYSliderSCB(void* userData, SoDraggerKit* slider);
  static void surfaceYSliderMCB(void* userData, SoDraggerKit* slider);
  static void surfaceYSliderFCB(void* userData, SoDraggerKit* slider);

  int getUserIdFromUniqueNodeName(
    SoNode* button, 
    SbString name
    );
  int getActiveLayer(
    );
  void showLayers(
    );
  void saveFile(
    //        SoNode* node
    int userID
    );
  void exportVRML2(
    int userID
    );
  static void autoexportVRML2(
    void *data,
    SoSensor *sensor
    );
  static void autoSave(
	  void *data, 
	  SoSensor *sensor
	  );
  void loadFile(
    int userID
    );
  static void browseCB(
    void* data, 
    const char *filePath
    );
  void clearScene(
    );
  void selectNearestObject(
    SbVec3f penPosition
    );
  void selectNearestObject(
    SbName objectName
    );
  void highlightNearestObject(
    SbVec3f penPosition,
    int userID
    );
  SbBool calculateNearestObject(
    SbVec3f penPosition,
    int& minimal, 
    float& minDistance
    );
  void wireframeObjects(
    );
  void deselectAll(
    );
  void deselectAllObjects(
    );
  // select all Objects after undo
  void selectAllObjects(
    SoCommandKit* command
    );
  // select all dependentObjects
  void selectAllDependentObjects(
    );
  // deselect all dependentObjects for undo the section
  void deselectAllDependentObjects(
    SoCommandKit* commandline
    );
  void deleteSelectedObjects(
    );
  void undodeleteSelectedObjects(
    SoCommandKit* command
    );
  void deleteOneObject(SbName ObjectName
    );
  void undodeleteOneObject(SbName ObjectName
    );
  void undo(
    );
  void undoPreview(
    );
  void redo(
    );
  void redoAll(
    );
  void undoslider(
    SoCommandKit* commandline
    );
  void redoslider(
    SoCommandKit* commandline
    );
  void undolayer(
    SoCommandKit* commandline
    );
  void redolayer(
    SoCommandKit* commandline
    );
  void undoObjectLayer(
    SoCommandKit* commandline
    );
  void redoObjectLayer(
    SoCommandKit* commandline
    );
  void undoPushButton(
    SoCommandKit* commandline
    );
  void redoPushButton(
    SoCommandKit* commandline
    );
  void undoLoadVRMLFile(
    SoCommandKit* commandline
    );
  void redoLoadVRMLFile(
    SoCommandKit* commandline
    );


  SbBool addLine(
    int userID
    );
  SbBool addAngleBisector(
    int userID
    );
  SbBool addPlane(
    SbBool normalPlane,
    SbBool symmetryPlane,
    int userID
    );
  SbBool addPlanethreePoints(
    int userID
    );
  SbBool addCube(
    int userID
    );
  SbBool addSphere(
    int userID
    );
  SbBool addCylinder(
    int userID
    );
  SbBool addCone(
    int userID
    );
  SbBool addIntersect(
    int userID
    );
  SbBool addSlice(
    int userID
    );
  SbBool addSweep(
    int userID
    );
  SbBool addBool(
    enumBoolOp type,
    int userID
    );
  SbBool addCurve(
    enumCurveOp type,
    int userID
    );
  SbBool addEllipse(
    enumCurveOp type,
    int userID
    );
  SbBool addSurface(
    enumSurfaceOp type,
    int userID
    );
  SbBool addPolygon(
    SbBool constrained,
    int userID
    );
  SbBool addPoint(
    SbVec3f eventPos,
    SbBool centreofLine,
    SbBool translation,
    int userID
    );
  SbBool translate(
    int userID,
    SbBool undo
    );
  SbBool rotate(
    int userID,
    SbBool undo
    );
  SbBool mirror(
    int userID,
    SbBool undo
    );
  
  SbBool measure(
    int userID
    );

  void countObjects(
    SbBool selected
    );
  static void previewCB(
    void *data, 
    SoSensor *sensor
    );
  static void findButtonMethod(
    void* userData, 
    SoButtonKit* button
    );
  
  static void layerButtonCB(
	  void* userData, 
	  SoSensor* button
	);
  static void layerActiveButtonCB(
	  void* userData, 
	  SoSensor* button
	);
  static void layerActiveButtonPCB(
    void* userData, 
    SoButtonKit* layerbutton
  );
  static void propLayerButtonCB(
	  void* userData, 
	  SoSensor* button
	);
  static void propLayerButtonPCB(
	  void* userData, 
	  SoButtonKit* layerbutton
	);
  static void selectionRootCB(
	  void* userData, 
	  SoSensor* sensor
	);

#ifdef USE_PUC
protected:
#define DEF_FIELD_AND_SENSOR_HELPER(sensor, funcCB, fiel) \
	SoSFString fiel; SoFieldSensor sensor; static void funcCB(void*,SoSensor*);

#define DEF_FIELD_AND_SENSOR(a) \
	DEF_FIELD_AND_SENSOR_HELPER(sensor_##a, cbFunc_##a, a);

	DEF_FIELD_AND_SENSOR(pointMode);
	DEF_FIELD_AND_SENSOR(lineCommand);
	DEF_FIELD_AND_SENSOR(degSlider);
	DEF_FIELD_AND_SENSOR(rightProjection);
	DEF_FIELD_AND_SENSOR(undoCommand);
#endif

private:
  SoFieldSensor* previewSensor[constructButtons], *previewSensor2[constructButtons];
  SoFieldSensor* layerSensor[usedLayers], *layerActiveSensor[usedLayers], *propLayerSensor[usedLayers];
  SoNodeSensor* selectionRootSensor;
  SoSFEnum style;
  SoSFUShort linePattern;
  SbBool preview, previewWorked;
  SbBool worked;
  SbVec3f buttonPressPosition;
  SbBool buttonPressed;
  SoSFBool firstStart;
  std::vector<int> previousNearestObject;  // nearest object per user
  int currentUserID;
  SbBool loadInWindowFlag;
  // TRUE if speech event "selected" has occured for the first time, otherwise FALSE
  SbBool selectedCalled; 
  SbBool pushButtonCalled; 
  SoSFInt32 pointCount, lineCount, planeCount, cubeCount, sphereCount, cylinderCount, 
	  coneCount, objectCount;
  SoSFInt32 selPointCount, selLineCount, selPlaneCount, selCubeCount, selSphereCount, 
      selCylinderCount, selConeCount, selObjectCount;
  SoSFInt32 numOfPips;
  // defines if transformation operations also generate copies of the original objects
  SbBool makeCopy;

  void initVariables();

  SbBool checkPipGeometry();
  SbBool checkWindowGeometry();

  void checkPipMasterMode(   //master/slave mode for pip sheet
    SoNode*	 pipSheet,              //override this to implement your
    SbBool masterMode              //behavior
    );               
  void checkPipConnections( 
    int uid, 
    SoNode * pipSheetContents 
    );

  SbBool isInterested(SoHandle3DEventAction *h3a);
  void handle3DEvent(SoHandle3DEventAction *h3a);
  
  void addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType, SoMFName* objects);
  void addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType, std::vector<int> objects, int userID);
  void addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType, SbVec3f eventPos, std::vector<int> objects, int userID);
  void addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType, int degreeold, int degree);
  void addCommandtoUndoReduList(SbName command, SbName objectName);
  void addCommandtoUndoReduList(SbName command, SbName objectName, int userID);
  void addCommandtoUndoReduList(SbName command, int activeLayerNew, int activeLayerOld);
  void addCommandtoUndoReduList(SbName command, std::vector<int> objects, int activeLayerNew, int activeLayerOld);
  void addCommandtoUndoReduList(SbName command, SbName objectName, float sliderValueNew, float sliderValueOld); 
  void addCommandtoUndoReduList(SbName command, const char* filePath, int appID);
  void addCommandtoUndoReduList(SbName command, std::vector<int> objects, int userID);
  
  // add components to root separator for horizontalProjection
  void addHorizontalProjection(SoSeparator* root);
  // add components to root separator for verticalProjection
  void addVerticalProjection(SoSeparator* root);
  // add components to root separator for leftProjection
  void addLeftProjection(SoSeparator* root);
  // add components to root separator for rightProjection
  void addRightProjection(SoSeparator* root);

  // used for setting fields with object count - needed for APRIL presentations
  void setObjectCount();

  // put index of object into translation list; pay attention to avoid multiple occurrences
  int putIntoTransformList(Object3dKit* object);
  // find add... method according to original object's type
  int findAddObjectMethod(Object3dKit* original, int userID);

  float neg;

  // PIP layer buttons
  // ATTENTION!!! ALL "global" buttons are not correctly shared to more than one
  // PipSheets! Multiple users do not work correctly. The layer buttons must be 
  // made "local" soon
  SoPushButton* Button[buttonNum];
  SoToggleButton *layer[usedLayers];
  SoRadioButton *layerActive[usedLayers];
  SoRadioButton *propLayer[usedLayers];
  
  SoSFFloat startvalueSlider; //save startvalue of the slider for undo
  
  SoSFInt32 counter;

  SoSFInt32 activeLayer;
  SoSFInt32 activeLayerOld;
  // Number of saved files
  SoSFInt32 saveNumber;
  int autosaveNumber; // does not have to be distributed, is only local save number for autosaving
  // list of point mode states (0 = Point mode turned off, 1 = turned ON) 
  // order: userIndex (starting with 0), independent of userKit userID
  SoMFInt32 drawPoints, gridWidth;
  SoMFBool switchPointModeOff;
  // total number of deleted objects
  SoSFUInt32 delNumber;
  // list with child numbers of deleted Objects
  std::vector<int> undoList, pointList, lineList, planeList, cubeList, 
    sphereList, cylinderList, coneList, objectList;

  // in case of makeCopy==FALSE:
  //   list of indices of points to translate, rotate, mirror, etc...
  // in case of makeCopy==TRUE:
  //   list of indices of all objects that will be copied
  std::vector<int> transformList;

  //set to true to prevent adding the command to undoredoList
  bool redoFlag;
  //set to true for adding the command to undoredoList on the same position as the original command
  bool loadFlag;

  //need for undo the preview object
  SoCommandKit* commandline;  

  std::vector<GridKit*> userGrid;
  
  // ATTENTION: 
  // Take care of window position!!!
  // When updating window (resize, move), the 2 variables
  // SbVec3f winpos and SbRotation winrotinverse must be updated!!!!!!!!!!!
  // otherwise, pen position will be calculated wrong!!!
  // not done yet
  SbVec3f winpos;
  SbRotation winrotinverse;
  
  static int round(float f);
}; //class SoCnDKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOCNDKIT_H_

// to be modified if different HMD is used ! 
// Sony Glastron: 22.5° vertical FOV, 30° horizontal FOV
// resolution 800x600 = 4/3 = SO_ASPECT_VIDEO
//const float ratio = SO_ASPECT_VIDEO;
// Real environment
// tall humans have a distance of 70cm between eye and hand
// => if distance origin-viewpoint is 70 cm, then origin
// can be reached without stepping forward
//const SbVec3f ViewportPosition(0.0,0.0,0.7);
//const float verticalFOV = .3927;	//22.5 degrees

//Testing environment:
//const float verticalFOV = .4;
//const SbVec3f ViewportPosition(0.0,0.0,5.0);

