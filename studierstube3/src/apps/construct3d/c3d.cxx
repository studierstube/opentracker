/* ===========================================================================
(C) 2000,2001,2002,2003 Vienna University of Technology
===========================================================================
NAME:       c3d.cxx
TYPE:       c++ code
PROJECT:    Studierstube Workspace
CONTENT:    Construct geometry in 3 dimensions
VERSION:    1.xx
===========================================================================
AUTHORS:    hk      Hannes Kaufmann
===========================================================================
LICENSE:    For license issues please contact kaufmann@ims.tuwien.ac.at
===========================================================================
HISTORY:

  24-Jan-01 23:47:10  jp      Strict ISO ANSI + Linux modifications
  23-Jan-01 00:00:00  hk      merged into STB CVS
  00-Dec-99 00:00:00  hk      created
=========================================================================== */

// use this define to enable automatic export of VRML files every 30 seconds
//#define AUTOEXPORT
// automatically saves the construct3d scene (as .iv) every 37 seconds
// #define AUTOSAVE

#include "c3d.h"
using namespace std;

#include <Inventor/nodes/SoComplexity.h>
#include <Inventor/nodes/SoEnvironment.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>

#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoOffscreenRenderer.h>
//#include <Inventor/nodes/SoTextureScalePolicy.h> // internal element
#include <Inventor/VRMLnodes/SoVRMLGroup.h>
#include <Inventor/actions/SoToVRML2Action.h>
#include <Inventor/nodes/SoSceneTexture2.h>

#include <stbapi/stbapi.h>
#include <stbapi/util/ivio.h>		// file reading routine
#include <stbapi/tracker/TrackerOT.h>
#include <stbapi/event3d/Stb3DEventGenerator.h>
#include <stbapi/tracker/SoTrakEngine.h>
#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/util/fileBrowser.h>
#include <stbapi/misc/SoBillboardKit.h>
// audio stuff
#include <input/SpeechModule.h>
#include <stbapi/interaction/SoSpeechGesture.h>
// sound routine; including sound.h does not work!!!!
#include <stbapi/util/sound.cxx>

#include <stbapi/workspace/SoApplicationKit.h>
#include <stbapi/context/SoContext.h>

// Construct3D Classes and Constants
#include "SoPointKit.h"
#include "SoLineKit.h"
#include "SoPlaneKit.h"
#include "SoCubeKit.h"
#include "SoSphereKit.h"
#include "SoCylinderKit.h"
#include "SoConeKit.h"
#include "SoTextKit.h"
#include "CnDconstants.h"
#include "SoBoolKit.h"
#include "SoCurveKit.h"
#include "SoSurfaceKit.h"
#include "SoIntersectKit.h"
#include "SoPolygonKit.h"
#include "SoUndoRedoListKit.h"
#include "MaterialConstants.h"

#ifdef USE_PUC
#include "SoAddUserWidgetConnectionToFanIn.h"
#include "SoSelectConnectionForUser.h"
#endif

//////////////////////////////////////
// declarations of non-class functions
bool beginequal(const char *longstr, const char *shortstr);


SO_KIT_SOURCE(SoCnDKit);
BASE3D_SOURCE(SoCnDKit);

void 
SoCnDKit::initClass(void)
{
  // init all acis api
  outcome a_return;
  
  // to turn off memory leak logging in ACIS
  // use also terminate_base() in the destructor
  //base_configuration* baseconfig = new base_configuration;
  //baseconfig->enable_audit_leaks=FALSE;
  //baseconfig->enable_audit_logs=FALSE;
  //initialize_base(baseconfig);
  a_return = api_start_modeller(0);
  a_return = api_initialize_faceter();
  a_return = api_initialize_constructors();
  a_return = api_initialize_booleans();
  a_return = api_initialize_intersectors();
  a_return = api_initialize_sweeping();
  
  
  if (!a_return.ok()) cout << "Error: can't initialize Acis api" << endl;
  
  SO_KIT_INIT_CLASS(SoCnDKit, SoContextKit, "SoContextKit");
  BASE3D_INIT_CLASS(SoCnDKit);
  
  GridKit::initClass();
  Object3dKit::initClass();
  SoPointKit::initClass();
  SoLineKit::initClass();
  SoPlaneKit::initClass();
  SoCubeKit::initClass();
  SoSphereKit::initClass();
  SoCylinderKit::initClass();
  SoConeKit::initClass();
  SoTextKit::initClass();
  SoBoolKit::initClass();
  SoCurveKit::initClass();
  SoSurfaceKit::initClass();
  SoIntersectKit::initClass();
  SoPolygonKit::initClass();
  SoCommandKit::initClass();
  SoUndoRedoListKit::initClass();
#ifdef USE_PUC
  SoSelectConnectionForUser::initClass();
  SoAddUserWidgetConnectionToFanIn::initClass();
#endif
}

SoContextKit*
SoCnDKit::factory()
{
  return new SoCnDKit;
}

