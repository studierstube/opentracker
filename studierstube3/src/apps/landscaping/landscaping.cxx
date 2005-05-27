// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       landscaping
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Implementation of main program and positioning tools
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:17:53  gh      last modification
//  17-sep-98 21:47:52  ds      created
// ===========================================================================


// landscaping is a demo application to show the capabilities of transparent
// props in the studierstube/vt framework. it allows creation and placement
// of artifacts (houses, trees etc.) for landscaping an island displayed on
// the table. the pip works like a palette from which various tools can be
// chosen and in particular new objects can be dragged and dropped into the
// scene.
// the landscaping base module (this file) covers the first three modes on 
// the pip: object creation, basic selection, scaling, coloring, deletion

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stbapi/workspace/StbCommunicator.h>
#include <stbapi/stbapi.h>

#include <Inventor/actions/SoAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoBoxHighlightRenderAction.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoSelection.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoTexture2.h>

#include <stbapi/seams/SoSeamKit.h>
#include <stbapi/interaction/SoDragSeparator.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/widgets/So3DLogSlider.h>
#include <stbapi/widgets/So3DCheckBox.h>
#include <stbapi/util/ivio.h>
#include <stbapi/misc/SoArtifactKit.h>
//#include <stbapi/misc/SoFishnet.h>

#include "piputil.h"
#include "fishtool.h"
#include "lasso.h"
#include "context.h"
#include "underground.h"
#include "snapshot.h"
#include <stbapi/misc/wstudierstube.h>

//-----------------------------------------------------------------------------
// globals and constants
//-----------------------------------------------------------------------------

// modes
const int MODE_BUILDING     = 0;
const int MODE_TREE         = 1;
const int MODE_COLORCHANGER = 2;
const int MODE_FISHING      = 3;
const int MODE_LASSO        = 4;
const int MODE_CONTEXT      = 5;
const int MODE_SNAPSHOT     = 6;

int programMode = MODE_BUILDING;
int extendSelectionMode = FALSE; 

SoSeparator* artifacts;
SoMaterial* colorChangerMat;
int pipSide; //0=front facing, 1=back facing

//-----------------------------------------------------------------------------
// callback functions
//-----------------------------------------------------------------------------

// select/deselect all artifacts
// selected: true to select all, false to deselect

void
setSelectionForAll(SbBool selected)
{
    for(int i=0; i<artifacts->getNumChildren(); i++)
        ((SoArtifactKit*)artifacts->getChild(i))->selected = selected;
}

//-----------------------------------------------------------------------------
// callback if an artifact has been pressed
// usually select it, or change color when in colormode
// data: pointer to artifact
// dragSep: drag separator of the artifact that triggered callback

void
cbArtifactPressed(void* data, SoDragSeparator* dragSep)
{
    SoArtifactKit* artifact = (SoArtifactKit*)data; //get artifact
    if(programMode == MODE_COLORCHANGER) //copy color from colorChangerMat
    {
        artifact->color = colorChangerMat->diffuseColor[0].getValue();
    }
    else //select
    {
        if(!extendSelectionMode) //select artifact, deselect all others
        {
            setSelectionForAll(FALSE);
            artifact->dragable = TRUE;
            artifact->selected = TRUE;
        }
        else //toggle selection for artifact
        {
            artifact->selected = !artifact->selected.getValue();
        }
    }
}

//-----------------------------------------------------------------------------
// callback of artifact is released
// data: pointer to artifact
// dragSep: drag separator that triggered callback
// make object not dragable (may be changed in future)

void
cbArtifactReleased(void* data, SoDragSeparator* dragSep)
{
    SoArtifactKit* artifact = (SoArtifactKit*)data;
    artifact->dragable = FALSE;
}

//-----------------------------------------------------------------------------
// create new artifact
// detector: drag separator from the building or tree pip sheet
// calls back when the artifact offered on the pip is picked
// a new artifact is created and added to the artifacts separator
// drag control is transfered to the new object, which is little bit of a 
// hack using the startDragging and stopDragging methods.

void
cbNewArtifact(void*, SoDragSeparator* detector)
{
    if(extendSelectionMode) return; //do not make new object if selecting

    // find out which model is currently on the pip
    GETCHILD(sw,SoSwitch,detector,0);
    GETCHILD(dragModel,SoSeparator,sw,sw->whichChild.getValue());

    // take dragging away from the pip drag separator
    detector->stopDragging();
    // create new artifact and set it up with the picked geometry
    NEWNODE(artifact,SoArtifactKit);

    artifact->dragButton = 0;

    artifact->setPressCallback(cbArtifactPressed,(void*)artifact);
    artifact->setReleaseCallback(cbArtifactReleased,(void*)artifact);
    artifact->startDragging(); //give dragging mode to new artifact
    artifact->setPart("geometry",dragModel);
    // compute the matrix of current position of the artifact on the pip
    SbMatrix matrix;
    assert(getPipMatrix(dragModel,matrix));
    artifact->setReferenceTransform(matrix);
    artifacts->addChild(artifact); //add to artifacts
    artifact->selected = TRUE; //artifact is selected
}

