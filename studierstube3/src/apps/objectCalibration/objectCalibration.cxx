/* ===========================================================================
    (C) 2000 Vienna University of Technology
   ===========================================================================
    NAME:       objectCalibration
    TYPE:       c++ code
    PROJECT:    Studierstube
    CONTENT:    object calibration - implementation
    VERSION:    2.5
   ===========================================================================
    AUTHORS:    rcs     Rainer Splechtna
                jp      Jan Prikryl
   ===========================================================================
    HISTORY:

    09-Jan-01 10:21:47  jp      last modification
    28-oct-99 18:15:00  rcs     created
=========================================================================== */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stbapi/stbapi.h>
#ifndef USE_SOQT_LIB
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/IntrinsicP.h>
#include <Xm/XmStrDefs.h>
#include <Inventor/Xt/SoXt.h>
#include <Inventor/Xt/SoXtRenderArea.h>
#else
#include <Inventor/Qt/SoQt.h>
#include <Inventor/Qt/SoQtRenderArea.h>
#endif
#include <Inventor/SoDB.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/SbLinear.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoPath.h>

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransformSeparator.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoText3.h>

#include <Inventor/SoPickedPoint.h>
#include <Inventor/details/SoDetail.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoAlarmSensor.h>
#include <Inventor/sensors/SoIdleSensor.h>
#include <Inventor/sensors/SoOneShotSensor.h>

#include <stbapi/tracker/SoTrakEngine.h>
#include <stbapi/workspace/StbCommunicator.h>
#include <stbapi/widgets/So3DButton.h> 
#include <stbapi/util/ivio.h>

#include <stbapi/resource/SoUserKit.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include "objectcalibration.h"

#define MAX_BUTTONS 3
#define ACTUAL_BUTTONS 1
#define MAX_TRACKER_BUTTONS 6

#define OC_STEP 0.001
#define OC_ANGLE M_PI/180.0

#define TRACKER_CHOICE 0
#define CALIBRATING 1
#define CONFIRM_DIALOG 2

#define YES 0
#define NO 1
#define CANCEL 2

#define OPTIMIZATION_FINISHED 1010
#define EXIT_APPLICATION 1001

#define FONT_SIZE 2.5

// this class contains data for the object calibration
class objectCalibration {
public:
    int user;
    StbCommunicator * comMan;
    SoWindowKit *window;    
    int quickStop;

    SoSeparator *    objectcalRoot;

    So3DButton *trackerButton[MAX_TRACKER_BUTTONS];
    So3DButton *pip3Buttons[3];
    SoPipSheetKit *mySheets;

    SoSeparator *objectGraph;
    SoTransform *offsetOld;    
    SoTrackedArtifactKit *object;

    objectCalibration() 
    {
        offsetOld = new SoTransform;
        offsetOld->ref();
    }
    ~objectCalibration()
    {
        offsetOld->unref();
    }
    SoPipSheetKit *setupPipSheet(void);
    void startObjectCalibration(void);
    void stopObjectCalibration(void);
    void objectMoveKeyInterpreter(const SoKeyboardEvent::Key pressed);
    SbRotation getMoverRotation(void);
    void moveObject(SbVec3f trans);
    void rotateObject(SbRotation rot);
    void resetObject();
    void setObjectTrackerTransform(SbRotation rot, SbVec3f trans);

    static void trackerCB(void *data, So3DButton *tb);
    static void confirmSheetCB(void *data, So3DButton *cb);

};



/*---------------------------- object calibration -------------------------------------------------*/

SbRotation objectCalibration::getMoverRotation(void)
{
    return object->getMover()->rotation.getValue();
}



void objectCalibration::setObjectTrackerTransform(SbRotation rot, SbVec3f trans)
{
   object->getOffset()->translation = object->getOffset()->translation.getValue() + trans;
   object->getOffset()->rotation = object->getOffset()->rotation.getValue()*rot;
}

void objectCalibration::resetObject()
{
   object->getOffset()->translation = offsetOld->translation.getValue();
   object->getOffset()->rotation = offsetOld->rotation.getValue();
}


void objectCalibration::moveObject(SbVec3f trans)
{
    setObjectTrackerTransform(SbRotation::identity(), trans);    
}

void objectCalibration::rotateObject(SbRotation rot)
{
    setObjectTrackerTransform(rot, SbVec3f(0,0,0));    
}