SoCnDKit::SoCnDKit()
{
  SO_KIT_CONSTRUCTOR(SoCnDKit);
  
  /* Parts for horizontal Projection */
  SO_KIT_ADD_CATALOG_ENTRY(horProSwi,SoSwitch,FALSE, this,\x0,FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(horProSep,SoSeparator,FALSE, horProSwi,\x0,FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(horProCam,SoOrthographicCamera,   FALSE,horProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(horProCom,SoComplexity,   FALSE,horProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(horProEnv,SoEnvironment,   FALSE,horProSep,\x0,TRUE);    
  SO_KIT_ADD_CATALOG_ENTRY(horProLig1,SoDirectionalLight,   FALSE,horProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(horProLig2,SoDirectionalLight,   FALSE,horProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(horProLig3,SoDirectionalLight,   FALSE,horProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(horProLig4,SoDirectionalLight,   FALSE,horProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(horProLig5,SoDirectionalLight,   FALSE,horProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(horProLig6,SoDirectionalLight,   FALSE,horProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(horProSel,SoSelection,     FALSE, horProSep,\x0,TRUE);
  
  /* Parts for vertical Projection */
  SO_KIT_ADD_CATALOG_ENTRY(vertProSwi,SoSwitch,FALSE, this,\x0,FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(vertProSep,SoSeparator,FALSE, vertProSwi,\x0,FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(vertProCam,SoOrthographicCamera,   FALSE,vertProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(vertProCom,SoComplexity,   FALSE,vertProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(vertProEnv,SoEnvironment,   FALSE,vertProSep,\x0,TRUE);    
  SO_KIT_ADD_CATALOG_ENTRY(vertProLig1,SoDirectionalLight,   FALSE,vertProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(vertProLig2,SoDirectionalLight,   FALSE,vertProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(vertProLig3,SoDirectionalLight,   FALSE,vertProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(vertProLig4,SoDirectionalLight,   FALSE,vertProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(vertProLig5,SoDirectionalLight,   FALSE,vertProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(vertProLig6,SoDirectionalLight,   FALSE,vertProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(vertProSel,SoSelection,     FALSE, vertProSep,\x0,TRUE);
  
  /* Parts for left Projection */
  SO_KIT_ADD_CATALOG_ENTRY(leftProSwi,SoSwitch,FALSE, this,\x0,FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(leftProSep,SoSeparator,FALSE, leftProSwi,\x0,FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(leftProCam,SoOrthographicCamera,   FALSE,leftProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(leftProCom,SoComplexity,   FALSE,leftProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(leftProEnv,SoEnvironment,   FALSE,leftProSep,\x0,TRUE);    
  SO_KIT_ADD_CATALOG_ENTRY(leftProLig1,SoDirectionalLight,   FALSE,leftProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(leftProLig2,SoDirectionalLight,   FALSE,leftProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(leftProLig3,SoDirectionalLight,   FALSE,leftProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(leftProLig4,SoDirectionalLight,   FALSE,leftProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(leftProLig5,SoDirectionalLight,   FALSE,leftProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(leftProLig6,SoDirectionalLight,   FALSE,leftProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(leftProSel,SoSelection,     FALSE, leftProSep,\x0,TRUE);
  
  /* Parts for left Projection */
  SO_KIT_ADD_CATALOG_ENTRY(rightProSwi,SoSwitch,FALSE, this,\x0,FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rightProSep,SoSeparator,FALSE, rightProSwi,\x0,FALSE);
  SO_KIT_ADD_CATALOG_ENTRY(rightProCam,SoOrthographicCamera,   FALSE,rightProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rightProCom,SoComplexity,   FALSE,rightProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rightProEnv,SoEnvironment,   FALSE,rightProSep,\x0,TRUE);    
  SO_KIT_ADD_CATALOG_ENTRY(rightProLig1,SoDirectionalLight,   FALSE,rightProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rightProLig2,SoDirectionalLight,   FALSE,rightProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rightProLig3,SoDirectionalLight,   FALSE,rightProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rightProLig4,SoDirectionalLight,   FALSE,rightProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rightProLig5,SoDirectionalLight,   FALSE,rightProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rightProLig6,SoDirectionalLight,   FALSE,rightProSep,\x0,TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(rightProSel,SoSelection,     FALSE, rightProSep,\x0,TRUE);
  
  SO_KIT_INIT_INSTANCE();
  BASE3D_CONSTRUCTOR;
  
  //not in initVariables
  //every new start would overwrite existing save files
  //saveNumber = 1;
  SO_NODE_ADD_FIELD(saveNumber, (1));
  saveNumber.setValue(1);
  autosaveNumber = 1; // does not have to be distributed, is only local save number for autosaving
  // new start does not change Point Button state (pressed or not)
  // therefore resetting value would be wrong
  //drawPoints = TRUE;
  
  SO_NODE_ADD_FIELD(objSum, (0));
  objSum.setValue(0);
  SO_NODE_ADD_FIELD(delNumber, (0));
  delNumber.setValue(0);
  SO_NODE_ADD_FIELD(firstStart, (TRUE));
  firstStart.setValue(TRUE);

  initVariables();
  
  undoList.reserve(100);
  pointList.reserve(100);
  lineList.reserve(100);
  planeList.reserve(100);
  cubeList.reserve(100);
  sphereList.reserve(100);
  cylinderList.reserve(100);
  coneList.reserve(100);
  objectList.reserve(100);
  
  previousNearestObject.reserve(20);
  userGrid.reserve(20);
  
  // fields of Base3D
  blocked.setValue(TRUE);
  stations.deleteValues(0);
  eventMask.setValue(BM_ALL_EVENTS);
  
  SoDB::createGlobalField("c3d_pointCount", SoSFInt32::getClassTypeId())->connectFrom(&pointCount);
  SoDB::createGlobalField("c3d_lineCount", SoSFInt32::getClassTypeId())->connectFrom(&lineCount);
  SoDB::createGlobalField("c3d_planeCount", SoSFInt32::getClassTypeId())->connectFrom(&planeCount);
  SoDB::createGlobalField("c3d_cubeCount", SoSFInt32::getClassTypeId())->connectFrom(&cubeCount);
  SoDB::createGlobalField("c3d_sphereCount", SoSFInt32::getClassTypeId())->connectFrom(&sphereCount);
  SoDB::createGlobalField("c3d_cylinderCount", SoSFInt32::getClassTypeId())->connectFrom(&cylinderCount);
  SoDB::createGlobalField("c3d_coneCount", SoSFInt32::getClassTypeId())->connectFrom(&coneCount);
  // all objects are stored in objectList
  // Boolean objects, Curves and Surfaces are only stored in the objectsList
  SoDB::createGlobalField("c3d_objectCount", SoSFInt32::getClassTypeId())->connectFrom(&objectCount);
  
  //global fields for selected objects
  SoDB::createGlobalField("c3d_selPointCount", SoSFInt32::getClassTypeId())->connectFrom(&selPointCount);
  SoDB::createGlobalField("c3d_selLineCount", SoSFInt32::getClassTypeId())->connectFrom(&selLineCount);
  SoDB::createGlobalField("c3d_selPlaneCount", SoSFInt32::getClassTypeId())->connectFrom(&selPlaneCount);
  SoDB::createGlobalField("c3d_selCubeCount", SoSFInt32::getClassTypeId())->connectFrom(&selCubeCount);
  SoDB::createGlobalField("c3d_selSphereCount", SoSFInt32::getClassTypeId())->connectFrom(&selSphereCount);
  SoDB::createGlobalField("c3d_selCylinderCount", SoSFInt32::getClassTypeId())->connectFrom(&selCylinderCount);
  SoDB::createGlobalField("c3d_selConeCount", SoSFInt32::getClassTypeId())->connectFrom(&selConeCount);
  SoDB::createGlobalField("c3d_selObjectCount", SoSFInt32::getClassTypeId())->connectFrom(&selObjectCount);
  
#ifdef AUTOEXPORT
  // create and schedule timer for auto export of whole scene as VRML every 30 seconds
  printf("VRML auto export timer scheduled. \n");
  SoTimerSensor* autoVRMLexport = new SoTimerSensor(autoexportVRML2, this);
  autoVRMLexport->setInterval(SbTime(30.0));
  autoVRMLexport->schedule();
#endif

#ifdef AUTOSAVE
  // create and schedule timer for auto save of construct3d scene as normal .iv every 37 seconds
  // (interval different to autoVRMLexport so that the saving does not happen at the same time if possible)
  printf("Auto save timer scheduled. \n");
  SoTimerSensor* autoSaveTimer = new SoTimerSensor(autoSave, this);
  autoSaveTimer->setInterval(SbTime(37.0));
  autoSaveTimer->schedule();
#endif

#ifdef USE_PUC
#define ADD_FIELD_AND_SENSOR_HELPER(sensor, funcCB, fiel) {SO_NODE_ADD_FIELD(fiel, ("")); sensor.setData(this); sensor.setFunction(funcCB); sensor.setPriority(0); sensor.attach(&fiel);}
#define ADD_FIELD_AND_SENSOR(a) { ADD_FIELD_AND_SENSOR_HELPER(sensor_##a, cbFunc_##a, a);} 

  ADD_FIELD_AND_SENSOR(lineCommand);
  ADD_FIELD_AND_SENSOR(pointMode);
  ADD_FIELD_AND_SENSOR(degSlider);
  ADD_FIELD_AND_SENSOR(rightProjection);
  ADD_FIELD_AND_SENSOR(undoCommand);
#endif
}

void
SoCnDKit::initVariables()
{
  objSum.setValue(0);
  delNumber.setValue(0);
  
  undoList.clear();
  
  style.setValue(SoDrawStyle::FILLED); // or LINES for preview
  linePattern.setValue(0xffff);
  worked = FALSE;
  buttonPressed = FALSE;
  previousNearestObject.clear();
  
  pointList.clear();
  lineList.clear();
  planeList.clear();
  cubeList.clear();
  sphereList.clear();
  cylinderList.clear();
  coneList.clear();
  objectList.clear();
  
  redoFlag = false;
  loadFlag = false;
  commandline = new SoCommandKit();
  
  preview = false;
  previewWorked = FALSE;
  
  loadInWindowFlag = FALSE;
  
  startvalueSlider.setValue(-1.0);
  
  userGrid.clear();
  
  activeLayerOld.setValue(-1);
  
  counter.setValue(0);
  selectedCalled = FALSE;
  pushButtonCalled = FALSE; 
  
  makeCopy = FALSE;
}

SoCnDKit::~SoCnDKit()
{
  api_terminate_intersectors();
  api_terminate_booleans();
  api_terminate_constructors();
  api_terminate_faceter();
  api_terminate_sweeping();
  api_stop_modeller();
  //terminate_base();
}

SoNode*
findNode(SoNode* root, SbString name, int userID)
{
//    printf("Using the c3d findNode routine\n");
    SoContext* userContext = new SoContext;
    userContext->index.setValue("Stb.Owner");
    userContext->value.setValue(SbString(userID));
    SoGroup* masterGroup = new SoGroup;
    masterGroup->ref();
    masterGroup->addChild(userContext);
    masterGroup->addChild(root);
    SoSearchAction sa;
    sa.setSearchingAll(TRUE);
    sa.setName(name);
    //sa.apply(root);
    sa.apply(masterGroup);
    SoFullPath* path = (SoFullPath*)sa.getPath();
    assert(path != NULL);
    masterGroup->unref();
    return path->getTail();
}


#ifdef USE_PUC
#define DEF_CB_FUNC(a, code) \
	void SoCnDKit::cbFunc_##a(void* data, SoSensor* sensor) { \
	SoCnDKit *self = (SoCnDKit*) data; assert(self!=NULL); \
	code;\
	}

#define DEF_CB_FUNC_DEBUG(field) \
	DEF_CB_FUNC(field, \
	printf(""#field"CB %s\n",self->field.getValue().getString()); \
	)

	DEF_CB_FUNC_DEBUG(pointMode)
	DEF_CB_FUNC_DEBUG(lineCommand)
	DEF_CB_FUNC_DEBUG(degSlider)
	DEF_CB_FUNC_DEBUG(rightProjection)
	DEF_CB_FUNC_DEBUG(undoCommand)
#endif

// Usage: 
// Returns the userId of the user who owns the pipsheet where a certain 
// button (=Node) is pressed by parsing the uniqueNodeName which is 
// generated for the distribution
// Node and button Name must be passed as arguments
// Example & algorithm:
// "Point_" is 6 characters long, Button name looks like this: 
// Point_5_CONTEXTKIT_7f76608
// we take the substring "5_CONTEXTKIT_7f76608" and 
// delete the rest after the userId 
// and convert the string into the integer userID
int 
SoCnDKit::getUserIdFromUniqueNodeName(SoNode* button, SbString name)
{
  int userID = -1;
  SbBool successful=FALSE;
  //+1 because of the "_" at the end of each unique node name such as
  //"Point_userID_23423...."
  int nameLength = name.getLength()+1; 
  
  SbString buttonName = button->getName().getString();
  SbString CutFromString = buttonName.getSubString(nameLength, -1);
  for (int i=1; i<CutFromString.getLength(); i++)
    if ((!successful) && (CutFromString.getSubString(i,i) == "_"))
    {
      CutFromString.deleteSubString(i, -1);
      userID = atoi(CutFromString.getString());
      successful=TRUE;
      return userID;
    }
    if (!successful)
      printf("C3D getUserIdFromUniqueNodeName error: Cannot get userId!\n");
    return userID;
}

// returns highest number of pressed layer button which is the active layer onto
// which we are drawing
int 
SoCnDKit::getActiveLayer()
{
  //    int activeLayer = 0;
  //    for (int i=0;i<usedLayers;i++)
  //        if (layer[i]->isPressed())
  //            activeLayer = i;
  int active = activeLayer.getValue();
  return active;
}

// Shows objects if on active layers, otherwise hides them 
// by setting the showLayer field to SO_SWITCH_NONE
void
SoCnDKit::showLayers()
{
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  for (int i=0;i<usedLayers;i++)
    // if layer is active, switch on all objects in this layer 
    if (layer[i]->on.getValue())
    {
      // get active layer, if i = active layer, leave colors untouched, otherwise fade down to layerDown color
      int aL = getActiveLayer();
      for (int n = 0; n < objSum.getValue(); n++) 
        if (O(n)->layer.getValue() == i)
          if (i == aL)
          {
            //printf("showlayer: activelayer is %d colorstate %d \n",aL, O(n)->colorState.getValue());
            O(n)->showLayer.setValue(SO_SWITCH_ALL);
            if (O(n)->colorState.getValue() > 3 )
              O(n)->colorState.setValue(O(n)->colorState.getValue() - 4);
          }
          else
          {
            // give inactive layer color to objects
            O(n)->showLayer.setValue(SO_SWITCH_ALL);
            // Draw objects which are wireframe in FILLED mode again
            if ((O(n)->name.getValue() != "Point") && (O(n)->name.getValue() != "Curve") &&
              (O(n)->name.getValue() != "Line"))
            {
              SoDrawStyle* drawstyle = (SoDrawStyle*) O(n)->getPart("style", false);
              if (drawstyle->style.getValue() == SoDrawStyle::LINES)
                drawstyle->style.setValue(SoDrawStyle::FILLED);
            }
            
            // every layer != activeLayer would increase the colorState value
            // therefore increase only once
            if (O(n)->colorState.getValue() < 4)
              O(n)->colorState.setValue(O(n)->colorState.getValue() + 4);
          }
    }
    else
    {
      for (int n = 0; n < objSum.getValue(); n++) 
        if (O(n)->layer.getValue() == i)
        {
          O(n)->showLayer.setValue(SO_SWITCH_NONE);
          // if layer switched off, deselect all objects in it
          if (selectionRoot->isSelected(O(n)))
            O(n)->deselect(selectionRoot);
        }
    }
}

// only construction = selection Node will be saved
// if passing root as an arguement, root can be saved too of course
// Savefiles have increasing numbers (saveNumber) save1.iv, save2.iv ...
// overwrites existing Savefiles
void
SoCnDKit::saveFile(int userID)
{
  //  SoUserKit *uk = SoUserManagerKit::getInstance()->getUserKit(userID);
  //  SoContextManagerKit::getInstance()->saveApplication(uk->getCurrentApp());
  //  return;
  
  SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
  //list->setNumofItems(list->getNumofItems() + 1);
  //  SoUndoRedoListKit *copyoflist;// = new SoUndoRedoListKit();
  
  //int numofItems = list->getNumofItems();
  //int listPos = list->getListPos();
  
  //copyoflist = (SoUndoRedoListKit*)list->copy(FALSE);
  //copyoflist->ref();
  //copyoflist->cleanList();
  
  
  SoOutput savefile;
  char filename[20];
  char number[2];
  sprintf(number, "%d", saveNumber.getValue());
  strcpy(filename,"save");                // name of save-files
  strncat(filename, number, 2);
  strncat(filename, ".iv", 3);
  //to save the savefile in the correct home directory - 
  // working directory and not in the workspace dir
  SbString fp  = "./save/";
  fp += filename;
  const char *fname = fp.getString();
  savefile.openFile(fname);
  
  SoWriteAction writeAction(&savefile);
  //writeAction.apply(copyoflist); //writes the "history list" to stdout
  writeAction.apply(list); //writes the "history list" to stdout
  
  savefile.closeFile();
  saveNumber.setValue(saveNumber.getValue()+1);
  
  SbString filePath  = "./";
  filePath += "sound/file_saved";
  sound(filePath);

  //list->setNumofItems(list->getNumofItems() - 1);
  
  //list->setNumofItems(numofItems);
  //list->setListPos(listPos);
  //copyoflist->unref();
  //delete copyoflist;
}

// Exports geometry only. All geometry inside Nodekits is exported
// Savefiles have increasing numbers (saveNumber) save1.wrl, save2.wrl ...
// overwrites existing Savefiles
void
SoCnDKit::exportVRML2(int userID)
{
  SoOutput savefile;
  char filename[20];
  char number[2];
  sprintf(number, "%d", saveNumber.getValue());
  strcpy(filename,"save");                // name of save-files
  strncat(filename, number, 2);
  strncat(filename, ".wrl", 4);
  //to save the savefile in the correct home directory - 
  // working directory and not in the workspace dir
  SbString fp  = "./save/";
  fp += filename;
  const char *fname = fp.getString();
  savefile.openFile(fname);
  
  SoToVRML2Action tovrml2;
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  tovrml2.apply(selectionRoot);
  SoVRMLGroup *newroot = tovrml2.getVRML2SceneGraph();
  newroot->ref();
  
  printf("Writing VRML2.0...\n");
  savefile.setHeaderString("#VRML V2.0 utf8");
  SoWriteAction wra(&savefile);
  wra.apply(newroot);
  
  savefile.closeFile();
  saveNumber.setValue(saveNumber.getValue()+1);
  
  SbString filePath  = "./";
  filePath += "sound/file_saved";
  sound(filePath);
}

// Exports all geometry as VRML automatically every time a TimerSensor is fired
// The scene is always exported to the same fixed filename /save/autoexport.wrl 
// and always overwrites the existing file.
void
SoCnDKit::autoexportVRML2(void *data, SoSensor *sensor)
{
  SoOutput savefile;
  //to save the savefile in the correct home directory - 
  // working directory and not in the workspace dir
  //SbString fp  = "./save/autoexport.wrl";
  SbString fp  = "T:labfuture.wrl";
  const char *fname = fp.getString();
  savefile.openFile(fname);
  
  SoToVRML2Action tovrml2;
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  tovrml2.apply(selectionRoot);
  SoVRMLGroup *newroot = tovrml2.getVRML2SceneGraph();
  newroot->ref();
  
  printf("Writing VRML2.0...\n");
  savefile.setHeaderString("#VRML V2.0 utf8");
  SoWriteAction wra(&savefile);
  wra.apply(newroot);
  savefile.closeFile();
  
  // audio debug output
  //SbString filePath  = "./";
  //filePath += "sound/file_saved";
  //sound(filePath);
}

// Save the construct3d scene as the usual save file every time a TimerSensor is fired
// The scene is exported to autosave/autosaveX.iv where X = 1..n
void
SoCnDKit::autoSave(void *data, SoSensor *sensor)
{
  SoCnDKit *self=(SoCnDKit*)data;
  SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
  list->setNumofItems(list->getNumofItems() + 1);
  
  SoOutput savefile;
  char filename[20];
  char number[2];
  sprintf(number, "%d", self->autosaveNumber);
  strcpy(filename,"autosave");                // name of save-files
  strncat(filename, number, 2);
  strncat(filename, ".iv", 3);
  //to save the savefile in the correct home directory - 
  // working directory and not in the workspace dir
  SbString fp  = "./autosave/";
  fp += filename;
  const char *fname = fp.getString();
  savefile.openFile(fname);
  
  SoWriteAction writeAction(&savefile);
  //writeAction.apply(copyoflist); //writes the "history list" to stdout
  writeAction.apply(list); //writes the "history list" to stdout
  
  savefile.closeFile();
  self->autosaveNumber++;
  
//  SbString filePath  = "./";
//  filePath += "sound/file_saved";
//  sound(filePath);

  list->setNumofItems(list->getNumofItems() - 1);
}


void 
SoCnDKit::browseCB(void* data, const char *filePath)
{
  SoCnDKit *self=(SoCnDKit*)data;
  SoUserKit *uk = SoUserManagerKit::getInstance()->getUserKit(self->currentUserID);
  
  // If loadInWindowFlag is set, then load an own application into a window
  // (WindowKit must be specified in the .iv file - error checking done by
  // SoContextManagerKit::loadApplication)
  // otherwise load a "normal" c3d construction
  if (self->loadInWindowFlag)
  {
    // switch back from loading pipsheet to main pipsheet
    uk->getPip()->getPipSheetKit(self->appID.getValue())->switchPipSheet(0);
    // Loads an iv file which contains a ContextKit (with empty PIP) and a Window 
    // which contains the VRML scene
    int uID = (uk != NULL)?(uk->getUserID()):(-1);
    SoNodeList appList;
    SbName localeName = StbWorkspace::getInstance()->getDefaultLocale();
    
    if (SoContextManagerKit::getInstance()->loadApplication(filePath,appList,uID, localeName) == 1) 
      self->addCommandtoUndoReduList("loadFile", filePath, ((SoApplicationKit*)appList[0])->getContext()->appID.getValue());
    
    self->loadInWindowFlag = FALSE;
    
    // fixme: should give focus back to C3D Application after loading 
    //        other application
    
    // does not work: would require setting a dependency (in project settings) to the loadVRMLKit dll	
    //  loadVRMLKit* loadVRML = new loadVRMLKit;
    //  loadVRML->position.setValue(SbVec3f(0.5,0.5,0.5));
    //  loadVRML->VRMLfilename.setValue("../apps/construct3d/content/hand.wrl");
    //  loadVRML->loadFile("../apps/construct3d/content/hand.wrl");
    //  loadVRML->setPosition(SbVec3f(0.5,0.5,0.5));
  }
  else
  {
    // load file
    SoInput mySceneInput;
    if (!mySceneInput.openFile(filePath)) 
      fprintf(stderr, "Cannot open file %s\n", filePath);
    
    // Read the whole file into the database
    SoSeparator *myGraph = SoDB::readAll(&mySceneInput);
    if (myGraph == NULL) 
      fprintf(stderr, "Problem reading file\n");
    mySceneInput.closeFile();
    
    //redo all command in the undoredoList
    if (myGraph != NULL)
    {
      self->clearScene();
      self->loadFlag = true;
      self->redoAll();
      self->loadFlag = false;
      // If loading worked, switch back to Main menu
      uk->getPip()->getPipSheetKit(self->appID.getValue())->switchPipSheet(0);
    }
  }
}

void
SoCnDKit::loadFile(int userID)
{
  SoUserKit *uk = SoUserManagerKit::getInstance()->getUserKit(userID);
  currentUserID = userID;
  
  fileBrowser* myOwnStbLoader = new fileBrowser;
  // we want to go there from currentDir to: ../apps/construct3d/save
  //myOwnStbLoader->changeLoadPath("../apps/construct3d/save");
  myOwnStbLoader->changeLoadPath("./save");
  //printf("currentdir %s \n", myOwnStbLoader->currentDir);
  
  myOwnStbLoader->setFilter(".iv",FALSE);
  myOwnStbLoader->setFileButtonCallback(browseCB, this);
  
  SoSeparator* loaderSheet=myOwnStbLoader->setupLoaderSheet();
  myOwnStbLoader->fixFilter();
  
  // move loader sheet to appropriate position on C3D PIP
  SoSeparator* scaledSep = new SoSeparator;
  SoTranslation* trans = new SoTranslation;
  trans->translation.setValue(0, 0.06, 0);
  scaledSep->addChild(trans);
  SoScale* scaling = new SoScale;
  scaling->scaleFactor.setValue(0.007f, 0.007f, 0.007f);
  scaledSep->addChild(scaling);
  scaledSep->addChild(loaderSheet);
  
  // texture on sheet button does not work
  SoTexture2* loadSheetTexture = new SoTexture2;
  loadSheetTexture->set("./load.gif");
  uk->getPip()->getPipSheetKit(this->appID.getValue())->addSheet(scaledSep, loadSheetTexture);
  
  // get number of current sheets
  //printf("Number of sheets: %d \n",uk->getPip()->getPipSheetKit(this->appID.getValue())->getSheets()->getNumChildren());
  int numOfSheets = uk->getPip()->getPipSheetKit(this->appID.getValue())->getSheets()->getNumChildren();
  
  uk->getPip()->getPipSheetKit(this->appID.getValue())->switchPipSheet(numOfSheets - 1);
}

// removes all objects from the current scene
void 
SoCnDKit::clearScene()
{
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  int num = selectionRoot->getNumChildren();
  int i;
  for (i = 0; i < num; i++) 
  {
    SoSeparator *looknode = (SoSeparator*) selectionRoot->getChild(0);
    if (selectionRoot->isSelected(looknode))
      selectionRoot->deselect(looknode);
    selectionRoot->removeChild(looknode); // removes whole seperator node
  }
  SbString filePath  = "./";
  filePath += "sound/welcome";
  sound(filePath);
  
  for (i=1; i < usedLayers;i++)
    if (layer[i]->on.getValue())
    {
      layer[i]->on.setValue(FALSE);
      //layer[i]->buttonColor.setValue(0.7,0.65,0.7);
    }
    
    initVariables();
}

Object3dKit*
SoCnDKit::O(int n)
{
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  Object3dKit *looknode = (Object3dKit*) selectionRoot->getChild(n);
  //    if (looknode->isOfType(Object3dKit::getClassTypeId()))
  //	{
  //		printf("HERE\n");
  //		Object3dKit* myNode;
  //		mynode = (Object3dKit*) looknode;
  //	}
  return looknode;
}

// Draws all selected Objects in Wireframe Mode
// if no objects are selected, all Objects are drawn in full (standard) mode
void
SoCnDKit::wireframeObjects()
{
  countObjects(TRUE);
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  if (selectionRoot->getNumSelected() == 0)
    for(int n = 0; n < objSum.getValue(); n++)
    {
      SoDrawStyle* drawstyle = (SoDrawStyle*) O(n)->getPart("style", false);
      drawstyle->style.setValue(SoDrawStyle::FILLED);
    }
    
    for(int n = 0; n < objSum.getValue(); n++)
    {
      if (selectionRoot->isSelected(O(n)))
      {
        if ((O(n)->name.getValue() != "Point") && (O(n)->name.getValue() != "Curve") &&
          (O(n)->name.getValue() != "Line"))
        {
          SoDrawStyle* drawstyle = (SoDrawStyle*) O(n)->getPart("style", false);
          if (drawstyle->style.getValue() == SoDrawStyle::LINES)
            drawstyle->style.setValue(SoDrawStyle::FILLED);
          else
            drawstyle->style.setValue(SoDrawStyle::LINES);
        }
      }
    }
}

void
SoCnDKit::deselectAll()
{
  countObjects(TRUE);
  addCommandtoUndoReduList("deselectAll", "object", "object", objectList, this->userID.getValue());
  
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  for(int n = 0; n < objSum.getValue(); n++)
  {
    O(n)->deselect(selectionRoot);
    //		SbVec3f x(0,0,0);
    //		printf("%3f \n", O(n)->getDistance(x));;
  }
}

void
SoCnDKit::deselectAllObjects()
{
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  for(int n = 0; n < objSum.getValue(); n++)
  {
    O(n)->deselect(selectionRoot);
    //		SbVec3f x(0,0,0);
    //		printf("%3f \n", O(n)->getDistance(x));;
  }
}

void
SoCnDKit::selectAllObjects(SoCommandKit* command)
{
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  for (int i=0; i < command->getNumofObjectNames(); i++)
  {
    Object3dKit *object = (Object3dKit*)SoNode::getByName(command->getObjectNames(i));
    object->select(selectionRoot);
  }
}

void 
SoCnDKit::selectAllDependentObjects()
{
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  countObjects(true);
  if(objectList.size() > 0)
  {
    O(objectList[0])->select(selectionRoot);
    O(objectList[0])->selectAllDependentObjects(selectionRoot);
    addCommandtoUndoReduList("selectdepenentObjects",O(objectList[0])->getName());
  }
}

void 
SoCnDKit::deselectAllDependentObjects(SoCommandKit* commandline)
{
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  (commandline->getObject())->deselectAllDependentObjects(selectionRoot);
  (commandline->getObject())->select(selectionRoot);
}

// minimal is the childNumber of the object with the shortest
// distance to penPosition
// minDistance is the actual distance of Object(minimal) to penPosition
// returns FALSE if no objects are in visible layers
// returns TRUE if visible objects are present
SbBool 
SoCnDKit::calculateNearestObject(SbVec3f penPosition, int& minimal, float& minDistance)
{
  float thisDistance = maxDistance;
  minDistance = maxDistance;
  int    i;//, minimal;
  SbBool anyVisible = FALSE;
  // check all Objects (i<=objSum)
  // calculate minimal distance to visible Elements only
  // and remember number 'minimal' of element with smallest distance
  for (i = 0; i < objSum.getValue(); i++) 
  {
    if (((O(i)->visible.getValue()) == SO_SWITCH_ALL) && 
      (O(i)->showLayer.getValue()) == SO_SWITCH_ALL)
    {
      thisDistance = O(i)->getDistance(penPosition);
      if (thisDistance<minDistance)
      {
        minDistance = thisDistance;
        minimal = i;
      }
      anyVisible = TRUE;
    }
  }
  return anyVisible;
}

void 
SoCnDKit::selectNearestObject(SbVec3f penPosition)
{
  if (!preview)
  {
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    if (selectionRoot->getNumChildren() > 0)        
    {
      int minimal;
      float minDistance;
      SbBool anyVisible = calculateNearestObject(penPosition, minimal, minDistance);

      if (!anyVisible)
      {
        SbString filePath  = "./";
        filePath += "sound/notenough";
        sound(filePath);
      }
      //        printf("minimal: \t%i %.3f \n", minimal , minDistance);
      if (minDistance > deselectDist)
        deselectAll();
      else
      {
        //select child with number "minimal" 
        O(minimal)->select(selectionRoot);
   	    // set layer button in Property Menu for current seleted object
        // for all users (done by propLayerButtonCB Sensor)
        if (selectionRoot->isSelected(O(minimal)))
        {
          // take care that propLayer Callback is not called when selecting
          // objects. Otherwise the callback would react as if the propLayer button
          // has been pressed and would throw all selected objects into the current layer
          int currentLayer = O(minimal)->layer.getValue();
//          propLayerSensor[currentLayer]->detach();
//          propLayer[currentLayer]->on.setValue(TRUE);          
//          propLayerSensor[currentLayer]->attach(&propLayer[currentLayer]->on);
        }
        // switching between the different objects (sorted list)
        // with the 1st button (confirmation with 2nd button) 
        // would possible here
      
        SbString filePath  = "./";
        filePath += "sound/click";
        sound(filePath);
      
        addCommandtoUndoReduList("select", O(minimal)->getName());
      }
    } else
    {
      SbString filePath  = "./";
      filePath += "sound/notenough";
      sound(filePath);
    }
    setObjectCount();
  }
}

void 
SoCnDKit::selectNearestObject(SbName objectName)
{
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  if (selectionRoot->getNumChildren() > 0)        
  {
    //select child with number "minimal" 
    Object3dKit* object = NULL;
    object = (Object3dKit*)SoNode::getByName(objectName);
    //if (object)
    {
      object->select(selectionRoot);
      
      SbString filePath  = "./";
      filePath += "sound/click";
      sound(filePath);      
    }
  }
  setObjectCount();
}

void 
SoCnDKit::highlightNearestObject(SbVec3f penPosition, int userID)
{
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  if ((selectionRoot->getNumChildren() > 0) && (!preview))
  {
    int minimal;
    float minDistance; // distance of O(minimal) to penPosition
    SbBool anyVisible = calculateNearestObject(penPosition, minimal, minDistance);
    if (!anyVisible)
      return;
    
    if (minDistance > deselectDist)
    {
      O(previousNearestObject[userID])->highlighted.setValue(FALSE);
      O(minimal)->highlighted.setValue(FALSE);
      return;
    }
    
    // We check if object is already highlighted, if not
    // it is highlighted and the previous highlighted object is dehighlighted
    // Object3dKit::isInterested sets dragging color independently
    // Highlight child with number "minimal"
    if (O(minimal)->dragBoxIntrusion.getValue() == FALSE)
    {
      if (previousNearestObject[userID] != minimal)
      {
        O(previousNearestObject[userID])->highlighted.setValue(FALSE);
        O(minimal)->setHighlightMaterial(O(minimal)->userID.getValue());
      }
      if (O(minimal)->highlighted.getValue() == FALSE)
      {
        O(minimal)->highlighted.setValue(TRUE);
        O(minimal)->setHighlightMaterial(userID);
        // set correct texture of user that is currently near the plane  
        if (O(minimal)->name.getValue() == "Plane")  
          if (O(minimal)->colorState.getValue() < 4) // only if plane is in an active state/layer  
            MaterialConstants::changePlaneTexture((SoPlaneKit*)O(minimal), 0, userID);
          //((SoPlaneKit*)O(minimal))->changePlaneTexture(0, userID);
          if (O(minimal)->name.getValue() == "Cube")  
            if (O(minimal)->colorState.getValue() < 4) // only if plane is in an active state/layer  
              MaterialConstants::changeCubeTexture((SoCubeKit*)O(minimal), 0, userID);
      }
      
      previousNearestObject[userID] = minimal;
      //printf("User %d has the nearest object %d \n",userID, minimal);
    }
  }
}

void
SoCnDKit::deleteSelectedObjects()
{
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  int num_selected = selectionRoot->getNumSelected();
  if (num_selected == 0)
  {
    SbString filePath  = "./";
    filePath += "sound/del";
    sound(filePath);
  } else
  {
    countObjects(TRUE);
    bool allselected = true;
	//ATTENTION: temporarily taking out feature that all dependent objects are automatically 
	// selected and deleted when pressing again - needed for Tschupik cube example
/*    for (int a=0; a < objectList.size(); a++)
    {
      if (O(objectList[a])->areAllDependentObjectsSelected(selectionRoot) == 0)
      {
        //O(objectList[a])->selectAllDependentObjects(selectionRoot);
        selectAllDependentObjects();
        allselected = false;
      }
    }
*/    
    if (allselected)
    {
      addCommandtoUndoReduList("delete", "object", "object", objectList, this->userID.getValue());   
      
      SoPath *selectedPath;
      int pathNumber = 0;
      for (int i = 0; i < num_selected; i++) 
      {
        SbBool isDeleted = FALSE;
        
        // here index 0 is used for getPath because:
        // assume you select 2 objects, first one gets deleted, then the
        // second one gets index 0 again !!! (not index 1 that it had
        // before deletion of the first object)
        selectedPath = selectionRoot->getPath(pathNumber);
        Object3dKit *parent = (Object3dKit*)selectedPath->getNode(1);
        // identify object by its childNumber and delete it
        int objNumber = parent->childNumber.getValue();
        printf("delete object num: % i\n",objNumber);
        
        for (int j=0;j<usedLayers;j++)
          if (layer[j]->on.getValue())
            if (parent->layer.getValue() == j)
            {
              O(objNumber)->deselect(selectionRoot);
              O(objNumber)->deleteObject();
              if (undoList.size() <= delNumber.getValue())
                undoList.push_back(objNumber);
              else
                undoList[delNumber.getValue()] = objNumber;
              delNumber.setValue(delNumber.getValue()+1);
              isDeleted = TRUE;
            }
            
            if (! isDeleted)
              pathNumber++;
            
      }
    }
  }
}

void
SoCnDKit::undodeleteSelectedObjects(SoCommandKit* command)
{
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  for (int i=0; i < command->getNumofObjectNames(); i++)
  {
    Object3dKit *object = (Object3dKit*)SoNode::getByName(command->getObjectNames(i));
    object->undodelete();
    object->select(selectionRoot);
  }
}

void
SoCnDKit::deleteOneObject(SbName ObjectName)
{
  std::cout << "delete one object" << std::endl;
  addCommandtoUndoReduList("deleteOne", ObjectName, this->userID.getValue());  
  Object3dKit *object = (Object3dKit*)SoNode::getByName(ObjectName);
  object->deleteObject();   
}

void
SoCnDKit::undodeleteOneObject(SbName ObjectName)
{
  Object3dKit *object = (Object3dKit*)SoNode::getByName(ObjectName);
  object->undodelete();   
}

void
SoCnDKit::undo()
{
  SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  if (list->getListPos() >= 0)
  {
    list->getCommandLine()->PrintoutNode();
    if (list->getCommand() == "add")
    {
      countObjects(TRUE);
      deselectAllObjects();
      list->getCommandLine()->Undo(selectionRoot, &objSum);
    }
	else if (list->getCommand() == "translate")
	{
		SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
		SoCommandKit* command = list->getCommandLine();
		// select objects, but with the last two Points swapped
		deselectAllObjects();
		int nrObjects = command->getNumofObjectNames();
		int c1; // counter
		Object3dKit* object;
		for(c1 = 0; c1<nrObjects-2; c1++) {
			object = (Object3dKit*)SoNode::getByName(command->getObjectNames(c1));
			object->select(selectionRoot);
		} // for c1
		object = (Object3dKit*)SoNode::getByName(command->getObjectNames(nrObjects-1));
		object->select(selectionRoot);
		object = (Object3dKit*)SoNode::getByName(command->getObjectNames(nrObjects-2));
		object->select(selectionRoot);
		translate(command->getUserID(), FALSE);
	}
	else if (list->getCommand() == "rotate")
	{
		SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
		SoCommandKit* command = list->getCommandLine();

		// select objects, but with the last two Points swapped
		deselectAllObjects();
		int nrObjects = command->getNumofObjectNames();
		int c1; // counter
		Object3dKit* object;
		for(c1 = 0; c1<nrObjects-2; c1++) {
			object = (Object3dKit*)SoNode::getByName(command->getObjectNames(c1));
			object->select(selectionRoot);
		} // for c1
		object = (Object3dKit*)SoNode::getByName(command->getObjectNames(nrObjects-1));
		object->select(selectionRoot);
		object = (Object3dKit*)SoNode::getByName(command->getObjectNames(nrObjects-2));
		object->select(selectionRoot);
		rotate(command->getUserID(), FALSE);
	}
	else if (list->getCommand() == "mirror")
	{
		SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
		SoCommandKit* command = list->getCommandLine();

		// select objects again
		deselectAllObjects();
		int nrObjects = command->getNumofObjectNames();
		int c1; // counter
		Object3dKit* object;
		for(c1 = 0; c1<nrObjects; c1++) {
			object = (Object3dKit*)SoNode::getByName(command->getObjectNames(c1));
			object->select(selectionRoot);
		} // for c1
		mirror(command->getUserID(), FALSE);
	}

    //TODO: convert IFs in ELSE IFs
	if (list->getCommand() == "select")
    {
      SoCommandKit* command = list->getCommandLine();
      selectNearestObject(command->getObjectName());
    }
    if (list->getCommand() == "move")
    {
      SoPointKit* point = (SoPointKit*)list->getObject();
      point->Update(list->getCommandLine()->getStartPosition());
    }
    if (list->getCommand() == "rebuild")
    {
      if ( (list->getCommandLine()->getObjectType() == "CurveBezier") || (list->getCommandLine()->getObjectType() == "CurvePoint") )
      {
        SoCurveKit* curve = (SoCurveKit*)list->getCommandLine()->getObject();
        curve->rebuild(list->getCommandLine()->getDegreeOld());
      }
    }
    if (list->getCommand() == "delete")
    {
      undodeleteSelectedObjects(list->getCommandLine());
    }
    if (list->getCommand() == "deleteOne")
    {
      undodeleteOneObject(list->getCommandLine()->getObjectName());
    }
    if (list->getCommand() == "deselectAll")
    {
      selectAllObjects(list->getCommandLine());
    }
    if (list->getCommand() == "slider")
    {
      undoslider(list->getCommandLine());
    }
    if (list->getCommand() == "selectdepenentObjects")
    {
      deselectAllDependentObjects(list->getCommandLine());
    }
    if (list->getCommand() == "layer")
    {
      undolayer(list->getCommandLine());
    }
    if (list->getCommand() == "changeObjectLayer")
    {
      undoObjectLayer(list->getCommandLine());
    }
    if ((list->getCommand() == "release") || (list->getCommand() == "press"))
    {
      undoPushButton(list->getCommandLine());
    }
    if (list->getCommand() == "loadFile")
    {
      undoLoadVRMLFile(list->getCommandLine());
    }
  }
  else 
  {
    std::cout << "no command in undolist" << std::endl;
    return;
  }
  
  // For correct highlighting set previousNearestObject[userID] to 0 after undo
  // in order to avoid conflicts with a non existing number 
  // in previousNearestObject - objects which have been removed with undo
  SoUserManagerKit* uk = SoUserManagerKit::getInstance();
  for (int i=0; i < uk->getUserNumber(); i++)
    previousNearestObject[i] = 0;
  
  list->printoutUndoRedoList();
  std::cout << "****************** "; list->getCommandLine()->PrintoutNode(); 
  // decrease list position after printing out undoredolist
  // otherwise listpos is wrong at printout and causes crash!
  list->decListPos();
  printf("Listpos %i \n",list->getListPos());
}

void
SoCnDKit::undoPreview()
{
  std::cout << "********* undoPreview" << std::endl;
  commandline->PrintoutNode();
  countObjects(TRUE);
  deselectAllObjects();
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  commandline->Undo(selectionRoot, &objSum);
}


void SoCnDKit::undoslider(SoCommandKit* commandline)
{
  SoUserManagerKit* uk = SoUserManagerKit::getInstance();
  int userNumber = uk->getUserNumber();
  //int userIndex = SoUserManagerKit::getInstance()->getUserKitIndex(userID.getValue());
  int userIndex = userID.getValue();
  
  SoNode* pipSheet = uk->getUserKit(userIndex)->getPip()->getPipSheetKit(appID.getValue());
  SoIncrementalSlider* slider = (SoIncrementalSlider*)findNode(pipSheet,makeUniqueNodeName(commandline->getObjectName().getString(), userIndex), userIndex);
  
  ((SoIncrementalSlider*)slider)->alphaIn.setValue(commandline->getSliderValueOld());
}

void SoCnDKit::redoslider(SoCommandKit* commandline)
{
  SoUserManagerKit* uk = SoUserManagerKit::getInstance();
  int userNumber = uk->getUserNumber();
  //int userIndex = SoUserManagerKit::getInstance()->getUserKitIndex(userID.getValue());
  int userIndex = userID.getValue();
  
  SoNode* pipSheet = uk->getUserKit(userIndex)->getPip()->getPipSheetKit(appID.getValue());
  SoIncrementalSlider* slider = (SoIncrementalSlider*)findNode(pipSheet,makeUniqueNodeName(commandline->getObjectName().getString(), userIndex), userIndex);
  
//  ((SoIncrementalSlider*)slider)->setAlpha(commandline->getSliderValueNew());
  ((SoIncrementalSlider*)slider)->alphaIn.setValue(commandline->getSliderValueNew());
}


void SoCnDKit::undolayer(SoCommandKit* commandline)
{
  this->activeLayer.setValue(commandline->getAktiveLayerOld());
  this->layerActive[commandline->getAktiveLayerOld()]->on.setValue(TRUE);
  
  this->layerActive[commandline->getAktiveLayerNew()]->on.setValue(FALSE);
  
  showLayers(); 
}

void SoCnDKit::redolayer(SoCommandKit* commandline)
{
  std::cout << "redo layer" << std::endl;
  this->activeLayer.setValue(commandline->getAktiveLayerNew());
  this->layerActive[commandline->getAktiveLayerNew()]->on.setValue(TRUE);
  
  this->layerActive[commandline->getAktiveLayerOld()]->on.setValue(FALSE);
  
  showLayers(); 
}

void SoCnDKit::undoObjectLayer(SoCommandKit* commandline)
{
  Object3dKit* object = NULL;
  for (int index=0; index < commandline->getNumofObjectNames(); index++)
  {
    object = (Object3dKit*)SoNode::getByName(commandline->getObjectNames(index));
    object->layer.setValue(commandline->getAktiveLayerOld());
  }
  
  this->propLayer[commandline->getAktiveLayerNew()]->on.setValue(FALSE);
  this->propLayer[commandline->getAktiveLayerOld()]->on.setValue(TRUE);
  
  selectAllObjects(commandline);
  
  showLayers(); 
}

void SoCnDKit::redoObjectLayer(SoCommandKit* commandline)
{
  Object3dKit* object = NULL;
  for (int index=0; index < commandline->getNumofObjectNames(); index++)
  {
    object = (Object3dKit*)SoNode::getByName(commandline->getObjectNames(index));
    object->layer.setValue(commandline->getAktiveLayerNew());
  }
  
  this->propLayer[commandline->getAktiveLayerNew()]->on.setValue(TRUE);
  this->propLayer[commandline->getAktiveLayerOld()]->on.setValue(FALSE);
  
  showLayers(); 
}

void SoCnDKit::undoPushButton(SoCommandKit* commandline)
{
  SoUserManagerKit* uk = SoUserManagerKit::getInstance();
  int userNumber = uk->getUserNumber();
  //int userIndex = SoUserManagerKit::getInstance()->getUserKitIndex(userID.getValue());
  //int userIndex = SoUserManagerKit::getInstance()->getUserKitIndex(commandline->getUserID());
  // hk: 7.12.2003 - changed userIndex calculation
  int userIndex = commandline->getUserID();
  
  SoNode* pipSheet = uk->getUserKit(userIndex)->getPip()->getPipSheetKit(appID.getValue());
  SoPushButton* button = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName(commandline->getObjectName().getString(), userIndex), userIndex);
  
  if (commandline->getCommand() == "press")
  {
    drawPoints.set1Value(userIndex, 0); //Point mode turned OFF
    button->on.setValue(FALSE);
  }
  if (commandline->getCommand() == "release")
  {
    drawPoints.set1Value(userIndex, 1); //Point mode turned ON
    button->on.setValue(TRUE);
  }
}

void SoCnDKit::redoPushButton(SoCommandKit* commandline)
{
  SoUserManagerKit* uk = SoUserManagerKit::getInstance();
  int userNumber = uk->getUserNumber();
  //int userIndex = SoUserManagerKit::getInstance()->getUserKitIndex(commandline->getUserID());
  // hk: 7.12.2003 - changed userIndex calculation
  int userIndex = commandline->getUserID();
  
  SoNode* pipSheet = uk->getUserKit(userIndex)->getPip()->getPipSheetKit(appID.getValue());
  SoPushButton* button = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName(commandline->getObjectName().getString(), userIndex), userIndex);
  
  if (commandline->getCommand() == "release")
  {
    drawPoints.set1Value(userIndex, 0); //Point mode turned OFF
    button->on.setValue(FALSE);
  }
  if (commandline->getCommand() == "press")
  {
    drawPoints.set1Value(userIndex, 1); //Point mode turned ON
    button->on.setValue(TRUE);
  }
}

void SoCnDKit::undoLoadVRMLFile(SoCommandKit* commandline)
{
  //stop the application 
  SoContextManagerKit::getInstance()->stopApplication(commandline->getAppID());
}

void SoCnDKit::redoLoadVRMLFile(SoCommandKit* commandline)
{
  SoUserKit *uk = SoUserManagerKit::getInstance()->getUserKit(currentUserID);
  
  // switch back from loading pipsheet to main pipsheet
  uk->getPip()->getPipSheetKit(appID.getValue())->switchPipSheet(0);
  // Loads an iv file which contains a ContextKit (with empty PIP) and a Window 
  // which contains the VRML scene
  int uID = (uk != NULL)?(uk->getUserID()):(-1);
  SoNodeList appList;
  SbName localeName = StbWorkspace::getInstance()->getDefaultLocale();
  
  if (SoContextManagerKit::getInstance()->loadApplication(commandline->getFilePath().getString(),appList,uID, localeName) == 1) 
    commandline->setAppID(((SoApplicationKit*)appList[0])->getContext()->appID.getValue());
  
}

void SoCnDKit::redoAll()
{
  SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List"); 

  int numofItems;
  numofItems = list->countNumofItems();
  
  std::cout << "numofitems: " << numofItems << std::endl;
  list->printoutUndoRedoList();

  list->setListPos(-1);
  
  for (int i = 0; i <= numofItems; i++)
  {
	std::cout << list->getListPos() + 1 << " " << i << " ";
    // redo command only if it is not identical to last command 
	// identical commands can happen due to a widget bug
	// if identical, skip current command and go to the next one
	// check for all fields that can be identical (not time, not commandPos)
	SoCommandKit *last_command = NULL;
	SoCommandKit *current_command = NULL;
	SbBool doubleEntry = FALSE;
		if (list->getListPos() >= 0)
			last_command = list->getCommandLine(list->getListPos());
		current_command = list->getCommandLine(list->getListPos() + 1);
	
		if (last_command && current_command)
		{
			if ((last_command->getCommand() == current_command->getCommand()) &&
				(last_command->getObjectName() == current_command->getObjectName()) &&
				(last_command->getObjectType() == current_command->getObjectType()) &&
				(last_command->getUserID() == current_command->getUserID()) &&
				(last_command->getNumofObjectNames() == current_command->getNumofObjectNames()))
			{
				// additionally check if all selected Objects are equal as well
				// equalSelectedObjects is FALSE if not all selected Objects are equal
				SbBool equalSelectedObjects = TRUE; 
				int j;
				for(j=0; j <= current_command->getNumofObjectNames(); j++)
					if (last_command->getinvolvedObjectName(j) != current_command->getinvolvedObjectName(j))
						equalSelectedObjects = FALSE;

				// The "select" command can be in a file multiple times 
				// - selecting and deselecting an object multiple times
				// moving the same object multiple times is ok too
				// ATTENTION: There might be other exceptions as well !!!
				if (equalSelectedObjects && 
					(current_command->getCommand() != "select") &&
					(current_command->getCommand() != "move"))
				{
					printf("RedoAll: MULTIPLE EQUAL ENTRIES IN FILE !!!!!!! I skip multiple equal entries! \n");
					doubleEntry = TRUE;
				}
			}
		}
		if (!doubleEntry)
			redo();
		else
			list->incListPos();
  }
  
  //set numofItems = undo_redo_List_pos 
  //because after redo the numofItems is not correct
  list->setNumofItems(numofItems);
}

void SoCnDKit::redo()
{
  // counter
  int c1;
  if (loadFlag)
    redoFlag = false; //write command to undoredoList
  else
    redoFlag = true; //don't write command to undoredoList
  
  SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
  
  if(((list->getNumofItems() > list->getListPos()) && list->getNumofItems() >= 0) || loadFlag)
  {
    SoCommandKit* command = NULL;
    command = list->getCommandLine(list->getListPos() + 1);
	
	if (command)
	{
		//std::cout << "***************** REDO *****************" << std::endl;
		std::cout << list->getListPos() + 1 << " " << list->countNumofItems() + 1 << " ";
		command->PrintoutNode();
    
		if (command->getCommand() == "add")
		{
		  if (command->getObjectType() == "Sphere")
			addSphere(command->getUserID());
		  else if (command->getObjectType() == "Line")
			addLine(command->getUserID());
		  else if (command->getObjectType() == "Normalplane")
			addPlane(true, false, command->getUserID());
		  else if (command->getObjectType() == "Symmetryplane")
			addPlane(true, true, command->getUserID());
		  else if (command->getObjectType() == "Plane")
			addPlane(false, false, command->getUserID());
		  else if (command->getObjectType() == "Cube")
			addCube(command->getUserID());
		  else if (command->getObjectType() == "Cylinder")
			addCylinder(command->getUserID());
		  else if (command->getObjectType() == "Cone")
			addCone(command->getUserID());
		  else if (command->getObjectType() == "BoolIntersection")
			addBool(BOOL_INTERSECTION, command->getUserID());
		  else if (command->getObjectType() == "BoolUnion")
			addBool(BOOL_UNION, command->getUserID());
		  else if (command->getObjectType() == "BoolCut")
			addBool(BOOL_CUT, command->getUserID());
		  else if (command->getObjectType() == "Intersect")
			addIntersect(command->getUserID());
		  else if (command->getObjectType() == "Slice")
			addSlice(command->getUserID());
		  else if (command->getObjectType() == "Sweep")
			addSweep(command->getUserID());
		  else if (command->getObjectType() == "AngleBisector")
			addAngleBisector(command->getUserID());
		  else if (command->getObjectType() == "Plane3Points")
			addPlanethreePoints(command->getUserID());
		  else if (command->getObjectType() == "CurveBezier")
			addCurve(CURVE_BEZIER, command->getUserID());
		  else if (command->getObjectType() == "CurvePoint")
			addCurve(CURVE_POINTS_ON, command->getUserID());
		  else if (command->getObjectType() == "Ellipse")
			addEllipse(ELLIPSE_CURVE, command->getUserID());
		  else if (command->getObjectType() == "Circle")
			addEllipse(CIRCLE,command->getUserID());
		  else if (command->getObjectType() == "SurfaceBspline")
			addSurface(SURFACE_B_SPLINE, command->getUserID());
		  else if (command->getObjectType() == "SurfacePoint")
			addSurface(SURFACE_POINTS_ON, command->getUserID());
		  else if (command->getObjectType() == "Point")
			addPoint(command->getPosition(), false, true, command->getUserID());
		  else if (command->getObjectType() == "fixPoint")
			addPoint(command->getPosition(), false, false, command->getUserID());
		  else if (command->getObjectType() == "centrePoint")
			addPoint(command->getPosition(), true, false, command->getUserID());
		  else if (command->getObjectType() == "Text")
			measure(command->getUserID());
		  if (!loadFlag)
			list->incListPos();    
		} 
		else if (command->getCommand() == "move")
		{
		  SoPointKit* point = (SoPointKit*)command->getObject();
		  command->PrintoutNode();
		  point->Update(command->getEndPosition());
		  list->incListPos();
		} 
		else if (command->getCommand() == "select")
		{
		  selectNearestObject(command->getObjectName());
		  list->incListPos();
		} 
		else if (command->getCommand() == "rebuild")
		{
		  if ( (command->getObjectType() == "CurveBezier") || (command->getObjectType() == "CurvePoint") )
		  {
			SoCurveKit* curve = (SoCurveKit*)command->getObject();
			curve->rebuild(command->getDegree());
			list->incListPos();
		  }
		} 
		else if (command->getCommand() == "delete")
		{
		  deleteSelectedObjects();
		  if (!loadFlag)
			list->incListPos();
		}
		else if (command->getCommand() == "deleteOne")
		{
		  deleteOneObject(command->getObjectName());
		  if (!loadFlag)
		    list->incListPos();
		}
		else if (command->getCommand() == "deselectAll")
		{
		  deselectAllObjects();
		  list->incListPos();
		} 
		else if (command->getCommand() == "selectdepenentObjects")
		{
		  selectAllDependentObjects();
		  list->incListPos();
		}
		else if (command->getCommand() == "slider")
		{
		  redoslider(command);
		  list->incListPos();
		}
		else if (command->getCommand() == "layer")
		{
		  redolayer(command);
		  list->incListPos();
		}
		else if (command->getCommand() == "changeObjectLayer")
		{
		  redoObjectLayer(command);
		  list->incListPos();
		}
		else if ((command->getCommand() == "press") || (command->getCommand() == "release"))
		{
		  redoPushButton(command);
		  list->incListPos();
		}
		else if (command->getCommand() == "loadFile")
		{
		  redoLoadVRMLFile(command);
		  list->incListPos();
		}
		else if (command->getCommand() == "translate")
		{
			SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
			deselectAllObjects();
			for(c1 = 0; c1<command->getNumofObjectNames(); c1++) {
				Object3dKit *object = (Object3dKit*)SoNode::getByName(command->getObjectNames(c1));
				object->select(selectionRoot);
			} // for c1
			translate(command->getUserID(), FALSE);
			list->incListPos();
		}
		else if (command->getCommand() == "rotate")
		{
			SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
			deselectAllObjects();
			for(c1 = 0; c1<command->getNumofObjectNames(); c1++) {
				Object3dKit *object = (Object3dKit*)SoNode::getByName(command->getObjectNames(c1));
				object->select(selectionRoot);
			} // for c1
			rotate(command->getUserID(), FALSE);
			list->incListPos();
		}
		else if (command->getCommand() == "mirror")
		{
			SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
			deselectAllObjects();
			for(c1 = 0; c1<command->getNumofObjectNames(); c1++) {
				Object3dKit *object = (Object3dKit*)SoNode::getByName(command->getObjectNames(c1));
				object->select(selectionRoot);
			} // for c1
			mirror(command->getUserID(), FALSE);
			list->incListPos();
		}
	 }
	 else
       std::cout << "no command in redolist commmandline == NULL" << std::endl;
  }
  else
    std::cout << "no command in redolist" << std::endl;
  
  //list->printoutUndoRedoList();
  redoFlag = false;
}

void SoCnDKit::addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType, SoMFName* objects)
{
  if ((!redoFlag) && (!preview))
  {
    SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
    list->addCommandtoUndoReduList(command, objectName, objectType, objects, -1);
  }
  if (preview)
    commandline->setCommandLine(command, objectName, objectType, objects, -1);
}

void SoCnDKit::addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType, std::vector<int> objects, int userID)
{
  std::cout << "redoFlag: " << redoFlag << " preview: " << preview << std::endl;
  //save all selected object for reselect after undo 
  SoMFName objectNames;
  objectNames.setNum(objects.size());
  for (unsigned int i=0; i < objects.size(); ++i)
    objectNames.set1(i,O(objects[i])->getName().getString());
  
  if ((!redoFlag) && (!preview))
  {   
    SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
    list->addCommandtoUndoReduList(command, objectName, objectType, &objectNames, userID);
    //list->printoutUndoRedoList();
  }
  if (preview)
  {
    commandline->setCommandLine(command, objectName, objectType, &objectNames, userID);
  }
}

void SoCnDKit::addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType, SbVec3f eventPos, std::vector<int> objects, int userID)
{
  std::cout << "redoFlag: " << redoFlag << " preview: " << preview << std::endl;
  //save all selected object for reselect after undo 
  SoMFName objectNames;
  objectNames.setNum(objects.size());
  for (unsigned int i=0; i < objects.size(); ++i)
    objectNames.set1(i,O(objects[i])->getName().getString());
  
  if ((!redoFlag) && (!preview))
  {   
    SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
    list->addCommandtoUndoReduList(command, objectName, objectType, eventPos, &objectNames, userID);
  }
}

void SoCnDKit::addCommandtoUndoReduList(SbName command, SbName objectName, SbName objectType,int degreeold, int degree)
{
  std::cout << "preview:" << preview << std::endl;
  if ((!redoFlag) && (!preview))
  {
    SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
    list->addCommandtoUndoReduList(command, objectName, objectType, degreeold, degree);
    list->printoutNode();
  }
}

void SoCnDKit::addCommandtoUndoReduList(SbName command, SbName objectName)
{
  if (!redoFlag && (!preview))
  {
    SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
    list->addCommandtoUndoReduList(command, objectName);
  }
}

void SoCnDKit::addCommandtoUndoReduList(SbName command, SbName objectName, int userID)
{
  if (!redoFlag && (!preview))
  {
    SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
    list->addCommandtoUndoReduList(command, objectName, userID);
  }
}

void SoCnDKit::addCommandtoUndoReduList(SbName command, int activeLayerNew, int activeLayerOld)
{
  SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
  list->addCommandtoUndoReduList(command, activeLayerNew, activeLayerOld);
}

void SoCnDKit::addCommandtoUndoReduList(SbName command, std::vector<int> objects, int activeLayerNew, int activeLayerOld)
{
  //save all selected object for reselect after undo 
  SoMFName objectNames;
  objectNames.setNum(objects.size());
  for (unsigned int i=0; i < objects.size(); ++i)
    objectNames.set1(i,O(objects[i])->getName().getString());  
  
  SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
  list->addCommandtoUndoReduList(command, &objectNames, activeLayerNew, activeLayerOld);
}

void SoCnDKit::addCommandtoUndoReduList(SbName command, SbName objectName, float sliderValueNew, float sliderValueOld)
{
  SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
  list->addCommandtoUndoReduList(command, objectName, sliderValueNew, sliderValueOld);
}

void SoCnDKit::addCommandtoUndoReduList(SbName command, const char* filePath, int appID)
{
  if (!redoFlag)
  {
    SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
    list->addCommandtoUndoReduList(command, filePath, appID);
  }
}

/**
* for transformation commands (translate, rotate, ...)
*/
void SoCnDKit::addCommandtoUndoReduList(SbName command, std::vector<int> objects, int userID)
{
	///////////////////////
	// convert object list:
	// make a SoMFName from std::vector<int>

	SoMFName objectNames;
	objectNames.setNum(objects.size());
	for (unsigned int i=0; i < objects.size(); ++i) {
		objectNames.set1(i,O(objects[i])->getName().getString());
	} // for every element of objects

	//////////////////////
	// add to command list
	SoUndoRedoListKit *list = (SoUndoRedoListKit*)SoNode::getByName("undo_redo_List");
	list->addCommandtoUndoReduList(command, &objectNames, userID);
}

// If selected is TRUE, the selected objects are counted, otherwise all.
// The elements of the stl vectors 'pointList'... are the numbers of the
// selected objects in the order how they were selected - starting with
// index 0.
// We do not need to count text
// Boolean objects, Curves and Surfaces are only stored in the objectsList
void 
SoCnDKit::countObjects(SbBool selected)
{
  int objects  = 0, objNum	= 0;
  int i;
  
  pointList.clear();
  lineList.clear();
  planeList.clear();
  cubeList.clear();
  sphereList.clear();
  cylinderList.clear();
  coneList.clear();
  objectList.clear(); // clear vector
  
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  if (objSum.getValue() > 0)
  {
    if (selected)
      objects = selectionRoot->getNumSelected();
    else
      objects = objSum.getValue();
    for (i = 0; i < objects; i++)
    {
      if (selected)
      {
        SoPath *selectedPath = selectionRoot->getPath(i);
        Object3dKit *parent = (Object3dKit*)selectedPath->getNode(1);
        objNum = parent->childNumber.getValue();
      } else
        objNum = i;
      if ((O(objNum)->visible.getValue()) == SO_SWITCH_ALL)
      {
        if (O(objNum)->name.getValue() == "Point")
        {
          pointList.push_back(objNum);
          //			pointList[pointSum] = objNum;
          objectList.push_back(objNum);
        }
        if (O(objNum)->name.getValue() == "Line")
        {
          lineList.push_back(objNum);
          objectList.push_back(objNum);
        }
        if (O(objNum)->name.getValue() == "Plane")
        {
          planeList.push_back(objNum);
          objectList.push_back(objNum); 
        }
        if (O(objNum)->name.getValue() == "Cube")
        {
          cubeList.push_back(objNum);
          // count for possible boolean operation (cube, sphere, cone, cylinder)
          objectList.push_back(objNum);  
        }
        if (O(objNum)->name.getValue() == "Sphere")
        {
          sphereList.push_back(objNum);
          
          objectList.push_back(objNum);
        }
        if (O(objNum)->name.getValue() == "Cylinder")
        {
          cylinderList.push_back(objNum);
          
          objectList.push_back(objNum);
        }
        if (O(objNum)->name.getValue() == "Cone")
        {
          coneList.push_back(objNum);
          
          objectList.push_back(objNum);
        }
        if (O(objNum)->name.getValue() == "Bool")
        {
          objectList.push_back(objNum);
        }
        if (O(objNum)->name.getValue() == "Curve")
        {
          objectList.push_back(objNum);
        }
        if (O(objNum)->name.getValue() == "Surface")
        {
          //std::cout << "soweit so gut Surface 2" << std::endl;
          objectList.push_back(objNum);
        }
      }
    }
    /*
	if (selected)
      for (i = 0; i < pointList.size(); i++) 
        printf("selected: %i %i \n",i,pointList[i]);
	*/
  }
}

// used for setting fields with object count - needed for APRIL presentations
void 
SoCnDKit::setObjectCount()
{
  //printf("Counting objects for APRIL...............\n");
  countObjects(FALSE);
  
  pointCount.setValue(pointList.size());
  //printf("Anz d. punkte %d \n",c3d_pointCount.getValue());
  lineCount.setValue(lineList.size());
  planeCount.setValue(planeList.size());
  cubeCount.setValue(cubeList.size());
  sphereCount.setValue(sphereList.size());
  cylinderCount.setValue(cylinderList.size()); 
  coneCount.setValue(coneList.size());
  // all objects are stored in objectList
  // Boolean objects, Curves and Surfaces are only stored in the objectsList
  objectCount.setValue(objectList.size());
  
  // counting selected objects
  countObjects(TRUE);
  selPointCount.setValue(pointList.size());
  //printf("Anz d. selektierten Punkte %d \n",selPointCount.getValue());
  selLineCount.setValue(lineList.size());
  selPlaneCount.setValue(planeList.size());
  selCubeCount.setValue(cubeList.size());
  selSphereCount.setValue(sphereList.size());
  selCylinderCount.setValue(cylinderList.size()); 
  selConeCount.setValue(coneList.size());
  selObjectCount.setValue(objectList.size());
}

SbBool
SoCnDKit::addLine(int userID)
{
  
  countObjects(TRUE);             // counts selected objects

  SoLineKit *O = new SoLineKit(objSum.getValue(), objectList, userID, &objSum, FALSE);
  if (O->s_return.ok())
  {
	  SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
	  draw->style.setValue(style.getValue());
	  draw->linePattern = linePattern; // use this line pattern for preview

	  O->layer.setValue(getActiveLayer()); 

	  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
	  selectionRoot->addChild(O);

	  objSum.setValue(objSum.getValue()+1);

	  addCommandtoUndoReduList("add", O->getName(), "Line", objectList, userID);

	  if (!O->s_return.ok() && !preview)
	  {
		  //if now object is created undo command
		  undo();
		  return FALSE;
	  }
	  else
		  return TRUE;

//  SoLineKit *O = new SoLineKit(objSum.getValue(), objectList, userID);
//  if (O->s_return.ok())
//  {
//    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
//    selectionRoot->addChild(O);
//    O->SetLineKit(objectList, &objSum);
//    O->Build();
//    
//    SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
//    draw->style.setValue(style.getValue());
//    draw->linePattern = linePattern; // use this line pattern for preview
//    
//    O->layer.setValue(getActiveLayer()); 
//    objSum.setValue(objSum.getValue()+1);
//    
//    addCommandtoUndoReduList("add", O->getName(), "Line", objectList, userID);
//    
//    if (!O->s_return.ok() && !preview)
//    {
//      //if now object is created undo command
//      undo();
//      return FALSE;
//    }
//    else
//      return TRUE;
  }
  else 
  {
    delete O;
    return false;
  }
  /*
  countObjects(TRUE);             // counts selected objects
  
    Object3dKit *O = new SoLineKit(objSum.getValue(), objectList, userID, &objSum);
    if (O->s_return.ok())
    {
    // for the distribution it seems to be necessary in case of the linekit 
    // to add it to the scenegraph (ref) before setting the style
    selectionRoot->addChild(O);
    
      SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
      draw->style = style;
      draw->linePattern = linePattern; // use this line pattern for preview
      
        O->layer.setValue(getActiveLayer());
        //selectionRoot->addChild(O);
        objSum.setValue(objSum.getValue()+1); 
        addCommandtoUndoReduList("add", O->getName(), "Line", objectList, userID);
        return TRUE;
        }
        else
        delete O;
        
          return FALSE;
  */
}

SbBool
SoCnDKit::addAngleBisector(int userID)
{
  countObjects(TRUE);             // counts selected objects
  
  SoLineKit *O = new SoLineKit(objSum.getValue(), objectList, userID, &objSum, TRUE);
  if (O->s_return.ok())
  {
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    selectionRoot->addChild(O);
    
    SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
    draw->style.setValue(style.getValue());
    draw->linePattern = linePattern; // use this line pattern for preview
    
    O->layer.setValue(getActiveLayer()); 
    objSum.setValue(objSum.getValue()+1);
    
    addCommandtoUndoReduList("add", O->getName(), "AngleBisector", objectList, userID);
    
    if (!O->s_return.ok() && !preview)
    {
      //if now object is created undo command
      undo();
      return FALSE;
    }
    else
      return TRUE;
  }
  else 
  {
    delete O;
    return false;
  }
}

SbBool
SoCnDKit::addPlane(SbBool normalPlane, SbBool symmetryPlane, int userID)
{
  countObjects(TRUE);             // counts selected objects
  SbName objectType;
  if (normalPlane)
    objectType = "Normalplane";
  if (symmetryPlane)
    objectType = "Symmetryplane";
  if (!normalPlane && !symmetryPlane)
    objectType = "Plane";
  
  printf("objecttype %s \n",objectType.getString());
  Object3dKit *O = new SoPlaneKit(objSum.getValue(), objectList, normalPlane, symmetryPlane, userID, &objSum);
  if (O->s_return.ok())
  {
    SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
    draw->style.setValue(style.getValue());
    
    O->layer.setValue(getActiveLayer());
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    selectionRoot->addChild(O);
    objSum.setValue(objSum.getValue()+1); 
    addCommandtoUndoReduList("add", O->getName(), objectType, objectList, userID);
    
    std::cout << "plane: " << O->getName().getString() << " , " << O->childNumber.getValue() << std::endl;
    
    return true;
  }
  else
  {
    delete O;
    return false;
  }
}

SbBool
SoCnDKit::addPlanethreePoints(int userID)
{
  //std::cout << "*******+++ da bin ich" << std::endl;
  countObjects(TRUE);             // counts selected objects
  
  Object3dKit *O = new SoPlaneKit(objSum.getValue(), objectList, userID);
  if (O->s_return.ok())
  {
    SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
    draw->style.setValue(style.getValue());
    
    O->layer.setValue(getActiveLayer());
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    selectionRoot->addChild(O);
    objSum.setValue(objSum.getValue()+1); 
    addCommandtoUndoReduList("add", O->getName(), "Plane3Points", objectList, userID);
    
    return true;
  }
  else
  {
    delete O;
    return false;
  }
}


SbBool
SoCnDKit::addCube(int userID)
{
    ///////////////
    // declarations

    // root of all Object3dKit objects, childnumber coincides with number in name of child
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");

    ////////////////
    // functionality

    // count selected objects
    // this results in objectList being a vector of the indices of the selected objects
  countObjects(TRUE);             // counts selected objects
  
  Object3dKit *O = new SoCubeKit(objSum.getValue(), objectList, userID);
  if (O->s_return.ok())
  {
    SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
    draw->style.setValue(style.getValue());
    
    O->layer.setValue(getActiveLayer());
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    selectionRoot->addChild(O);
    objSum.setValue(objSum.getValue()+1); 
    addCommandtoUndoReduList("add", O->getName(), "Cube", objectList, userID);
    return TRUE;
  }
  else
  {
    delete O;
    return FALSE;
  }
}


SbBool
SoCnDKit::addSphere(int userID)
{ 
  countObjects(TRUE);             // counts selected objects
  
  Object3dKit *O = new SoSphereKit(objSum.getValue(), objectList, userID);
  if (O->s_return.ok())
  {
    SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
    draw->style.setValue(style.getValue());
    
    O->layer.setValue(getActiveLayer());
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    selectionRoot->addChild(O);
    objSum.setValue(objSum.getValue()+1); 
    addCommandtoUndoReduList("add", O->getName(), "Sphere", objectList, userID);
    return true;
  }
  else
  {
    delete O;
    return false;
  }
}

SbBool
SoCnDKit::addCylinder(int userID)
{
  countObjects(TRUE);             // counts selected objects
  
  Object3dKit *O = new SoCylinderKit(objSum.getValue(), objectList, userID);
  if (O->s_return.ok())
  {
    SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
    draw->style.setValue(style.getValue());
    
    O->layer.setValue(getActiveLayer());
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    selectionRoot->addChild(O);
    objSum.setValue(objSum.getValue()+1); 
    addCommandtoUndoReduList("add", O->getName(), "Cylinder", objectList, userID);
    return true;
  }
  else
  {
    delete O;
    return false;
  }
}

SbBool
SoCnDKit::addCone(int userID)
{
  countObjects(TRUE);             // counts selected objects
  
  Object3dKit *O = new SoConeKit(objSum.getValue(), objectList, userID);
  if (O->s_return.ok())
  {
    SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
    draw->style.setValue(style.getValue());
    
    O->layer.setValue(getActiveLayer());
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    selectionRoot->addChild(O);
    objSum.setValue(objSum.getValue()+1); 
    addCommandtoUndoReduList("add", O->getName(), "Cone", objectList, userID);
    return TRUE;
  }
  else
  {
    delete O;
    return FALSE;
  }
}

SbBool
SoCnDKit::addBool(enumBoolOp type, int userID)
{
  printf("BooleanType %d num of Objects %d \n", type, objectList.size());
  SbName objectType;
  if (type == BOOL_INTERSECTION)
    objectType = "BoolIntersection";
  if (type == BOOL_UNION)
    objectType = "BoolUnion";
  if (type == BOOL_CUT)
    objectType = "BoolCut";
  
  countObjects(TRUE);             // counts selected objects
  
  Object3dKit *O = new SoBoolKit(objSum.getValue(), objectList, type, userID);
  if (O->s_return.ok())
  {
    SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
    draw->style.setValue(style.getValue());
    
    O->layer.setValue(getActiveLayer());
    //O->layer.setValue(boolObjectLayer);
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    selectionRoot->addChild(O);
    objSum.setValue(objSum.getValue()+1); 
    addCommandtoUndoReduList("add", O->getName(), objectType, objectList, userID);
    
    if (!preview && !loadFlag && !redoFlag)
    {
      //      SoCnDKit::O(objectList[0])->layer.setValue(boolBaseObjectLayer);
      //      SoCnDKit::O(objectList[1])->layer.setValue(boolBaseObjectLayer);
      //layer[boolObjectLayer]->on.setValue(TRUE);
      //layer[boolBaseObjectLayer]->on.setValue(TRUE);
      
      // delete transparent objects which produce ugly artefacts
      // only for Boolean Difference. 
      // For Union and Intersection both original objects are deleted
      // no moving into layers anymore
      if (type == BOOL_CUT)
      {
        deleteOneObject((SoCnDKit::O(objectList[0]))->getName());
      }
      else
      {
        deleteOneObject((SoCnDKit::O(objectList[0]))->getName());
        deleteOneObject((SoCnDKit::O(objectList[1]))->getName());
      }
      /*      
      if (type == BOOL_UNION)
      layer[boolBaseObjectLayer]->on.setValue(FALSE);
      if (type == BOOL_INTERSECTION)
      layer[boolBaseObjectLayer]->on.setValue(FALSE);
      */      
    }
    //    showLayers();
    return TRUE;
    
  }
  else
  {
    delete O;
    return FALSE;
  }
}

SbBool
SoCnDKit::addIntersect(int userID)
{
  countObjects(TRUE);             // counts selected objects
/*
  //Object3dKit *O = new SoIntersectKit(objectList, selectionRoot, &objSum);
  SoIntersectKit *O = new SoIntersectKit(objSum.getValue());
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  selectionRoot->addChild(O);
  //O->layer.setValue(getActiveLayer());
  O->SetIntersectKit(objectList, selectionRoot, &objSum, userID);

  //O->Build(); 
  objSum.setValue(objSum.getValue()+1);

  addCommandtoUndoReduList("add", O->getName(), "Intersect", objectList, userID);
*/

//-----

  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  SoIntersectKit* O = new SoIntersectKit(objectList, selectionRoot, &objSum, userID);
//  selectionRoot->addChild(O);
  if (O->s_return.ok())
  {
	  objSum.setValue(objSum.getValue()+1);
      addCommandtoUndoReduList("add", O->getName(), "Intersect", objectList, userID);
	  return TRUE;
  }
  else
  {
	  delete O;
	  return FALSE;
  }
//-----
  /*
  if ((!O->s_return.ok()) && !preview)
  {
    //if now object is created undo command
    undo();
    return FALSE;
  }
  else
    return TRUE;
  */
}

SbBool
SoCnDKit::addSlice(int userID)
{
  countObjects(TRUE);             // counts selected objects
  
  //Object3dKit *O = new SoIntersectKit(objectList, selectionRoot, &objSum);
  SoIntersectKit *O = new SoIntersectKit(objSum.getValue());
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  selectionRoot->addChild(O);
  //O->layer.setValue(getActiveLayer());
  
  O->SetIntersectKitSlice(objectList, selectionRoot, &objSum, userID);
  
  //O->Build(); 
  objSum.setValue(objSum.getValue()+1);
  
  addCommandtoUndoReduList("add", O->getName(), "Slice", objectList, userID);
  
  if (!O->s_return.ok() && !preview)
  {
    //if now object is created undo command
    undo();
    return FALSE;
  }
  else
  { 
    if (!preview && !loadFlag && !redoFlag)
    {
      if((SoCnDKit::O(objectList[0]))->isOfType(SoPlaneKit::getClassTypeId()))
      {
        deleteOneObject((SoCnDKit::O(objectList[1]))->getName());
      }
      else
      {
        deleteOneObject((SoCnDKit::O(objectList[0]))->getName());
      }
      //printf("Slice in layer %d \n",O->layer.getValue());
    }
    return TRUE;
  }
}

SbBool
SoCnDKit::addSweep(int userID)
{
  countObjects(TRUE);             // counts selected objects
  
  if (objectList.size() >= 2)
  {
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    if (((Object3dKit*)selectionRoot->getChild(objectList[0]))->isOfType(SoPointKit::getClassTypeId())
       || ((Object3dKit*)selectionRoot->getChild(objectList[1]))->isOfType(SoPointKit::getClassTypeId()))
    {
      return addEllipse(CIRCLE, userID);
    }
  }
 
  SoSurfaceKit *O = new SoSurfaceKit(&objSum, objectList, userID);

  if (O->a_return.ok())
  {
    SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
    draw->style.setValue(style.getValue());
    O->layer.setValue(getActiveLayer());
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    selectionRoot->addChild(O);

    objSum.setValue(objSum.getValue()+1);

    addCommandtoUndoReduList("add", O->getName(), "Sweep", objectList, userID);

    return TRUE;
  }
  else
  {
    delete O;
    return FALSE;
  }
}

SbBool
SoCnDKit::addCurve(enumCurveOp type, int userID)
{
  countObjects(TRUE); // counts selected objects
  
  SbName objectType;
  if (type == CURVE_BEZIER)
    objectType = "CurveBezier";
  if (type == CURVE_POINTS_ON)
    objectType = "CurvePoint";
  
  // get degree slider
  SoUserManagerKit* uk = SoUserManagerKit::getInstance();
  int userNumber = uk->getUserNumber();
  //int userIndex = SoUserManagerKit::getInstance()->getUserKitIndex(this->userID.getValue());
  int userIndex = this->userID.getValue();
  
  SoNode* pipSheet = uk->getUserKit(userIndex)->getPip()->getPipSheetKit(appID.getValue());
  SoIncrementalSlider* DegreeSlider = (SoIncrementalSlider*)findNode(pipSheet,makeUniqueNodeName("DEGREE_U", userIndex), userIndex);
  
  unsigned int pointcount = 0;
  unsigned int curvecount = 0;
  unsigned int objectcount = 0;
  while ( (objectcount < objectList.size()) &&
    ((O(objectList[objectcount]))->isOfType(SoCurveKit::getClassTypeId()) || 
    (O(objectList[objectcount]))->isOfType(SoPointKit::getClassTypeId())) )
  {
    if( (O(objectList[objectcount]))->isOfType(SoCurveKit::getClassTypeId()) )
      curvecount++;
    if( (O(objectList[objectcount]))->isOfType(SoPointKit::getClassTypeId()) )
      pointcount++;
    objectcount++;
  }
  
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  // rebuild curve with new degree
  if ((objectcount == 1) && (curvecount == 1) && (pointcount == 0))
  {
    SoCurveKit* Curve = (SoCurveKit*)(O(objectList[0]));
    int degreeold = Curve->degree.getValue();
    //if (Curve->rebuild((int)DegreeSlider->valueOutput.getValue()))
          //addCommandtoUndoReduList("rebuild", Curve->getName(), objectType, degreeold, (int)DegreeSlider->valueOutput.getValue());
    if (Curve->rebuild((int)DegreeSlider->value.getValue()))
        addCommandtoUndoReduList("rebuild", Curve->getName(), objectType, degreeold, (int)DegreeSlider->value.getValue());
    return TRUE;
  }
  /*  
  // delete one point from curve
  if (((objectcount == 2) && (curvecount == 1) && (pointcount == 1)))
  {
  SoCurveKit* Curve = (SoCurveKit*)(O(objectList[0]));
  if (Curve->deletePoint( (O(pointList[0])),type))
  addCommandtoUndoReduList("deletepoint", Curve->getName(), objectType, degreeold, degree.getValue());
  }
  */
  //build new curve
  if ((objectcount >= 3) && (curvecount == 0) && (pointcount >= 3))
  {
    Object3dKit *O = new SoCurveKit(objSum.getValue(), objectList, type, (int)DegreeSlider->value.getValue(), userID);
    if (O->s_return.ok())
    {
      SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
      draw->style.setValue(style.getValue());
      draw->linePattern = linePattern; // use this line pattern for preview
      
      O->layer.setValue(getActiveLayer());
      selectionRoot->addChild(O);
      objSum.setValue(objSum.getValue()+1); 
      addCommandtoUndoReduList("add", O->getName(), objectType, objectList, userID);
      return TRUE;
    }
    else
    {
      delete O;
      return FALSE;
    }    
  }
  return FALSE;
}

SbBool 
SoCnDKit::addEllipse(enumCurveOp type, int userID)
{
  countObjects(TRUE);             // counts selected objects
  
  SbName objectType;
  if (type == ELLIPSE_CURVE)
    objectType = "Ellipse";
  if (type == CIRCLE)
    objectType = "Circle";
  
  Object3dKit *O = new SoCurveKit(objSum.getValue(), objectList, type, 0, userID);
  if (O->s_return.ok())
  {
    SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
    draw->style.setValue(style.getValue());
    draw->linePattern = linePattern; // use this line pattern for preview
    
    O->layer.setValue(getActiveLayer());
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    selectionRoot->addChild(O);
    objSum.setValue(objSum.getValue()+1); 
    addCommandtoUndoReduList("add", O->getName(), objectType, objectList, userID);
    return TRUE;
  }
  else
  {
    delete O;
    return FALSE;
  }
}


SbBool 
SoCnDKit::addSurface(enumSurfaceOp type, int userID)
{
  countObjects(TRUE); // counts selected objects
  
  // get all sliders
  SoUserManagerKit* uk = SoUserManagerKit::getInstance();
  int userNumber = uk->getUserNumber();
  //int userIndex = SoUserManagerKit::getInstance()->getUserKitIndex(this->userID.getValue());
  int userIndex = this->userID.getValue();
  
  SoNode* pipSheet = uk->getUserKit(userIndex)->getPip()->getPipSheetKit(appID.getValue());
  SoIncrementalSlider* DegreeXSlider = (SoIncrementalSlider*)findNode(pipSheet,makeUniqueNodeName("DEGREE_U", userIndex), userIndex);
  SoIncrementalSlider* DegreeYSlider = (SoIncrementalSlider*)findNode(pipSheet,makeUniqueNodeName("DEGREE_V", userIndex), userIndex);
  SoIncrementalSlider* SurfaceXSlider = (SoIncrementalSlider*)findNode(pipSheet,makeUniqueNodeName("POINT_U", userIndex), userIndex);
  SoIncrementalSlider* SurfaceYSlider = (SoIncrementalSlider*)findNode(pipSheet,makeUniqueNodeName("POINT_V", userIndex), userIndex);
//  SoSFInt32 DegreeX; DegreeX.setValue(int(DegreeXSlider->valueOutput.getValue()));
//  SoSFInt32 DegreeY; DegreeY.setValue(int(DegreeYSlider->valueOutput.getValue()));
//  SoSFInt32 SurfaceX; SurfaceX.setValue(int(SurfaceXSlider->valueOutput.getValue()));
//  SoSFInt32 SurfaceY; SurfaceY.setValue(int(SurfaceYSlider->valueOutput.getValue()));
  SoSFInt32 DegreeX; DegreeX.setValue(int(DegreeXSlider->value.getValue()));
  SoSFInt32 DegreeY; DegreeY.setValue(int(DegreeYSlider->value.getValue()));
  SoSFInt32 SurfaceX; SurfaceX.setValue(int(SurfaceXSlider->value.getValue()));
  SoSFInt32 SurfaceY; SurfaceY.setValue(int(SurfaceYSlider->value.getValue()));

  printf("DegreeX is %i, DegreeY is %i, SurfaceX is %i, SurfaceY is %i\n",DegreeX.getValue(),DegreeY.getValue(),SurfaceX.getValue(),SurfaceY.getValue());
  SbName objectType;
  if (type == SURFACE_B_SPLINE)
  {
    objectType = "SurfaceBspline";
    // if this simple check is not done here, the destructor of the SoSurfaceKit will crash later when trying to remove 
    // sensors to nonexisting points
    if (SurfaceX.getValue() <= DegreeX.getValue())
    {
      printf("For degree %d in u-direction you need at least %d+1 control points", DegreeX.getValue(),DegreeX.getValue());
      return FALSE;
    }
    if (SurfaceY.getValue() <= DegreeY.getValue())
    {
      printf("For degree %d in v-direction you need at least %d+1 control points", DegreeY.getValue(),DegreeY.getValue());
      return FALSE;
    }
  }
  if (type == SURFACE_POINTS_ON)
    objectType = "SurfacePoint";
  
  if (objectList.size() >= 4)
  {
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    Object3dKit *O = new SoSurfaceKit(&objSum, objectList, type, SurfaceX, SurfaceY, DegreeX, DegreeY, selectionRoot, userID);
    
    printf("Surface s_return error number %d ----\n",O->s_return.error_number());
    if (O->s_return.ok())
    {
      SoDrawStyle* draw = (SoDrawStyle*) O->getPart("style",false);
      draw->style.setValue(style.getValue());
      printf("Creating surface -----------------------------------------------------------\n");
      O->layer.setValue(getActiveLayer());
      SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
      selectionRoot->addChild(O);
      objSum.setValue(objSum.getValue()+1); 
      addCommandtoUndoReduList("add", O->getName(), objectType, objectList, userID);
      return TRUE;
    }
    else
    {
      delete O;
      return FALSE;
    }
  }
  return FALSE;
}


SbBool SoCnDKit::addPolygon(SbBool constrained, int userID)
{
  countObjects(TRUE);
  //std::vector<Object3dKit*> Points(100);
  
  Object3dKit *O = new SoPointKit(objSum.getValue(), objectList, userID);
  if (O->s_return.ok())
  {
    O->layer.setValue(getActiveLayer());
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    selectionRoot->addChild(O);
    objSum.setValue(objSum.getValue()+1); 
    addCommandtoUndoReduList("add", O->getName(), "Point", objectList, userID);
    
    if (!redoFlag && !loadFlag)
    {
      O->select(selectionRoot);
      addCommandtoUndoReduList("select", O->getName());
    }
    return TRUE;
  }
  else
  {
    delete O;
    return FALSE;
  }
  /*
  if (pointList.size() >= 3)
  {
  unsigned int i;
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  for (i = 0; i < pointList.size(); i++) 
  {
  Points[i] = (O(pointList[i]));
  }
  //    Object3dKit *C = new SoPolygonKit(objSum.getValue(), Points, type, pointList.size(), 0);
  Object3dKit *C = new SoPolygonKit(objSum.getValue(), Points, pointList.size(), constrained);
  
    C->layer.setValue(getActiveLayer());
    for (i = 0; i <  pointList.size(); i++) 
    {
    O(pointList[i])->deselect(selectionRoot);
    }
    
      if(C->s_return.ok())
      {
      std::cout << "add polygon: " << pointList.size() << std::endl;
      SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
      selectionRoot->addChild(C);
      objSum.setValue(objSum.getValue()+1);
      for (i = 0; i < pointList.size(); i++) 
      {
      O(pointList[i])->addDependentObjectsName(C->getName());
      }
      }
      else
      delete C;
      } 
  */
}

/**
 * translation == FALSE sets a fixed Point, TRUE generates a regular point
**/
SbBool SoCnDKit::addPoint(SbVec3f eventPos, SbBool centreofLine, SbBool translation, int userID)
{
  if (!preview)
  {
    countObjects(TRUE); // counts selected objects
  
    if (centreofLine)
    {
      Object3dKit *O = new SoPointKit(objSum.getValue(), objectList, userID);
      if (O->s_return.ok())
      {
        O->layer.setValue(getActiveLayer());
        SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
        selectionRoot->addChild(O);
        objSum.setValue(objSum.getValue()+1); 
        addCommandtoUndoReduList("add", O->getName(), "centrePoint", objectList, userID);
      
        /*
	    if (!redoFlag && !loadFlag)
        {
          O->select(selectionRoot);
          addCommandtoUndoReduList("select", O->getName());
        }
	    */
        // used for setting fields with object count - needed for APRIL presentations
        setObjectCount();
      
        return TRUE;
      }
      else
      {
        delete O;
        return FALSE;
      }
    }
    else
    {
      Object3dKit *O = new SoPointKit(objSum.getValue(), objectList, eventPos, userID);
      if (O->s_return.ok())
      {
        O->layer.setValue(getActiveLayer());
        SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
        selectionRoot->addChild(O);
        objSum.setValue(objSum.getValue()+1); 
        if (translation)
        {
          addCommandtoUndoReduList("add", O->getName(), "Point", eventPos, objectList, userID);
        }
        else 
        {
          addCommandtoUndoReduList("add", O->getName(), "fixPoint", eventPos, objectList, userID);
          O->translationOn.setValue(FALSE);
		  O->rotationOn.setValue(FALSE);
        }
      
        if (!redoFlag && !loadFlag)
        {
          O->select(selectionRoot);
          addCommandtoUndoReduList("select", O->getName());
        }
        // used for setting fields with object count - needed for APRIL presentations
        setObjectCount();
      
        return TRUE;
      }
      else
      {
        delete O;
        return FALSE;
      }
    }
  }
  else
    return FALSE;
}

/**
* find add... method according to original object's type
*
* right now we only support the following object types:
*  P - Point
*  L - Line
*  Z - Cylinder
*  K - Cone
*  Q - Cube
*  E - Plane
*  S - Sphere
*  B - Bool
*/
int SoCnDKit::findAddObjectMethod(Object3dKit* original, int userID)
{
    switch(original->getName().getString()[0]) {
    case 'L':
        addLine(userID);
        break;
    case 'Z':
        addCylinder(userID);
        break;
    case 'K':
        addCone(userID);
        break;
    case 'Q':
        addCube(userID);
        break;
    case 'E':
        addPlane(TRUE, TRUE, userID);
        break;
    case 'S':
        addSphere(userID);
        break;
    case 'B':
        addBool((enumBoolOp)((SoBoolKit*)(original))->m_BoolOp.getValue(), userID);
        break;
    } // switch first letter of object name
    return 0; // no error
}

/**
* translate(int userID)
*   - translation of selected objects except the last two objects
*   - last two objects have to be points, they represent the translation vector
*   - if doAddCommand == TRUE:  default case; command is added to the undoRedoList
*   - if doAddCommand == FALSE: no command will be added to the undoRedoList 
*                       (translate() method is called by the undo or redo method)
*/
SbBool
SoCnDKit::translate(int userID, SbBool doAddCommand = TRUE)
{
    ///////////////
    // declarations

    int c1; // counter
    int nrSelectedObjects; // number of selected objects
    std::vector<int>::iterator objectListIterator;      // iterator thru selected objects
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    SbBool areAllFree = TRUE;
    SoSFVec3f translationVector;
    SoPointKit* translationVectorStart;
    SoPointKit* translationVectorEnd;

    ////////////////
    // functionality

    // count selected objects
    // this results in objectList being a vector of the indices of the selected objects
    countObjects(TRUE);
    nrSelectedObjects = objectList.size();

    ////////////////////////////////////////////////////
    // testing code
    // BEGIN

    std::cout << "translate() called with following objectList:" << endl;

    // for every element of objectList:
    //   - print its name
    //   - print its dependentObject names
    //   - print object names where it is in dependentObject list (counter-dependence)
    for(
        objectListIterator = objectList.begin();
        objectListIterator != objectList.end(); 
        objectListIterator++
    ) {
        ////////////////
        // print my name

        Object3dKit* looknode1 = O(*objectListIterator);
        std::cout << looknode1->getName() << endl;

        /////////////////////////////////
        // print my dependentObject names

        for (c1=1; c1 < looknode1->dependentObjectsName.getNum(); c1++) {
            // if name not empty
            //   (i just took this from Object3dKit::selectAllDependentObjects(SoSelection*)
            //    but i have no idea in which situation it might be empty)
            if (looknode1->dependentObjectsName[c1] != "") {
                Object3dKit *dependentObject = (Object3dKit*)SoNode::getByName(looknode1->dependentObjectsName[c1]);
                std::cout << "  - with dependent object: " << dependentObject->getName() << endl;
            } // if name not empty
        } // for every dependent object

        /////////////////////////////////////////////////////////////////////////////
        // print object names where i am in dependentObject list (counter-dependence)
        // i.e. print my counterDependentObject names

        for (c1=1; c1 < looknode1->counterDependentObjectsName.getNum(); c1++) {
            // if name not empty
            if (looknode1->counterDependentObjectsName[c1] != "") {
                Object3dKit *counterDependentObject = (Object3dKit*)SoNode::getByName(looknode1->counterDependentObjectsName[c1]);
                std::cout << "  - with counter-dependent object [new method]: " << counterDependentObject->getName() << endl;
            } // if name not empty
        } // for every counter-dependent object

    } // for every element of objectList

    // testing code
    // END
    ////////////////////////////////////////////////////

    // - check if at least 3 objects are selected
    // - check if last two objects are Points
    // - calculate the oriented distance (translation vector) between the last two objects (points)
    // - go thru every but the last two objects (objects to translate):
    //   - if it's a non-locked Point: add to it transformList
    //   - else: call getAllPointsFromAncestors / getAllAncestors (dependent on makeCopy)
    //     - check if 'areAllFree' is always true
    //     - combine the returned 'objectNumbersList's

    std::cout << "checking if at least 3 objects are selected..." << endl;
    if (nrSelectedObjects>=3) {
        std::cout << "checking if translation vector is valid..." << endl;
        if(
            (O(objectList[nrSelectedObjects-2])->getName().getString()[0]=='P')&&
            (O(objectList[nrSelectedObjects-1])->getName().getString()[0]=='P')
            ) {
                translationVectorStart = (SoPointKit*)(O(objectList[nrSelectedObjects-2])); // 2nd last point
                translationVectorEnd   = (SoPointKit*)(O(objectList[nrSelectedObjects-1])); // last point
                // calculate vector
                translationVector.setValue(
                    translationVectorEnd->pCoord.getValue() - translationVectorStart->pCoord.getValue()
                    ); // setValue
                // generate translation-list
                std::cout << "generating translation-list..." << endl;
                for(c1=0; c1<nrSelectedObjects-2; c1++) {
                    if(O(objectList[c1])->getName().getString()[0]=='P'){
                        if(O(objectList[c1])->isLocked.getValue()==FALSE) {
                            std::cout << "objectList-element " << O(objectList[c1])->getName() << " (free point)" << endl;
                            putIntoTransformList(O(objectList[c1]));
                        } // if object nr. c1 is a not locked
                        else {
                            std::cout << "objectList-element " << O(objectList[c1])->getName() << " (locked point)" << endl;
                            areAllFree = FALSE;
                            goto GeneratingTransformListFinished;
                        }
                    } // if object nr. c1 is a Point
                    else {
                        std::vector<int> foundObjectNumbersList;
                        if(makeCopy) {
                            O(objectList[c1])->getAllAncestors(&foundObjectNumbersList, &areAllFree);
                        } // if makeCopy
                        else {
                            O(objectList[c1])->getAllPointsFromAncestors(&foundObjectNumbersList, &areAllFree);
                        } // if else makeCopy
                        if(areAllFree==FALSE) {
                            std::cout << "objectList-element " << O(objectList[c1])->getName() << " with locked ancestor points" << endl;
                            goto GeneratingTransformListFinished;
                        } // if locked Points exist
                        else {
                            if(makeCopy) {
                                putIntoTransformList(O(objectList[c1])); // put non-point object into copy-list as well
                            } // if makeCopy
                            std::cout << "objectList-element " << O(objectList[c1])->getName() << " with these free ancestor points:" << endl;
                            std::vector<int>::iterator foundObjectNumbersListIterator;
                            for(
                                foundObjectNumbersListIterator = foundObjectNumbersList.begin();
                                foundObjectNumbersListIterator != foundObjectNumbersList.end(); 
                            foundObjectNumbersListIterator++
                                ) {
                                    std::cout << O(*foundObjectNumbersListIterator)->getName() << endl;
                                    putIntoTransformList(O(*foundObjectNumbersListIterator));
                                } // for every element of foundObjectNumbersList
                        } // if else locked Points exist
                    } // if else object nr. c1 is a Point
                } // for every element of objectList but the last two
            } // if last two selected objects are Points
    } // if at least 3 objects selected

GeneratingTransformListFinished: // we jump here on early termination
    if(areAllFree==FALSE) {
        transformList.clear();
    } // if locked Points exist

    //////////////////////
    // print transformList
    std::vector<int>::iterator transformListIterator; // iterator thru transformList
    std::cout << "translation-list:" << endl;
    for(
        transformListIterator =  transformList.begin();
        transformListIterator != transformList.end(); 
        transformListIterator++
    ) {
        std::cout << (*transformListIterator) << endl;
    } // for every element of transformList

    // - if "makeCopy"
    //   - generate copies of objects in transformList, points translated
    // - if else "makeCopy"
    //   - change the positions of all points from transformList

    if(!transformList.empty()) {
        // transformList must be sorted in case makeCopy==TRUE, 
        // this ensures that prerequisite objects exist for creation of each new object
        if (makeCopy) {
            sort(transformList.begin(), transformList.end());
            // ATTENTION: Insert only one time before the copy sequence a 
            //            deselectAll command, so that all previous objects
            //            get deselected. It is done in the code below but must
            //            also be done in the undoredolist
            addCommandtoUndoReduList("deselectAll", "object", "object", objectList, this->userID.getValue());
        } // if makeCopy

        //////////////////////
        // execute translation
        for(
            transformListIterator =  transformList.begin();
            transformListIterator != transformList.end(); 
        transformListIterator++
            ) {
                if (makeCopy) {
                    // Points are copied at translated location
                    // for all other objects:
                    //   - select the copies of the objects that were selected at creation of the original
                    //   - call the appropriate SoCnDKit::add... function
                    // all original objects handshake their copies using their copyObject pointer

                    //////////////////////////////////////
                    // if transformListIterator is a Point
                    if(O(*transformListIterator)->getName().getString()[0]=='P'){
                        deselectAllObjects();
                        // make the copy at translated location
                        addPoint(
                            ((SoPointKit*)(O(*transformListIterator)))->pCoord.getValue() + translationVector.getValue(), 
                            FALSE, 
                            TRUE, 
                            userID
                            ); // addPoint
                        // let the original know it's copy
                        O(*transformListIterator)->copyObject = O(objSum.getValue()-1); // should be the last object in the scene
                    } // if transformListIterator is a Point

                    //////////////////////////////////////////
                    // if transformListIterator is not a Point
                    else {

                        deselectAllObjects();
                        //////////////////////////////////////////////////////////////////////////////////
                        // select the copies of the objects that were selected at creation of the original

                        // i assume getObjectList() returns the selection-list at the time of the objects creation
                        SoMFName* creationSelectionList = O(*transformListIterator)->getObjectList();
                        for(c1=0; c1<creationSelectionList->getNum(); c1++) {
                            // check if copyObject exists, that's not sure for each member of creationSelectionList
                            if (((Object3dKit*)SoNode::getByName((*creationSelectionList)[c1]))->copyObject!=NULL) {
                                // select copy
                                ((Object3dKit*)SoNode::getByName((*creationSelectionList)[c1]))->copyObject->select(selectionRoot);
                            } // if copyObject exists
                        } // for c1 over each value of SoMFName

                        // using the selection made before, we add a new object
                        findAddObjectMethod(O(*transformListIterator), userID);

                        // let the original know it's copy
                        O(*transformListIterator)->copyObject = O(objSum.getValue()-1);

                    } // if transformListIterator is not a Point
                } // if makeCopy
                else {
                    ((SoPointKit*)(O(*transformListIterator)))->pCoord.setValue(
                        ((SoPointKit*)(O(*transformListIterator)))->pCoord.getValue()+
                        translationVector.getValue()
                        ); // setValue
                } // if else makeCopy
            } // for every element of transformList

            // after copy make a useful selection for repeated copy
            if (makeCopy) {

                deselectAllObjects();

                // - select all copies
                // - clean up the copyObject pointers afterwards
                for(
                    transformListIterator =  transformList.begin();
                    transformListIterator != transformList.end(); 
                transformListIterator++
                    ) {
                        O(*transformListIterator)->copyObject->select(selectionRoot);
                        O(*transformListIterator)->copyObject = NULL;
                    } // for every element of transformList

                    // select translation vector
                    translationVectorStart->select(selectionRoot);
                    translationVectorEnd->select(selectionRoot);
            } // if makeCopy

            if (doAddCommand) {
                addCommandtoUndoReduList("translate", objectList, userID);
            } // if doAddCommand
    } // if transformList not empty

    transformList.clear();

    return TRUE;
}

/** 
* put index of given object into transform-list
* - pay attention to avoid multiple occurrences
*
* returns: int - error code
*/
int 
SoCnDKit::putIntoTransformList(Object3dKit* object)
{
    ///////////////
    // declarations
    std::vector<int>::iterator transformListIterator; // iterator thru transformList
    int objectNumber = object->childNumber.getValue();
    // have a boolean initially set to false, becoming true if 
    // and only if objectNumber is found in transformList
    bool objectNumberIsInTransformList = false;

    ////////////////
    // functionality

    // seach previous occurrence
    for(
        transformListIterator =  transformList.begin();
        transformListIterator != transformList.end(); 
        transformListIterator++
    ) {
        if(*transformListIterator==objectNumber) {
            objectNumberIsInTransformList = true; // set trigger true
        } // if objectNumber found in list
    } // for every element of transformList

    // if new then put it in
    if(!objectNumberIsInTransformList){
        transformList.push_back(objectNumber);
    } // if new

    return 0; // no error
}

/**
* rotate(int userID)
*   - rotate all selected objects except the last three objects
*   - last three objects have to be a line and two points, they represent the rotation angle
*     (ask robert for document 'notizen01.jpg')
*   - if doAddCommand == TRUE:  default case; command is added to the undoRedoList
*   - if doAddCommand == FALSE: no command will be added to the undoRedoList 
*                       (rotate() method is called by the undo or redo method)
*/
SbBool
SoCnDKit::rotate(int userID, SbBool doAddCommand = TRUE)
{
    ///////////////
    // declarations

    int c1; // counter
    int nrSelectedObjects; // number of selected objects
    std::vector<int>::iterator objectListIterator;      // iterator thru selected objects
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    SbBool areAllFree = TRUE;
    SoLineKit*  rotationAxis;
    SoPointKit* rotationStart;
    SoPointKit* rotationEnd;
    SbMatrix rotationAroundFreeAxis; // transformation matrix

    ////////////////
    // functionality

    // count selected objects
    // this results in objectList being a vector of the indices of the selected objects
    countObjects(TRUE);
    nrSelectedObjects = objectList.size();

    // - check if at least 4 objects are selected
    // - check if last two objects are Points
    // - check if 3rd last object is a Line
    // - calculate the perpendicular from the last two objects (points) to the 3rd last object (line)
    // - calculate the angle between these perpendiculars (rotation angle)
    // - calculate transformation matrix
    // - go thru every but the last three objects (objects to rotate):
    //   - if it's a non-locked Point: add to it transformList
    //   - else: call getAllPointsFromAncestors / getAllAncestors (dependent on makeCopy)
    //     - check if 'areAllFree' is always true
    //     - combine the returned 'objectNumbersList's

    std::cout << "checking if at least 4 objects are selected..." << endl;
    if (nrSelectedObjects>=4) {
        std::cout << "checking if rotation axis/angle (line + 2 points) is valid..." << endl;
        if(
            (O(objectList[nrSelectedObjects-3])->getName().getString()[0]=='L')&&
            (O(objectList[nrSelectedObjects-2])->getName().getString()[0]=='P')&&
            (O(objectList[nrSelectedObjects-1])->getName().getString()[0]=='P')
        ) {
            rotationAxis  = (SoLineKit*)(O(objectList[nrSelectedObjects-3]));
            rotationStart = (SoPointKit*)(O(objectList[nrSelectedObjects-2]));
            rotationEnd   = (SoPointKit*)(O(objectList[nrSelectedObjects-1]));
            SbVec3f perpendicular1 = 
                rotationStart->pCoord.getValue()-
                SbLine(
                rotationAxis->pCoord.getValue(),
                rotationAxis->qCoord.getValue()
                ).getClosestPoint(rotationStart->pCoord.getValue());
            SbVec3f perpendicular2 = 
                rotationEnd->pCoord.getValue()-
                SbLine(
                rotationAxis->pCoord.getValue(),
                rotationAxis->qCoord.getValue()
                ).getClosestPoint(rotationEnd->pCoord.getValue());
            SbRotation rotation1To2(perpendicular1, perpendicular2);
            SbVec3f translationAxisToOrigin = SbLine(
                rotationAxis->pCoord.getValue(),
                rotationAxis->qCoord.getValue()
                ).getClosestPoint(SbVec3f(0, 0, 0));

            // calculate transformation matrix
            // use SbMatrix::setTransform which sets M = -Tc * Ro-¹ * S * Ro * R * T * Tc
            // see http://doc.coin3d.org/Coin/classSbMatrix.html#a28
            rotationAroundFreeAxis.setTransform(
                SbVec3f(0, 0, 0),                // T:  translation
                rotation1To2,                    // R:  rotation, (2nd last point to last point)
                SbVec3f(1, 1, 1),                // S:  scaleFactor
                SbRotation(SbVec3f(0, 0, 1), 0), // Ro: scaleOrientation
                translationAxisToOrigin          // Tc: center,   (translationAxisToOrigin)
                );

            // generate rotation-list
            std::cout << "generating rotation-list..." << endl;
            for(c1=0; c1<nrSelectedObjects-3; c1++) {
                if(O(objectList[c1])->getName().getString()[0]=='P'){
                    if(O(objectList[c1])->isLocked.getValue()==FALSE) {
                        std::cout << "objectList-element " << O(objectList[c1])->getName() << " (free point)" << endl;
                        putIntoTransformList(O(objectList[c1]));
                    } // if object nr. c1 is a not locked
                    else {
                        std::cout << "objectList-element " << O(objectList[c1])->getName() << " (locked point)" << endl;
                        areAllFree = FALSE;
                        goto GeneratingTransformListFinished;
                    }
                } // if object nr. c1 is a Point
                else {
                    std::vector<int> foundObjectNumbersList;
                    if(makeCopy) {
                        O(objectList[c1])->getAllAncestors(&foundObjectNumbersList, &areAllFree);
                    } // if makeCopy
                    else {
                        O(objectList[c1])->getAllPointsFromAncestors(&foundObjectNumbersList, &areAllFree);
                    } // if else makeCopy
                    if(areAllFree==FALSE) {
                        std::cout << "objectList-element " << O(objectList[c1])->getName() << " with locked ancestor points" << endl;
                        goto GeneratingTransformListFinished;
                    } // if locked Points exist
                    else {
                        if(makeCopy) {
                            putIntoTransformList(O(objectList[c1])); // put non-point object into copy-list as well
                        } // if makeCopy
                        std::cout << "objectList-element " << O(objectList[c1])->getName() << " with these free ancestor points:" << endl;
                        std::vector<int>::iterator foundObjectNumbersListIterator;
                        for(
                            foundObjectNumbersListIterator = foundObjectNumbersList.begin();
                            foundObjectNumbersListIterator != foundObjectNumbersList.end(); 
                        foundObjectNumbersListIterator++
                            ) {
                                std::cout << O(*foundObjectNumbersListIterator)->getName() << endl;
                                putIntoTransformList(O(*foundObjectNumbersListIterator));
                            } // for every element of foundObjectNumbersList
                    } // if else locked Points exist
                } // if else object nr. c1 is a Point
            } // for every element of objectList but the last three
        } // if last two selected objects are Points and 3rd last is a Line
    } // if at least 4 objects selected

GeneratingTransformListFinished: // we jump here on early termination
    if(areAllFree==FALSE) {
        transformList.clear();
    } // if locked Points exist

    //////////////////////
    // print transformList
    std::vector<int>::iterator transformListIterator; // iterator thru rotation-list
    std::cout << "rotation-list:" << endl;
    for(
        transformListIterator =  transformList.begin();
        transformListIterator != transformList.end(); 
        transformListIterator++
    ) {
        std::cout << (*transformListIterator) << endl;
    } // for every element of transformList

    // - if "makeCopy"
    //   - generate copies of objects in transformList, points rotated
    // - if else "makeCopy"
    //   - change the positions of all points from transformList

    if(!transformList.empty()) {
        // transformList must be sorted in case makeCopy==TRUE, 
        // this ensures that prerequisite objects exist for creation of each new object
        if (makeCopy) {
            sort(transformList.begin(), transformList.end());
            // ATTENTION: Insert only one time before the copy sequence a 
            //            deselectAll command, so that all previous objects
            //            get deselected. It is done in the code below but must
            //            also be done in the undoredolist
            addCommandtoUndoReduList("deselectAll", "object", "object", objectList, this->userID.getValue());
        } // if makeCopy

        ///////////////////
        // execute rotation
        for(
            transformListIterator =  transformList.begin();
            transformListIterator != transformList.end(); 
            transformListIterator++
        ) {
            if (makeCopy) {

                // Points are copied at rotated location
                // for all other objects:
                //   - select the copies of the objects that were selected at creation of the original
                //   - call the appropriate SoCnDKit::add... function
                // all original objects handshake their copies using their copyObject pointer

                //////////////////////////////////////
                // if transformListIterator is a Point
                if(O(*transformListIterator)->getName().getString()[0]=='P'){

                    // calculate location
                    SbVec3f newPCoord;
                    // apply transformation matrix
                    rotationAroundFreeAxis.multVecMatrix(
                        ((SoPointKit*)(O(*transformListIterator)))->pCoord.getValue(),  // src
                        newPCoord                                                       // dst
                        );     

                    // make the copy at rotated location
                    deselectAllObjects();
                    addPoint(
                        newPCoord,
                        FALSE, 
                        TRUE, 
                        userID
                        ); // addPoint
                    // let the original know it's copy
                    O(*transformListIterator)->copyObject = O(objSum.getValue()-1); // should be the last object in the scene
                } // if transformListIterator is a Point

                //////////////////////////////////////////
                // if transformListIterator is not a Point
                else {

                    deselectAllObjects();
                    //////////////////////////////////////////////////////////////////////////////////
                    // select the copies of the objects that were selected at creation of the original

                    // i assume getObjectList() returns the selection-list at the time of the objects creation
                    SoMFName* creationSelectionList = O(*transformListIterator)->getObjectList();
                    for(c1=0; c1<creationSelectionList->getNum(); c1++) {
                        // check if copyObject exists, that's not sure for each member of creationSelectionList
                        if (((Object3dKit*)SoNode::getByName((*creationSelectionList)[c1]))->copyObject!=NULL) {
                            // select copy
                            ((Object3dKit*)SoNode::getByName((*creationSelectionList)[c1]))->copyObject->select(selectionRoot);
                        } // if copyObject exists
                    } // for c1 over each value of SoMFName

                    // using the selection made before, we add a new object
                    findAddObjectMethod(O(*transformListIterator), userID);

                    // let the original know it's copy
                    O(*transformListIterator)->copyObject = O(objSum.getValue()-1);
                } // if transformListIterator is not a Point
            } // if makeCopy
            else {
                SbVec3f newPCoord;
                // apply transformation matrix
                rotationAroundFreeAxis.multVecMatrix(
                    ((SoPointKit*)(O(*transformListIterator)))->pCoord.getValue(),  // src
                    newPCoord                                                       // dst
                    );     
                ((SoPointKit*)(O(*transformListIterator)))->pCoord.setValue(newPCoord); // setValue
            } // if else makeCopy
        } // for every element of transformList

        // after copy make a useful selection for repeated copy
        if (makeCopy) {

            deselectAllObjects();

            // - select all copies
            // - clean up the copyObject pointers afterwards
            for(
                transformListIterator =  transformList.begin();
                transformListIterator != transformList.end(); 
            transformListIterator++
                ) {
                    O(*transformListIterator)->copyObject->select(selectionRoot);
                    O(*transformListIterator)->copyObject = NULL;
                } // for every element of transformList

                // select rotation axis, start and end point
                rotationAxis->select(selectionRoot);
                rotationStart->select(selectionRoot);
                rotationEnd->select(selectionRoot);
        } // if makeCopy

        if (doAddCommand) {
            addCommandtoUndoReduList("rotate", objectList, userID);
        } // if doAddCommand
    } // if we can rotate something

    transformList.clear();

    return TRUE;
} // rotate()

/**
* mirror(int userID)
*   - mirror all selected objects except the last object
*   - last object has to be a plane, it represents the mirror plane
*   - if doAddCommand == TRUE:  default case; command is added to the undoRedoList
*   - if doAddCommand == FALSE: no command will be added to the undoRedoList 
*                       (mirror() method is called by the undo or redo method)
*/
SbBool
SoCnDKit::mirror(int userID, SbBool doAddCommand=TRUE)
{
    ///////////////
    // declarations

    int c1; // counter
    int nrSelectedObjects; // number of selected objects
    std::vector<int>::iterator objectListIterator;      // iterator thru selected objects
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    SbBool areAllFree = TRUE;
    SbPlane* mirrorPlane;
    SoPlaneKit* mirrorPlaneObject;

    ////////////////
    // functionality

    // count selected objects
    // this results in objectList being a vector of the indices of the selected objects
    countObjects(TRUE);
    nrSelectedObjects = objectList.size();

    // - check if at least 2 objects are selected
    // - check if last object is a Plane
    // - go thru every but the last object (objects to mirror):
    //   - if it's a non-locked Point: add to it transformList
    //   - else: call getAllPointsFromAncestors
    //     - check if 'areAllFree' is always true
    //     - combine the returned 'objectNumbersList's

    std::cout << "checking if at least 2 objects are selected..." << endl;
    if (nrSelectedObjects>=2) {
        std::cout << "checking if mirror plane is valid..." << endl;
        if(
            (O(objectList[nrSelectedObjects-1])->getName().getString()[0]=='E')
        ) {
            mirrorPlaneObject = (SoPlaneKit*)(O(objectList[nrSelectedObjects-1]));
            /*
            * build a SbPlane from the mirror-plane (last selected object)
            *   ATTENTION!!!
            *   works only if the SoPlaneKit has PointOneName, PointTwoName and PointThreeName set correctly!
            *   we find these points, take their coordinates and put them into the constructor of SbPlane
            */
            mirrorPlane = new SbPlane(
                (
                (SoPointKit*)SoNode::getByName(
                mirrorPlaneObject->PointOneName.getValue()
                )
                )->pCoord.getValue(),
                (
                (SoPointKit*)SoNode::getByName(
                mirrorPlaneObject->PointTwoName.getValue()
                )
                )->pCoord.getValue(),
                (
                (SoPointKit*)SoNode::getByName(
                mirrorPlaneObject->PointThreeName.getValue()
                )
                )->pCoord.getValue()
                ); // new SbPlane
            // generate mirror-list
            std::cout << "generating mirror-list..." << endl;
            for(c1=0; c1<nrSelectedObjects-1; c1++) {
                if(O(objectList[c1])->getName().getString()[0]=='P'){
                    if(O(objectList[c1])->isLocked.getValue()==FALSE) {
                        std::cout << "objectList-element " << O(objectList[c1])->getName() << " (free point)" << endl;
                        putIntoTransformList(O(objectList[c1]));
                    } // if object nr. c1 is a not locked
                    else {
                        std::cout << "objectList-element " << O(objectList[c1])->getName() << " (locked point)" << endl;
                        areAllFree = FALSE;
                        goto GeneratingTransformListFinished;
                    }
                } // if object nr. c1 is a Point
                else {
                    std::vector<int> foundObjectNumbersList;
                    if(makeCopy) {
                        O(objectList[c1])->getAllAncestors(&foundObjectNumbersList, &areAllFree);
                    } // if makeCopy
                    else {
                        O(objectList[c1])->getAllPointsFromAncestors(&foundObjectNumbersList, &areAllFree);
                    } // if else makeCopy
                    if(areAllFree==FALSE) {
                        std::cout << "objectList-element " << O(objectList[c1])->getName() << " with locked ancestor points" << endl;
                        goto GeneratingTransformListFinished;
                    } // if locked Points exist
                    else {
                        if(makeCopy) {
                            putIntoTransformList(O(objectList[c1])); // put non-point object into copy-list as well
                        } // if makeCopy
                        std::cout << "objectList-element " << O(objectList[c1])->getName() << " with these free ancestor points:" << endl;
                        std::vector<int>::iterator foundObjectNumbersListIterator;
                        for(
                            foundObjectNumbersListIterator = foundObjectNumbersList.begin();
                            foundObjectNumbersListIterator != foundObjectNumbersList.end(); 
                            foundObjectNumbersListIterator++
                        ) {
                            std::cout << O(*foundObjectNumbersListIterator)->getName() << endl;
                            putIntoTransformList(O(*foundObjectNumbersListIterator));
                        } // for every element of foundObjectNumbersList
                    } // if else locked Points exist
                } // if else object nr. c1 is a Point
            } // for every element of objectList but the last one
        } // if last selected object is a Plane
    } // if at least 2 objects selected

GeneratingTransformListFinished: // we jump here on early termination
    if(areAllFree==FALSE) {
        transformList.clear();
    } // if locked Points exist

    //////////////////////
    // print transformList
    std::vector<int>::iterator transformListIterator; // iterator thru mirror-list
    std::cout << "mirror-list:" << endl;
    for(
        transformListIterator =  transformList.begin();
        transformListIterator != transformList.end(); 
        transformListIterator++
    ) {
        std::cout << (*transformListIterator) << endl;
    } // for every element of transformList

    // - if "makeCopy"
    //   - generate copies of objects in transformList, points mirrored
    // - if else "makeCopy"
    //   - change the positions of all points from transformList

    if(!transformList.empty()) {
        // transformList must be sorted in case makeCopy==TRUE, 
        // this ensures that prerequisite objects exist for creation of each new object
        if (makeCopy) {
            sort(transformList.begin(), transformList.end());
            // ATTENTION: Insert only one time before the copy sequence a 
            //            deselectAll command, so that all previous objects
            //            get deselected. It is done in the code below but must
            //            also be done in the undoredolist
            addCommandtoUndoReduList("deselectAll", "object", "object", objectList, this->userID.getValue());
        } // if makeCopy

        ////////////////////
        // execute mirroring
        for(
            transformListIterator =  transformList.begin();
            transformListIterator != transformList.end(); 
            transformListIterator++
        ) {
            if (makeCopy) {
                // Points are copied at translated location
                // for all other objects:
                //   - select the copies of the objects that were selected at creation of the original
                //   - call the appropriate SoCnDKit::add... function
                // all original objects handshake their copies using their copyObject pointer

                //////////////////////////////////////
                // if transformListIterator is a Point
                if(O(*transformListIterator)->getName().getString()[0]=='P'){
                    // calculate new position
                    SbVec3f oldPCoord;
                    SbVec3f newPCoord;
                    oldPCoord = ((SoPointKit*)(O(*transformListIterator)))->pCoord.getValue();
                    // if assume mirrorPlane->getNormal() is length 1
                    newPCoord = oldPCoord - 2 * mirrorPlane->getDistance(oldPCoord) * mirrorPlane->getNormal();

                    // make the copy at mirrored location
                    deselectAllObjects();
                    addPoint(
                        newPCoord, 
                        FALSE, 
                        TRUE, 
                        userID
                        ); // addPoint
                    // let the original know it's copy
                    O(*transformListIterator)->copyObject = O(objSum.getValue()-1); // should be the last object in the scene
                } // if transformListIterator is a Point

                //////////////////////////////////////////
                // if transformListIterator is not a Point
                else {

                    deselectAllObjects();
                    //////////////////////////////////////////////////////////////////////////////////
                    // select the copies of the objects that were selected at creation of the original

                    // i assume getObjectList() returns the selection-list at the time of the objects creation
                    SoMFName* creationSelectionList = O(*transformListIterator)->getObjectList();
                    for(c1=0; c1<creationSelectionList->getNum(); c1++) {
                        // check if copyObject exists, that's not sure for each member of creationSelectionList
                        if (((Object3dKit*)SoNode::getByName((*creationSelectionList)[c1]))->copyObject!=NULL) {
                            // select copy
                            ((Object3dKit*)SoNode::getByName((*creationSelectionList)[c1]))->copyObject->select(selectionRoot);
                        } // if copyObject exists
                    } // for c1 over each value of SoMFName

                    // using the selection made before, we add a new object
                    findAddObjectMethod(O(*transformListIterator), userID);

                    // let the original know it's copy
                    O(*transformListIterator)->copyObject = O(objSum.getValue()-1);

                } // if transformListIterator is not a Point
            } // if makeCopy
            else {
                SbVec3f oldPCoord;
                SbVec3f newPCoord;

                oldPCoord = ((SoPointKit*)(O(*transformListIterator)))->pCoord.getValue();
                // if assume mirrorPlane->getNormal() is length 1

                newPCoord = oldPCoord - 2 * mirrorPlane->getDistance(oldPCoord) * mirrorPlane->getNormal();
                ((SoPointKit*)(O(*transformListIterator)))->pCoord.setValue(newPCoord); // setValue
            } // if else makeCopy
        } // for every element of transformList

        // after copy make a useful selection for repeated copy
        if (makeCopy) {

            deselectAllObjects();

            // - select all copies
            // - clean up the copyObject pointers afterwards
            for(
                transformListIterator =  transformList.begin();
                transformListIterator != transformList.end(); 
                transformListIterator++
            ) {
                O(*transformListIterator)->copyObject->select(selectionRoot);
                O(*transformListIterator)->copyObject = NULL;
            } // for every element of transformList

            // select mirror plane
            mirrorPlaneObject->select(selectionRoot);
        } // if makeCopy

        if (doAddCommand) {
            addCommandtoUndoReduList("mirror", objectList, userID);
        } // if doAddCommand
    } // if we can mirror something

    transformList.clear();

    return TRUE;
} // mirror()


SbBool SoCnDKit::measure(int userID)
{
  countObjects(TRUE);             // counts selected objects
  int pointCount = pointList.size();
  int lineCount = lineList.size();
  // measure point distance
  if (pointCount>1)
  {
    //Object3dKit *T = new SoTextKit(objSum.getValue(), O(pointList[0]), O(pointList[1]));
    Object3dKit *T = new SoTextKit(objSum.getValue(), objectList, userID);
    if (T->s_return.ok())
    {          
      T->layer.setValue(getActiveLayer());
      SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
      selectionRoot->addChild(T);
      objSum.setValue(objSum.getValue()+1); 
      addCommandtoUndoReduList("add", T->getName(), T->name.getValue(), objectList, userID);
      return TRUE;
    }
    else
    {
      delete T;
      return FALSE;
    }
    /*	  
    SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
    T->layer.setValue(getActiveLayer());
    O(pointList[0])->deselect(selectionRoot);
    O(pointList[1])->deselect(selectionRoot);
    selectionRoot->addChild(T);
    objSum.setValue(objSum.getValue()+1); */
  } 

  // measure smallest angle between 2 lines
  if (lineCount == 2)
  {
	  Object3dKit *T = new SoTextKit(objSum.getValue(), objectList, userID);
	  if (T->s_return.ok())
	  {          
		  T->layer.setValue(getActiveLayer());
		  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
		  selectionRoot->addChild(T);
		  objSum.setValue(objSum.getValue()+1); 
		  addCommandtoUndoReduList("add", T->getName(), T->name.getValue(), objectList, userID);
		  return TRUE;
	  }
	  else
	  {
		  delete T;
		  return FALSE;
	  }
  } 

  return FALSE;
}


/*
SoMultiSwitch*
SoCnDKit::addUserStructure(int userId)
{
layerSwitch[userId] = new SoMultiSwitch;
SoSelection* layer[usedLayers];
for (int i=0;i<usedLayers;i++)
{
layer[i] = new SoSelection;

  //        NEWNODE(testcube, SoCube);
  //        layer[i]->addChild(testcube);
  
    layerSwitch[userId]->addChild(layer[i]);
    // default: display only first layer of new user
    layerSwitch[userId]->whichChildren.set1Value(0,0);
    }
    return layerSwitch[userId];
    }
*/

SbBool
SoCnDKit::checkWindowGeometry()
{
  printf("Construct3D: checking window geometry\n");
  
  // no windows saved in the field: create new window for Construct3D    
  if (((SoGroup*)windowGroup.getValue())->getNumChildren()==0)
  {
    printf("Construct3D: Creating new window\n");
    
    SoWindowKit* windowKit = new SoWindowKit;
    //    wc.position.setValue(-0.3, -0.7, 0);
    windowKit->position.setValue(-0.2, 0.4, 0.7);
//	windowKit->position.setValue(-0.2, 0.3, 0); //good position for earth-satellite example
    windowKit->orientation.setValue(SbVec3f(1., 0., 0.), -1.5707963 );
    //wc.size.setValue(1.6, 1.6, 1.6);
    windowKit->size.setValue(2.5, 2.5, 2.5);
    // if style is set, it breaks the window
    //windowKit->style = 6+24+32;
    // wc.state = 0;
    windowKit->state = SoWindowKit::MAXIMIZED;
    windowKit->title = SbString("Construct3D");
    
    NEWNODE(root,SoSeparator);
    root->ref();
    
    SoSelection* selectionRoot = new SoSelection;
    selectionRoot->setName("selectionRoot");
    selectionRoot->policy = SoSelection::SHIFT;
    
    windowKit->thickness.setValue(0.005);
    
    // ATTENTION!!!!!!!!!!!!!
    // if Window changes, these 2 variables must be updated!!!!
    winpos = windowKit->position.getValue();
    winrotinverse = windowKit->orientation.getValue().inverse();
    // construct geometry inside window
    SoSeparator* clientVolume = windowKit->getClientVolume();
    
    NEWNODE(complexity,SoComplexity);
    //complexity->value = 0.55;
    complexity->value = 0.85;
    //complexity->value = 1.0;
    //complexity->textureQuality = 0.1;
    complexity->textureQuality = 1;
    root->addChild(complexity);
    
    NEWNODE(enviro,SoEnvironment);
    enviro->ambientIntensity = 0.1;
    root->addChild(enviro);
    
	// get the preRoot from StbWorkspace - it is added to the scene before
	// all other objects and enables us to add global lights that 
	// are valid for all user devices and 3D objects in the scene
	SoGroup* first = StbWorkspace::getInstance()->preRoot;
    // add lightening - 6 Directional lights from the axis directions - back and front
    NEWNODE(light1, SoDirectionalLight);
    light1->direction.setValue(1,0,0);
    light1->intensity = 0.4; // 0.4
    
    NEWNODE(light2, SoDirectionalLight);
    light2->direction.setValue(0,1,0);
    light2->intensity = 0.4; // 0.4
    
    NEWNODE(light3, SoDirectionalLight);
    light3->direction.setValue(0,0,1);
    light3->intensity = 0.3; // 0.4
    
    NEWNODE(light4, SoDirectionalLight);
    light4->direction.setValue(-1,0,0);
    light4->intensity = 0.5;
    
    NEWNODE(light5, SoDirectionalLight);
    light5->direction.setValue(0,-1,0);
    light5->intensity = 0.4;
    
    NEWNODE(light6, SoDirectionalLight);
    light6->direction.setValue(0,0,-1);
    light6->intensity = 0.5;  //0.4

	first->addChild(light1);
	first->addChild(light2);
	first->addChild(light3);
	first->addChild(light4);
	first->addChild(light5);
	first->addChild(light6);

    /*
    NEWNODE(shapehint, SoShapeHints);
    shapehint->shapeType = SoShapeHints::UNKNOWN_SHAPE_TYPE;
    shapehint->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    root->addChild(shapehint);
    */
    NEWNODE(surrounding,SoSeparator);
    NEWNODE(myScale,SoScale);//    scale coord.system down from 50cm to 30cm
    myScale->scaleFactor.setValue(0.4f, 0.4f, 0.4f);
//    surrounding->addChild(readFile("earth.iv", "./content/"));
	surrounding->addChild(myScale);
    surrounding->addChild(readFile("achsen.iv", "./"));
    root->addChild(surrounding);
    
    /*    
    NEWNODE(yText, SoBillboardKit);
    SoTransform* ytrans = (SoTransform*) yText->getPart("transform", false);
    SoSeparator* ySep = (SoSeparator*) yText->getPart("content", false);
    //SoText3* testtext = new SoText3;
    //testtext->string.set1Value(0,"das ist ein test");
    //ytrans->translation.setValue(1,0,0);
    //SoCube* newbox = new SoCube;
    //ySep->addChild(newbox);
    ySep->addChild(readFile("y_text.iv", "../apps/construct3d/"));
    //ySep->addChild(testtext);
    root->addChild(yText);
    */
    
    SbString filePath  = "./";
    filePath += "sound/welcome";
    sound(filePath);
    
    // add components to root separator for horizontalProjection
    addHorizontalProjection(root);
    
    // add components to root separator for verticalProjection  
    addVerticalProjection(root);
    
    // add components to root separator for leftProjection  
    addLeftProjection(root);
    
    // add components to root separator for rightProjection  
    addRightProjection(root);
    
    root->addChild(selectionRoot);
    clientVolume->addChild(root);
    //        saveFile(clientVolume);
    
    selectionRootSensor = new SoNodeSensor(selectionRootCB, this);
    selectionRootSensor->setPriority(0);
    selectionRootSensor->attach(selectionRoot);
    
    // add undo_redo_List to scenegraph
    SoUndoRedoListKit* List = new SoUndoRedoListKit();
    root->addChild(List); 
    
    //    loadFileInWindow(0, SbString("../apps/construct3d/content/viviani.iv"));
    
    if(windowKit != NULL) 
    {
      if((windowKit->getName().getString()==NULL)||!windowKit->getName())
      {
        SbString windowString;
        windowString+=getName().getString();
        windowString+="_WINDOW_";
        windowString+="0";
        windowKit->setName(windowString);
      }
      ((SoGroup*)windowGroup.getValue())->addChild(windowKit);
      return TRUE;
    }
    return FALSE;
  } else
  {
  /*      printf("Getting selectionRoot \n");
  selectionRoot = new SoSelection;
  SoSearchAction sa;
  sa.setSearchingAll(TRUE);
  sa.setName("selectionRoot");
  sa.apply((SoGroup*)windowGroup.getValue());
  SoFullPath* path = (SoFullPath*)sa.getPath();
  if (path == NULL) printf("No selectionRoot found!");
  selectionRoot = (SoSelection*)path->getTail();
    */
  }
  return TRUE;
}


SbBool
SoCnDKit::checkPipGeometry()
{
  return TRUE;
}

void 
SoCnDKit::checkPipConnections( int uid, SoNode * pipSheetContents )
{
  SoNode* pipSheet = pipSheetContents;
  printf("CnDKit: checkPipConnections actualUser-ID: %d\n",uid);
  
  // Point Mode for all users is turned on (set to 1) by default
  // and saved in the drawPoints list
  drawPoints.set1Value(uid, 1);
  //point mode in default state = Turned ON; not switched off yet
  switchPointModeOff.set1Value(uid, FALSE);	
  gridWidth.set1Value(uid,0); // grid turned off by default
  // ATTENTION!!! Only works if users are sequentially numbered!
  // Switches PIPs for following users on 
  // It is sufficient to call useResources instead of forceFocusChange
  if (uid > 0) 
  {
//	  SoUserKit * userKit = SoUserManagerKit::getInstance()->getUserKit(uid);
//	  userKit->getPip()->useResources(appID.getValue());
	  // only if done this way, distribution works correctly 
	  // (otherwise if newly joined user creates a point before other users, it crashes)
	  forceFocusChange(uid);
  }
  numOfPips.setValue(uid);

#ifdef USE_PUC
//--------------------------------------------------------------------------------------------------
// put this into a function
  // find all SoSelectConnectionForUser nodes
  SoSearchAction search;
  search.setType( SoSelectConnectionForUser::getClassTypeId());
  search.setInterest( SoSearchAction::ALL );
  search.setSearchingAll( TRUE );
  search.apply( pipSheet );
  SoPathList & list = search.getPaths();

  printf("found %d instances of SoSelectConnectionForUser\n", list.getLength());
  // set their images to not default to copy the contents
  for( int i = 0; i < list.getLength(); i++ )
  {
	  SoFullPath * path = (SoFullPath *) list[i];
	  assert( path->getTail()->isOfType( SoSelectConnectionForUser::getClassTypeId()));
	  SoSelectConnectionForUser * fConnect = ((SoSelectConnectionForUser *)path->getTail());
	  printf("perform connection for %s with uid %d\n", fConnect->getName().getString(), uid);

	  fConnect->setup(uid, this);
  }
//--------------------------------------------------------------------------------------------------
#endif

  // for all users:
  // add a Grid
  // add the Scenegraph structure with user switch and layers
  //const int32_t *userIDs = SoUserManagerKit::getUserKitFromNode(pipSheet);
  //uid = user Index
  //int uid = SoUserManagerKit::getUserKitIndex(userIDs[i]);
  
  // ATTENTION !!!
  // grid turned off by default - program this properly if grid is needed
  // get clientvolume from windowkit and attach grid
  //  userGrid[uid] = new GridKit();
  //  root->addChild(userGrid[uid]);

  // Point button
  setUniqueNodeName(findNode(pipSheet,"POINT", uid),uid);
  
  // File menu
  setUniqueNodeName(findNode(pipSheet,"SAVE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"LOAD", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"IMPORT", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"EXPORT_VRML", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"HELP_NOTES", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"NEW_START", uid),uid);

  // View menu
  setUniqueNodeName(findNode(pipSheet,"TOP_BUTTON", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"FRONT_BUTTON", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"LEFT_BUTTON", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"RIGHT_BUTTON", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"WIREFRAME", uid),uid);

  // Edit menu
  setUniqueNodeName(findNode(pipSheet,"DESELECT", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"UNDO_BUTTON", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"REDO_BUTTON", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"DELETE_BUTTON", uid),uid);

  //Layer menu
  setUniqueNodeName(findNode(pipSheet,"LAYER_1", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"LAYER_2", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"LAYER_3", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"LAYER_4", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"LAYER_5", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"LAYER_6", uid),uid);

  setUniqueNodeName(findNode(pipSheet,"LAYER_1_VIS", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"LAYER_2_VIS", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"LAYER_3_VIS", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"LAYER_4_VIS", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"LAYER_5_VIS", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"LAYER_6_VIS", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"MOVE_TO", uid),uid);

  // 2D menu
  setUniqueNodeName(findNode(pipSheet,"LINE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"PLANE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"CIRCLE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"ELLIPSE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"TRIANGLE", uid),uid);

  // 3d menu
  setUniqueNodeName(findNode(pipSheet,"SPHERE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"CYLINDER", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"CONE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"BOX", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"POINTS_SURFACE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"B_SPLINE_SURFACE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"B_SPLINE_CURVE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"POINTS_CURVE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"DEGREE_U", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"DEGREE_V", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"POINT_U", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"POINT_V", uid),uid);

  // Construct menu
  setUniqueNodeName(findNode(pipSheet,"NORMAL_PLANE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"TANGENTIAL_PLANE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"PLANE_SYMMETRY", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"ARC_BISECTOR", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"TANGENT", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"NORMAL_VECTOR", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"MID_POINT", uid),uid);

  setUniqueNodeName(findNode(pipSheet,"DISTANCE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"ANGLE", uid),uid);

  // Intersect
  setUniqueNodeName(findNode(pipSheet,"INTERSECTION", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"BOOLEAN_UNION", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"BOOLEAN_INTERSECTION", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"BOOLEAN_DIFFERENCE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"SLICE", uid),uid);
  
  // Transform
  setUniqueNodeName(findNode(pipSheet,"COPY_ON_OFF", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"TRANSLATE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"ROTATE", uid),uid);
  setUniqueNodeName(findNode(pipSheet,"MIRROR", uid),uid);

  setUniqueNodeName(findNode(pipSheet,"SWEEP", uid),uid);
}

void
SoCnDKit::previewCB(void *data, SoSensor *sensor)
{ 
	SoCnDKit *self=(SoCnDKit*)data;
	if(self->isMaster)
	{
		SoButtonKit* button;
		SoFieldSensor* sens = (SoFieldSensor*)sensor;
		button = (SoButtonKit*)sens->getAttachedField()->getContainer();
		printf("Chosen Button is highlighted %s \n",button->getName().getString());
		
		// if button is highlighted, and if it is ON too, 
		// then a normal button press occured
		if (button->highlighted.getValue() == TRUE)
		{
			if (button->on.getValue() == TRUE)
			{
				self->preview = FALSE;
				printf("PREVIEW: Button pressed, set style back \n");
				self->style.setValue(SoDrawStyle::FILLED);
				self->linePattern.setValue(0xffff);
				// remove wireframe object before generating full object
				// when button is released, a wireframe model will be generated again
				// which will be deleted in the last section of this Callback
				// only undo Preview object if preview really worked (previewWorked == TRUE)
				if ((self->objSum.getValue() > 0) && (self->worked == TRUE) && (self->previewWorked == TRUE))
				{
					//			printf("self-worked %d, previewworked %d \n",self->worked, self->previewWorked);
					self->undoPreview();
					// necessary to fix the bug occuring in combination with studierstube widget bug - 
					// when widget is highlighted in an unstable way, c3d came *here* twice with 
					// "worked = TRUE". A 2nd undoPreview was tried and caused a bad crash.
					self->worked = FALSE;
				}
			}
			else
			{
				if (!self->preview)
				{
					self->preview = TRUE;
					self->previewWorked = TRUE;
					printf("PREVIEW: highlighted and off \n");
					self->style.setValue(SoDrawStyle::LINES);
					self->linePattern.setValue(0xf0f0);
					findButtonMethod(data, button);
				}
			}
		}
		else
		{		
			// this part is executed if the preview worked and if pen leaved the button geometry
			if (self->previewWorked)
			{
				self->preview = FALSE;
				self->previewWorked = FALSE;
				printf("PREVIEW: not highlighted or on \n");
				// remove last object in scenegraph = preview object IF preview worked (worked=TRUE)
				if ((self->objSum.getValue() > 0) && (self->worked == TRUE))
					//self->O(self->objSum.getValue() - 1)->Undo(self->selectionRoot,&self->objSum);
					self->undoPreview();
				self->worked = FALSE;
				self->style.setValue(SoDrawStyle::FILLED);
				self->linePattern.setValue(0xffff);
			}
			self->worked = FALSE;
		}
	}
}

// Hardcoding userID - SEQUENTIALLY - ATTENTION: UGLY AND BAD
// it was not possible for me to get in checkPipMasterMode the userID 
// out of just the pipSheet Node since the pipSheets of (both) users 
// are equal (same node, same .iv file).
// Therefore using a global variable userPipSheet to count if this method 
// has already been called before - increasing the value each time.
// ATTENTION: Will probably NOT work in distribution anymore
int userPipSheet = 0;

void
SoCnDKit::checkPipMasterMode(SoNode* pipSheet, SbBool masterMode)
{
    // Hardcoding userID - SEQUENTIALLY - ATTENTION: UGLY AND BAD
    // it was not possible for me to get here the userID with just the pipSheet Node
    // since the pipSheets of (both) users are equal (same node, same .iv file).
    // Therefore using a global variable userPipSheet to count if this method 
    // has already been called before - increasing the value each time.
    // ATTENTION: Will probably NOT work in distribution anymore
  //int uid = getUserFromSheet(pipSheet);
    int uid = 0;
    uid = userPipSheet;
    userPipSheet++;

  printf("userID in CnDKit:CheckPipMasterMode %d, user number %i \n",uid, SoUserManagerKit::getInstance()->getUserNumber());
  
  previousNearestObject[uid] = 0;
  
  //2D
  Button[0] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("LINE",uid), uid);
  Button[1] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("PLANE",uid), uid);
  Button[2] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("CIRCLE",uid), uid);
  Button[3] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("ELLIPSE",uid), uid);
  Button[4] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("TRIANGLE",uid), uid);

  //3D
  Button[5] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("SPHERE",uid), uid);
  Button[6] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("CYLINDER",uid), uid);
  Button[7] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("CONE",uid), uid);
  Button[8] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("BOX",uid), uid);
  Button[9] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("POINTS_SURFACE",uid), uid);
  Button[10] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("B_SPLINE_SURFACE",uid), uid);
  Button[11] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("B_SPLINE_CURVE",uid), uid);
  Button[12] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("POINTS_CURVE",uid), uid);

  //Construct
  Button[13] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("NORMAL_PLANE",uid), uid);
  Button[14] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("TANGENTIAL_PLANE",uid), uid);
  Button[15] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("PLANE_SYMMETRY",uid), uid);
  Button[16] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("ARC_BISECTOR",uid), uid);
  Button[17] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("TANGENT",uid), uid);
  Button[18] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("NORMAL_VECTOR",uid), uid);
  Button[19] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("MID_POINT",uid), uid);
  Button[20] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("DISTANCE",uid), uid);
  Button[21] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("ANGLE",uid), uid);

  //Intersect
  Button[22] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("INTERSECTION",uid), uid);
  Button[23] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("BOOLEAN_UNION",uid), uid);
  Button[24] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("BOOLEAN_INTERSECTION",uid), uid);
  Button[25] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("BOOLEAN_DIFFERENCE",uid), uid);
  Button[26] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("SLICE",uid), uid);

  //Transform
  Button[27] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("SWEEP",uid), uid);

  // no previews from here downwards
  Button[28] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("TRANSLATE",uid), uid);
  Button[29] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("ROTATE",uid), uid);
  Button[30] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("MIRROR",uid), uid);
  

  //File
  Button[31] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("SAVE",uid), uid);
  Button[32] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("LOAD",uid), uid);
  Button[33] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("IMPORT",uid), uid);
  Button[34] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("EXPORT_VRML",uid), uid);
  //Button[X] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("HELP_NOTES",uid));
  Button[35] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("NEW_START",uid), uid);

  //View
  Button[36] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("WIREFRAME",uid), uid);

  //Edit
  Button[37] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("DESELECT",uid), uid);
  Button[38] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("UNDO_BUTTON",uid), uid);
  Button[39] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("REDO_BUTTON",uid), uid);
  Button[40] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("DELETE_BUTTON",uid), uid);

  Button[41] = (SoPushButton*)findNode(pipSheet,makeUniqueNodeName("MOVE_TO",uid), uid);


  
  SoToggleButton* pointPress = (SoToggleButton*)findNode(pipSheet,makeUniqueNodeName("POINT",uid), uid);
  pointPress->on.setValue(TRUE);

  SoToggleButton* horizontalProjectionPress = (SoToggleButton*)findNode(pipSheet,makeUniqueNodeName("TOP_BUTTON",uid), uid);
  horizontalProjectionPress->on.setValue(FALSE);

  SoToggleButton* verticalProjectionPress = (SoToggleButton*)findNode(pipSheet,makeUniqueNodeName("FRONT_BUTTON",uid), uid);
  verticalProjectionPress->on.setValue(FALSE);

  SoToggleButton* leftProjectionPress = (SoToggleButton*)findNode(pipSheet,makeUniqueNodeName("LEFT_BUTTON",uid), uid);
  leftProjectionPress->on.setValue(FALSE);

  SoToggleButton* rightProjectionPress = (SoToggleButton*)findNode(pipSheet,makeUniqueNodeName("RIGHT_BUTTON",uid), uid);
  rightProjectionPress->on.setValue(FALSE);

  SoToggleButton* copyPress = (SoToggleButton*)findNode(pipSheet,makeUniqueNodeName("COPY_ON_OFF",uid), uid);
  copyPress->on.setValue(FALSE);

  
  //  SoToggleButton *layer[usedLayers];
  layer[0] = (SoToggleButton*)findNode(pipSheet,makeUniqueNodeName("LAYER_1_VIS",uid), uid);
  layer[1] = (SoToggleButton*)findNode(pipSheet,makeUniqueNodeName("LAYER_2_VIS",uid), uid);
  layer[2] = (SoToggleButton*)findNode(pipSheet,makeUniqueNodeName("LAYER_3_VIS",uid), uid);
  layer[3] = (SoToggleButton*)findNode(pipSheet,makeUniqueNodeName("LAYER_4_VIS",uid), uid);
  layer[4] = (SoToggleButton*)findNode(pipSheet,makeUniqueNodeName("LAYER_5_VIS",uid), uid);
  layer[5] = (SoToggleButton*)findNode(pipSheet,makeUniqueNodeName("LAYER_6_VIS",uid), uid);
  layer[0]->on.setValue(TRUE);
  
  //  SoRadioButton *layerActive[usedLayers];
  layerActive[0] = (SoRadioButton*)findNode(pipSheet,makeUniqueNodeName("LAYER_1",uid), uid);
  layerActive[1] = (SoRadioButton*)findNode(pipSheet,makeUniqueNodeName("LAYER_2",uid), uid);
  layerActive[2] = (SoRadioButton*)findNode(pipSheet,makeUniqueNodeName("LAYER_3",uid), uid);
  layerActive[3] = (SoRadioButton*)findNode(pipSheet,makeUniqueNodeName("LAYER_4",uid), uid);
  layerActive[4] = (SoRadioButton*)findNode(pipSheet,makeUniqueNodeName("LAYER_5",uid), uid);
  layerActive[5] = (SoRadioButton*)findNode(pipSheet,makeUniqueNodeName("LAYER_6",uid), uid);
  layerActive[0]->on.setValue(TRUE);
  activeLayer.setValue(0);
  
