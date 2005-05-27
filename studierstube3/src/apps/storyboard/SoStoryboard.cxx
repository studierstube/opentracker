// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoStoryboard.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Demo application for workspace - multi user app
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#include <assert.h>
#include <math.h>
#include <Inventor/nodes/SoPointLight.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/engines/SoTransformVec3f.h>
#include <Inventor/engines/SoCalculator.h>
#include <Inventor/engines/SoCompose.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/util/ivio.h>
#include <stbapi/misc/ivmacros.h>
#include <stbapi/misc/SoMultMatrix.h>
#include <stbapi/misc/OnScreenRenderer.h>
#include <stbapi/interaction/SoDragSeparator.h>
#include <stbapi/workspace/SoContextKitSub.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/window3d/SoBorderKit.h>
#include <stbapi/widgets/So3DCheckBox.h>
#include <stbapi/widgets/So3DRadioButton.h>
#include <stbapi/widgets/So3DIntSlider.h>
#include <stbapi/interaction/SoDragKit.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include "SoStoryboard.h"
#include "SoSorterKit.h"
#include "storyboard.h"

SO_KIT_SOURCE(SoStoryboard);

CONTEXT_APPLICATION(SoStoryboard);

SoWindowKit* SoStoryboard::staticWindow = NULL;
int          SoStoryboard::instanceNum  = 0;
SoSeparator* SoStoryboard::sceneSource  = NULL;
SoSorterKit* SoStoryboard::sorter       = NULL;
//SoSorterKit* SoStoryboard::sorterBig    = NULL;
SoTexture2*  SoStoryboard::silverScreenTexture = NULL;

//SoSeparator* SoStoryboard::bigScene     = NULL;
//SoPointLight* SoStoryboard::bigSceneLight = NULL;
//SoPerspectiveCamera* SoStoryboard::bigSceneCam = NULL;

//----------------------------------------------------------------------------

void
SoStoryboard::initClass(void)
{
    SO_KIT_INIT_CLASS(SoStoryboard, SoContextKit, "SoContextKit");
    SoSorterKit::initClass();
}

//----------------------------------------------------------------------------