void objectCalibration::objectMoveKeyInterpreter(const SoKeyboardEvent::Key pressed) 
{
    SbVec3f x(1,0,0), y(0,1,0), z(0,0,1);
    switch (pressed)
    {    
        default:    break;
        case SoKeyboardEvent::INSERT:
            rotateObject(SbRotation(x,OC_ANGLE));
            break;

#ifndef USE_SOQT_LIB
#ifdef WIN32
        case SoKeyboardEvent::KEY_DELETE:
#else
        case SoKeyboardEvent::DELETE:
#endif
#else		
        case SoKeyboardEvent::DELETE:
#endif
            rotateObject(SbRotation(x,-OC_ANGLE));
            break;
        case SoKeyboardEvent::HOME:
            rotateObject(SbRotation(y,OC_ANGLE));
            break;
        case SoKeyboardEvent::END:
            rotateObject(SbRotation(y,-OC_ANGLE));
            break;
           case SoKeyboardEvent::PAGE_UP:
            rotateObject(SbRotation(z,OC_ANGLE));
            break;
        case SoKeyboardEvent::PAGE_DOWN:
            rotateObject(SbRotation(z,-OC_ANGLE));
            break;
   
        case SoKeyboardEvent::UP_ARROW:
            moveObject(SbVec3f(0,OC_STEP,0));
            break;
        case SoKeyboardEvent::DOWN_ARROW:
            moveObject(SbVec3f(0,-OC_STEP,0));
            break;
        case SoKeyboardEvent::LEFT_ARROW:
            moveObject(SbVec3f(-OC_STEP,0,0));
            break;
        case SoKeyboardEvent::RIGHT_ARROW:
            moveObject(SbVec3f(OC_STEP,0,0));
            break;
        case SoKeyboardEvent::PAD_4:
            moveObject(SbVec3f(0,0,-OC_STEP));
            break;
        case SoKeyboardEvent::PAD_6:
            moveObject(SbVec3f(0,0,OC_STEP));
            break;


        case SoKeyboardEvent::RETURN:
            break; 
        case SoKeyboardEvent::R: case SoKeyboardEvent::BACKSPACE:
             resetObject();
             break;
    }    
}




void objectcalKeyEventHandle(void *data, SoEventCallback *myEvent) 
{
    objectCalibration *oc = (objectCalibration *)data;    
    const SoEvent *event = myEvent->getEvent();
   SoKeyboardEvent::Key pressed;

    if (SoKeyboardEvent::isKeyPressEvent(event, SoKeyboardEvent::ANY)) {
        pressed = ((SoKeyboardEvent *)event)->getKey();    
        if (pressed == SoKeyboardEvent::C) 
            oc->stopObjectCalibration();
        else
            oc->objectMoveKeyInterpreter(pressed);
    }

    myEvent->setHandled();
    
}




SoSeparator *createText(char *name)
{
    // set font
   SoSeparator *textRoot = new SoSeparator;

   SoFont *myFont = new SoFont;
   myFont->name.setValue("Times New Roman");
   myFont->size = FONT_SIZE;
   
   SoText3 *text = new SoText3;
   text->string.setValue(name);
   text->justification = SoText3::CENTER;

   SoMaterial *textMaterial = new SoMaterial;
    textMaterial->diffuseColor.setValue(0.0,0.0,0.0);
    textMaterial->specularColor.setValue(0.0,0.0,0.0);
    textMaterial->ambientColor.setValue(0.0,0.0,0.0);
   textMaterial->emissiveColor.setValue(0.0,0.0,0.0); 

   SoTransform *textTransformation = new SoTransform;
   textTransformation->translation.setValue(0,2.1,0.5);
   textTransformation->rotation.setValue(SbVec3f(1,0,0), -1.57);

    textRoot->addChild(textTransformation);
   textRoot->addChild(myFont);
   textRoot->addChild(textMaterial);
   textRoot->addChild(text);

    return textRoot;
}


SoSeparator *createPressedText(char *name)
{
    // set font
   SoSeparator *textRoot = new SoSeparator;

   SoFont *myFont = new SoFont;
   myFont->name.setValue("Times New Roman");
   myFont->size = FONT_SIZE;
   
   SoText3 *text = new SoText3;
   text->string.setValue(name);
   text->justification = SoText3::CENTER;

   SoMaterial *textMaterial = new SoMaterial;
    textMaterial->diffuseColor.setValue(0.0,0.0,0.0);
    textMaterial->specularColor.setValue(0.0,0.0,0.0);
    textMaterial->ambientColor.setValue(0.0,0.0,0.0);
   textMaterial->emissiveColor.setValue(0.0,0.0,0.0); 

   SoTransform *textPressedTransformation = new SoTransform;
   textPressedTransformation->translation.setValue(0,0.1,0.5);
   textPressedTransformation->rotation.setValue(SbVec3f(1,0,0), -1.57);

    textRoot->addChild(textPressedTransformation);
   textRoot->addChild(myFont);
   textRoot->addChild(textMaterial);
   textRoot->addChild(text);

    return textRoot;
}