//  SoSlider *gridSlider;
  //  SoSlider *transparencySlider;
  SoIncrementalSlider *degreeSlider;
  SoIncrementalSlider *degreeSliderY;
  SoIncrementalSlider *surfaceXSlider;
  SoIncrementalSlider *surfaceYSlider;
//  gridSlider = (SoSlider*)findNode(pipSheet,makeUniqueNodeName("GridSlider",uid));
  //  transparencySlider = (SoSlider*)findNode(pipSheet,makeUniqueNodeName("TransparencySlider",uid));
  degreeSlider = (SoIncrementalSlider*)findNode(pipSheet,makeUniqueNodeName("DEGREE_U",uid), uid);
  degreeSliderY = (SoIncrementalSlider*)findNode(pipSheet,makeUniqueNodeName("DEGREE_V",uid), uid);
  surfaceXSlider = (SoIncrementalSlider*)findNode(pipSheet,makeUniqueNodeName("POINT_U",uid), uid);
  surfaceYSlider = (SoIncrementalSlider*)findNode(pipSheet,makeUniqueNodeName("POINT_V",uid), uid);
  degreeSlider->value.setValue(2.0);
  degreeSliderY->value.setValue(2.0);
  surfaceXSlider->value.setValue(4.0);
  surfaceYSlider->value.setValue(4.0);

  if (masterMode) // if context is master -> set callbacks
  {
    //    SoUserKit* uk = SoUserManagerKit::getInstance()->getUserKitFromNode(pipSheet);
    //    if (uk==NULL)
    //        printf("UserKit not found");
    //    else
    //    {
    //        int userID = uk->getUserID();
    
    pointPress->setPressCallback(pointButtonCB, this);
    pointPress->setReleaseCallback(pointButtonCB, this);
    
    copyPress->setPressCallback(pushButtonCB, this);
    copyPress->setReleaseCallback(pushButtonCB, this);

    horizontalProjectionPress->setPressCallback(horizontalProjectionPCB, this);
    horizontalProjectionPress->setReleaseCallback(horizontalProjectionRCB, this);
    
    verticalProjectionPress->setPressCallback(verticalProjectionPCB, this);
    verticalProjectionPress->setReleaseCallback(verticalProjectionRCB, this);
    
    leftProjectionPress->setPressCallback(leftProjectionPCB, this);
    leftProjectionPress->setReleaseCallback(leftProjectionRCB, this);
    
    rightProjectionPress->setPressCallback(rightProjectionPCB, this);
    rightProjectionPress->setReleaseCallback(rightProjectionRCB, this);
    
    for (int i=0;i<usedLayers;i++)
    {
      layerSensor[i] = new SoFieldSensor(layerButtonCB, this);
      layerSensor[i]->attach(&layer[i]->on);
      
      // set NO releaseCallback for the Radiobuttons! 
      // Otherwise the ReleaseCallbacks of all buttons are called when 
      // pressing one RadioButton
      //layerActive[i]->setPressCallback(layerActiveButtonCB, this);
      layerActiveSensor[i] = new SoFieldSensor(layerActiveButtonCB, this);
      layerActiveSensor[i]->attach(&layerActive[i]->on);
      layerActive[i]->setPressCallback(layerActiveButtonPCB, this);
    }
    
    //    for (int i=0;i<usedLayers;i++)
    //    {
    //      layer[i]->setPressCallback(layerButtonCB, this);
    //      layer[i]->setReleaseCallback(layerButtonCB, this);
    //    }
    
//    gridSlider->setStartCallback(gridSliderSCB, this);
//    gridSlider->setMoveCallback(gridSliderMCB, this);
//    gridSlider->setFinishCallback(gridSliderFCB, this);
    
//    degreeSlider->setStartCallback(degSliderSCB, this);
//    degreeSlider->setMoveCallback(degSliderMCB, this);
//    degreeSlider->setFinishCallback(degSliderFCB, this);
//    
//    degreeSliderY->setStartCallback(degSliderYSCB, this);
//    degreeSliderY->setMoveCallback(degSliderYMCB, this);
//    degreeSliderY->setFinishCallback(degSliderYFCB, this);
//    
//    surfaceXSlider->setStartCallback(surfaceXSliderSCB, this);
//    surfaceXSlider->setMoveCallback(surfaceXSliderMCB, this);
//    surfaceXSlider->setFinishCallback(surfaceXSliderFCB, this);
//    
//    surfaceYSlider->setStartCallback(surfaceYSliderSCB, this);
//    surfaceYSlider->setMoveCallback(surfaceYSliderMCB, this);
//    surfaceYSlider->setFinishCallback(surfaceYSliderFCB, this);
  }
  else  // if context is slave -> remove callbacks
  {
    pointPress->removePressCallback();
    pointPress->removeReleaseCallback();
    
    horizontalProjectionPress->removePressCallback();
    horizontalProjectionPress->removeReleaseCallback();
    
    verticalProjectionPress->removePressCallback();
    verticalProjectionPress->removeReleaseCallback();
    
    leftProjectionPress->removePressCallback();
    leftProjectionPress->removeReleaseCallback();
    
    rightProjectionPress->removePressCallback();
    rightProjectionPress->removeReleaseCallback();
    
    for (int i=0;i<usedLayers;i++)
    {
      layer[i]->removePressCallback();
      layer[i]->removeReleaseCallback();
      
      //layerActive[i]->removePressCallback();
      layerActive[i]->removeReleaseCallback();
      
//      propLayer[i]->removePressCallback();
//      propLayer[i]->removeReleaseCallback();
    }
    
//    gridSlider->removeStartCallback();
//    gridSlider->removeMoveCallback();
//    gridSlider->removeFinishCallback();
    
    //    transparencySlider->removeStartCallback();
    //    transparencySlider->removeMoveCallback();
    //    transparencySlider->removeFinishCallback();
    
//    degreeSlider->removeStartCallback();
//    degreeSlider->removeMoveCallback();
//    degreeSlider->removeFinishCallback();
//    
//    degreeSliderY->removeStartCallback();
//    degreeSliderY->removeMoveCallback();
//    degreeSliderY->removeFinishCallback();
//    
//    surfaceXSlider->removeStartCallback();
//    surfaceXSlider->removeMoveCallback();
//    surfaceXSlider->removeFinishCallback();
//    
//    surfaceYSlider->removeStartCallback();
//    surfaceYSlider->removeMoveCallback();
//    surfaceYSlider->removeFinishCallback();
  }
  
  int i;
  // set callback routine 
  for (i=0;i<buttonNum;i++)
  {
    if (masterMode) // if context is master -> set callbacks
      Button[i]->setReleaseCallback(pushButtonCB, this);
    else  // if context is slave -> remove callbacks
      Button[i]->removeReleaseCallback();
  }
  // add preview Callback only for construction buttons, where preview
  // makes sense. These are all buttons until number constructButtons
  // ATTENTION: detach sensors for SLAVEs maybe??
  for (i=0;i<constructButtons;i++)
  {
    previewSensor[i] = new SoFieldSensor(previewCB, this);
	printf("Button %d, name %s \n",i,Button[i]->getName().getString());
	printf("Button %d, highlight value %i \n",i,Button[i]->highlighted.getValue());
    previewSensor[i]->attach(&Button[i]->highlighted);
    previewSensor2[i] = new SoFieldSensor(previewCB, this);
    previewSensor2[i]->attach(&Button[i]->on);
  }
  
  
  // Line_0_CONSTRUCT
  // Creating Global fields for all buttons for APRIL interaction
  for (i=0;i<buttonNum;i++)
  {
    SbString buttonName = Button[i]->getName().getString();
    int namelength = buttonName.getLength();
    for (int j=namelength;j>0;j--)
    {
      if (buttonName.getSubString(j,j) == "_")
        buttonName.deleteSubString(j, -1);
      
    }
    // names now look like "Line", "Box",...
    buttonName+="_";
    buttonName+=SbString(uid);
    SbString pureButtonName = buttonName;
    SbString onButtonName = pureButtonName;
    onButtonName+="on";
    SbString highlightedButtonName = pureButtonName;
    highlightedButtonName+="highlighted";
    
    //    printf("Button %d, user 0 ON name %s \n",i,onButtonName.getString());
    //    printf("Button %d, user 0 ON name %s \n",i,highlightedButtonName.getString());
    SoDB::createGlobalField( onButtonName, SoSFBool::getClassTypeId())->connectFrom(&Button[i]->on);
    SoDB::createGlobalField( highlightedButtonName, SoSFBool::getClassTypeId())->connectFrom(&Button[i]->highlighted);
  }
  // only creating global fields for user 0 here
  SoDB::createGlobalField( "Point_0on", SoSFBool::getClassTypeId())->connectFrom(&pointPress->on);
  SoDB::createGlobalField( "Point_0highlighted", SoSFBool::getClassTypeId())->connectFrom(&pointPress->highlighted);
  
  SoDB::createGlobalField( "Layer1_0on", SoSFBool::getClassTypeId())->connectFrom(&layer[0]->on);
  SoDB::createGlobalField( "Layer1_0highlighted", SoSFBool::getClassTypeId())->connectFrom(&layer[0]->highlighted);
  SoDB::createGlobalField( "Layer2_0on", SoSFBool::getClassTypeId())->connectFrom(&layer[1]->on);
  SoDB::createGlobalField( "Layer2_0highlighted", SoSFBool::getClassTypeId())->connectFrom(&layer[1]->highlighted);
  SoDB::createGlobalField( "Layer3_0on", SoSFBool::getClassTypeId())->connectFrom(&layer[2]->on);
  SoDB::createGlobalField( "Layer3_0highlighted", SoSFBool::getClassTypeId())->connectFrom(&layer[2]->highlighted);
  SoDB::createGlobalField( "Layer4_0on", SoSFBool::getClassTypeId())->connectFrom(&layer[3]->on);
  SoDB::createGlobalField( "Layer4_0highlighted", SoSFBool::getClassTypeId())->connectFrom(&layer[3]->highlighted);
  
  SoDB::createGlobalField( "Layer1Active_0on", SoSFBool::getClassTypeId())->connectFrom(&layerActive[0]->on);
  SoDB::createGlobalField( "Layer1Active_0highlighted", SoSFBool::getClassTypeId())->connectFrom(&layerActive[0]->highlighted);
  SoDB::createGlobalField( "Layer2Active_0on", SoSFBool::getClassTypeId())->connectFrom(&layerActive[1]->on);
  SoDB::createGlobalField( "Layer2Active_0highlighted", SoSFBool::getClassTypeId())->connectFrom(&layerActive[1]->highlighted);
  SoDB::createGlobalField( "Layer3Active_0on", SoSFBool::getClassTypeId())->connectFrom(&layerActive[2]->on);
  SoDB::createGlobalField( "Layer3Active_0highlighted", SoSFBool::getClassTypeId())->connectFrom(&layerActive[2]->highlighted);
  SoDB::createGlobalField( "Layer4Active_0on", SoSFBool::getClassTypeId())->connectFrom(&layerActive[3]->on);
  SoDB::createGlobalField( "Layer4Active_0highlighted", SoSFBool::getClassTypeId())->connectFrom(&layerActive[3]->highlighted);
  