//-----------------------------------------------------------------------------
// callback for switching to building mode

void
cbTabBuilding(void*, So3DButton*)
{
    programMode = MODE_BUILDING;
    setFishingMode(FALSE);
    setCurrentSeamVisibility(FALSE);
}

//-----------------------------------------------------------------------------
// callback for switching to tree mode

void
cbTabTree(void*, So3DButton*)
{
    programMode = MODE_TREE;
    setFishingMode(FALSE);
    setCurrentSeamVisibility(FALSE);
}

//-----------------------------------------------------------------------------
// callback for switching to color changer mode

void
cbTabColorchanger(void*, So3DButton*)
{
    programMode = MODE_COLORCHANGER;
    setFishingMode(FALSE);
    setCurrentSeamVisibility(FALSE);
}

//-----------------------------------------------------------------------------
// callback for switching to fishnet selection mode

void
cbTabFishing(void*, So3DButton*)
{
    programMode = MODE_FISHING;
    resetFishing();
    setFishingMode(TRUE);
    setCurrentSeamVisibility(FALSE);
}

//-----------------------------------------------------------------------------
// callback for switching to lasso selection mode

void
cbTabLasso(void*, So3DButton*)
{
    programMode = MODE_LASSO;
    resetLasso();
    setFishingMode(FALSE);
    setCurrentSeamVisibility(FALSE);
}

//-----------------------------------------------------------------------------
// callback for switching to context sensitive display mode

void
cbTabContext(void*, So3DButton*)
{
    programMode = MODE_CONTEXT;
    setFishingMode(FALSE);
    setCurrentSeamVisibility(FALSE);
}

//-----------------------------------------------------------------------------
// callback for switching to snapshot mode

void
cbTabSnapshot(void*, So3DButton*)
{
    programMode = MODE_SNAPSHOT;
    setFishingMode(FALSE);
    setCurrentSeamVisibility(TRUE);
}

//-----------------------------------------------------------------------------
// callback for toggling normal selection/extend selection
// button: toggle button

void
cbToggleExtendSelection(void*, So3DCheckBox* button)
{
    extendSelectionMode = button->pressed.getValue();
}

//-----------------------------------------------------------------------------
// copy current color to all selected artifacts

void
cbCopyColor(void*, So3DButton*)
{
    for(int i=0; i<artifacts->getNumChildren(); i++)
    {
        SoArtifactKit* artifact = (SoArtifactKit*)artifacts->getChild(i);
        if(artifact->selected.getValue()) artifact->color =
            colorChangerMat->diffuseColor[0].getValue();
    }
}

//-----------------------------------------------------------------------------
// delete all selected artifacts

void
cbDeleteSelectedArtifacts(void*, So3DButton*)
{
    int max = artifacts->getNumChildren();

    for(int i=max-1; i>=0; i--)
    {
        SoArtifactKit* artifact = (SoArtifactKit*)artifacts->getChild(i);
        if(artifact->selected.getValue()) 
        {
            artifacts->removeChild(i);
        }
    }
}

//-----------------------------------------------------------------------------
// unselect all artifacts

void
cbUnselectArtifacts(void*, So3DButton*)
{
    setSelectionForAll(FALSE);
}

//-----------------------------------------------------------------------------
// select all artifacts

void
cbSelectAllArtifacts(void*, So3DButton*)
{
    setSelectionForAll(TRUE);
}

//-----------------------------------------------------------------------------
// save scene

void
cbSaveScene(void*, So3DButton*)
{
    writeFile("artifacts.iv",artifacts);
}

//-----------------------------------------------------------------------------
// load scene

void
cbLoadScene(void*, So3DButton*)
{
    artifacts = readFile("artifacts.iv");
    AGETNODE(gsep,SoSeparator,"GROUND");
    gsep->replaceChild(0,artifacts);
    fishnetNode->artifacts = artifacts;
}

//-----------------------------------------------------------------------------
// scale slider callback - scale all selected artifacts
// slider: pointer to scale slider