void objectCalibration::stopObjectCalibration(void)
{
    mySheets->switchPipSheet(CONFIRM_DIALOG);    
}


void objectCalibration::confirmSheetCB(void *data, So3DButton *cb)
{
    objectCalibration *oc = (objectCalibration *)data;    

    if (oc->pip3Buttons[YES] == cb) {
        char trackerName[50];
        sprintf(trackerName,"calibratedArtifact%d.iv",oc->object->station.getValue());
        char fullName[1024];
        strcpy(fullName, oc->comMan->workingDirectory);
        strcat(fullName, trackerName);
        // create a frugal copy of the calibrated object
        SoTrackedArtifactKit *to = new SoTrackedArtifactKit();
        to->ref();
        to->getMover()->translation.disconnect();
        to->getMover()->rotation.disconnect();
        to->station = oc->object->station;
        to->getOffset()->translation = oc->object->getOffset()->translation;
        to->getOffset()->rotation = oc->object->getOffset()->rotation;
        to->setGeometry(oc->object->getGeometry());
        writeFile(fullName,to);
        to->unref();

        oc->mySheets->switchPipSheet(TRACKER_CHOICE);    
        oc->objectcalRoot->removeAllChildren();

    }
    else if (oc->pip3Buttons[NO] == cb) {
        oc->resetObject();
        oc->mySheets->switchPipSheet(TRACKER_CHOICE);               
        oc->objectcalRoot->removeAllChildren();
    }

    else if (oc->pip3Buttons[CANCEL] == cb) {
        oc->mySheets->switchPipSheet(CALIBRATING);    
    }            
}


void objectCalibration::startObjectCalibration(void)
{
    SoSeparator *objectCalibGraph = new SoSeparator;        
    objectcalRoot->addChild(objectCalibGraph);

    // save old offset
    offsetOld->translation = object->getOffset()->translation.getValue();
    offsetOld->rotation = object->getOffset()->rotation.getValue();

    // set key-event-handler
    SoEventCallback *mEvent = new SoEventCallback;
    objectcalRoot->addChild(mEvent);
    mEvent->addEventCallback(SoKeyboardEvent::getClassTypeId(), objectcalKeyEventHandle, this);        

    mySheets->switchPipSheet(CALIBRATING);    
}



void objectCalibration::trackerCB(void *data, So3DButton *tb)
{
    int i;
    objectCalibration *oc = (objectCalibration *)data;    

    for (i=0; i<MAX_TRACKER_BUTTONS; i++) 
        if (oc->trackerButton[i] == tb)
            break;
    SoUserKit *user;
    user = SoUserKit::getUserKit(oc->comMan->getUserFromStation(i));
    if (user) {
        if (oc->comMan->getPenFromUser(user->getUserID()) == i)
            oc->object = user->getPen();
        else if (oc->comMan->getPipFromUser(user->getUserID()) == i)
            oc->object = user->getPip();        
        else if (oc->comMan->getHmdFromUser(user->getUserID()) == i)
            oc->object = user->getDisplay();
    }
    else
        return;
    oc->startObjectCalibration();
}