//  SoDB::createGlobalField( "PropLayer1_0on", SoSFBool::getClassTypeId())->connectFrom(&propLayer[0]->on);
//  SoDB::createGlobalField( "PropLayer1_0highlighted", SoSFBool::getClassTypeId())->connectFrom(&propLayer[0]->highlighted);
//  SoDB::createGlobalField( "PropLayer2_0on", SoSFBool::getClassTypeId())->connectFrom(&propLayer[1]->on);
//  SoDB::createGlobalField( "PropLayer2_0highlighted", SoSFBool::getClassTypeId())->connectFrom(&propLayer[1]->highlighted);
//  SoDB::createGlobalField( "PropLayer3_0on", SoSFBool::getClassTypeId())->connectFrom(&propLayer[2]->on);
//  SoDB::createGlobalField( "PropLayer3_0highlighted", SoSFBool::getClassTypeId())->connectFrom(&propLayer[2]->highlighted);
//  SoDB::createGlobalField( "PropLayer4_0on", SoSFBool::getClassTypeId())->connectFrom(&propLayer[3]->on);
//  SoDB::createGlobalField( "PropLayer4_0highlighted", SoSFBool::getClassTypeId())->connectFrom(&propLayer[3]->highlighted);
}


// only called by pressing button with the name "Point"
// distribution generates unique node name for each user
// getting userId out of this node name
void SoCnDKit::pointButtonCB(void* userData, SoButtonKit* button)
{
  printf("SoCnDKit::pointButtonCB, button %s pressed!\n", button->getName().getString());
  
  SoCnDKit* self = (SoCnDKit*)userData;
  
  SbString buttonName = button->getName().getString();
  if (button->on.getValue() == TRUE)
    self->addCommandtoUndoReduList("press", buttonName.getSubString(0,4), self->userID.getValue());
  else
    self->addCommandtoUndoReduList("release", buttonName.getSubString(0,4), self->userID.getValue());
  
  // always turn off point mode of the user who's Point mode button is pressed
  // (independent of WHO actually presses the button)
  int userID = self->getUserIdFromUniqueNodeName(button, "Point");
  // First user has number 0
  //  SoUserManagerKit* uk = SoUserManagerKit::getInstance();
  //  int userNumber = uk->getUserNumber();
  //int userIndex = SoUserManagerKit::getInstance()->getUserKitIndex(userID);
  if (self->drawPoints[userID] == 0)
  { 
    //printf("POINT MODE ON, user %i \n", userID);
    // take care that HIGHLIGHT color is turned OFF when user turns off selection mode
    // quick fix: do this by selecting the nearest object to a penPosition of (0,0,10)
    // because no object will be within deselectDist Range, highlighting will be turned OFF
    self->highlightNearestObject(SbVec3f(0,0,10.0f), userID);
    
    self->drawPoints.set1Value(userID, 1); //Point mode turned ON
  }
  else
  { 
    //printf("POINT MODE OFF, user %i \n", userID);
    self->drawPoints.set1Value(userID, 0); //Point mode turned OFF
  }
}