void
cbScale(void*, So3DLogSlider* slider)
{
    float value = slider->currentValue.getValue();
    for(int i=0; i<artifacts->getNumChildren(); i++)
    {
        SoArtifactKit* artifact = (SoArtifactKit*)artifacts->getChild(i);
        if(artifact->selected.getValue()) 
        {
            SoScale* scale = (SoScale*)artifact->getPart("scale",TRUE);
            scale->scaleFactor = SbVec3f(value,value,value);
        }
    }
}

//-----------------------------------------------------------------------------
//compute the pip side - is the user looking at the front side (=0) of the pip
//or the back side (=1)? we assume that the normal of the pip is pointing in
//the direction where the user's eye is, i.e. if the angle between pip surface
//normal N and vector pip-center to eye (called E) is smaller than 90 degree, 
//we are seeing the front. this boils down to a dot product N*E < 0
//this can further be simplified: get the eye E in pip space, i.e. origin =
//pip center and normal is aligned with the y-axis. then the sign of the
//y-coordinate of E determines the pip side.
// returns: 0 for front side, 1 for back side

int
computePipSide()
{
return 0;
/*    SbMatrix matrix;
    SbVec3f eyeW,eyeP,isectP; //W=world,P=PIP
    assert(getMatrix(pipCoord,matrix));
    eyeW = WStudierstube::getMiddleEyePos();
    matrix.inverse().multVecMatrix(eyeW,eyeP);
    return eyeP[1]>0;
*/
}

//-----------------------------------------------------------------------------
// callback to be executes for every frame
// action: action that issued callback - only gl render action is considered

void
cbEveryFrame(void*, SoAction* action)
{
    if(action->getTypeId() != SoGLRenderAction::getClassTypeId()) return;

    if(programMode == MODE_CONTEXT) doContext(); //do context for every frame

    pipSide = computePipSide(); //do underground if pip backside visible
}

//-----------------------------------------------------------------------------
// setup function
//-----------------------------------------------------------------------------

// wire all callback to their buttons or other dialog elements

void
wireScene(SoSeparator* sceneRoot)
{
    AGETNODE(buildingDragSep,SoDragSeparator,"BUILDING_DETECTOR");
    buildingDragSep->setPressCallback(cbNewArtifact);

    AGETNODE(treeDragSep,SoDragSeparator,"TREE_DETECTOR");
    treeDragSep->setPressCallback(cbNewArtifact);

    AVGETNODE(colorChangerMat,SoMaterial,"SELECTED_COLOR");

    AVGETNODE(artifacts,SoSeparator,"ARTIFACTS");

    AGETNODE(extendSelectionButton,So3DCheckBox,"EXTEND_SELECTION_BUTTON");
    extendSelectionButton->addPressCallback(cbToggleExtendSelection,NULL);

    AGETNODE(copyColorButton,So3DButton,"COPY_COLOR_BUTTON");
    copyColorButton->addPressCallback(cbCopyColor);

    AGETNODE(deleteButton,So3DButton,"DELETE_BUTTON");
    deleteButton->addPressCallback(cbDeleteSelectedArtifacts);

    AGETNODE(unselectButton,So3DButton,"UNSELECT_BUTTON");
    unselectButton->addPressCallback(cbUnselectArtifacts);

    AGETNODE(selectAllButton,So3DButton,"SELECT_ALL_BUTTON");
    selectAllButton->addPressCallback(cbSelectAllArtifacts);

    AGETNODE(xRaySeam,SoSeamKit,"XRAY_SEAM");
    xRaySeam->setBGColor(SbColor(0,0.2,0.2));

    AGETNODE(loadSceneButton,So3DButton,"LOAD_SCENE_BUTTON");
    loadSceneButton->addPressCallback(cbLoadScene);

    AGETNODE(saveSceneButton,So3DButton,"SAVE_SCENE_BUTTON");
    saveSceneButton->addPressCallback(cbSaveScene);

    AGETNODE(scaleSlider,So3DLogSlider,"SCALE_SLIDER");
    scaleSlider->addPositionChangeCallback(cbScale,NULL);
    AVGETNODE(scaleSlider,So3DLogSlider,"SCALE_SLIDER_TREE");
    scaleSlider->addPositionChangeCallback(cbScale,NULL);

    // callback for every frame deals with fishing and underground stuff
    SoCallback* cbEveryFrameNode = new SoCallback;
    cbEveryFrameNode->setCallback(cbEveryFrame);
    sceneRoot->addChild(cbEveryFrameNode);
}

//-----------------------------------------------------------------------------
// for some odd reason, loading the terrain texture together with the
// landscape.iv file does not work -- some Inventor bug or side effect?!
// [not used at the moment]