SoContextKit*
SoStoryboard::factory()
{
    return new SoStoryboard();
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoStoryboard::SoStoryboard()
:
    figSep(NULL),
    camSep(NULL),
    sceneSwitch(NULL),
	bigScene(NULL),
	bigSceneCam(NULL),
	bigSceneLight(NULL),
	cam1(NULL),
    player0Switch(NULL),
    player1Switch(NULL),
    player2Switch(NULL),
    player3Switch(NULL)
{
    SO_KIT_CONSTRUCTOR(SoStoryboard);
    SO_KIT_ADD_FIELD(thisInstanceNum, (0));
    SO_KIT_INIT_INSTANCE();
    setSearchingChildren(TRUE);
//printf("new storyboard\n");
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoStoryboard::~SoStoryboard()
{
    if(bigScene != NULL) bigScene->unref();
    destructor();
}

//----------------------------------------------------------------------------
// createWindowButtonCB: callback to create a new window
//----------------------------------------------------------------------------

void
SoStoryboard::createWindowButtonCB(void* data, So3DButton* button)
{
    SoStoryboard* self = (SoStoryboard*)data;
    int uid = self->getUserFromSheet(button);
    ContextManager::getInstance()->newContext(
        self->getAppName(),self->comm->workingDirectory,uid,TRUE,"");
}

//----------------------------------------------------------------------------
// deleteWindowButtonCB: callback to delete new window
//----------------------------------------------------------------------------

void
SoStoryboard::deleteWindowButtonCB(void* data, So3DButton* button)
{
    SoStoryboard* self = (SoStoryboard*)data;
    self->sorter->remove(self->getIconId());
    //self->sorterBig->remove(self->getIconId());
    self->scheduleDelete();
}

//----------------------------------------------------------------------------
// createWindowGeometry
// create window and attach scene graph to it. scene graph containts
// a sphere the color of which can be set by buttons on pip
//----------------------------------------------------------------------------

SoWindowKit*
SoStoryboard::createWindowGeometry(int index)
{
    if(index == 0) 
    {
        return createFirstWindow();
    }
    else if(index == 1) 
    {
        createCameraScene();
        createIcon();
        return createSecondWindow();
    }
    else return NULL; // indicate error
}

//----------------------------------------------------------------------------

SoWindowKit*
SoStoryboard::createFirstWindow()
{
    instanceNum++;
	thisInstanceNum.enableNotify(FALSE);
    thisInstanceNum.setValue(instanceNum);

    const float len = 0.25;
    const int block = 3;
    const float eps = 0.05;
    WindowCreate wc;
    SoWindowKit::defaultWindow(wc);
    wc.style &= ~(WS_CLIENTVOLUMEINVISIBLE|WS_RESIZEABLE);
    wc.size = SbVec3f(len,len,len);
    float ywin = (len+eps) * floor((thisInstanceNum.getValue()-1) / (block*block));
    float zwin = (len+eps) * floor((thisInstanceNum.getValue()-1) / block);
    float xwin = (len+eps) * ((thisInstanceNum.getValue()-1) % block);
	//ywin -= 0.7;
    wc.position = SbVec3f(xwin,ywin,zwin);
    SoWindowKit* windowKit = comm->createWindow(NULL, &wc, NULL, NULL);

    SoSeparator* clientVolume = windowKit->getClientVolumeSeparator();

    if(sceneSource == NULL)
        sceneSource = readFile("scene.iv",comm->workingDirectory);
    figSep = readFile("figures.iv",comm->workingDirectory);

       camSep = readFile("cameras.iv",comm->workingDirectory);
    clientVolume->addChild(figSep);
    clientVolume->addChild(camSep);
    
    player0Switch = (SoSwitch*)findNode(figSep,"PLAYER0_SWITCH");
	setUniqueNodeName(player0Switch,0);
    player1Switch = (SoSwitch*)findNode(figSep,"PLAYER1_SWITCH");
	setUniqueNodeName(player1Switch,0);
    player2Switch = (SoSwitch*)findNode(figSep,"PLAYER2_SWITCH");
	setUniqueNodeName(player2Switch,0);
    player3Switch = (SoSwitch*)findNode(figSep,"PLAYER3_SWITCH");
	setUniqueNodeName(player3Switch,0);

	setUniqueNodeName(findNode(camSep,"CAMERA1"),0);

    GETCHILD(staticSceneSwitch,SoSwitch,sceneSource,0);
    sceneSwitch = new SoSwitch;
    for(int i=0; i<staticSceneSwitch->getNumChildren(); i++)
        sceneSwitch->addChild(staticSceneSwitch->getChild(i));
    sceneSwitch->whichChild = 0;
	sceneSwitch->setName("SCENE_SWITCH");
	setUniqueNodeName(sceneSwitch,0);
    clientVolume->addChild(sceneSwitch);
    windowKit->setIconSize(SbVec3f(0.05,0.05,0.05));
	figSep->setName("FIG_SEP");
	setUniqueNodeName(figSep,0);
    return windowKit;
}

//----------------------------------------------------------------------------

void
SoStoryboard::updateTexture(void* userData, SoDragKit* drag)
{
    SoStoryboard* self = (SoStoryboard*)userData;
	if(self->divObject)
	    self->divObject->atomicActionBegin();
    // search for first instantiated viewer
    SoStudierstubeViewer* viewer = NULL;
    for(int i=0; i<SoUserKit::getUserNumber(); i++)
    {
        viewer = SoUserKit::getUserKitSequentially(i)->getDisplay()->getViewer();
        if(viewer != NULL) break; // found a viewer, exit loop
    }
    assert(viewer != NULL); // must have found a viewer by now

    // render "slide"
    SbViewportRegion vpr = viewer->getViewportRegion();

    const int size = 128;
    SbColor bg(1,1,1);
    SoTexture2* iconTexture = self->sorter->getTexture(self->getIconId());
//printf("update texture on context %d: ",self->thisInstanceNum.getValue());
//printNode(iconTexture);
    //SoTexture2* iconBig = self->sorterBig->getTexture(self->getIconId());
    assert(iconTexture != NULL);
    assert(silverScreenTexture != NULL);
    self->silverScreenTexture->image.connectFrom(&iconTexture->image);
    //iconBig->image.connectFrom(&iconTexture->image);

    self->bigSceneCam->position    = self->cam1->translation;
    self->bigSceneCam->orientation = self->cam1->rotation;
    self->bigSceneLight->location  = self->cam1->translation;

	fprintf(stderr,"before onscreenrenderscene touch\n");
    onScreenRenderScene(self->bigScene, vpr, iconTexture, size, size, bg);
	fprintf(stderr,"after onscreenrenderscene touch\n");

	fprintf(stderr,"before silverscreentexture touch\n");
    self->silverScreenTexture->touch();
	fprintf(stderr,"after silverscreentexture touch\n");
    //iconBig->touch();
	if(self->divObject)
	    self->divObject->atomicActionEnd();

}

//----------------------------------------------------------------------------

SoWindowKit*
SoStoryboard::createSecondWindow()
{
    if(staticWindow == NULL)
    {
        WindowCreate wc;
        SoWindowKit::defaultWindow(wc);
        wc.style &= ~(WS_MOVEABLE|WS_RESIZEABLE);
        wc.size = SbVec3f(1.1,1.1,0.2);
        wc.position = SbVec3f(0.0,0.0,-4.0);
        staticWindow = comm->createWindow(NULL, &wc, NULL, NULL);
        
        // add content
        NEWNODE(silverScreenGroup,SoGroup);
        NEWNODE(silverScreen, SoCube);
        NEWNODE(sorterScreenGroup,SoGroup);
        NEWNODE(viewSwitch,SoSwitch);
        NEWNODE(sorterXf,SoTransform);
        silverScreenTexture = new SoTexture2;
        silverScreen->width = 1.0;
        silverScreen->height = 1.0;
        silverScreen->depth = 0.01;
        silverScreenGroup->addChild(silverScreenTexture);
        silverScreenTexture->setName("SILVERSCREENTEXTURE");
        silverScreenGroup->addChild(silverScreen);
        sorterXf->scaleFactor = SbVec3f(0.04,0.04,0.04);
        sorterXf->rotation = SbRotation(SbVec3f(1,0,0),M_PI/2);
        sorterXf->translation = SbVec3f(-0.4,0.4,0);
        viewSwitch->whichChild = 0;
        silverScreenGroup->setName("SILVER_SCREEN_GROUP");
        viewSwitch->setName("VIEW_SWITCH");
        sorterScreenGroup->addChild(sorterXf);
        //sorterScreenGroup->addChild(sorterBig);
        sorterScreenGroup->addChild(sorter);
        viewSwitch->addChild(silverScreenGroup);
        viewSwitch->addChild(sorterScreenGroup);
        staticWindow->getClientVolumeSeparator()->addChild(viewSwitch);
        return staticWindow;
    }
    else return NULL;
}

//----------------------------------------------------------------------------

void
SoStoryboard::createCameraScene()
{
//--this stuff sets up separator for big scene - preliminary
    bigScene = new SoSeparator;
    bigScene->ref();
    bigSceneCam = new SoPerspectiveCamera;
    bigSceneCam->nearDistance = 0.001;
    bigSceneCam->farDistance = 100;
    bigSceneCam->heightAngle = M_PI/3.0; // 60 degrees vertical fisheye
    bigSceneCam->viewportMapping = SoCamera::LEAVE_ALONE;
    //NEWNODE(light,SoPointLight);
	bigSceneLight = new SoPointLight;
    bigScene->addChild(bigSceneCam);
    bigScene->addChild(bigSceneLight);
    bigScene->addChild(figSep);
    bigScene->addChild(sceneSwitch);
	bigScene->setName("BIG_SCENE");
	fprintf(stderr,"creating big scene for context %s\n", getName().getString());
	setUniqueNodeName(bigScene, 0);
	bigSceneCam->setName("BIG_SCENE_CAM");
	setUniqueNodeName(bigSceneCam, 0);
	bigSceneLight->setName("BIG_SCENE_LIGHT");
	setUniqueNodeName(bigSceneLight, 0);
}

//----------------------------------------------------------------------------

void
SoStoryboard::createIcon()
{
	if(divObject)
		fprintf(stderr,"div alloced!\n");
	else
		fprintf(stderr,"div NOT alloced!\n");

//--create entry in sorter
    if(sorter==NULL) 
    {
        sorter = new SoSorterKit();
		sorter->setMasterMode(isMaster);
        sorter->setName("SORTER_KIT");
        sorter->xspacing = 8.0;
        sorter->yspacing = 8.0;
    }
    /*if(sorterBig == NULL) 
    {
        sorterBig = new SoSorterKit;
        sorterBig->setName("BIG_SORTER_KIT");
        sorterBig->xspacing = 8.0;
        sorterBig->yspacing = 8.0;
        sorterBig->ref();
    }*/
 

    if(isMaster || thisInstanceNum.getValue() == 1)
    {
        SoTexture2* texture = new SoTexture2;
        SbString txName = "TX2_";
        txName += getIconId();
        texture->setName(txName);
		sorter->add(getIconId(),texture,-1);
        //sorterBig->add(getIconId(),new SoTexture2,-1);
    }
}

//----------------------------------------------------------------------------

SbString
SoStoryboard::getIconId()
{
    SbString id("TEXTURE_");
    id += SbString(thisInstanceNum.getValue());
    return id;
}

//----------------------------------------------------------------------------

void
SoStoryboard::checkBoxCB(void* userData, So3DCheckBox* button)
{
    ((SoSwitch*)userData)->whichChild = button->pressed.getValue();
}

//----------------------------------------------------------------------------

void
SoStoryboard::sliderCB(void* userData, So3DIntSlider* slider)
{
    ((SoSwitch*)userData)->whichChild = slider->currentValue.getValue();
}

//----------------------------------------------------------------------------
// Reads the pip sheet and sets the button callbacks
//----------------------------------------------------------------------------

SoNode*
SoStoryboard::createPipSheet(int uid)
{
    SoSeparator* newPipSheet = readFile("sheet.iv",comm->workingDirectory);
    newPipSheet->ref();

    SoGroup* sorterGroup = (SoGroup*)findNode(newPipSheet,"SORTER_GROUP");
    sorterGroup->setName("");
    if(thisInstanceNum.getValue() == 1) sorterGroup->addChild(sorter);

    SbString &radioName=makeUniqueNodeName("RADIO",uid);
    So3DRadioButton* radioButton = (So3DRadioButton*)findNode(newPipSheet,"SORTER_BUTTON");
	setUniqueNodeName(radioButton,uid);
    radioButton->separatorName = radioName;
    radioButton = (So3DRadioButton*)findNode(newPipSheet,"CTRL_BUTTON");
	setUniqueNodeName(radioButton,uid);
    radioButton->separatorName = radioName;

	setUniqueNodeName(findNode(newPipSheet,"CREATE_BUTTON"),uid);
	setUniqueNodeName(findNode(newPipSheet,"DELETE_BUTTON"),uid);
	setUniqueNodeName(findNode(newPipSheet,"FIGURE1_BUTTON"),uid);
	setUniqueNodeName(findNode(newPipSheet,"FIGURE2_BUTTON"),uid);
	setUniqueNodeName(findNode(newPipSheet,"FIGURE3_BUTTON"),uid);
	setUniqueNodeName(findNode(newPipSheet,"FIGURE4_BUTTON"),uid);
	setUniqueNodeName(findNode(newPipSheet,"SCENE_SLIDER"),uid);
	setUniqueNodeName(findNode(newPipSheet,"VIEW_BUTTON"),uid);

    newPipSheet->unrefNoDelete();
    return newPipSheet;
}

//----------------------------------------------------------------------------
// need to make sure the window 1 (shared by all contexts) is not deleted
// until the last context is deleted

void
SoStoryboard::closeWindow(int i)
{
    ContextManager* cm = ContextManager::getInstance();
    int left = cm->getContextNum(getAppName());
    if(i != 1 || left <= 1) // if not window 1 or nothing left
        SoContextKit::closeWindow(i); // do the normal thing

    //for some unknown reason, the last remaining window does not get
    //the focus in some cases. the following corrects this:
    if(left == 2 && i == 1) // only for last window
    {
        SoContextKit* ctx = cm->getAnyContext(getAppName());
        assert(ctx != NULL);
        comm->sendMessage(
            CONTEXTKIT_FOCUSCHANGE, comm->appID, comm->appID, ctx);
    }
}

//----------------------------------------------------------------------------

void
SoStoryboard::setMasterMode(SbBool masterMode)
{
    SoContextKit::setMasterMode(masterMode);
	if(cam1 == NULL)
        cam1 = (SoDragKit*)SoDragKit::getByName(makeUniqueNodeName("CAMERA1",0));
    if(sorter == NULL)
		sorter = (SoSorterKit*)SoNode::getByName("SORTER_KIT");
	sorter->setMasterMode(masterMode);
    if(masterMode)
    {
        cam1->setReleaseCallback(updateTexture, this);
    }
	else // slave mode
    {
        cam1->removeReleaseCallback(updateTexture, this);
	}
}
    
//----------------------------------------------------------------------------

void
SoStoryboard::setSheetMasterMode(SoNode* pipSheet, SbBool masterMode)
{
    int uid=getUserFromSheet(pipSheet);

    So3DButton* bCre =
		(So3DButton*)findNode(pipSheet, makeUniqueNodeName("CREATE_BUTTON",uid));
    So3DButton* bDel =
		(So3DButton*)findNode(pipSheet, makeUniqueNodeName("DELETE_BUTTON",uid));
    So3DCheckBox* player0check = 
		(So3DCheckBox*)findNode(pipSheet, makeUniqueNodeName("FIGURE1_BUTTON",uid));
    So3DCheckBox* player1check = 
		(So3DCheckBox*)findNode(pipSheet, makeUniqueNodeName("FIGURE2_BUTTON",uid));
    So3DCheckBox* player2check = 
		(So3DCheckBox*)findNode(pipSheet, makeUniqueNodeName("FIGURE3_BUTTON",uid));
    So3DCheckBox* player3check = 
		(So3DCheckBox*)findNode(pipSheet, makeUniqueNodeName("FIGURE4_BUTTON",uid));
    So3DIntSlider* slider = 
		(So3DIntSlider*)findNode(pipSheet, makeUniqueNodeName("SCENE_SLIDER",uid));
    So3DCheckBox* viewButton = 
		(So3DCheckBox*)findNode(pipSheet, makeUniqueNodeName("VIEW_BUTTON",uid));
    AGETNODE(viewSwitch,SoSwitch,"VIEW_SWITCH");

    sceneSwitch = (SoSwitch*)SoSwitch::getByName(makeUniqueNodeName("SCENE_SWITCH",0));

    if(sorter == NULL)
		sorter = (SoSorterKit*)SoNode::getByName("SORTER_KIT");
    
	if(cam1 == NULL)
        cam1 = (SoDragKit*)SoDragKit::getByName(makeUniqueNodeName("CAMERA1",0));
	if(player0Switch == NULL)
        player0Switch = (SoSwitch*)SoSwitch::getByName(makeUniqueNodeName("PLAYER0_SWITCH",0));
	if(player1Switch == NULL)
        player1Switch = (SoSwitch*)SoSwitch::getByName(makeUniqueNodeName("PLAYER1_SWITCH",0));
	if(player2Switch == NULL)
        player2Switch = (SoSwitch*)SoSwitch::getByName(makeUniqueNodeName("PLAYER2_SWITCH",0));
	if(player3Switch == NULL)
        player3Switch = (SoSwitch*)SoSwitch::getByName(makeUniqueNodeName("PLAYER3_SWITCH",0));

	if(silverScreenTexture == NULL)
		silverScreenTexture = (SoTexture2*)SoTexture2::getByName("SILVERSCREENTEXTURE");

	if(figSep == NULL)
		figSep = (SoSeparator*)SoSeparator::getByName(makeUniqueNodeName("FIG_SEP",0));

	if(bigScene == NULL)
	{
        bigScene = (SoSeparator*)SoSeparator::getByName(makeUniqueNodeName("BIG_SCENE",0));
		if(bigScene == NULL) createCameraScene();
	}   

	if(thisInstanceNum.getValue()>instanceNum) instanceNum=thisInstanceNum.getValue();

    if(masterMode) // set callbacks
    {
        bCre->addReleaseCallback(createWindowButtonCB,this);
        bDel->addReleaseCallback(deleteWindowButtonCB,this);
        player0check->addPressCallback(checkBoxCB, player0Switch);
        player1check->addPressCallback(checkBoxCB, player1Switch);
        player2check->addPressCallback(checkBoxCB, player2Switch);
        player3check->addPressCallback(checkBoxCB, player3Switch);
        slider->addPositionChangeCallback(sliderCB, sceneSwitch);
        viewButton->addPressCallback(checkBoxCB, viewSwitch);
    }
    else // slave mode - remove callbacks
    {
        bCre->removeReleaseCallback(createWindowButtonCB);
        bDel->removeReleaseCallback(deleteWindowButtonCB);
        player0check->removePressCallback(checkBoxCB);
        player1check->removePressCallback(checkBoxCB);
        player2check->removePressCallback(checkBoxCB);
        player3check->removePressCallback(checkBoxCB);
        slider->removePositionChangeCallback(sliderCB);
        viewButton->removePressCallback(checkBoxCB);
    }
    //fprintf(stderr,"SoStoryboard::setSheetMasterMode exit\n");
}

//----------------------------------------------------------------------------
// things to do when we get the focus

void 
SoStoryboard::focusChange(int uid)
{
    //fprintf(stderr,"SoStoryboard::focusChange\n");
    // make sure the silver screen window belongs to active context
    //comm->setWindowCallback(getWindow(1), staticWindowEventCB, this);

    if(isMaster)
    {    
        // add window content here (use switch with references!)
        AGETNODE(silverScreenGroup,SoGroup,"SILVER_SCREEN_GROUP");
        //silverScreenGroup->replaceChild(0,texture);
        // update texture on silver screen
        updateTexture(this,NULL);
        //select in sorter
        sorter->select(getIconId());
    }
}

//----------------------------------------------------------------------------

int
SoStoryboard::getNumWindows()
{
    return 2; // default is 1 window per context
}

//----------------------------------------------------------------------------
//eof