// visible layers
void SoCnDKit::layerButtonCB(void* userData, SoSensor* sensor)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  SoButtonKit* button;
  SoFieldSensor* sens = (SoFieldSensor*)sensor;
  button = (SoButtonKit*)sens->getAttachedField()->getContainer();
  printf("LayerbuttonCB: Button is pressed %s \n",button->getName().getString());
  
  // ATTENTION: Works for 9 layers only (!!!)
  // setting correct colors when layer buttons are pressed
  SbString buttonName = button->getName().getString();
  SbString CutFromString = buttonName.getSubString(0, 6);
  int layerValue = atoi(CutFromString.getSubString(6, 6).getString()) - 1;
  
  // First user has number 0
  SoUserManagerKit* uk = SoUserManagerKit::getInstance();
  int userNumber = uk->getUserNumber();
  //  int userIndex = SoUserManagerKit::getInstance()->getUserKitIndex(userID);
  
  int userID = self->getUserIdFromUniqueNodeName(button, "LAYER_X_VIS");
  for (int i=0; i<userNumber; i++)
  {
    if ((i != userID) && (uk->getUserKit(i)->getPip() != NULL)) // ATTENTION: userID must be sequential!
    {
      // turn on same layer button for all other users
      printf("ButtonName %s \n", buttonName.getSubString(0, 10).getString()); 
      SoNode* pipSheet = uk->getUserKit(i)->getPip()->getPipSheetKit(self->appID.getValue());
      // ATTENTION: Only works for 9 layers
      self->layer[layerValue] = (SoToggleButton*)findNode(pipSheet,self->makeUniqueNodeName(buttonName.getSubString(0, 10).getString(),i), i);
      self->layerSensor[layerValue]->detach();
      self->layer[layerValue]->on.setValue(button->on.getValue());
      self->layerSensor[layerValue]->attach(&self->layer[layerValue]->on);
    }
  }
  
  // Active Layer cannot be turned invisible!
  if (self->layerActive[layerValue]->on.getValue())
    self->layerActive[layerValue]->on.setValue(TRUE);
  
  self->showLayers();
}