SoPipSheetKit *objectCalibration::setupPipSheet(void)
{
    mySheets = new SoPipSheetKit();

    // set TRACKER_CHOICE sheet
    SoSeparator *pipSep = new SoSeparator;

    char ivName[512];
    strcpy(ivName, comMan->workingDirectory);
    strcat(ivName, "trackerbutton.iv");

    char name[16];
    int  i;

    for (i=0;i<MAX_TRACKER_BUTTONS;i++) {
        trackerButton[i] = (So3DButton *)readNodeFromFile(ivName);
        trackerButton[i]->setName("");

        sprintf(name, "Tracker %d",i);

        trackerButton[i]->setText(createText(name));
        trackerButton[i]->setPressedText(createPressedText(name));

        trackerButton[i]->addReleaseCallback(trackerCB, (void *)this);
    }

    SoTranslation *zTrans = new SoTranslation;
    zTrans->translation.setValue(-14.0*(MAX_TRACKER_BUTTONS/2-1),0,14);

    SoTranslation *xTrans = new SoTranslation;
    xTrans->translation.setValue(14,0,0);
 
    SoRotationXYZ *startRot = new SoRotationXYZ;
    startRot->axis = SoRotationXYZ::X;
    startRot->angle = 1.57;

    pipSep->addChild(startRot);

    for (i=0;i<MAX_TRACKER_BUTTONS/2;i++) {
        pipSep->addChild(trackerButton[i]);
        if (i<(MAX_TRACKER_BUTTONS/2-1)) 
           pipSep->addChild(xTrans);
    }        

    pipSep->addChild(zTrans);

    for (i=MAX_TRACKER_BUTTONS/2;i<MAX_TRACKER_BUTTONS;i++) {
        pipSep->addChild(trackerButton[i]);
     if (i<MAX_TRACKER_BUTTONS-1) 
        pipSep->addChild(xTrans);
    }        

    mySheets->addSheet(pipSep);
    
// CALIBRATING  ---------------------------

    pipSep = new SoSeparator;
    pipSep->addChild(startRot);
    pipSep->addChild(createText("calibrating ..."));
    mySheets->addSheet(pipSep);    

// CONFIRM_DIALOG  ---------------------------

    strcpy(ivName, comMan->workingDirectory);
    strcat(ivName, "objectcal_pip3.iv");
    pipSep = readFile(ivName);
    
    // read Buttons
    pip3Buttons[0] = (So3DButton *) 
        SoNode::getByName("YES_BUTTON");        
    pip3Buttons[1] = (So3DButton *) 
        SoNode::getByName("NO_BUTTON");        
    pip3Buttons[2] = (So3DButton *) 
        SoNode::getByName("CANCEL_BUTTON");        

    pip3Buttons[0]->setName("");
    pip3Buttons[1]->setName("");
    pip3Buttons[2]->setName("");

    pip3Buttons[0]->setText(createText("YES"));
    pip3Buttons[0]->setPressedText(createPressedText("YES"));
    pip3Buttons[1]->setText(createText("NO"));
    pip3Buttons[1]->setPressedText(createPressedText("NO"));
    pip3Buttons[2]->setText(createText("CANCEL"));
    pip3Buttons[2]->setPressedText(createPressedText("CANCEL"));

    // set callbacks 
    for (i=0;i<3;i++) 
        pip3Buttons[i]->addReleaseCallback(confirmSheetCB, this);

    mySheets->addSheet(pipSep);

//    -------------------------------------------

    mySheets->pipParts = SoPipSheetKit::FULL ^ SoPipSheetKit::WINDOW_TABS ^
                    SoPipSheetKit::SHEET_TABS;
    return mySheets;    
}

//-----------------------------------------------------------------------
//--- Message handler routine of workspace application                  ---
//-----------------------------------------------------------------------
void OBJECTCALIBRATION_API Stb_getMessage(void *, int message , int , void *data)
{
    if (message == EXIT_APPLICATION) {
        printf("stopping message received\n");
    }
}


int appRunning[MAX_USERS] = {0,0,0,0};

//-----------------------------------------------------------------------
//--- Initialization routine of workspace application                       ---
//-----------------------------------------------------------------------
void OBJECTCALIBRATION_API Stb_init(int userID, StbCommunicator *cMan)
{
    objectCalibration *userData = new objectCalibration();
    cMan->setExitData((void *)userData);

    if (appRunning[SoUserKit::getUserKitIndex(userID)]) {
        userData->quickStop = 1;
        printf("Object calibration is already running\n");
        cMan->stopApplication(cMan->appID, TRUE);
        return;
    }
    else {
        userData->quickStop = 0;
        appRunning[SoUserKit::getUserKitIndex(userID)] = 1;
        printf("starting application: Object calibration\n");
    }

    userData->comMan = cMan;
    userData->user = userID;

	WindowCreate wc;
	SoWindowKit *windowKit = NULL;

	wc.position.setValue(0,0,0);
	wc.orientation = SbRotation::identity();
	wc.size.setValue(0,0,0);
	wc.style = WS_NOTITLE | WS_NOBORDER;
	wc.state = BM_NORMAL;
	wc.title = SbString("");
	wc.eventMask = Base3D::BM_ALL_EVENTS;
	wc.stationMask = Base3D::BM_ALL_STATIONS;
	wc.clientEventMask = Base3D::BM_ALL_EVENTS;
	wc.clientStationMask = Base3D::BM_ALL_STATIONS;

    windowKit = cMan->createWindow(NULL, &wc, NULL, NULL);
    userData->window = windowKit;
    userData->window->maximize();

    // setup basic application graph
    SoSeparator *root = windowKit->getClientVolumeSeparator();
    userData->objectcalRoot = root;

    // create application pip sheet    
    cMan->setPipGeometry(userID, userData->setupPipSheet());

}


//-----------------------------------------------------------------------
//--- Cleanup routine of workspace application                            ---
//-----------------------------------------------------------------------
SbBool OBJECTCALIBRATION_API Stb_exit(void *data)
{
    objectCalibration *userData = (objectCalibration *)data;
    delete(userData);
    printf("stopping application\n");
    return TRUE;
}