void 
addTerrainTextureHack()
{
    AGETNODE(tsep,SoSeparator,"TERRAIN");
    NEWNODE(tx,SoTexture2);
    tsep->insertChild(tx,0);
    tx->filename = "data/noise.rgb";
    AVGETNODE(tsep,SoSeparator,"WATER");
    tx = new SoTexture2;
    tsep->insertChild(tx,0);
    tx->filename = "data/water.rgb";
}

//-----------------------------------------------------------------------------
// decorate all buttons with textures

#define ASSIGNTEXT(buttonName,textureName) \
                       AVGETNODE(button,SoBase3DButton,buttonName); \
                       AVGETNODE(texture,SoTexture2,textureName); \
                       button->setupTexture(texture,0,0)

void
loadButtonTextures()
{
    SoSeparator* sep = readFile("data/buttontextures.iv");
    SoBase3DButton* button;
    SoTexture2* texture;
    ASSIGNTEXT("B_TAB_BUTTON","BUILDINGS_TEXTURE");
    ASSIGNTEXT("T_TAB_BUTTON","TREES_TEXTURE");
    ASSIGNTEXT("C_TAB_BUTTON","COLORIZE_TEXTURE");
    ASSIGNTEXT("F_TAB_BUTTON","FISH_TEXTURE");
    ASSIGNTEXT("L_TAB_BUTTON","LASSO_TEXTURE");
    ASSIGNTEXT("O_TAB_BUTTON","CONTEXT_TEXTURE");
    ASSIGNTEXT("S_TAB_BUTTON","SEAMFILE_TEXTURE");
    ASSIGNTEXT("ARRANGE_BUTTON","ARRANGE_TEXTURE");
    ASSIGNTEXT("SEAM_LOCK_BUTTON","CAMERA_TEXTURE");
    ASSIGNTEXT("COPY_COLOR_BUTTON","COLORIZE_TEXTURE");
    ASSIGNTEXT("DELETE_BUTTON","DELETE_TEXTURE");
    ASSIGNTEXT("UNSELECT_BUTTON","DESELECTALL_TEXTURE");
    ASSIGNTEXT("EXIT_BUTTON","EXIT_TEXTURE");
    ASSIGNTEXT("EXTEND_SELECTION_BUTTON","EXTEND_TEXTURE");
    ASSIGNTEXT("LOAD_SCENE_BUTTON","LOAD_TEXTURE");
    ASSIGNTEXT("SEAM_MAX_BUTTON","MAXIMIZE_TEXTURE");
    ASSIGNTEXT("SAVE_SCENE_BUTTON","SAVE_TEXTURE");
    ASSIGNTEXT("SELECT_ALL_BUTTON","SELECTALL_TEXTURE");
    ASSIGNTEXT("SEAM_STORE_BUTTON","SAVE_TEXTURE");
    ASSIGNTEXT("SEAM_PICKUP_BUTTON","LOAD_TEXTURE");
}

//-----------------------------------------------------------------------
// Initialization routine of workspace application
//-----------------------------------------------------------------------

STBAPI_API void 
Stb_init(int userID, StbCommunicator* cMan)
{
printf("init\n");

    WStudierstube::init(userID,cMan);
//    SoSeparator* sceneRoot = WStudierstube::readSceneFile("landscape.iv");
    SoSeparator* pipRoot = WStudierstube::createPip();
    // setup pip sheets
printf("sheets\n");
    WStudierstube::addPipSheet("buildingsheet.iv","B",TRUE,cbTabBuilding);
    WStudierstube::addPipSheet("treesheet.iv","T",FALSE,cbTabTree);
    WStudierstube::addPipSheet("colorsheet.iv","C",FALSE,cbTabColorchanger);
    WStudierstube::addPipSheet("fishsheet.iv","F",FALSE,cbTabFishing);
    WStudierstube::addPipSheet("lassosheet.iv","L",FALSE,cbTabLasso);
    WStudierstube::addPipSheet("contextsheet.iv","O",FALSE,cbTabContext);
    WStudierstube::addPipSheet("snapshotsheet.iv","S",FALSE,cbTabSnapshot);
    WStudierstube::addPipCommon("common.iv");
    // create viewer
    // init all modules

printf("set up geom & pip\n");

/*    wireScene(sceneRoot);
    setupFishing(sceneRoot,pipRoot);
    setupLasso();
    setupContext();
    goUnderground();
    setupSnapshot();
    loadButtonTextures();
*/
}

//-----------------------------------------------------------------------
// Cleanup routine of workspace application
//-----------------------------------------------------------------------

STBAPI_API SbBool 
Stb_exit(void*)
{
    return TRUE;
}
//-----------------------------------------------------------------------------
//eof