// active layer
void SoCnDKit::layerActiveButtonCB(void* userData, SoSensor* sensor)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  SoButtonKit* button;
  SoFieldSensor* sens = (SoFieldSensor*)sensor;
  button = (SoButtonKit*)sens->getAttachedField()->getContainer();
  // return if any RadioButton calls this callback that is turned off
  if (!button->on.getValue())
    return;
  
  // ATTENTION: Works for 9 layers only (!!!)
  SbString buttonName = button->getName().getString();
  SbString CutFromString = buttonName.getSubString(0, 5);
  int layerValue = atoi(CutFromString.getSubString(5, 5).getString()) - 1;
  //printf("Layer value %d \n",layerValue);
  
  // First user has number 0
  SoUserManagerKit* uk = SoUserManagerKit::getInstance();
  int userNumber = uk->getUserNumber();
  //  int userIndex = SoUserManagerKit::getInstance()->getUserKitIndex(userID);
  
  int userID = self->getUserIdFromUniqueNodeName(button, "LAYER_X");
  for (int i=0; i<userNumber; i++)
  {
    if ((i != userID) && (uk->getUserKit(i)->getPip() != NULL)) // ATTENTION: userID must be sequential!
    {
      // turn on same layerActive button for all other users
      printf("LayerActive: ButtonName %s \n", buttonName.getSubString(0, 6).getString()); 
      SoNode* pipSheet = uk->getUserKit(i)->getPip()->getPipSheetKit(self->appID.getValue());
      // ATTENTION: Only works for 9 layers
      self->layerActive[layerValue] = (SoRadioButton*)findNode(pipSheet,self->makeUniqueNodeName(buttonName.getSubString(0, 6).getString(),i), i);
      self->layerActiveSensor[layerValue]->detach();
      self->layerActive[layerValue]->on.setValue(button->on.getValue());
      self->layerActiveSensor[layerValue]->attach(&self->layerActive[layerValue]->on);
    }
  }
  
  // If active layer is not visible -> set it visible and active!
  if ((self->layerActive[layerValue]->on.getValue() == TRUE) && (self->layer[layerValue]->on.getValue() == FALSE))
    self->layer[layerValue]->on.setValue(TRUE);
  self->activeLayer.setValue(layerValue);
  
  self->showLayers();
}


void SoCnDKit::layerActiveButtonPCB(void* userData, SoButtonKit* layerbutton)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  
  // ATTENTION: Works for 9 layers only (!!!)
  SbString buttonName = layerbutton->getName().getString();
  SbString CutFromString = buttonName.getSubString(0, 5);
  int layerValue = atoi(CutFromString.getSubString(5, 5).getString()) - 1;
  
  //save old active layer
  if (self->activeLayerOld.getValue() == -1)
  {
    self->activeLayerOld.setValue(self->activeLayer.getValue());
  }
  
  //add command to undo redo list
  self->addCommandtoUndoReduList("layer", layerValue, self->activeLayerOld.getValue());
  self->activeLayerOld.setValue(-1);
}

//void SoCnDKit::propLayerButtonCB(void* userData, SoButtonKit* button)
void SoCnDKit::propLayerButtonCB(void* userData, SoSensor* sensor)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  SoButtonKit* button;
  SoFieldSensor* sens = (SoFieldSensor*)sensor;
  button = (SoButtonKit*)sens->getAttachedField()->getContainer();
  
  // ATTENTION: Works for 9 layers only (!!!)
  // returns number of PropLayer (button number) that has been pressed
  SbString buttonName = button->getName().getString();
  SbString CutFromString = buttonName.getSubString(0, 9);
  int layerValue = atoi(CutFromString.getSubString(9, 9).getString()) - 1;
  
  // turn all other radiobuttons off - only one proplayer button can be selected
  // workaround because SoRadioButton does not work correctly here!
  // As Radiobuttons do not work here, it woule be best to use ToggleButtons and 
  // simulate the behaviour of RadioButtons!!
  /*  for (int j=0; j<usedLayers; j++)
  {
  if (layerValue != j)
  {
  self->propLayerSensor[layerValue]->detach();
  self->propLayer[j]->on.setValue(FALSE);
  self->propLayerSensor[layerValue]->attach(&self->propLayer[layerValue]->on);
  }
  }
  */
  // only continue if button is pressed
  if (!button->on.getValue())
    return;
  
  // First user has number 0
  SoUserManagerKit* uk = SoUserManagerKit::getInstance();
  int userNumber = uk->getUserNumber();
  //  int userIndex = SoUserManagerKit::getInstance()->getUserKitIndex(userID);
  
  int userID = self->getUserIdFromUniqueNodeName(button, "PropLayerX");
  // turn on same propLayer button for all other users
  for (int i=0; i<userNumber; i++)
  {
    if ((i != userID) && (uk->getUserKit(i)->getPip() != NULL)) // ATTENTION: userID must be sequential!
    {
      printf("PropLayer: ButtonName %s \n", buttonName.getSubString(0, 9).getString()); 
      SoNode* pipSheet = uk->getUserKit(i)->getPip()->getPipSheetKit(self->appID.getValue());
      // ATTENTION: Only works for 9 layers
      self->propLayer[layerValue] = (SoRadioButton*)findNode(pipSheet,self->makeUniqueNodeName(buttonName.getSubString(0, 9).getString(),i), i);
      self->propLayerSensor[layerValue]->detach();
      self->propLayer[layerValue]->on.setValue(button->on.getValue());
      self->propLayerSensor[layerValue]->attach(&self->propLayer[layerValue]->on);
    }
  }
  
  printf("layernumber %d \n",layerValue);
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  int num_selected = selectionRoot->getNumSelected();
  SoPath *selectedPath;
  if (num_selected > 0)
  {
    for (int i = 0; i < num_selected; i++) 
    {
      selectedPath = selectionRoot->getPath(i);
      Object3dKit *parent = (Object3dKit*)selectedPath->getNode(1);
      int objNumber = parent->childNumber.getValue();
      self->O(objNumber)->layer.setValue(layerValue);
      printf("Object %d is now in layer %d \n",objNumber, layerValue);
    }
    
    self->showLayers();
  }
}

void SoCnDKit::propLayerButtonPCB(void* userData, SoButtonKit* layerbutton)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  
  // ATTENTION: Works for 9 layers only (!!!)
  // returns number of PropLayer (button number) that has been pressed
  SbString buttonName = layerbutton->getName().getString();
  SbString CutFromString = buttonName.getSubString(0, 9);
  int layerValue = atoi(CutFromString.getSubString(9, 9).getString()) - 1;
  
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  int num_selected = selectionRoot->getNumSelected();
  if (num_selected > 0)
  {
    self->countObjects(true);
    //add command to undo redo list
    self->addCommandtoUndoReduList("changeObjectLayer", self->objectList, layerValue, self->activeLayer.getValue());
    self->showLayers();
  }
}

