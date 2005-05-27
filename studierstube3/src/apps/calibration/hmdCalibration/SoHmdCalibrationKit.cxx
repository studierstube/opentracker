// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoHmdCalibrationKit.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    
//  VERSION:    3.0
// ===========================================================================
//  AUTHORS:    rcs Rainer Splechtna
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#include <cassert>
#include <iostream>

#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/events/SoKeyboardEvent.h>

#include <Inventor/sensors/SoTimerSensor.h>

#include <stbapi/util/ivio.h>

#include <stbapi/workspace/defines.h>
#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/workspace/SoApplicationKit.h>

#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/resource/viewer/SoDisplayMode.h>

#include "SoHmdCalibrationKit.h"
#include "../utils/cameraOptimizer.h"


const int CAMERA_OPTIMIZATION_FINISHED = 7000;

struct CameraOptimizationMessage 
{
    int appID;
    SoHmdCalibrationKit *caller;
    SbBool valid;
    SoOffAxisCamera *camera;
};


//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoHmdCalibrationKit);

//--initialization of context in Studierstube
//CONTEXT_APPLICATION(SoHmdCalibrationKit);

//----------------------------------------------------------------------------
// static class initialization
//----------------------------------------------------------------------------

void
SoHmdCalibrationKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoHmdCalibrationKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoContextKit*
SoHmdCalibrationKit::factory()
{
    return new SoHmdCalibrationKit;  
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoHmdCalibrationKit::SoHmdCalibrationKit()
    :
    keepCheckBox(NULL), testCheckBox(NULL), 
    root(NULL),
    appState(START), 
    samplingMode(FULL), 
    oldCamera(NULL), 
    newCamera(NULL),
    stereoMode(FALSE),
    keepCamera(FALSE), viewer(NULL), pButton(0), sButton(1), viewportSize(1,1),
    relativeSampling(FALSE), leftGraph(NULL), rightGraph(NULL), calibPoint(0),
    textMode(FALSE), sampler(NULL)
{
    SO_KIT_CONSTRUCTOR(SoHmdCalibrationKit);
    // These fields' values may be read from file
    SO_KIT_ADD_FIELD(referencePoints, (0,0));
    SO_KIT_ADD_FIELD(nearMarkerDistance, (0.2));
    SO_KIT_ADD_FIELD(farMarkerDistance, (0.5));
    SO_KIT_ADD_FIELD(epsilon, (0.15));
    SO_KIT_ADD_FIELD(enabledColor, (1,0,0));
    SO_KIT_ADD_FIELD(disabledColor, (0.8,0.8,0.8));
    SO_KIT_ADD_FIELD(averageErrorThreshold, (0.01));
    SO_KIT_ADD_FIELD(validDistance, (0.0125));
    SO_KIT_ADD_FIELD(useMirrorMode, (FALSE));
    SO_KIT_ADD_FIELD(dataDirectory, ("../apps/calibration/hmdcalibration/"));
    // only internally used 
    SO_KIT_ADD_FIELD(validPenPosition, (FALSE));

    SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoHmdCalibrationKit::~SoHmdCalibrationKit()
{
    if (appState == SAMPLING_LEFT_EYE_DATA || 
        appState == SAMPLING_RIGHT_EYE_DATA)
        remove2DCamera();

    if (sampler)
        delete(sampler);

    if (newCamera)
        newCamera->unref();
    
    if (oldCamera)
        oldCamera->unref();

    //    switchFocusToSuite(userID.getValue());
}

//----------------------------------------------------------------------------

void SoHmdCalibrationKit::restartCB(void *data,SoButtonKit *)
{
    fprintf(stderr, "restart\n");

    // reset PIP2
    SoHmdCalibrationKit *actual = (SoHmdCalibrationKit *)data;
    switch (actual->appState)
    {
        case SAMPLING_LEFT_EYE_DATA: 
        case SAMPLING_RIGHT_EYE_DATA:
            actual->remove2DCamera();
            break;
        case SAMPLING_FINISHED:   
            actual->keepCheckBox->on = 0;     
            actual->testCheckBox->on = 0;     
            testCB(data, actual->testCheckBox);
            if (actual->newCamera) 
            {
                actual->setActiveCamera(actual->oldCamera);
                actual->newCamera->unref();
                actual->newCamera = NULL;     
            }
            break;
        default: break;
    }
    // switch to PIP1
    actual->appState = START;
    ((SoPipSheetKit *)actual->getPipSheet(actual->userID.getValue()))->
        switchPipSheet(FIRST_SHEET);
}

//----------------------------------------------------------------------------

void SoHmdCalibrationKit::testCB(void *data,SoButtonKit *tb)
{
    using namespace std;

    SoHmdCalibrationKit *actual = (SoHmdCalibrationKit *)data;

    //fprintf(stderr, "t: %d\n", actual->testCheckBox->on.getValue());
    if (actual->testCheckBox->on.getValue())         
    {
        SoSeparator *testGraph = readFile("testGraph.iv",
                                          actual->dataDirectory.getValue().getString());
        if (!testGraph)
            return;
        SoTransform *mover = new SoTransform;
        SoTrakEngine *trak = new SoTrakEngine;
	trak->station.setValue(
                               SoUserManagerKit::getInstance()->
                               getPenFromUser(actual->userID.getValue()));
        mover->translation.connectFrom(&trak->translation);
        mover->rotation.connectFrom(&trak->rotation);
        SoSeparator *tRoot = new SoSeparator;
        tRoot->setName("testRoot");
        tRoot->addChild(mover);
        tRoot->addChild(testGraph);
        actual->root->addChild(tRoot);
    }
    else 
    {
        SoNode *found = findNode(actual->root, "testRoot");
        if (found)
            actual->root->removeChild(found);
    }
}

//----------------------------------------------------------------------------

void SoHmdCalibrationKit::keepNewCameraCB(void *data,SoButtonKit *kb)
{
    SoHmdCalibrationKit *actual = (SoHmdCalibrationKit *)data;

    //fprintf(stderr, "t: %d\n", kb->on.getValue());
    if (kb->on.getValue()) {
        assert(actual->newCamera);
        // set new camera 
        actual->setActiveCamera(actual->newCamera);
    }
    else 
        // set old camera 
        actual->setActiveCamera(actual->oldCamera);
}

//----------------------------------------------------------------------------

void SoHmdCalibrationKit::exitCB(void *data,SoButtonKit *)
{
    SoHmdCalibrationKit *actual = (SoHmdCalibrationKit *)data;
    actual->scheduleDelete();
}

//----------------------------------------------------------------------------

void SoHmdCalibrationKit::startFullCB(void *data,SoButtonKit *)
{
    using namespace std;
  
    cerr << "---------------- START CALIBRATION ----------------" << endl;
    SoHmdCalibrationKit *actual = (SoHmdCalibrationKit *)data;
    if (actual->appState == START)
    {
        actual->samplingMode = FULL;
        actual->startCalibration();
    }
}

//----------------------------------------------------------------------------

void SoHmdCalibrationKit::startShortCB(void *data, SoButtonKit *)
{
    fprintf(stderr, "short calibration \n");
    //SoHmdCalibrationKit *actual = (SoHmdCalibrationKit *)data;  
}

//----------------------------------------------------------------------------
// Reads the pip sheet and sets the button callbacks
//----------------------------------------------------------------------------

SbBool
SoHmdCalibrationKit::checkPipGeometry()
{
    return TRUE;
}
    
//----------------------------------------------------------------------------

SbBool 
SoHmdCalibrationKit::checkWindowGeometry()
{
    using namespace std;

    // retrieve used viewer
    viewer = SoUserManagerKit::getInstance()->getUserKit(userID.getValue())->
        getDisplay()->getViewer();
    if (!viewer)
    {
        fprintf(stderr, "patholgical case: no viewer\n");
        exit(1);
    }
    // get and store display and camera control state
    if (viewer->getDisplayMode()->isStereoMode()) {
        cerr << "STEREO mode enabled." << endl;
        stereoMode = TRUE;
    }
    if (!viewer->getCameraControlMode()->isViewing())
        relativeSampling = TRUE;

    if (relativeSampling)
        fprintf(stderr, "sampling relatively\n");
    // get and store the primary and secondary button of the pen
    pButton = SoUserManagerKit::getInstance()->getUserKit(userID.getValue())->
        getPen()->primaryButton.getValue();
    sButton = SoUserManagerKit::getInstance()->getUserKit(userID.getValue())->
        getPen()->secondaryButton.getValue();

    assert(getWindow(0));
    root = getWindow(0)->getClientVolume();
    // setup basic application scene graph
    initCalibrationGraph();
    saveOldCamera();

    return TRUE;
}

//----------------------------------------------------------------------------

void
SoHmdCalibrationKit::checkPipConnections( int uid, SoNode * pipSheetContents )
{
    if (uid != userID.getValue())
        return;

    SoNode *psk = pipSheetContents;
    char buffer[100];

    // PIP SHEET 1

    SoPushButton* bFull = (SoPushButton*)findNode(psk,"FULL_CALIBRATION_BUTTON");
    sprintf(buffer,"FULL_CALIBRATION_BUTTON_%s", getName().getString());
    bFull->setName(buffer);
    SoPushButton* bShort = (SoPushButton*)findNode(psk,"SHORT_CALIBRATION_BUTTON");
    sprintf(buffer,"SHORT_CALIBRATION_BUTTON_%s", getName().getString());
    bShort->setName(buffer);
    SoPushButton* bExit = (SoPushButton*)findNode(psk,"EXIT_BUTTON");
    sprintf(buffer,"EXIT_BUTTON_%s", getName().getString());
    bExit->setName(buffer);

    // set callbacks 
    bFull->setPressCallback(startFullCB, this);
    bShort->setPressCallback(startShortCB, this);
    bExit->setPressCallback(exitCB, this);

    // PIP SHEET 2

    SoPushButton* bRestart = (SoPushButton*)findNode(psk,"RESTART_BUTTON");
    sprintf(buffer,"RESTART_BUTTON_%s", getName().getString());
    bRestart->setName(buffer);
    keepCheckBox = (SoToggleButton*)findNode(psk,"KEEP_BUTTON");
    sprintf(buffer,"KEEP_BUTTON_%s", getName().getString());
    keepCheckBox->setName(buffer);
    SoPushButton* bExit2 = (SoPushButton*)findNode(psk,"EXIT_BUTTON");
    sprintf(buffer,"EXIT_BUTTON_2_%s", getName().getString());
    bExit2->setName(buffer);
    testCheckBox = (SoToggleButton*)findNode(psk,"TEST_BUTTON");
    sprintf(buffer,"TEST_BUTTON_%s", getName().getString());
    testCheckBox->setName(buffer);

    // set callbacks 
    bRestart->setPressCallback(restartCB, this);
    bExit2->setPressCallback(exitCB, this);
    keepCheckBox->setPressCallback(keepNewCameraCB, this);
    keepCheckBox->setReleaseCallback(keepNewCameraCB, this);
    testCheckBox->setPressCallback(testCB, this);
    testCheckBox->setReleaseCallback(testCB, this);

}

//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::checkPipMasterMode( SoNode * pipSheetContents, SbBool masterMode )
{
    // only the user, who started the application gets a sheet,
    // hence nothing has to be done in this method
}

//----------------------------------------------------------------------------
// CAMERA MANIPULATIONS
//----------------------------------------------------------------------------
// Build orthographic cameras and use them

void 
SoHmdCalibrationKit::set2DCamera()
{
    using namespace std;

    cerr << "SoHmdCalibrationKit::set2DCamera()" << endl;
    SoOrthographicCamera *_2DCamera = new SoOrthographicCamera;
    _2DCamera->viewportMapping = SoCamera::LEAVE_ALONE;
    _2DCamera->aspectRatio = viewportSize[0]/viewportSize[1];
    _2DCamera->height = viewportSize[1];
    _2DCamera->nearDistance = 0.5;
    _2DCamera->farDistance = 500;

    // make orthographic cameras currently used cameras
    viewer->getStereoCameraKit()->getStereoSwitch()->insertChild(_2DCamera,0);
    viewer->getStereoCameraKit()->getStereoSwitch()->insertChild(_2DCamera,1);

    // disable mover of camera
    SoTransform *mover;
    mover = viewer->getStereoCameraKit()->getTransform();
//     mover->translation.enableConnection(FALSE);
    mover->translation.getConnectedEngine(oldConnectedFieldTranslation);
    mover->translation.disconnect();
    mover->translation.touch();

//     mover->rotation.enableConnection(FALSE);
    mover->rotation.getConnectedEngine(oldConnectedFieldRotation);
    mover->rotation.disconnect();
    mover->rotation.touch();

    mover->translation.setValue(0,0,0);
    mover->rotation.setValue(0,0,0,1);

    SoTimerSensor *sensor = new SoTimerSensor(updateCB, mover);
    sensor->setInterval(0.2); //scheduled once per second
    sensor->schedule();

}

//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::remove2DCamera()
{
    using namespace std;
    cerr << "SoHmdCalibrationKit::remove2DCamera()" << endl;

    viewer->getStereoCameraKit()->getStereoSwitch()->removeChild(0);
    cerr << "SoHmdCalibrationKit::first child removed" << endl;
    viewer->getStereoCameraKit()->getStereoSwitch()->removeChild(0);
    cerr << "SoHmdCalibrationKit::second child removed" << endl;

    // enable mover again

    if (oldConnectedFieldTranslation)
        viewer->getStereoCameraKit()->getTransform()->translation.connectFrom(oldConnectedFieldTranslation);

    if (oldConnectedFieldRotation)
        viewer->getStereoCameraKit()->getTransform()->rotation.connectFrom(oldConnectedFieldRotation);

    cerr << "SoHmdCalibrationKit::remove2DCamera() done." << endl;
}

//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::setActiveCamera(SoStereoCameraKit *camKit)
{
    viewer->getStereoCameraKit()->getCameraLeft()->
        copyFieldValues(camKit->getCameraLeft());
    viewer->getStereoCameraKit()->getCameraRight()->
        copyFieldValues(camKit->getCameraRight());
}

//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::saveOldCamera()
{
    oldCamera = new SoStereoCameraKit;        
    oldCamera->ref(); 
    oldCamera->getCameraLeft()->copyFieldValues(
                                                viewer->getStereoCameraKit()->getCameraLeft());    
    oldCamera->getCameraRight()->copyFieldValues(
                                                 viewer->getStereoCameraKit()->getCameraRight());    
}

//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::initCalibrationGraph()
{
    SoEventCallback *event = new SoEventCallback;
    root->addChild(event);
    event->addEventCallback(SoKeyboardEvent::getClassTypeId(), keyEventCB, this);              

    SoSwitch *whichGraph = new SoSwitch;
    leftGraph = new SoSeparator;
    whichGraph->addChild(leftGraph);
    rightGraph = new SoSeparator;
    whichGraph->addChild(rightGraph);
    root->addChild(whichGraph);
    whichGraph->whichChild.connectFrom
        (&(viewer->getStereoCameraKit()->getStereoSwitch()->whichChild));
}


//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::startCalibration()
{
    using namespace std;
    cerr << "SoHmdCalibrationKit::startCalibration()" << endl;

    ((SoPipSheetKit *)getPipSheet(userID.getValue()))->switchPipSheet(EMPTY_SHEET);

    calibPoint = 0;
    textMode = FALSE;
    appState =  SAMPLING_LEFT_EYE_DATA;

    SbVec2s vps = viewer->getViewportRegion().getViewportSizePixels();
    viewportSize.setValue(vps[0],vps[1]);
    printf("vpX: %f vpY: %f\n", viewportSize[0], viewportSize[1]);
    set2DCamera();
    displayPoint();
    
    if (sampler)
        sampler->clearAllSamples();
    else
    {
        SoMFInt32 stations;
        int penStation;
        stations.setValue(penStation = SoUserManagerKit::getInstance()->
                          getPenFromUser(userID.getValue()));
        stations.set1Value(1,SoUserManagerKit::getInstance()->
                           getHmdFromUser(userID.getValue()));

        sampler = new SoSampler;
        sampler->setStationsToSample(stations);
        sampler->initButtonTriggering(penStation, pButton, FALSE, FALSE,
                                      buttonTriggered,this);
    }
    sampler->startButtonTriggering();

    cerr << "SoHmdCalibrationKit::startCalibration() done ." << endl;
}

//----------------------------------------------------------------------------

void
SoHmdCalibrationKit::buttonTriggered(void *data)
{
    SoHmdCalibrationKit *actual = (SoHmdCalibrationKit *)data;
    actual->calibControl(actual->pButton);  
}

//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::keyEventCB(void *data, SoEventCallback *myEvent) 
{
    const SoEvent *event = myEvent->getEvent();
    SoKeyboardEvent::Key pressed;
 
    SoHmdCalibrationKit *actual = (SoHmdCalibrationKit *)data;

    if (SoKeyboardEvent::isKeyPressEvent(event, SoKeyboardEvent::ANY)) {
        pressed = ((SoKeyboardEvent *)event)->getKey(); 
        switch (pressed)
        {
            case SoKeyboardEvent::F8:
                exitCB(data, NULL);
                break;
            case SoKeyboardEvent::S: 
                startFullCB(data,NULL);
                break;
            case SoKeyboardEvent::F4: 
                restartCB(data,NULL);
                break;
            case SoKeyboardEvent::F2:
                if (actual->appState == SAMPLING_FINISHED)
                {
                    actual->keepCheckBox->on = 
                        !actual->keepCheckBox->on.getValue(); 
                    actual->keepCheckBox->adaptGeometryToState();
                    keepNewCameraCB(data, actual->keepCheckBox);
                }
                break;
            case SoKeyboardEvent::F1:
                if (actual->appState == SAMPLING_FINISHED)
                {
                    actual->testCheckBox->on = 
                        !actual->testCheckBox->on.getValue(); 
                    actual->testCheckBox->adaptGeometryToState();
                    testCB(data, actual->testCheckBox);
                }
                break;
            case SoKeyboardEvent::BACKSPACE:
                actual->calibControl(actual->sButton);
                break;
            case SoKeyboardEvent::SPACE:
                actual->calibControl(actual->pButton);
                break;
            default: break;
        }
    }

    myEvent->setHandled();       
}

//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::calibControl(int button) 
{
    using namespace std;

    cerr << "!!!!!!!!!! SoHmdCalibrationKit::calibControl()" << endl;

    if (!(appState == SAMPLING_LEFT_EYE_DATA || 
          appState == SAMPLING_RIGHT_EYE_DATA)) 
        return;
         
    if (textMode) 
    {
        textMode = FALSE;
        displayPoint();
        return;
    }
        
    if (button == pButton && 
        (validPenPosition.getValue() || !relativeSampling)) 
    {
        sampler->sample();
        calibPoint++; 
        if (calibPoint == referencePoints.getNum()*2)     
            startCameraOptimization();
        else
            displayPoint();
    }
    else if (button == sButton) 
    {
        if (calibPoint > 0) 
        {
            calibPoint--;
            sampler->stepBack();
            displayPoint();
        }
    }
    cerr << "!!!!!!!!!! SoHmdCalibrationKit::calibControl() done." << endl;

}

//----------------------------------------------------------------------------

SoSeparator *
SoHmdCalibrationKit::build3Dpoint(const SbVec2f &point2D, SbBool nearDistance)
{
    using namespace std;

    cerr << "!!!!!!!!!! SoHmdCalibrationKit::build3Dpoint()" << endl;
    SoSeparator *point3D = new SoSeparator;

    // set position of point in camera space
    SoTranslation *trans = new SoTranslation;
    // get position in camera space and scale factor
    float x = (float)(point2D[0]*viewportSize[0] - viewportSize[0]/2.0);
    float y = (float)(point2D[1]*viewportSize[1] - viewportSize[1]/2.0);

    trans->translation = (SbVec3f(x,y,-100)); 
    point3D->addChild(trans);

    // add shape 
    char mName[512];
    strcpy(mName, dataDirectory.getValue().getString());
    if (nearDistance)
        strcat(mName, "markerNear.iv");
    else
        strcat(mName, "markerFar.iv");

    SoSeparator *marker;
    marker = readFile(mName);
    if (marker == NULL) 
        exit(1);
    point3D->addChild(createDistanceSensitiveColor(nearDistance));
    point3D->addChild(marker);

    cerr << "!!!!!!!!!! SoHmdCalibrationKit::build3Dpoint() done." << endl;

    return point3D;

}

//----------------------------------------------------------------------------

SoMaterial *
SoHmdCalibrationKit::createDistanceSensitiveColor(SbBool nearDistance)
{
    SoMaterial *material = new SoMaterial;

    if (relativeSampling)
    {
        SoTrakEngine *pen = new SoTrakEngine;
	pen->station.setValue(
                              SoUserManagerKit::getInstance()->
                              getPenFromUser(userID.getValue()));
        SoTrakEngine *head = new SoTrakEngine;
	head->station.setValue(
                               SoUserManagerKit::getInstance()->
                               getHmdFromUser(userID.getValue()));
        
        SoCalculator *calc = new SoCalculator;
        calc->A.connectFrom(&pen->translation);
        calc->B.connectFrom(&head->translation);
        calc->C.connectFrom(&enabledColor);
        calc->D.connectFrom(&disabledColor);
        if (nearDistance)
            calc->a.connectFrom(&nearMarkerDistance);
        else
            calc->a.connectFrom(&farMarkerDistance);
        calc->b.connectFrom(&epsilon);
        
        calc->expression.setValue("ta = length(A-B); tf = (a-b); tg = (a+b); td = (ta > tf) ? 1 : 0; te = (ta < tg) ? 1 : 0; tb = td + te; th = (tb > 1) ? 1 : 0; oC = (th > 0) ? (C) : (D); oa = th");
        
        //calc->expression.setValue("ta = length(A-B); oa = ta; oC = C");
	
        material->emissiveColor.connectFrom(&calc->oC);
        material->diffuseColor.setValue(0,0,0);
        material->ambientColor.setValue(0,0,0);
        
        validPenPosition.connectFrom(&calc->oa);
    }
    else
    {
        material->emissiveColor = enabledColor.getValue();
        material->diffuseColor.setValue(0,0,0);
        material->ambientColor.setValue(0,0,0);
    }

    return material;
}

//----------------------------------------------------------------------------

SoSeparator *
SoHmdCalibrationKit::build3DText(const char *name)
{
    using namespace std;
    cerr << "SoHmdCalibrationKit::build3DText()" << endl;
    // set font
    SoSeparator *textRoot = new SoSeparator;
  
    SoNode *textFormat;
    textFormat = readNodeFromFile("textFormat.iv",
                                  dataDirectory.getValue().getString());
    if (!textFormat)
        exit(-1);
    textRoot->addChild(textFormat);

    SoText3 *text = new SoText3;
    text->string.setValue(name);
    text->justification = SoText3::CENTER;
    textRoot->addChild(text);

    cerr << "SoHmdCalibrationKit::build3DText() done." << endl;
    return textRoot;
}

//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::displayPoint() 
{
    using namespace std;
    cerr << "!!!!!!!!!! SoHmdCalibrationKit::displayPoint()" << endl;

    SoSeparator *root;

    if (appState ==  SAMPLING_LEFT_EYE_DATA)
        root = leftGraph;
    else
        root = rightGraph;

    root->removeAllChildren();
    root->addChild(
                   build3Dpoint(referencePoints[calibPoint%referencePoints.getNum()],
                                calibPoint>= referencePoints.getNum()));
    cerr << "!!!!!!!!!! SoHmdCalibrationKit::displayPoint() done." << endl;
}

//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::displayText(const char *s)
{
    using namespace std;
    cerr << "!!!!!!!!!! SoHmdCalibrationKit::displayText()" << endl;
    SoSeparator *root;
    if (appState ==  SAMPLING_LEFT_EYE_DATA)
        root = leftGraph;
    else
        root = rightGraph;
    root->removeAllChildren();
    root->addChild(build3DText(s));
    textMode = TRUE;
    cerr << "!!!!!!!!!! SoHmdCalibrationKit::displayText() done." << endl;
}

//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::startCameraOptimization() 
{
    sampler->stopButtonTriggering();
    displayText("calculating camera ...");
    // pid = createThread(doCameraOptimization, this);
    doCameraOptimization(this);
}

//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::doCameraOptimization(void *data) 
{
    using namespace std;

    cerr << "++++++++ SoHmdCalibrationKit::doCameraOptimization(): do optimization" << endl;
    SoHmdCalibrationKit *actual = (SoHmdCalibrationKit *)data;

    int penStation = SoUserManagerKit::getInstance()->
        getPenFromUser(actual->userID.getValue());
    int headStation = SoUserManagerKit::getInstance()->
        getHmdFromUser(actual->userID.getValue());

    SoOffAxisCamera *camera;

    SoMFVec2f *realReferencePoints;
    SoMFVec2f mirroredReferencePoints;
    if (actual->useMirrorMode.getValue())
    {
        for (int i=0; i<actual->referencePoints.getNum(); i++)
            mirroredReferencePoints.set1Value(i, 
                                              1.0f - actual->referencePoints[i][0],
                                              actual->referencePoints[i][1]);
        
        realReferencePoints = &mirroredReferencePoints;
    }
    else
        realReferencePoints = &actual->referencePoints;


    char dataFile[500];
    
    const char *logName;
    if (actual->appState == SAMPLING_RIGHT_EYE_DATA)
    {
        sprintf(dataFile,"%suser%d_camopt_log_right.txt", 
                actual->dataDirectory.getValue().getString(), actual->userID.getValue());    
    }
    else
    {
        sprintf(dataFile,"%suser%d_camopt_log_left.txt", 
                actual->dataDirectory.getValue().getString(), actual->userID.getValue());    
    }
    logName = dataFile;

    // calculate and optimize camera
    SbBool valid = buildCamera(
                               camera,
                               *realReferencePoints,
                               actual->sampler->samples[penStation].translations,
                               actual->sampler->samples[penStation].rotations,
                               actual->sampler->samples[headStation].translations,
                               actual->sampler->samples[headStation].rotations,
                               actual->relativeSampling,
                               actual->averageErrorThreshold.getValue(),
                               actual->validDistance.getValue(),
                               logName                        
                               );   
       
    CameraOptimizationMessage *msg = (CameraOptimizationMessage *) 
        malloc(sizeof(CameraOptimizationMessage));
    msg->valid  = valid;
    msg->camera = camera;
    msg->appID  = actual->appID.getValue();
    msg->caller = actual;

    mirroredReferencePoints.deleteValues(0);

    actual->getAppKit()->setName("hmdCalibration");
    // send message 
    //StbMsgManager::getInstance()->sendMessage(CAMERA_OPTIMIZATION_FINISHED, 
    //  actual->appID.getValue(), actual->appID.getValue(), (void *) msg);
    AppMessageCBFunc(NULL, CAMERA_OPTIMIZATION_FINISHED, msg->appID, msg);
}

//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::validateCamera(SbBool valid, SoOffAxisCamera *camera) 
{
    // log sampledPoints for active camera
    char dataFile[1000];
    sprintf(dataFile,"%suser%dsampledata.dat",dataDirectory.getValue().getString(), 
            userID.getValue());
    logSampleData(dataFile);

    if (!valid)
    {
        // do resampling
        calibPoint = 0;
        displayText("repeat calibration ... press button");
        sampler->clearAllSamples();
        sampler->startButtonTriggering();           
        return;
    }

    if (!newCamera)
    {
        newCamera = new SoStereoCameraKit;
        newCamera->ref();
    }

    // if points for left (first) camera are sampled using stereo mode
    if ((appState ==  SAMPLING_LEFT_EYE_DATA) && stereoMode) 
    {
        newCamera->getCameraLeft()->copyFieldValues(camera);    

        calibPoint = 0;
        appState =  SAMPLING_RIGHT_EYE_DATA;
            
        textMode = FALSE;
        leftGraph->removeAllChildren();
        displayPoint();

        sampler->clearAllSamples();
        sampler->startButtonTriggering();    
    }
    // if points for right (second) camera are sampled or only one camera shall
    // be calibrated
    else 
    {
        if (appState == SAMPLING_RIGHT_EYE_DATA)
        {
            newCamera->getCameraRight()->copyFieldValues(camera);   
            SbVec3f eyeDistance = newCamera->getCameraRight()->eyepointPosition.getValue() -
                newCamera->getCameraLeft()->eyepointPosition.getValue();
            SbVec3f planeDistanceR = newCamera->getCameraRight()->position.getValue() -
                newCamera->getCameraRight()->eyepointPosition.getValue();
            SbVec3f planeDistanceL = newCamera->getCameraLeft()->position.getValue() -
                newCamera->getCameraLeft()->eyepointPosition.getValue();
            newCamera->eyeOffsetLeft.setValue(SbVec3f(eyeDistance.length(), 
                                                      planeDistanceL.length(),
                                                      planeDistanceR.length()
                                                      ));

            rightGraph->removeAllChildren();
        }
        else
        {
            newCamera->getCameraLeft()->copyFieldValues(camera);    
            newCamera->getCameraRight()->copyFieldValues(camera);    
            leftGraph->removeAllChildren();
        }
        validPenPosition.disconnect(); // to delete internal calculator engine

        appState = SAMPLING_FINISHED;
        remove2DCamera();
        ((SoPipSheetKit *)getPipSheet(userID.getValue()))->switchPipSheet(SECOND_SHEET);
        keepCheckBox->on = TRUE;
        keepCheckBox->adaptGeometryToState();
        keepNewCameraCB(this, keepCheckBox);

        // save cameraKit to file "user[userID]HMD.iv" in the working directory
        sprintf(dataFile,"%suser%dHMD.iv", dataDirectory.getValue().getString(), userID.getValue());
        printf("writing camera to %s ...\n", dataFile);
        writeFile(dataFile, newCamera); 
    }
    camera->ref();
    camera->unref();
}

//----------------------------------------------------------------------------

void 
SoHmdCalibrationKit::logSampleData(char *dataFile) 
{
    FILE *datalog;
    printf("HMD-calibration: writing to %s\n",dataFile);
    if (appState == SAMPLING_LEFT_EYE_DATA) 
    {
        datalog = fopen(dataFile, "w");
        fprintf(datalog, "%d\n", referencePoints.getNum());
    }
    else
        datalog = fopen(dataFile, "a");

    int penStation = SoUserManagerKit::getInstance()->
        getPenFromUser(userID.getValue());
    int headStation = SoUserManagerKit::getInstance()->
        getHmdFromUser(userID.getValue());
    float x,y,z;
    float sx,sy;
    float hx,hy,hz,hrx,hry,hrz,hrw;
    for (int i=0; i<referencePoints.getNum(); i++)
    {
        sampler->samples[penStation].translations[i].getValue(x,y,z);
        sampler->samples[headStation].translations[i].getValue(hx,hy,hz);
        sampler->samples[headStation].rotations[i].getValue(hrx,hry,hrz,hrw);
        referencePoints[i].getValue(sx,sy);
#if defined(_VIDEO_LIBRARY)
        if (useMirrorMode.getValue())
            sx = 1.0-sx;
#endif
        fprintf(datalog, "%3.20f %3.20f %3.20f %f %f\n%3.20f %3.20f %3.20f\n%1.8f %1.8f %1.8f %1.8f\n",
                x,y,z,sx,sy,hx,hy,hz,hrx,hry,hrz,hrw);
        sampler->samples[penStation].translations[i+referencePoints.getNum()]
            .getValue(x,y,z);
        sampler->samples[headStation].translations[i+referencePoints.getNum()]
            .getValue(hx,hy,hz);
        sampler->samples[headStation].rotations[i+referencePoints.getNum()]
            .getValue(hrx,hry,hrz,hrw);
        fprintf(datalog, "%3.20f %3.20f %3.20f %f %f\n%3.20f %3.20f %3.20f\n%1.8f %1.8f %1.8f %1.8f\n",
                x,y,z,sx,sy,hx,hy,hz,hrx,hry,hrz,hrw);
    }
    fclose(datalog);
}

//----------------------------------------------------------------------------

AppMessageCB*
SoHmdCalibrationKit::getAppMessageCB() 
{
    return AppMessageCBFunc;
}

//----------------------------------------------------------------------------

void
SoHmdCalibrationKit::AppMessageCBFunc(void* userData,
                                      int message,
                                      int sender,
                                      void* data)
{
    if (message == CAMERA_OPTIMIZATION_FINISHED)
    {
        CameraOptimizationMessage *actual = (CameraOptimizationMessage *)data;
        if (actual->appID == sender)
            actual->caller->validateCamera(actual->valid, actual->camera);
        /*DWORD lpExitCode;
          if (GetExitCodeThread((HANDLE) actual->caller->pid, 
          &lpExitCode))
          fprintf(stderr, "exit code of thread: %d\n", lpExitCode );     
        */
        delete actual;
    }
}

void SoHmdCalibrationKit::updateCB(void *data, SoSensor *) {

    using namespace std;
   
    SoTransform *mover = (SoTransform *)data;

    if ( mover->translation.getValue() != SbVec3f(0.0, 0.0, 0.0) )
    {

    	cerr << "translation connection enabled:";
    	if (mover->translation.isConnectionEnabled()) {
        cerr << "TRUE";
    } else {
        cerr << "FALSE";
    }
    cerr << endl;

    cerr << "mover translation: " 
         << mover->translation.getValue()[0] << " "  
         << mover->translation.getValue()[1] << " " 
         << mover->translation.getValue()[2]
         << endl;

    SbVec3f axis;
    float rot;
    mover->rotation.getValue().getValue(axis,rot);
    cerr << "mover rotation: " 
         << axis[0] << " "  
         << axis[1] << " " 
         << axis[2] << " " << rot
         << endl;
    }
}


/* ===========================================================================
   End of SoHmdCalibrationKit.cxx
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