void SoCnDKit::gridSliderSCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  self->startvalueSlider.setValue(((SoSpeechSlider*)slider)->getAlpha());
}

void SoCnDKit::gridSliderMCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  
  //double slidval = slider->currentValue.getValue();
  double slidval = ((SoSpeechSlider*)slider)->getAlpha();
  // normalize slider value between 0..5
  double test = floor(slidval*5);
  int sv = (int)test;
  
  int userID = self->getUserIdFromUniqueNodeName(slider, "GridSlider");
  // First user has number 0
  int userIndex = SoUserManagerKit::getInstance()->getUserKitIndex(userID);
  self->gridWidth.set1Value(userIndex,sv);
  
  // set to discrete position 
  double normVal = test/5;
  ((SoSpeechSlider*)slider)->setAlpha((float)normVal);
  //slider->setCurrentValue((float)normVal);
  //printf("slidval %.3f \n",slidval);
  //printf("sv %d \n",sv);
}

void SoCnDKit::gridSliderFCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  if ((float)round(((SoSpeechSlider*)slider)->valueOutput.getValue()) != self->startvalueSlider.getValue())
    self->addCommandtoUndoReduList("slider", "GridSlider", (float)((SoSpeechSlider*)slider)->getAlpha(), self->startvalueSlider.getValue());
}

/* Transparency slider removed from PIP
void SoCnDKit::transSliderSCB(void* userData, SoDraggerKit* slider)
{
SoCnDKit* self = (SoCnDKit*)userData;
self->startvalueSlider.setValue(((SoSpeechSlider*)slider)->getAlpha());
}

  void SoCnDKit::transSliderMCB(void* userData, SoDraggerKit* slider)
  { 
  SoCnDKit* self = (SoCnDKit*)userData;
  
    //double slidval = slider->currentValue.getValue();
    double slidval = ((SoSlider*)slider)->getAlpha();
    printf("slider value = %f\n", (float)slidval);
    
      SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
      int num_selected = selectionRoot->getNumSelected();
      SoPath *selectedPath;
      if (num_selected > 0)
      for (int i = 0; i < num_selected; i++) 
      {
      selectedPath = selectionRoot->getPath(i);
      Object3dKit *parent = (Object3dKit*)selectedPath->getNode(1);
      int objNumber = parent->childNumber.getValue();
      // if this is used, delayed blend mode and no other transparency mode will 
      // deliver the correct results. no satisfying transparency mode available!
      //        if (slidval == 0) 
      //            slidval= 0.001;
      self->O(objNumber)->setTransparency((float)slidval);
      }
      }
      
        void SoCnDKit::transSliderFCB(void* userData, SoDraggerKit* slider)
        {
        SoCnDKit* self = (SoCnDKit*)userData;
        if (((SoSpeechSlider*)slider)->value.getValue() != self->startvalueSlider.getValue())
        self->addCommandtoUndoReduList("slider", "TransparencySlider", ((SoSpeechSlider*)slider)->getAlpha(), self->startvalueSlider.getValue());
        }
*/
void SoCnDKit::degSliderSCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  self->startvalueSlider.setValue((float)((SoSpeechSlider*)slider)->getAlpha());
}

void SoCnDKit::degSliderMCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  // set to discrete position 
  ((SoSpeechSlider*)slider)->setAlpha((float)(round(((SoSpeechSlider*)slider)->valueOutput.getValue())-2)*((SoSpeechSlider*)slider)->getStep());
}

void SoCnDKit::degSliderFCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  if ((float)round(((SoSpeechSlider*)slider)->valueOutput.getValue()) != self->startvalueSlider.getValue())
    self->addCommandtoUndoReduList("slider", "DegreeSlider", (float)((SoSpeechSlider*)slider)->getAlpha(), self->startvalueSlider.getValue());
}

void SoCnDKit::degSliderYSCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  self->startvalueSlider.setValue((float)((SoSpeechSlider*)slider)->getAlpha());
}

void SoCnDKit::degSliderYMCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  // set to discrete position 
  ((SoSpeechSlider*)slider)->setAlpha((float)(round(((SoSpeechSlider*)slider)->valueOutput.getValue())-2)*((SoSpeechSlider*)slider)->getStep());
}

void SoCnDKit::degSliderYFCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  if ((float)round(((SoSpeechSlider*)slider)->valueOutput.getValue()) != self->startvalueSlider.getValue())
    self->addCommandtoUndoReduList("slider", "DegreeSliderY", (float)((SoSpeechSlider*)slider)->getAlpha(), self->startvalueSlider.getValue());
}

void SoCnDKit::surfaceXSliderSCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  self->startvalueSlider.setValue((float)((SoSpeechSlider*)slider)->getAlpha());
}

void SoCnDKit::surfaceXSliderMCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  // set to discrete position 
  ((SoSpeechSlider*)slider)->setAlpha((float)(round(((SoSpeechSlider*)slider)->valueOutput.getValue())-2)*((SoSpeechSlider*)slider)->getStep());
}

void SoCnDKit::surfaceXSliderFCB(void* userData, SoDraggerKit* slider)
{
  printf("SURFACE X SLIDER END CALLBACK\n");
  SoCnDKit* self = (SoCnDKit*)userData;
  if ((float)round(((SoSlider*)slider)->valueOutput.getValue()) != self->startvalueSlider.getValue())
    self->addCommandtoUndoReduList("slider", "SurfaceXSlider", (float)((SoSlider*)slider)->getAlpha(), self->startvalueSlider.getValue());
}

void SoCnDKit::surfaceYSliderSCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  self->startvalueSlider.setValue((float)((SoSpeechSlider*)slider)->getAlpha());
}


void SoCnDKit::surfaceYSliderMCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  // set to discrete position 
  ((SoSpeechSlider*)slider)->setAlpha((float)(round(((SoSpeechSlider*)slider)->valueOutput.getValue())-2)*((SoSpeechSlider*)slider)->getStep());
}

void SoCnDKit::surfaceYSliderFCB(void* userData, SoDraggerKit* slider)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  if ((float)round(((SoSpeechSlider*)slider)->valueOutput.getValue()) != self->startvalueSlider.getValue())
    self->addCommandtoUndoReduList("slider", "SurfaceYSlider", (float)((SoSpeechSlider*)slider)->getAlpha(), self->startvalueSlider.getValue());
}

void SoCnDKit::selectionRootCB(void* userData, SoSensor* sensor)
{
  SbBool update = false;
  SoCnDKit* self = (SoCnDKit*)userData;
  
  SoField* fi = self->selectionRootSensor->getTriggerField();
  if (fi != NULL)
  {
    SoNode* node = self->selectionRootSensor->getTriggerNode();
    //std::cout << "SoField: " << fi->getTypeId().getName().getString() << " SoNode: " << node->getName().getString() << std::endl;
    if ((node->isOfType(SoPointKit::getClassTypeId())) &&
      (!fi->isOfType(SoSFBool::getClassTypeId())) &&
      (!fi->isOfType(SoSFInt32::getClassTypeId())) &&
      (!fi->isOfType(SoMFColor::getClassTypeId())) &&
      (fi->isOfType(SoSFRotation::getClassTypeId())) )
    {
      //This is called when a point is moving
      //      std::cout << "***** 1 Trigger This is called when a point is moving: " << node->getName().getString() << std::endl;
      //update = true;
    }
    else if (fi->getTypeId().getName() == "SFEnum") 
    {
      //This is only called when a abject are selected
      //      std::cout << "***** 2 Trigger This is only called when an object is selected: " << node->getName().getString() << std::endl;
      update = true;
    }
    else if ((node->isOfType(SoPointKit::getClassTypeId())) &&
      (!fi->isOfType(SoSFBool::getClassTypeId())) &&
      (fi->isOfType(SoSFInt32::getClassTypeId())) &&
      (!fi->isOfType(SoMFColor::getClassTypeId())))
    {
      //This is only called when a point are selected
      //      std::cout << "***** 3 Trigger This is only called when a point is selected" << node->getName().getString() << std::endl;
      update = true;
    } 
  }
  else
  {
    // This is only called when new point is generated
    // is not called when objects get selected for instance
    //    printf("************* 4 Trigger This is only called when new point is generated \n");
    update = true;
  }
  
  SoSelection* selectionRoot = (SoSelection*)SoNode::getByName("selectionRoot");
  SoSwitch* horsw = (SoSwitch*)SoNode::getByName("horizontalProjectionSwitch");
  if  (update && (horsw->whichChild.getValue() == SO_SWITCH_ALL))
  {     
    
    self->setPart("horProSel",selectionRoot);
    
    SoSeparator* View = (SoSeparator*)self->getAnyPart("horProSep",false);
    //View->addChild(self->selectionRoot);
    
    SbViewportRegion vp;
    vp.setWindowSize(SbVec2s(1024, 1024));
    vp.setViewport(SbVec2f(0, 0),SbVec2f(1, 1));
    
    //update the size and pos of face with the size of the boundingbox of the objects
    SoGetBoundingBoxAction* BBAction = new SoGetBoundingBoxAction(vp);
    selectionRoot->getBoundingBox(BBAction);
    float cx, cy, cz;
    BBAction->getCenter().getValue(cx, cy, cz);
    SbBox3f bbox = BBAction->getBoundingBox();
    float sx, sy, sz;
    bbox.getSize(sx, sy, sz);
    
    if (((ABS(cy) + sy/2) > 0.1) || ((ABS(cz) + sz/2) > 0.1) || (bbox.getMin()[0] < -0.1))
    {
      float size;
      size = float(MAX((ABS(cy) + sy/2) , (ABS(cz) + sz/2)) + 0.1);
      float pos;
      pos = float(MAX((ABS(cx) + sx/2) , 0.1) + 0.1);
      
      SoVertexProperty* prop = (SoVertexProperty*)SoNode::getByName("horizontalProjectionVertex");
      prop->vertex.set1Value(0, -pos, -size, -size); 
      prop->vertex.set1Value(1, -pos, size, -size);
      prop->vertex.set1Value(2, -pos, size, size);
      prop->vertex.set1Value(3, -pos, -size, size);
      
      SoOrthographicCamera* camera = (SoOrthographicCamera*)self->getPart("horProCam",false);
      camera->height.setValue(size*2);
    }

    SoSceneTexture2* tex = (SoSceneTexture2*)SoNode::getByName("horizontalProjection");
    tex->ref();
    tex->size.setValue(SbVec2s(1024, 1024));
    tex->transparencyFunction.setValue(SoSceneTexture2::ALPHA_TEST);
    tex->model.setValue(SoSceneTexture2::REPLACE);
    tex->scene.setValue(View);
    
  }     
  
  SoSwitch* vertsw = (SoSwitch*)SoNode::getByName("verticalProjectionSwitch");
  if  (update && (vertsw->whichChild.getValue() == SO_SWITCH_ALL))
  {
    
    self->setPart("vertProSel",selectionRoot);
    
    SoSeparator* View = (SoSeparator*)self->getAnyPart("vertProSep",false);
    //View->addChild(self->selectionRoot);
    
    SbViewportRegion vp;
    vp.setWindowSize(SbVec2s(1024, 1024));
    vp.setViewport(SbVec2f(0, 0),SbVec2f(1, 1));
    
    //update the size and pos of face with the size of boundingbox of the objects
    SoGetBoundingBoxAction* BBAction = new SoGetBoundingBoxAction(vp);
    selectionRoot->getBoundingBox(BBAction);
    float cx, cy, cz;
    BBAction->getCenter().getValue(cx, cy, cz);
    SbBox3f bbox = BBAction->getBoundingBox();
    float sx, sy, sz;
    bbox.getSize(sx, sy, sz);
    
    if (((ABS(cy) + sy/2) > 0.1) || ((ABS(cx) + sx/2) > 0.1) || (bbox.getMin()[2] < -0.1))
    {
      float size;
      size = float(MAX((ABS(cy) + sy/2) , (ABS(cx) + sx/2)) + 0.1);
      float pos;
      pos = float(MAX((ABS(cz) + sz/2) ,  0.1) + 0.1);
      
      SoVertexProperty* prop = (SoVertexProperty*)SoNode::getByName("verticalProjectionVertex");
      prop->vertex.set1Value(0, -size, -size, -pos); 
      prop->vertex.set1Value(1, size, -size, -pos);
      prop->vertex.set1Value(2, size, size, -pos);
      prop->vertex.set1Value(3, -size, size, -pos);
      
      SoOrthographicCamera* camera = (SoOrthographicCamera*)self->getPart("vertProCam",false);
      camera->height.setValue(size*2);
    }

    SoSceneTexture2* tex = (SoSceneTexture2*)SoNode::getByName("verticalProjection");
    tex->ref();
    tex->size.setValue(SbVec2s(1024, 1024));
    tex->transparencyFunction.setValue(SoSceneTexture2::ALPHA_TEST);
    tex->model.setValue(SoSceneTexture2::REPLACE);
    tex->scene.setValue(View);
    
  }     
  
  SoSwitch* leftsw = (SoSwitch*)SoNode::getByName("leftProjectionSwitch");
  if  (update && (leftsw->whichChild.getValue() == SO_SWITCH_ALL))
  {
    
    self->setPart("leftProSel",selectionRoot);
    
    SoSeparator* View = (SoSeparator*)self->getAnyPart("leftProSep",false);
    //View->addChild(self->selectionRoot);
    
    SbViewportRegion vp;
    vp.setWindowSize(SbVec2s(1024, 1024));
    vp.setViewport(SbVec2f(0, 0),SbVec2f(1, 1));
    
    //update the size and pos of face with the size of boundingbox of the objects
    SoGetBoundingBoxAction* BBAction = new SoGetBoundingBoxAction(vp);
    selectionRoot->getBoundingBox(BBAction);
    float cx, cy, cz;
    BBAction->getCenter().getValue(cx, cy, cz);
    SbBox3f bbox = BBAction->getBoundingBox();
    float sx, sy, sz;
    bbox.getSize(sx, sy, sz);
    
    
    if (((ABS(cz) + sz/2) > 0.1) || ((ABS(cx) + sx/2) > 0.1) || (bbox.getMin()[1] > 0.1))
    {
      float size;
      size = float(MAX((ABS(cz) + sz/2), (ABS(cx) + sx/2)) + 0.1);
      float pos;
      pos = float(MAX((ABS(cy) + sy/2), 0.1) + 0.1); 
      
      SoVertexProperty* prop = (SoVertexProperty*)SoNode::getByName("leftProjectionVertex");
      prop->vertex.set1Value(0, -size,pos,-size); 
      prop->vertex.set1Value(1, size, pos,-size);
      prop->vertex.set1Value(2, size, pos, size);
      prop->vertex.set1Value(3, -size,pos, size);
      
      SoOrthographicCamera* camera = (SoOrthographicCamera*)self->getPart("leftProCam",false);
      camera->height.setValue(size*2);
    }
    
    /*
    if (((ABS(cy) + sy/2) > 0.5) || ((ABS(cz) + sz/2) > 0.5) || (bbox.getMax()[0] > 0.5))
    {
      float size;
      size = MAX((ABS(cy) + sy/2) , (ABS(cz) + sz/2));
      float pos;
      pos =  MAX((ABS(cx) + sx/2), 0.4) + 0.1;
      
      SoVertexProperty* prop = (SoVertexProperty*)SoNode::getByName("leftProjectionVertex");
      prop->vertex.set1Value(0, pos, -size, -size); 
      prop->vertex.set1Value(1, pos, size, -size);
      prop->vertex.set1Value(2, pos, size, size);
      prop->vertex.set1Value(3, pos, -size,size);
      
      SoOrthographicCamera* camera = (SoOrthographicCamera*)self->getPart("leftProCam",false);
      camera->height.setValue(size*2);
    }
    */
    SoSceneTexture2* tex = (SoSceneTexture2*)SoNode::getByName("leftProjection");
    tex->ref();
    tex->size.setValue(SbVec2s(1024, 1024));
    tex->transparencyFunction.setValue(SoSceneTexture2::ALPHA_TEST);
    tex->model.setValue(SoSceneTexture2::REPLACE);
    tex->scene.setValue(View);
    
  } 
  
  SoSwitch* rightsw = (SoSwitch*)SoNode::getByName("rightProjectionSwitch");
  if  (update && (rightsw->whichChild.getValue() == SO_SWITCH_ALL))
  {
    
    self->setPart("rightProSel",selectionRoot);
    
    SoSeparator* View = (SoSeparator*)self->getAnyPart("rightProSep",false);
    //View->addChild(self->selectionRoot);
    
    SbViewportRegion vp;
    vp.setWindowSize(SbVec2s(1024, 1024));
    vp.setViewport(SbVec2f(0, 0),SbVec2f(1, 1));
    
    //update the size and pos of face with the size of boundingbox of the objects
    SoGetBoundingBoxAction* BBAction = new SoGetBoundingBoxAction(vp);
    selectionRoot->getBoundingBox(BBAction);
    float cx, cy, cz;
    BBAction->getCenter().getValue(cx, cy, cz);
    SbBox3f bbox = BBAction->getBoundingBox();
    float sx, sy, sz;
    bbox.getSize(sx, sy, sz);
    /* org
    if (((ABS(cy) + sy/2) > 0.5) || ((ABS(cz) + sz/2) > 0.5) || (bbox.getMin()[0] < -0.5))
    {
      float size;
      size = MAX((ABS(cy) + sy/2) , (ABS(cz) + sz/2)) + 0.1;
      float pos;
      pos = MAX((ABS(cx) + sx/2) , 0.4) + 0.1;
      
      SoVertexProperty* prop = (SoVertexProperty*)SoNode::getByName("rightProjectionVertex");
      prop->vertex.set1Value(0, -pos, -size, -size); 
      prop->vertex.set1Value(1, -pos, size, -size);
      prop->vertex.set1Value(2, -pos, size, size);
      prop->vertex.set1Value(3, -pos, -size, size);
      
      SoOrthographicCamera* camera = (SoOrthographicCamera*)self->getPart("rightProCam",false);
      camera->height.setValue(size*2);
    }
    */
    if (((ABS(cz) + sz/2) > 0.1) || ((ABS(cx) + sx/2) > 0.1) || (bbox.getMin()[1] > 0.1))
    {
      float size;
      size = float(MAX((ABS(cz) + sz/2), (ABS(cx) + sx/2)) + 0.1);
      float pos;
      pos = float(MAX((ABS(cy) + sy/2), 0.1) + 0.1); 
      
      SoVertexProperty* prop = (SoVertexProperty*)SoNode::getByName("rightProjectionVertex");
      prop->vertex.set1Value(0, -size,-pos,-size); 
      prop->vertex.set1Value(1, size, -pos,-size);
      prop->vertex.set1Value(2, size, -pos, size);
      prop->vertex.set1Value(3, -size,-pos, size);
      
      SoOrthographicCamera* camera = (SoOrthographicCamera*)self->getPart("rightProCam",false);
      camera->height.setValue(size*2);
    }

    SoSceneTexture2* tex = (SoSceneTexture2*)SoNode::getByName("rightProjection");
    tex->ref();
    tex->size.setValue(SbVec2s(1024, 1024));
    tex->transparencyFunction.setValue(SoSceneTexture2::ALPHA_TEST);
    tex->model.setValue(SoSceneTexture2::REPLACE);
    tex->scene.setValue(View);
    
  } 
  
}         

void SoCnDKit::addHorizontalProjection(SoSeparator* root)
{
  //SoTexture2* tex = new SoTexture2;
  SoSceneTexture2* tex = new SoSceneTexture2;
  tex->setName("horizontalProjection");
  
  SoFaceSet* face = new SoFaceSet;
  face->setName("horizontalProjectionFace");
  SoVertexProperty* prop = new SoVertexProperty;
  prop->setName("horizontalProjectionVertex");
  
  /*
  prop->vertex.set1Value(0, -0.5,0.5,-0.5); 
  prop->vertex.set1Value(1, 0.5,0.5,-0.5);
  prop->vertex.set1Value(2, 0.5,0.5,0.5);
  prop->vertex.set1Value(3, -0.5,0.5,0.5);
  */
  prop->vertex.set1Value(0, -0.1,-0.1, -0.1); 
  prop->vertex.set1Value(1, -0.1,0.1, -0.1);
  prop->vertex.set1Value(2, -0.1,0.1, 0.1);
  prop->vertex.set1Value(3, -0.1,-0.1, 0.1);
  prop->normal.set1Value(0,1, 0, 0);
  prop->texCoord.set1Value(0,0, 0);
  prop->texCoord.set1Value(1,1, 0);
  prop->texCoord.set1Value(2,1, 1);
  prop->texCoord.set1Value(3,0, 1);
  face->vertexProperty.setValue(prop);
  
  SoSeparator* separator = new SoSeparator;
  separator->setName("horizontalProjectionSeparator");
  
  SoSwitch* sw = new SoSwitch;
  sw->setName("horizontalProjectionSwitch");
  sw->whichChild.setValue(SO_SWITCH_NONE);
  
  separator->addChild(tex);
  separator->addChild(face);
  sw->addChild(separator);
  root->addChild(sw);
  
  //end add components to root separator for horizontalProjection 
  
  //set all parts for horizontal projection
  
  SoComplexity* complexity = (SoComplexity*)getPart("horProCom",false);
  complexity->value = 0.85;
  complexity->textureQuality = 1;
  
  SoEnvironment* enviro = (SoEnvironment*)getPart("horProEnv",false);
  enviro->ambientIntensity = 0.1;
  
  SoDirectionalLight* light1 = (SoDirectionalLight*)getPart("horProLig1",false);
  light1->direction.setValue(1,0,0);
  light1->intensity = 0.4; 
  
  SoDirectionalLight* light2 = (SoDirectionalLight*)getPart("horProLig2",false);
  light2->direction.setValue(0,1,0);
  light2->intensity = 0.4;
  
  SoDirectionalLight* light3 = (SoDirectionalLight*)getPart("horProLig3",false);
  light3->direction.setValue(0,0,1);
  light3->intensity = 0.3;
  
  SoDirectionalLight* light4 = (SoDirectionalLight*)getPart("horProLig4",false);
  light4->direction.setValue(-1,0,0);
  light4->intensity = 0.5;
  
  SoDirectionalLight* light5 = (SoDirectionalLight*)getPart("horProLig5",false);
  light5->direction.setValue(0,-1,0);
  light5->intensity = 0.4;
  
  SoDirectionalLight* light6 = (SoDirectionalLight*)getPart("horProLig6",false);
  light6->direction.setValue(0,0,-1);
  light6->intensity = 0.5;
  
  SoOrthographicCamera* camera = (SoOrthographicCamera*)getPart("horProCam",false);
  /*
  camera->height.setValue(1.f);
  camera->position.setValue(0,-2,0);
  camera->pointAt(SbVec3f(0,0,0));
  camera->nearDistance.setValue(0);
  */
  camera->height.setValue(1.f);
  camera->position.setValue(2,0,0);
  camera->pointAt(SbVec3f(0,0,0), SbVec3f(0,0,1));
  camera->nearDistance.setValue(0);
}

void SoCnDKit::addVerticalProjection(SoSeparator* root)
{
  //SoTexture2* tex = new SoTexture2;
  SoSceneTexture2* tex = new SoSceneTexture2;
  tex->setName("verticalProjection");
  
  SoFaceSet* face = new SoFaceSet;
  SoVertexProperty* prop = new SoVertexProperty;
  prop->setName("verticalProjectionVertex");
  prop->vertex.set1Value(0, -0.1,-0.1, -0.1); 
  prop->vertex.set1Value(1, 0.1,-0.1, -0.1);
  prop->vertex.set1Value(2, 0.1,0.1, -0.1);
  prop->vertex.set1Value(3, -0.1,0.1, -0.1);
  prop->normal.set1Value(0,1, 0, 0);
  prop->texCoord.set1Value(0,0, 0);
  prop->texCoord.set1Value(1,1, 0);
  prop->texCoord.set1Value(2,1, 1);
  prop->texCoord.set1Value(3,0, 1);
  face->vertexProperty.setValue(prop);
  
  SoSeparator* separator = new SoSeparator;
  separator->setName("verticalProjectionSeparator");
  
  SoSwitch* sw = new SoSwitch;
  sw->setName("verticalProjectionSwitch");
  sw->whichChild.setValue(SO_SWITCH_NONE);
  
  separator->addChild(tex);
  separator->addChild(face);
  sw->addChild(separator);
  root->addChild(sw);
  
  //end add components to root separator for verticalProjection 
  
  //set all parts for vertical projection
  
  SoComplexity* complexity = (SoComplexity*)getPart("vertProCom",false);
  complexity->value = 0.85;
  complexity->textureQuality = 1;
  
  SoEnvironment* enviro = (SoEnvironment*)getPart("vertProEnv",false);
  enviro->ambientIntensity = 0.1;
  
  SoDirectionalLight* light1 = (SoDirectionalLight*)getPart("vertProLig1",false);
  light1->direction.setValue(1,0,0);
  light1->intensity = 0.4; 
  
  SoDirectionalLight* light2 = (SoDirectionalLight*)getPart("vertProLig2",false);
  light2->direction.setValue(0,1,0);
  light2->intensity = 0.4;
  
  SoDirectionalLight* light3 = (SoDirectionalLight*)getPart("vertProLig3",false);
  light3->direction.setValue(0,0,1);
  light3->intensity = 0.3;
  
  SoDirectionalLight* light4 = (SoDirectionalLight*)getPart("vertProLig4",false);
  light4->direction.setValue(-1,0,0);
  light4->intensity = 0.5;
  
  SoDirectionalLight* light5 = (SoDirectionalLight*)getPart("vertProLig5",false);
  light5->direction.setValue(0,-1,0);
  light5->intensity = 0.4;
  
  SoDirectionalLight* light6 = (SoDirectionalLight*)getPart("vertProLig6",false);
  light6->direction.setValue(0,0,-1);
  light6->intensity = 0.5;
  
  SoOrthographicCamera* camera = (SoOrthographicCamera*)getPart("vertProCam",false);
  camera->height.setValue(1.f);
  camera->position.setValue(0,0,2);
  camera->pointAt(SbVec3f(0,0,0));
  camera->nearDistance.setValue(0);
}

void SoCnDKit::addLeftProjection(SoSeparator* root)
{
  //SoTexture2* tex = new SoTexture2;
  SoSceneTexture2* tex = new SoSceneTexture2;
  tex->setName("leftProjection");
  
  SoFaceSet* face = new SoFaceSet;
  SoVertexProperty* prop = new SoVertexProperty;
  prop->setName("leftProjectionVertex");

  prop->vertex.set1Value(0, -0.1,0.1,-0.1); 
  prop->vertex.set1Value(1, 0.1,0.1,-0.1);
  prop->vertex.set1Value(2, 0.1,0.1,0.1);
  prop->vertex.set1Value(3, -0.1,0.1,0.1);
  /*
  prop->vertex.set1Value(0, 0.5,-0.5, -0.5); 
  prop->vertex.set1Value(1, 0.5,0.5, -0.5);
  prop->vertex.set1Value(2, 0.5,0.5, 0.5);
  prop->vertex.set1Value(3, 0.5,-0.5, 0.5);
  */
  prop->normal.set1Value(0,1, 0, 0);
  prop->texCoord.set1Value(0,0, 0);
  prop->texCoord.set1Value(1,1, 0);
  prop->texCoord.set1Value(2,1, 1);
  prop->texCoord.set1Value(3,0, 1);
  face->vertexProperty.setValue(prop);
  
  SoSeparator* separator = new SoSeparator;
  separator->setName("leftProjectionSeparator");
  
  SoSwitch* sw = new SoSwitch;
  sw->setName("leftProjectionSwitch");
  sw->whichChild.setValue(SO_SWITCH_NONE);
  
  separator->addChild(tex);
  separator->addChild(face);
  sw->addChild(separator);
  root->addChild(sw);
  
  //end add components to root separator for verticalProjection 
  
  //set all parts for left projection
  
  SoComplexity* complexity = (SoComplexity*)getPart("leftProCom",false);
  complexity->value = 0.85;
  complexity->textureQuality = 1;
  
  SoEnvironment* enviro = (SoEnvironment*)getPart("leftProEnv",false);
  enviro->ambientIntensity = 0.1;
  
  SoDirectionalLight* light1 = (SoDirectionalLight*)getPart("leftProLig1",false);
  light1->direction.setValue(1,0,0);
  light1->intensity = 0.4; 
  
  SoDirectionalLight* light2 = (SoDirectionalLight*)getPart("leftProLig2",false);
  light2->direction.setValue(0,1,0);
  light2->intensity = 0.4;
  
  SoDirectionalLight* light3 = (SoDirectionalLight*)getPart("leftProLig3",false);
  light3->direction.setValue(0,0,1);
  light3->intensity = 0.3;
  
  SoDirectionalLight* light4 = (SoDirectionalLight*)getPart("leftProLig4",false);
  light4->direction.setValue(-1,0,0);
  light4->intensity = 0.5;
  
  SoDirectionalLight* light5 = (SoDirectionalLight*)getPart("leftProLig5",false);
  light5->direction.setValue(0,-1,0);
  light5->intensity = 0.4;
  
  SoDirectionalLight* light6 = (SoDirectionalLight*)getPart("leftProLig6",false);
  light6->direction.setValue(0,0,-1);
  light6->intensity = 0.5;
  
  SoOrthographicCamera* camera = (SoOrthographicCamera*)getPart("leftProCam",false);
  /* org
  camera->height.setValue(1.f);
  camera->position.setValue(-2,0,0);
  camera->pointAt(SbVec3f(0,0,0), SbVec3f(0,0,1));
  camera->nearDistance.setValue(0);
  */
  camera->height.setValue(1.f);
  camera->position.setValue(0,-2,0);
  camera->pointAt(SbVec3f(0,0,0));
  camera->nearDistance.setValue(0);
}

void SoCnDKit::addRightProjection(SoSeparator* root)
{
  //SoTexture2* tex = new SoTexture2;
  SoSceneTexture2* tex = new SoSceneTexture2;
  tex->setName("rightProjection");
  
  SoFaceSet* face = new SoFaceSet;
  SoVertexProperty* prop = new SoVertexProperty;
  prop->setName("rightProjectionVertex");

  prop->vertex.set1Value(0, -0.1,-0.1,-0.1); 
  prop->vertex.set1Value(1, 0.1,-0.1,-0.1);
  prop->vertex.set1Value(2, 0.1,-0.1,0.1);
  prop->vertex.set1Value(3, -0.1,-0.1,0.1);
  /* org
  prop->vertex.set1Value(0, -0.5,-0.5, -0.5); 
  prop->vertex.set1Value(1, -0.5,0.5, -0.5);
  prop->vertex.set1Value(2, -0.5,0.5, 0.5);
  prop->vertex.set1Value(3, -0.5,-0.5, 0.5);
  */
  prop->normal.set1Value(0,1, 0, 0);
  prop->texCoord.set1Value(0,0, 0);
  prop->texCoord.set1Value(1,1, 0);
  prop->texCoord.set1Value(2,1, 1);
  prop->texCoord.set1Value(3,0, 1);
  face->vertexProperty.setValue(prop);
  
  SoSeparator* separator = new SoSeparator;
  separator->setName("rightProjectionSeparator");
  
  SoSwitch* sw = new SoSwitch;
  sw->setName("rightProjectionSwitch");
  sw->whichChild.setValue(SO_SWITCH_NONE);
  
  separator->addChild(tex);
  separator->addChild(face);
  sw->addChild(separator);
  root->addChild(sw);
  
  //end add components to root separator for verticalProjection 
  
  //set all parts for right projection
  
  SoComplexity* complexity = (SoComplexity*)getPart("rightProCom",false);
  complexity->value = 0.85;
  complexity->textureQuality = 1;
  
  SoEnvironment* enviro = (SoEnvironment*)getPart("rightProEnv",false);
  enviro->ambientIntensity = 0.1;
  
  SoDirectionalLight* light1 = (SoDirectionalLight*)getPart("rightProLig1",false);
  light1->direction.setValue(1,0,0);
  light1->intensity = 0.4; 
  
  SoDirectionalLight* light2 = (SoDirectionalLight*)getPart("rightProLig2",false);
  light2->direction.setValue(0,1,0);
  light2->intensity = 0.4;
  
  SoDirectionalLight* light3 = (SoDirectionalLight*)getPart("rightProLig3",false);
  light3->direction.setValue(0,0,1);
  light3->intensity = 0.3;
  
  SoDirectionalLight* light4 = (SoDirectionalLight*)getPart("rightProLig4",false);
  light4->direction.setValue(-1,0,0);
  light4->intensity = 0.5;
  
  SoDirectionalLight* light5 = (SoDirectionalLight*)getPart("rightProLig5",false);
  light5->direction.setValue(0,-1,0);
  light5->intensity = 0.4;
  
  SoDirectionalLight* light6 = (SoDirectionalLight*)getPart("rightProLig6",false);
  light6->direction.setValue(0,0,-1);
  light6->intensity = 0.5;
  
  SoOrthographicCamera* camera = (SoOrthographicCamera*)getPart("rightProCam",false);
  /* org
  camera->height.setValue(1.f);
  camera->position.setValue(2,0,0);
  camera->pointAt(SbVec3f(0,0,0), SbVec3f(0,0,1));
  camera->nearDistance.setValue(0);
  */
  camera->height.setValue(1.f);
  camera->position.setValue(0,2,0);
  camera->pointAt(SbVec3f(0,0,0));//, SbVec3f(0,0,-1));
  camera->nearDistance.setValue(0);
}

bool beginequal(const char *longstr, const char *shortstr)
{
  int i = 0;
  while(longstr[i] == shortstr[i] && shortstr[i] != 0) ++i;
  return(shortstr[i] == 0);
}

void
SoCnDKit::findButtonMethod(void* userData, SoButtonKit* button)
{
	SoCnDKit* self = (SoCnDKit*)userData;
	if (self->isMaster)
	{
		int userID = 0;
		//set back state
		self->worked = FALSE;
		
        //File
    if (beginequal(button->getName().getString(),"SAVE"))
    {
        userID = self->getUserIdFromUniqueNodeName(button, "SAVE");
        self->saveFile(userID);
        //self->saveFile(self->selectionRoot);
    }
    else if (beginequal(button->getName().getString(),"LOAD"))
    {
        userID = self->getUserIdFromUniqueNodeName(button, "LOAD");
        // LOAD disabled until load menu has the same look as C3D menu
        //self->loadFile(userID);
    }
    else if (beginequal(button->getName().getString(),"IMPORT"))
    {
        userID = self->getUserIdFromUniqueNodeName(button, "IMPORT");
        self->loadInWindowFlag = TRUE;
        // LOAD disabled until load menu has the same look as C3D menu
        // self->loadFile(userID);
    }
    else if (beginequal(button->getName().getString(),"EXPORT_VRML"))
    {
        userID = self->getUserIdFromUniqueNodeName(button, "EXPORT_VRML");
        self->exportVRML2(userID);
    }
    else if (beginequal(button->getName().getString(),"NEW_START"))
        self->clearScene();
        
    //View
    else if (beginequal(button->getName().getString(),"WIREFRAME"))
        self->wireframeObjects();

    //Edit
    else if (beginequal(button->getName().getString(),"DESELECT"))
        self->deselectAll();
    else if (beginequal(button->getName().getString(),"UNDO_BUTTON"))
        self->undo();
    else if (beginequal(button->getName().getString(),"REDO_BUTTON"))
        self->redo();
    else if (beginequal(button->getName().getString(),"DELETE_BUTTON"))
        self->deleteSelectedObjects();
//    else if (beginequal(button->getName().getString(),"SelDepObj"))
//    {
//			self->selectAllDependentObjects();
//    }

    //Layers - Move To
    else if (beginequal(button->getName().getString(),"MOVE_TO"))
    {
        // TODO: MUST BE IMPLEMENTED!!!
    }

    //2D
	else if(beginequal(button->getName().getString(), "LINE"))
		{
			userID = self->getUserIdFromUniqueNodeName(button, "LINE");
			self->worked = self->addLine(userID);
		}
       
        // PLANE was moved down AFTER "PLANE_SYMMETRY" because the 
        // beginequal method finds the PLANE button first and always calls
        // addPlane with the normal plane parameters instead of PLANE_SYMMETRY
        // should better rename PLANE_SYMMETRY

		else if (beginequal(button->getName().getString(),"CIRCLE"))
		{
			userID = self->getUserIdFromUniqueNodeName(button, "CIRCLE");
			self->worked = self->addEllipse(CIRCLE, userID);
		}
		else if (beginequal(button->getName().getString(),"ELLIPSE"))
		{
			userID = self->getUserIdFromUniqueNodeName(button, "ELLIPSE");
			self->worked = self->addEllipse(ELLIPSE_CURVE, userID);
		}
        else if (beginequal(button->getName().getString(),"TRIANGLE"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "TRIANGLE");
            self->worked = self->addPlanethreePoints(userID);
        }

        //3D
		else if (beginequal(button->getName().getString(),"SPHERE"))
		{
			userID = self->getUserIdFromUniqueNodeName(button, "SPHERE");
			//	  printf("UserID in findbutton %d \n",userID);
			self->worked = self->addSphere(userID);
		}
		else if (beginequal(button->getName().getString(),"CYLINDER"))
		{
			userID = self->getUserIdFromUniqueNodeName(button, "CYLINDER");
			self->worked = self->addCylinder(userID);
		}
		else if (beginequal(button->getName().getString(),"CONE"))
		{
			userID = self->getUserIdFromUniqueNodeName(button, "CONE");
			self->worked = self->addCone(userID);
		}
        else if (beginequal(button->getName().getString(),"BOX"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "BOX");
            self->worked = self->addCube(userID);
        }
        else if (beginequal(button->getName().getString(),"POINTS_SURFACE"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "POINTS_SURFACE");
            self->worked = self->addSurface(SURFACE_POINTS_ON, userID);
        }
        else if (beginequal(button->getName().getString(),"B_SPLINE_SURFACE"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "B_SPLINE_SURFACE");
            self->worked = self->addSurface(SURFACE_B_SPLINE, userID);
        }
        else if (beginequal(button->getName().getString(),"B_SPLINE_CURVE"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "B_SPLINE_CURVE");
            self->worked = self->addCurve(CURVE_BEZIER, userID);
        }
        else if (beginequal(button->getName().getString(),"POINTS_CURVE"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "POINTS_CURVE");
            self->worked = self->addCurve(CURVE_POINTS_ON, userID);
        }

        //Construct		
        else if (beginequal(button->getName().getString(),"NORMAL_PLANE"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "NORMAL_PLANE");
            self->worked = self->addPlane(TRUE, FALSE, userID); // normal but not symmetry plane
        }
        else if (beginequal(button->getName().getString(),"TANGENTIAL_PLANE"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "TANGENTIAL_PLANE");
            self->worked = self->addPlane(FALSE, FALSE, userID); // normal but not symmetry plane
        }
        else if (beginequal(button->getName().getString(),"PLANE_SYMMETRY"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "PLANE_SYMMETRY");
            self->worked = self->addPlane(TRUE, TRUE, userID);  // plane is normal and symmetral
        }
        // PLANE was moved here AFTER "PLANE_SYMMETRY" because the 
        // beginequal method finds the PLANE button first and always calls
        // addPlane with the normal plane parameters instead of PLANE_SYMMETRY
        // should better rename PLANE_SYMMETRY
        else if (beginequal(button->getName().getString(),"PLANE"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "PLANE");
            self->worked = self->addPlane(FALSE, FALSE, userID);// no normal and no symmetry plane
        }
        else if (beginequal(button->getName().getString(),"ARC_BISECTOR"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "ARC_BISECTOR");
            self->worked = self->addAngleBisector(userID);
        }
        else if(beginequal(button->getName().getString(), "TANGENT"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "TANGENT");
            self->worked = self->addLine(userID);
        }
        else if (beginequal(button->getName().getString(),"NORMAL_VECTOR"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "NORMAL_VECTOR");
            self->worked = self->addLine(userID);
        }
        else if (beginequal(button->getName().getString(),"MID_POINT"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "MID_POINT");
            self->worked = self->addPoint(SbVec3f(0,0,0), true, false, userID);
        }
        else if (beginequal(button->getName().getString(),"DISTANCE"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "DISTANCE");
            self->worked = self->measure(userID);
        }
        else if (beginequal(button->getName().getString(),"ANGLE"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "ANGLE");
            self->worked = self->measure(userID);
        }

        //Intersect
        else if (beginequal(button->getName().getString(),"INTERSECTION"))
		{
			userID = self->getUserIdFromUniqueNodeName(button, "INTERSECTION");
			self->worked = self->addIntersect(userID);
		}
        else if (beginequal(button->getName().getString(),"BOOLEAN_UNION"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "BOOLEAN_UNION");
            self->worked = self->addBool(BOOL_UNION, userID);
        }
        else if (beginequal(button->getName().getString(),"BOOLEAN_INTERSECTION"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "BOOLEAN_INTERSECTION");
            self->worked = self->addBool(BOOL_INTERSECTION, userID);
        }
        else if (beginequal(button->getName().getString(),"BOOLEAN_DIFFERENCE"))
		{
			userID = self->getUserIdFromUniqueNodeName(button, "BOOLEAN_DIFFERENCE");
			self->worked = self->addBool(BOOL_CUT, userID);
		}
		else if (beginequal(button->getName().getString(),"SLICE"))
		{
			userID = self->getUserIdFromUniqueNodeName(button, "SLICE");
			self->worked = self->addSlice(userID);
		}

        //Transform
        else if (beginequal(button->getName().getString(),"COPY_ON_OFF"))
        {
            // copy is independant of userID
            self->makeCopy = button->on.getValue();
            //printf("Makecopy is %i now \n",self->makeCopy);
        }
        else if (beginequal(button->getName().getString(),"TRANSLATE"))
        {    
            userID = self->getUserIdFromUniqueNodeName(button, "TRANSLATE");
            //self->userGrid[SoUserManagerKit::getInstance()->getUserKitIndex(userID)]->turnOn();
            self->worked = self->translate(userID, TRUE);
        }
        else if (beginequal(button->getName().getString(),"ROTATE"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "ROTATE");
            self->worked = self->rotate(userID, TRUE);
        }
        else if (beginequal(button->getName().getString(),"MIRROR"))
        {
            userID = self->getUserIdFromUniqueNodeName(button, "MIRROR");
            self->worked = self->mirror(userID, TRUE);
        }
		else if (beginequal(button->getName().getString(),"SWEEP"))
		{
			userID = self->getUserIdFromUniqueNodeName(button, "SWEEP");
			self->worked = self->addSweep(userID);
		}

        // used for setting fields with object count - needed for APRIL presentations
		self->setObjectCount();
  }
}

void SoCnDKit::pushButtonCB(void* userData, SoButtonKit* button)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  printf("SoCnDKit::pushButtonCB, button %s pressed!\n", button->getName().getString());
  findButtonMethod(userData, button);
  
  // this callback is not called in case of a preview, that's good
  // check if switch flag is FALSE and if pointMode is still on
  //  if ((self->switchPointModeOff[userID] == FALSE) && (drawPoints[userID] == 0))
  
  // if buttonNummer < constructButtons  dann pointbutton fuer richtigen user ausschalten und 
  // ein switchflag (fuer richtigen user) setzen
  
}

void SoCnDKit::horizontalProjectionPCB(void* userData, SoButtonKit* button)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  printf("SoCnDKit::horizontalPorjectionPCB, button %s pressed!\n", button->getName().getString());
  
  SoSwitch* sw = (SoSwitch*)SoNode::getByName("horizontalProjectionSwitch");
  //SoSwitch* sw = (SoSwitch*)self->getAnyPart("horProSwi", false);
  sw->whichChild.setValue(SO_SWITCH_ALL);
  
  self->selectionRootCB(userData, NULL);
}

void SoCnDKit::horizontalProjectionRCB(void* userData, SoButtonKit* button)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  printf("SoCnDKit::horizontalPorjectionRCB, button %s pressed!\n", button->getName().getString());
  
  SoSwitch* sw = (SoSwitch*)SoNode::getByName("horizontalProjectionSwitch");
  //SoSwitch* sw = (SoSwitch*)self->getAnyPart("horProSwi", false);
  sw->whichChild.setValue(SO_SWITCH_NONE);
}

void SoCnDKit::verticalProjectionPCB(void* userData, SoButtonKit* button)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  printf("SoCnDKit::verticalPorjectionPCB, button %s pressed!\n", button->getName().getString());
  
  SoSwitch* sw = (SoSwitch*)SoNode::getByName("verticalProjectionSwitch");
  //SoSwitch* sw = (SoSwitch*)self->getAnyPart("horProSwi", false);
  sw->whichChild.setValue(SO_SWITCH_ALL);
  
  self->selectionRootCB(userData, NULL);
}

void SoCnDKit::verticalProjectionRCB(void* userData, SoButtonKit* button)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  printf("SoCnDKit::verticalPorjectionRCB, button %s pressed!\n", button->getName().getString());
  
  SoSwitch* sw = (SoSwitch*)SoNode::getByName("verticalProjectionSwitch");
  //SoSwitch* sw = (SoSwitch*)self->getAnyPart("horProSwi", false);
  sw->whichChild.setValue(SO_SWITCH_NONE);
}

void SoCnDKit::leftProjectionPCB(void* userData, SoButtonKit* button)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  printf("SoCnDKit::leftPorjectionPCB, button %s pressed!\n", button->getName().getString());
  
  SoSwitch* sw = (SoSwitch*)SoNode::getByName("leftProjectionSwitch");
  //SoSwitch* sw = (SoSwitch*)self->getAnyPart("horProSwi", false);
  sw->whichChild.setValue(SO_SWITCH_ALL);
  
  self->selectionRootCB(userData, NULL);
}

void SoCnDKit::leftProjectionRCB(void* userData, SoButtonKit* button)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  printf("SoCnDKit::leftPorjectionRCB, button %s pressed!\n", button->getName().getString());
  
  SoSwitch* sw = (SoSwitch*)SoNode::getByName("leftProjectionSwitch");
  //SoSwitch* sw = (SoSwitch*)self->getAnyPart("horProSwi", false);
  sw->whichChild.setValue(SO_SWITCH_NONE);
}

void SoCnDKit::rightProjectionPCB(void* userData, SoButtonKit* button)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  printf("SoCnDKit::rightPorjectionPCB, button %s pressed!\n", button->getName().getString());
  
  SoSwitch* sw = (SoSwitch*)SoNode::getByName("rightProjectionSwitch");
  //SoSwitch* sw = (SoSwitch*)self->getAnyPart("horProSwi", false);
  sw->whichChild.setValue(SO_SWITCH_ALL);
  
  self->selectionRootCB(userData, NULL);
}

void SoCnDKit::rightProjectionRCB(void* userData, SoButtonKit* button)
{
  SoCnDKit* self = (SoCnDKit*)userData;
  printf("SoCnDKit::rightPorjectionRCB, button %s pressed!\n", button->getName().getString());
  
  SoSwitch* sw = (SoSwitch*)SoNode::getByName("rightProjectionSwitch");
  //SoSwitch* sw = (SoSwitch*)self->getAnyPart("horProSwi", false);
  sw->whichChild.setValue(SO_SWITCH_NONE);
}

// is processed before handle3dEvent
SbBool 
SoCnDKit::isInterested(SoHandle3DEventAction *h3a)
{
	
	if(firstStart.getValue())	
	{
		// finished setting up everything
		// loading StartUp file
		SoInput myinput;
		if (myinput.openFile("loadOnStart.iv")) 
		{
			// Read the whole file into the database
			SoSeparator *myGraph = SoDB::readAll(&myinput);
			if (myGraph == NULL) 
				fprintf(stderr, "Problem reading file\n");
			myinput.closeFile();
			
			//redo all command in the undoredoList
			if (myGraph != NULL)
			{
				loadFlag = true;
				printf("Loading Construct3D startup file loadOnStart.iv \n");
				redoAll();
				loadFlag = false;
			}
		} 
		firstStart.setValue(FALSE);
	}

  So3DEvent* event = (So3DEvent*)h3a->getEvent();
  if(event->getStation() == 6 && event->getButton(0) == TRUE)
  {
    int uid;
    TrackerOT *tracker = (TrackerOT*)Stb3DEventGenerator::getTracker();
    Context *context = tracker->getContext();
    SpeechModule *SRModule = (SpeechModule*)context->getModule("SpeechRecoConfig");
    assert(SRModule != NULL);
    
    unsigned long CommandId = (DWORD)event->getTranslation()[0];
    unsigned long SpeechSetId = (DWORD)event->getTranslation()[1];
    
    // retrieve the first local user id in the system
    for(int i = 0; i < SoUserManagerKit::getInstance()->getUserNumber(); ++i)
    {
      if(SoUserManagerKit::getInstance()->getUserKitSequentially(i)->local.getValue() == TRUE)
      {
        uid = SoUserManagerKit::getInstance()->getUserKitSequentially(i)->getUserID();
        break;
      }
    }
    
    if(uid != -1)
    {
      // Command id for 1st sheet is 1000, for 2nd 1001 ... so only sub 1000
      // from command id and you get the sheet id.
      unsigned long PipSheet = CommandId - 1000;
      // not necessary if using unsigned long:
      //if(PipSheet >= 0 && PipSheet < 5)
      if(PipSheet < 5)
        ((SoPipSheetKit*)getPipSheet(uid))->switchPipSheet(PipSheet);
      
      // do select command
      std::string Command;
      if(SRModule->GetCommand(CommandId, SpeechSetId, Command))
      {
        if(Command.compare("select") == 0)
        {
          //SbVec3f p = Stb3DEventGenerator::getTranslationField(SoUserManagerKit::getInstance()->getPenFromUser(uid))->getValue();
          if (!selectedCalled)
          {
            selectedCalled = TRUE;
            return FALSE;
          }
          
          SbVec3f eventPos = Stb3DEventGenerator::getTranslationField(SoUserManagerKit::getInstance()->getPenFromUser(uid))->getValue();
          // Pen and PIP are positioned at local origin
          // to get correct Pen position in window, we must calculate 
          // window position and orientation
          SoWindowKit *win = this->getWindow(0);
          winpos = win->position.getValue();
          winrotinverse = win->orientation.getValue().inverse();
          winrotinverse.multVec(eventPos - winpos, eventPos);
          
          selectNearestObject(eventPos);
          //printf("*********** position: %f %f %f \n", eventPos[0], eventPos[1], eventPos[2]);
          selectedCalled = FALSE;
          return FALSE;
        }
		if(Command.compare("set") == 0)
        {
          printf("push button called.\n");
          pushButtonCalled = TRUE;
          return FALSE;
        }
      }
    }
  }
  
  return TRUE;
}

// is called if isInterested is TRUE
void 
SoCnDKit::handle3DEvent(SoHandle3DEventAction* h3a)
{
  So3DEvent* event = (So3DEvent*)h3a->getEvent();
  const SoPath *rootPath = h3a->getPathFromRoot();
  SoUserManagerKit *uk = SoUserManagerKit::getInstance();
  int uid = uk->getUserFromStation(event->getStation());
  
  SbVec3f eventPos;
  // uid = -1 results from events generated by markers or speech events
  // They do not interest me here.
  if (uid != -1)
  {
    // Catch all button click events that happen inside a bounding box around the PIP  
    // I eliminate these events in order to avoid that users set Points or  
    // select/deselect objects if they miss a button on the PIP  
	if (event->getType() == So3DEvent::ET_BUTTON0_EVENT || pushButtonCalled)  
    {  
		int i;
		for (i = 0; i < numOfPips.getValue()+1 ; i++)
		{
			SoPipKit* userPip = uk->getUserKit(i)->getPip();
			if (userPip == NULL)
				printf("No PIP found for user %i",i);
			SbVec3f pipsize = userPip->pipSize.getValue();  
			SbVec3f pipPosition = userPip->getMover()->translation.getValue();  
			SbRotation pipRotation = userPip->getMover()->rotation.getValue();  
			//    printf("PIPposition %f %f %f \n",pipPosition[0],pipPosition[1],pipPosition[2]);  
			//Current PIP Size: 0.300000 0.200000 0.010000  
			//pipsize delivers coordinates where y and z are mixed  
			SbXfBox3f pipBoundingBox = 
				SbXfBox3f(SbVec3f(float(-pipsize[0]/2 - 0.05), 
								  float(-pipsize[2]/2 - 0.02), 
								  float(-pipsize[1]/2 - 0.02)),  
						SbVec3f(float(pipsize[0]/2 + 0.05), 
										float(pipsize[2]/2 + 0.06), 
										float(pipsize[1]/2 + 0.02))); //SbVec3f(0.2, 0.05, 0.15)  
			SbMatrix trans;  
			// set Rotation and Translation at once (Matrix = T*R*S), otherwise wrong Transformation Matrix is calculated  
			trans.setTransform(pipPosition, pipRotation, SbVec3f(1,1,1));  
			pipBoundingBox.transform(trans);  
			SbBool intersect = pipBoundingBox.intersect(event->getTranslation());  
			// if inside extended PIP bounding Box  
			if (intersect)  
			{  
				printf("Pen inside extended PIP geometry \n");  
				h3a->setHandled();
				pushButtonCalled = FALSE;
				return;
			}  
		}
	}
	
    // We check with the help of this variable if a button has been clicked and
    // if the user moved the pen between click and release. If there was no move
    // than selection of points also works while being inside the dragGeometry
    // of the points. Point setting (inside dragGeometry) also works if there 
    // was no or minimal move between click and release
    if(buttonPressed == TRUE) // DRAGGER RELEASE
    {
      // printf("Button released, Event nach Buttonpress\n");
      // only if button was released and NOT moved between press and release action, 
      // keep buttonPress TRUE, otherwise - if movement too much, 
      // set buttonPressed to FALSE
      // printf("DISTANCE: %f \n",(event->getTranslation() - buttonPressPosition).length());
      if ((event->getTranslation() - buttonPressPosition).length() > dragMoveTolerance) // moved not more than 5mm
        buttonPressed = FALSE;
    }
    
    // get the transformation from the root 
    SbViewportRegion vp;
    SoGetMatrixAction ma(vp);
    rootPath->ref();
    ma.apply((SoFullPath*)rootPath);
    rootPath->unrefNoDelete();
    SbMatrix globalToLocal = ma.getInverse();
    
    // transform the rotation part of the 3D event
    SbMatrix rotMatrix;
    event->getRotation().getValue(rotMatrix);
    SbMatrix resMatrix;
    resMatrix = rotMatrix * globalToLocal;
    //    SbVec3f t,s; SbRotation rot, so;
    //    resMatrix.getTransform( t, rot, s, so );
    
    // transfrom the position part of the 3D event
    // if the button was released and buttonPressed is still TRUE, then
    // calculate the event position of the previous press event 
    // (saved in buttonPressPosition)
    if (buttonPressed == TRUE)
      globalToLocal.multVecMatrix( buttonPressPosition, eventPos);
    else 
      globalToLocal.multVecMatrix( event->getTranslation(), eventPos);
    
    SoWindowKit *win = this->getWindow(0);
    winpos = win->position.getValue();
    winrotinverse = win->orientation.getValue().inverse();
    // get PEN position in window
    // crashes system sometimes if out of this if{}
    winrotinverse.multVec(event->getTranslation()-winpos, eventPos);
    

    // PROBLEM WITH DISTRIBUTION!!!
    // The order of users is not preserved when starting the application distributed
    // The slave would return a userIndex of 1 for user 0 and a userIndex of 0 for user 1
    // for the master it is the other way round
    // Therefore we are using the pure uid 
    // This means that in the userKit, the userID must be specified sequentially 0,1,2,....
    //int userIndex = uk->getUserKitIndex(uid);
    
    // Do this only in Master mode (color state will be transmitted anyway)
    if ((drawPoints[uid] == 0) && isMaster) // Point mode turned off - selection ON
      highlightNearestObject(eventPos, uid);
    
    // SoDragKit handles all Pen Input. If a Button is pressed and the Pen
    // is near a DragKit Object, the DragKit does NOT return the 
    // Button released state. It handles the event and "swallows" the
    // Button released event.
    if ((event->getType() == So3DEvent::ET_BUTTON0_EVENT) &&
      (event->getButton(So3DEvent::BUTTON0) == TRUE) || buttonPressed || pushButtonCalled)
    {
	  pushButtonCalled = FALSE;
      // setting MasterMode_Event state must be done here before any events are handled internally

#ifdef USE_DIV
		SoHandle3DEventAction modeAction;
		So3DEvent modeEvent;
		if(isMaster)
			modeEvent.setType(So3DEvent::ET_MASTERMODE_EVENT);
		else
			modeEvent.setType(So3DEvent::ET_SLAVEMODE_EVENT);
		modeAction.setEvent(&modeEvent);
		modeAction.apply(this);
#endif

      
      // If Master state is not checked here, the slave adds Points himself. 
     // This would cause points to be double in the scenegraph of the slave!!! 
     if (isMaster) 
     { 

        if(!buttonPressed) 
        {
		  pushButtonCalled = FALSE;
          buttonPressed=TRUE;
          buttonPressPosition = event->getTranslation();
          //printf("Button Pressed, next event will be after dragging or point setting\n");
          //return TRUE; 
          h3a->setHandled();
          return;
        }
        else
          buttonPressed = FALSE;

        if (drawPoints[uid] == 1) // Point mode turned on
        {
          //return addPoint(eventPos, userID.getValue());
          //return addPoint(eventPos, userIndex);
          addPoint(eventPos, false, true, uid);
          h3a->setHandled();
          return;
        }
        else // select point and allow dragging
        {
          selectNearestObject(eventPos);
          //return TRUE;
          h3a->setHandled();
          return;
          // do not set event to handled, need event for point dragging
        }
      }
      else // set event handled for slave 
        h3a->setHandled(); 

    }
  }
}

int SoCnDKit::round(float f)
{
  if ((f - ((int)f)) > 0.5)
    return ((int)f + 1);
  else 
    return (int)f; 
}


/* ===========================================================================
End of contextcnd2.cxx
===========================================================================
Automatic Emacs configuration follows.
Local Variables:
mode:c++
c-basic-offset: 4
eval: (c-set-offset 'substatement-open 0)
eval: (c-set-offset 'case-label '+)
eval: (c-set-offset 'statement 'c-lineup-runin-statements)
eval: (setq indent-tabs-mode nil)
End:
=========================================================================== */
