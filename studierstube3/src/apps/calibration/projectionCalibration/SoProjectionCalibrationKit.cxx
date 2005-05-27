// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoProjectionCalibrationKit.cxx
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rcs Rainer Splechtna
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#include <cassert>

#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <stbapi/widgets/So3DButton.h>
#include <stbapi/util/ivio.h>

#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/resource/viewer/SoDisplayMode.h>
#include <stbapi/resource/viewer/SoTrackedViewpointControlMode.h>
#include <stbapi/resource/viewer/SoTrackedViewpointMobileDisplayControlMode.h>
#include <stbapi/util/zpthread.h>

#include "../utils/cameraOptimizer.h"
#include "../utils/So2DMenueKit.h"
#include "SoProjectionCalibrationKit.h"
#include "SoAngularConstraintEngine.h"

#define UpperL 0
#define LowerL 1
#define LowerR 2
#define ORIGIN 3

//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoProjectionCalibrationKit);

//--initialization of context in Studierstube
//CONTEXT_APPLICATION(SoProjectionCalibrationKit);

//----------------------------------------------------------------------------

void
SoProjectionCalibrationKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoProjectionCalibrationKit, SoContextKit, "SoContextKit");
}

//----------------------------------------------------------------------------

SoContextKit*
SoProjectionCalibrationKit::factory()
{
    return new SoProjectionCalibrationKit;  
}

//----------------------------------------------------------------------------

SoProjectionCalibrationKit::SoProjectionCalibrationKit()
:
newCamera(NULL), oldCamera(NULL),
root(NULL), appState(START), stereoMode(FALSE),
viewer(NULL), pButton(0), sButton(1), viewportSize(1,1),
leftGraph(NULL), rightGraph(NULL), calibPoint(0),
textMode(FALSE), sampler(NULL), menueHook(NULL)
{
    SO_KIT_CONSTRUCTOR(SoProjectionCalibrationKit);
    // These fields' values may be read from file
    SO_KIT_ADD_FIELD(referencePointsPlane, (0,0));
    SO_KIT_ADD_FIELD(referencePointsEye, (0,0));
    SO_KIT_ADD_FIELD(markerDistance, (0.2));
    SO_KIT_ADD_FIELD(epsilon, (1.0));
    SO_KIT_ADD_FIELD(enabledColor, (1,0,0));
    SO_KIT_ADD_FIELD(disabledColor, (0.8,0.8,0.8));
    SO_KIT_ADD_FIELD(validDistance, (0.0125));
    SO_KIT_ADD_FIELD(minimumAngle, (0.35));
    SO_KIT_ADD_FIELD(dataDirectory, ("../apps/calibration/projectionCalibration/"));
    // only internally used 
    SO_KIT_ADD_FIELD(validPenPosition, (TRUE));

    SO_KIT_INIT_INSTANCE();

    SoMessageKit::initClass();
    So2DMenueKit::initClass();
}

//----------------------------------------------------------------------------

SoProjectionCalibrationKit::~SoProjectionCalibrationKit()
{
    if (appState == SAMPLING_LEFT_EYE_DATA || 
        appState == SAMPLING_RIGHT_EYE_DATA || 
        appState == SAMPLING_PLANE_DATA)
        remove2DCamera();

    if (sampler)
        delete(sampler);

    if (newCamera)
        newCamera->unref();
    
    if (oldCamera)
        oldCamera->unref();

    if (menueHook)
        SoUserManagerKit::getInstance()->getUserKit(userID.getValue())
            ->exclusiveGeometryRoot->removeChild(menueHook);

    //switchFocusToSuite(userID.getValue());
}

//----------------------------------------------------------------------------

SbBool
SoProjectionCalibrationKit::checkPipGeometry()
{
	// init menues
	menues[0] = this->loadMenue("menue1.iv");
	menues[1] = this->loadMenue("menue2.iv");

	switchMenue(MAIN);
	
    return TRUE;
}
    
//----------------------------------------------------------------------------

SbBool 
SoProjectionCalibrationKit::checkWindowGeometry()
{
    // retrieve used viewer
    viewer = SoUserManagerKit::getInstance()->getUserKit(userID.getValue())->
        getDisplay()->getViewer();
    if (!viewer)
    {
        fprintf(stderr, "pathological case: no viewer\n");
        exit(1);
    }
	
    // comment the following statements out to test with desktopcontrol-mode 
    if (!( (viewer->getCameraControlMode()->
            isOfType(SoTrackedViewpointControlMode::getClassTypeId()))
       ||  (viewer->getCameraControlMode()->
            isOfType(SoTrackedViewpointMobileDisplayControlMode::getClassTypeId())) ))
    {
		const char* actCamMode=viewer->getCameraControlMode()->getTypeId().getName().getString();
		fprintf(stderr, "Detected ClassTypeID %s\n",actCamMode );
		fprintf(stderr, "this application does only support camera control modes:\n\
            TrackedViewpointControlMode and TrackedViewpointMobileDisplayControlMode\n");
		exit(1);
	}

    // get and store display and camera control state
    if (viewer->getDisplayMode()->isStereoMode())
        stereoMode = TRUE;

    // get and store the primary and secondary button of the pen
    pButton = SoUserManagerKit::getInstance()->getUserKit(userID.getValue())->
        getPen()->primaryButton.getValue();
    sButton = SoUserManagerKit::getInstance()->getUserKit(userID.getValue())->
        getPen()->secondaryButton.getValue();

    // setup basic application graph
    assert(getWindow(0));
    SoWindowKit* wk = getWindow(0);
    //root = wk->getClientVolume();
    root = wk->SoWindowKit::getClientVolume();
    saveOldCamera();

    initCalibrationGraph();
          
    return TRUE;
}

//----------------------------------------------------------------------------

void
SoProjectionCalibrationKit::checkPipConnections( int uid, SoNode * pipSheetContents )
{
    if (uid != userID.getValue())
        return;

    SoNode *psk = pipSheetContents;
}

//----------------------------------------------------------------------------

void 
SoProjectionCalibrationKit::checkPipMasterMode( SoNode * pipSheetContents, SbBool masterMode )
{
    // only the user, who started the application gets a sheet,
    // hence nothing has to be done in this method
}

//----------------------------------------------------------------------------

void
SoProjectionCalibrationKit::menueCB(void *data, So2DMenueKit *m)
{
    SoProjectionCalibrationKit *actual = (SoProjectionCalibrationKit *)data;
    if (actual->appState == START)
        switch (m->getSelectedEntry())
        {
            case START_FULL:
                actual->startProjectionPlaneCalibration();
                break;
            case START_EYE:
                actual->startEyePositionCalibration();
                break;
            case EXIT:
                actual->scheduleDelete();
                break;
            default: break;
        }
    else if (actual->appState == SAMPLING_FINISHED)
        switch (m->getSelectedEntry())
        {
            case EXIT_KEEP:
				fprintf(stderr, "exit and keep chosen\n");
                actual->scheduleDelete();
                break;
            case EXIT_DUMP:
                actual->setActiveCamera(actual->oldCamera);
                actual->scheduleDelete();
                break;
            case RESTART:
                actual->setActiveCamera(actual->oldCamera);
                actual->newCamera->unref();
                actual->newCamera = NULL;
                actual->appState = START;
				actual->switchMenue(MAIN);
                //actual->initMenue(actual->userID.getValue(), "menue1.iv");
                break;
            default: break;
        }
}

//----------------------------------------------------------------------------

So2DMenueKit * 
SoProjectionCalibrationKit::loadMenue(SbString name)
{
	if (!menueHook)
	{
		menueHook = new SoSeparator;
		SoUserManagerKit::getInstance()->getUserKit(userID.getValue())
		   ->exclusiveGeometryRoot->addChild(menueHook);
	}
    So2DMenueKit *menue = (So2DMenueKit *)readNodeFromFile
        (name.getString(), dataDirectory.getValue().getString());
    if (menue == NULL)
        return NULL;

    menue->setCallback(menueCB, this);
	menueHook->addChild(menue);

	return menue;
}

//----------------------------------------------------------------------------

void 
SoProjectionCalibrationKit::switchMenue(enum menues which)
{
	// first disable all menues
	for (int i=0; i<2; i++)
	{
		menues[i]->enableMenue(FALSE);
		menues[i]->enableButtonControl(FALSE, -1);
	}

	// now enable the specified menue, if not all menues should stay disabled 
	if (which != DISABLED)
	{
		menues[which]->enableMenue(TRUE);
		menues[which]->enableButtonControl(TRUE, userID.getValue());
	}
}    

//----------------------------------------------------------------------------
// CAMERA MANIPULATIONS
//----------------------------------------------------------------------------
// Build orthographic cameras and use them

void 
SoProjectionCalibrationKit::set2DCamera()
{
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
    mover->translation.enableConnection(FALSE);
    mover->rotation.enableConnection(FALSE);
    mover->translation.setValue(0,0,0);
    mover->rotation.setValue(0,0,0,1);
}

//----------------------------------------------------------------------------

void 
SoProjectionCalibrationKit::remove2DCamera()
{
    viewer->getStereoCameraKit()->getStereoSwitch()->removeChild(0);
    viewer->getStereoCameraKit()->getStereoSwitch()->removeChild(0);
    // enable mover again
    viewer->getStereoCameraKit()->getTransform()->translation.enableConnection(TRUE);
    viewer->getStereoCameraKit()->getTransform()->rotation.enableConnection(TRUE);
}

//----------------------------------------------------------------------------

void 
SoProjectionCalibrationKit::setActiveCamera(SoStereoCameraKit *camKit)
{
    viewer->getStereoCameraKit()->getCameraLeft()->
        copyFieldValues(camKit->getCameraLeft());
    viewer->getStereoCameraKit()->getCameraRight()->
        copyFieldValues(camKit->getCameraRight());

	viewer->getStereoCameraKit()->eyeOffsetLeft = camKit->eyeOffsetLeft.getValue();	
	viewer->getStereoCameraKit()->eyeOffsetRight = camKit->eyeOffsetRight.getValue();
	viewer->getStereoCameraKit()->displayOffsetLeft = camKit->displayOffsetLeft.getValue();	
	viewer->getStereoCameraKit()->displayOffsetRight = camKit->displayOffsetRight.getValue();	
	viewer->getStereoCameraKit()->displayRotationLeft = camKit->displayRotationLeft.getValue();	
	viewer->getStereoCameraKit()->displayRotationRight = camKit->displayRotationRight.getValue();	
}

//----------------------------------------------------------------------------

void 
SoProjectionCalibrationKit::saveOldCamera()
{
    oldCamera = new SoStereoCameraKit;        
    oldCamera->ref(); 
    oldCamera->getCameraLeft()->copyFieldValues(
        viewer->getStereoCameraKit()->getCameraLeft());    
    oldCamera->getCameraRight()->copyFieldValues(
        viewer->getStereoCameraKit()->getCameraRight());    

	oldCamera->eyeOffsetLeft = viewer->getStereoCameraKit()->eyeOffsetLeft.getValue();	
	oldCamera->eyeOffsetRight = viewer->getStereoCameraKit()->eyeOffsetRight.getValue();	
	oldCamera->displayOffsetLeft = viewer->getStereoCameraKit()->displayOffsetLeft.getValue();	
	oldCamera->displayOffsetRight = viewer->getStereoCameraKit()->displayOffsetRight.getValue();	
	oldCamera->displayRotationLeft = viewer->getStereoCameraKit()->displayRotationLeft.getValue();	
	oldCamera->displayRotationRight = viewer->getStereoCameraKit()->displayRotationRight.getValue();	
}

//----------------------------------------------------------------------------

void 
SoProjectionCalibrationKit::initCalibrationGraph()
{
    SoEventCallback *event = new SoEventCallback;
    root->addChild(event);
    event->addEventCallback(SoKeyboardEvent::getClassTypeId(), keyEventCB, this);              
	// TODO is this necessary ???
	event->addEventCallback(SoMouseButtonEvent::getClassTypeId(), mouseButtonEventCB, this);              

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
SoProjectionCalibrationKit::initSampler()
{
    if (sampler)
        sampler->clearAllSamples();
    else
    {
        SoMFInt32 stations;
        int penStation;
        stations.setValue(penStation = SoUserManagerKit::getInstance()->
            getPenFromUser(userID.getValue()));
        stations.set1Value(1, SoUserManagerKit::getInstance()->
            getHmdFromUser(userID.getValue()));

		// stations[2] is for display tracker data
		// if the display is tracked 
		if (viewer->getCameraControlMode()->
			isOfType(SoTrackedViewpointMobileDisplayControlMode::getClassTypeId()))
		{
			int displayStation=((SoTrackedViewpointMobileDisplayControlMode*)
				viewer->getCameraControlMode())->displayStation.getValue();
			stations.set1Value(2,displayStation);
		}

        sampler = new SoSampler;
        sampler->setStationsToSample(stations);
        sampler->initButtonTriggering(penStation, pButton, FALSE, FALSE,
                                      buttonTriggered,this);
    }
}

//----------------------------------------------------------------------------

void 
SoProjectionCalibrationKit::startProjectionPlaneCalibration()
{
    SbVec2s vps;
    vps = viewer->getViewportRegion().getViewportSizePixels();
    viewportSize.setValue(vps[0],vps[1]);

    printf("vpX: %f vpY: %f\n", viewportSize[0], viewportSize[1]);

    calibPoint = 0;
    validPenPosition = TRUE;
    appState = SAMPLING_PLANE_DATA;
	switchMenue(DISABLED);
    set2DCamera();

    displayText("Press button to start projection plane calibration !");
    
    initSampler();
    sampler->startButtonTriggering();
}

//----------------------------------------------------------------------------

void 
SoProjectionCalibrationKit::startEyePositionCalibration()
{
    if (appState == START)
    {
        SbVec2s vps = viewer->getViewportRegion().getViewportSizePixels();
        viewportSize.setValue(vps[0],vps[1]);
        printf("vpX: %f vpY: %f\n", viewportSize[0], viewportSize[1]);
    }

    if (appState != SAMPLING_PLANE_DATA)
        set2DCamera();

    if (newCamera == NULL)
    {
        newCamera = new SoStereoCameraKit;
        newCamera->ref();
        newCamera->getCameraLeft()->copyFieldValues(oldCamera->getCameraLeft());    
        newCamera->getCameraRight()->copyFieldValues(oldCamera->getCameraRight());    
		// was missing
		newCamera->displayOffsetLeft=oldCamera->displayOffsetLeft;
		newCamera->displayOffsetRight=oldCamera->displayOffsetRight;
		newCamera->displayRotationLeft=oldCamera->displayRotationLeft;
		newCamera->displayRotationRight=oldCamera->displayRotationRight;
    }

	switchMenue(DISABLED);
 
    calibPoint = 0;
	raysP0.deleteValues(0);
    raysP1.deleteValues(0);
    appState = SAMPLING_LEFT_EYE_DATA;

    displayText("Press button to start LEFT eye point calibration !");

    initSampler();
    sampler->startButtonTriggering();
}

//----------------------------------------------------------------------------
/**
 * transforms the pen samples into display tracker space
 */
void
SoProjectionCalibrationKit::samplesIntoDisplayTrackSpace()
{
    if (!viewer->getCameraControlMode()->
        isOfType(SoTrackedViewpointMobileDisplayControlMode::getClassTypeId()))
    { // if the display is not tracked 
		return;
	}
	else
	{
		int penStation = SoUserManagerKit::getInstance()->
			getPenFromUser(userID.getValue());
		int displayStation = ((SoTrackedViewpointMobileDisplayControlMode*)
			viewer->getCameraControlMode())->displayStation.getValue();
		SbVec3f posTracker,posSample;
		SbVec3f v0;
		SbRotation rotTracker;
		for (int i=0;i<sampler->samples[displayStation].translations.getNum();i++)
		{
			posTracker   = sampler->samples[displayStation].translations[i];
			rotTracker   = sampler->samples[displayStation].rotations[i];
			posSample   = sampler->samples[penStation].translations[i];
			// transformation into tracker space
			v0 = posSample - posTracker;
			(rotTracker.inverse().multVec(v0, posSample)) ;

			sampler->samples[penStation].translations.set1Value(i,posSample);
		}	
	}
}

//----------------------------------------------------------------------------

void
SoProjectionCalibrationKit::buttonTriggered(void *data)
{
    SoProjectionCalibrationKit *actual = (SoProjectionCalibrationKit *)data;
    actual->calibControl(actual->pButton);  
}

//----------------------------------------------------------------------------

void 
SoProjectionCalibrationKit::keyEventCB(void *data, SoEventCallback *myEvent) 
{
    const SoEvent *event = myEvent->getEvent();
    SoKeyboardEvent::Key pressed;
 
    SoProjectionCalibrationKit *actual = (SoProjectionCalibrationKit *)data;

    if (SoKeyboardEvent::isKeyPressEvent(event, SoKeyboardEvent::ANY)) {
        pressed = ((SoKeyboardEvent *)event)->getKey(); 
        switch (pressed)
        {
            case SoKeyboardEvent::Q:
                actual->scheduleDelete();
                break;
            case SoKeyboardEvent::S:
                actual->startProjectionPlaneCalibration();
                break;
            case SoKeyboardEvent::E:
                actual->startEyePositionCalibration();
                break;
            case SoKeyboardEvent::SPACE:
                actual->calibControl(actual->pButton);
                break;
            case SoKeyboardEvent::BACKSPACE:
                actual->calibControl(actual->sButton);
                break;
            default: break;
        }
    }

    myEvent->setHandled();
}
//----------------------------------------------------------------------------
void 
SoProjectionCalibrationKit::mouseButtonEventCB(void *data, SoEventCallback *myEvent) 
{
    const SoEvent *event = myEvent->getEvent();
    SoProjectionCalibrationKit *actual = (SoProjectionCalibrationKit*)data;

	if(SoMouseButtonEvent::isButtonPressEvent (event, SoMouseButtonEvent::ANY))
	{
		// fprintf(stderr, "SoMouseButtonEvent!\n ");
		SoMouseButtonEvent::Button pressed;
		pressed = ((SoMouseButtonEvent *)event)->getButton();
		switch (pressed)
		{
		case SoMouseButtonEvent::BUTTON2: //middle button
		//case SoMouseButtonEvent::BUTTON4: //scrollwheel up !!just coin, not OInventor
			break;
		case SoMouseButtonEvent::BUTTON3: //rightmost button
		//case SoMouseButtonEvent::BUTTON5: //scrollwheel down !!just coin, not OInventor
			actual->calibControl(actual->sButton);
			break;
		case SoMouseButtonEvent::BUTTON1: //leftmost button
			actual->calibControl(actual->pButton);
			break;
		default:break;
		myEvent->setHandled();
		}
	}
}
//----------------------------------------------------------------------------

void 
SoProjectionCalibrationKit::calibFinished() 
{
    appState = SAMPLING_FINISHED;    
    remove2DCamera();
    assert(newCamera);
    setActiveCamera(newCamera);

	// TODO
    //((SoPipSheetKit *)getPipSheet(userID.getValue()))->switchPipSheet(FIRST);
    //initMenue(userID.getValue(), "menue2.iv");
	switchMenue(POST_CALIB_DIALOG);
}

//----------------------------------------------------------------------------
void 
SoProjectionCalibrationKit::calibControl(int button) 
{
    if (appState == START ||
        appState == SAMPLING_FINISHED)
        return;
         
    if (textMode) 
    {
        textMode = FALSE;
        displayPoint();
        return;
    }
        
    if ((button == pButton) && validPenPosition.getValue()) 
    {
        sampler->sample();
        if (appState == SAMPLING_LEFT_EYE_DATA ||
            appState == SAMPLING_RIGHT_EYE_DATA)
            calcRay();
        calibPoint++; 
        if (appState == SAMPLING_PLANE_DATA)
            if (calibPoint == referencePointsPlane.getNum())     
            {
                sampler->stopButtonTriggering();
                leftGraph->removeAllChildren();
                calcPlane();            
                startEyePositionCalibration();
            }
            else
                displayPoint();
        else
            if (calibPoint == referencePointsEye.getNum())     
            {
                sampler->stopButtonTriggering();
                // to delete internal calculator engine
                validPenPosition.disconnect(); 
                if (calcEye() == FALSE)
				{
					calibPoint = 0;
					raysP0.deleteValues(0);
					raysP1.deleteValues(0);
					
					displayText("Press button to repeat eye point calibration !");
					
					initSampler();
					sampler->startButtonTriggering();
				}	
				else 
				{
					if (appState == SAMPLING_LEFT_EYE_DATA)
					{
						leftGraph->removeAllChildren();
						if (stereoMode)
						{
							calibPoint = 0;
							raysP0.deleteValues(0);
							raysP1.deleteValues(0);
							
							appState = SAMPLING_RIGHT_EYE_DATA;
							
							displayText("Press button to start RIGHT eye point calibration !");
								
							initSampler();
							sampler->startButtonTriggering();
						}
						else
						{
							calibFinished();
						}
					}
					else if (appState == SAMPLING_RIGHT_EYE_DATA)
					{
						rightGraph->removeAllChildren();
						calibFinished();
					}       
				}
            }
            else
                displayPoint();
    }
    else if (button == sButton) 
    {
        if (calibPoint > 0) 
        {
            calibPoint--;
            if (appState == SAMPLING_LEFT_EYE_DATA ||
                appState == SAMPLING_RIGHT_EYE_DATA)
            {
                raysP0.deleteValues(calibPoint);
                raysP1.deleteValues(calibPoint);
            }
            sampler->stepBack();
            displayPoint();
        }
    }
}

//----------------------------------------------------------------------------

SoSeparator *
SoProjectionCalibrationKit::build3Dpoint(const SbVec2f &point2D)
{
    SoSeparator *point3D = new SoSeparator;

    // set position of point in camera space
    SoTranslation *trans = new SoTranslation;
    // get position in camera space and scale factor
    float x = point2D[0]*viewportSize[0] - viewportSize[0]/2.0;
    float y = point2D[1]*viewportSize[1] - viewportSize[1]/2.0;

    trans->translation = (SbVec3f(x,y,-100)); 
    point3D->addChild(trans);

    // add shape 
    // TODO: just read once
    char mName[512];
    strcpy(mName, dataDirectory.getValue().getString());
    if (appState == SAMPLING_PLANE_DATA)
        strcat(mName, "markerPlane.iv");
    else
    {
        strcat(mName, "markerEye.iv");
        point3D->addChild(createDistanceSensitiveColor());
    }
    SoSeparator *marker;
    marker = readFile(mName);
    if (marker == NULL) 
        exit(1);

    point3D->addChild(marker);

    return point3D;
}

//----------------------------------------------------------------------------

SoMaterial *
SoProjectionCalibrationKit::createDistanceSensitiveColor()
{
  SoTrakEngine *pen = new SoTrakEngine();  
  pen->
    station.setValue(SoUserManagerKit::getInstance()->getPenFromUser(userID.getValue()));
  SoTrakEngine *head = new SoTrakEngine();
  head->
    station.setValue(SoUserManagerKit::getInstance()->getHmdFromUser(userID.getValue()));

    SoAngularConstraint *ac = new SoAngularConstraint;
    ac->headPosition.connectFrom(&head->translation);
    ac->headRotation.connectFrom(&head->rotation);
    ac->penPosition.connectFrom(&pen->translation);
    ac->raysP0.connectFrom(&raysP0);
    ac->raysP1.connectFrom(&raysP1);
    ac->minimumAngle.connectFrom(&minimumAngle);
    ac->validDistance.connectFrom(&validDistance);
    ac->markerPosition.connectFrom(&newCamera->getCameraLeft()->position);
    
    SoCalculator *calc = new SoCalculator;
    calc->A.connectFrom(&pen->translation);
    calc->B.connectFrom(&head->translation);
    calc->C.connectFrom(&enabledColor);
    calc->D.connectFrom(&disabledColor);
    calc->a.connectFrom(&markerDistance);
    calc->b.connectFrom(&epsilon);
    calc->c.connectFrom(&ac->valid);
    calc->expression.setValue("ta = length(A-B);\
                               tb = (ta > a-b) && (ta < a+b) && c;\
                               oC = tb > 0 ? C : D;\
                               oa = tb");

    SoMaterial *material = new SoMaterial;
    material->emissiveColor.connectFrom(&calc->oC);
    material->diffuseColor.setValue(0,0,0);
    material->ambientColor.setValue(0,0,0);

    validPenPosition.connectFrom(&calc->oa);

    return material;
}


//----------------------------------------------------------------------------

SoSeparator *
SoProjectionCalibrationKit::build3DText(const SoMFString &text)
{
    // set font
    SoSeparator *textRoot = new SoSeparator;
  
    SoNode *textFormat;
    textFormat = readNodeFromFile("textFormat.iv", dataDirectory.getValue().getString());
    if (!textFormat)
        exit(-1);
    textRoot->addChild(textFormat);

    SoText3 *_text = new SoText3;
    
    _text->string = text;//.setValue(name);
    _text->justification = SoText3::CENTER;
    textRoot->addChild(_text);

    return textRoot;
}

//----------------------------------------------------------------------------

void 
SoProjectionCalibrationKit::displayPoint() 
{
    SoSeparator *root;
    if (appState ==  SAMPLING_LEFT_EYE_DATA || 
        appState ==  SAMPLING_PLANE_DATA)
        root = leftGraph;
    else
        root = rightGraph;
    root->removeAllChildren();
    if (appState ==  SAMPLING_PLANE_DATA)
        root->addChild(
            build3Dpoint(referencePointsPlane[calibPoint]));
    else
    {
        root->addChild(
            build3Dpoint(referencePointsEye[calibPoint]));
        char t[10];
        SoMFString text;
        sprintf(t, "Point: %d", calibPoint+1);
        text.set1Value(4,t);
        root->addChild(build3DText(text));
    }
}

//----------------------------------------------------------------------------

void 
SoProjectionCalibrationKit::displayText(const char *text)
{
    SoSeparator *root;
    if (appState ==  SAMPLING_RIGHT_EYE_DATA)
        root = rightGraph;
    else
        root = leftGraph;
    root->removeAllChildren();
    SoMFString help;
    help.setValue(text);
    root->addChild(build3DText(help));
    textMode = TRUE;
}

//---------------------------------------------------------------------------

void 
SoProjectionCalibrationKit::calcRay()
{
    int penStation = SoUserManagerKit::getInstance()->
        getPenFromUser(userID.getValue());
    int headStation = SoUserManagerKit::getInstance()->
        getHmdFromUser(userID.getValue());

    SbVec3f    headPos   = sampler->samples[headStation].translations[calibPoint];
    SbRotation headRot   = sampler->samples[headStation].rotations[calibPoint];
    SbVec3f    penPos    = sampler->samples[penStation].translations[calibPoint];

	// was missing from axel's code
	SbVec3f    markerPos;
	if (viewer->getCameraControlMode()->
			isOfType(SoTrackedViewpointMobileDisplayControlMode::getClassTypeId()))
	{
		int displayStation=((SoTrackedViewpointMobileDisplayControlMode*)
			viewer->getCameraControlMode())->displayStation.getValue();
	    SbVec3f    displayPos   = sampler->samples[displayStation].translations[calibPoint];
		SbRotation displayRot   = sampler->samples[displayStation].rotations[calibPoint];
		// calc marker position in world coordinates
		markerPos = newCamera->displayOffsetLeft.getValue();
		displayRot.multVec(markerPos, markerPos);
		markerPos += displayPos;
	}
	else
	    markerPos = newCamera->getCameraLeft()->position.getValue();
	SbVec3f p0, p1;
    headRot.inverse().multVec(penPos    - headPos, p0);
    headRot.inverse().multVec(markerPos - headPos, p1);

    assert(calibPoint == raysP0.getNum());
    raysP0.set1Value(calibPoint, p0);
    raysP1.set1Value(calibPoint ,p1);
}

//---------------------------------------------------------------------------

SbBool
SoProjectionCalibrationKit::calcEye()
{
    SoMFVec3f samples;
    int i;

    for(i=0; i < raysP0.getNum(); i++)
        samples.set1Value(i, raysP0[i]);
    for(i=0; i < raysP1.getNum(); i++)
        samples.set1Value(i+raysP0.getNum(), raysP1[i]);

    SbVec3f eyeOffset;
    eyeOffset = calculateEyepointPosition(
        samples, validDistance.getValue(), TRUE);
	if (eyeOffset == SbVec3f(-1,-1,-1))
		return FALSE;

    if (appState == SAMPLING_LEFT_EYE_DATA)
    {
        newCamera->eyeOffsetLeft = eyeOffset;
        sampler->logSampledData("eyeSamples.txt");
    }
    else if (appState == SAMPLING_RIGHT_EYE_DATA)
    {
        newCamera->eyeOffsetRight = eyeOffset;
        sampler->logSampledData("eyeSamples.txt", TRUE);
    }

    char dataFile[1000];
    // save cameraKit to file "user[userID]ProjectionCamera.iv" in 
    // the working directory
    sprintf(dataFile,"%suser%dProjectionCamera.iv", 
            dataDirectory.getValue().getString(), userID.getValue());
    printf("writing camera to %s ...\n", dataFile);
    writeFile(dataFile, newCamera); 

	return TRUE;
};

//---------------------------------------------------------------------------
/**
 * does the calculations for finding the center and orientation of the display plane
 */
void 
SoProjectionCalibrationKit::calcPlane()
{
	if (viewer->getCameraControlMode()->
			isOfType(SoTrackedViewpointMobileDisplayControlMode::getClassTypeId()))
		samplesIntoDisplayTrackSpace();
	
	int penStation = SoUserManagerKit::getInstance()->
		getPenFromUser(userID.getValue());

    SbVec3f height,width,x,y,z,tmp;
	/* normal vectors */
    x.setValue(1,0,0);   
    y.setValue(0,1,0);
    z.setValue(0,0,1);

	/* gets a plane and points projected unto it in world coordinates from the penStation samples */
    SbPlane pp;
    SoMFVec3f pop; 
    optimizePlane(sampler->samples[penStation].translations, pp, pop);        
   
	/* takes the first 4 points originating from the pen samples as display corners */
    SbVec3f lb, lt, rt, rb;
    lt = pop[0]; lb = pop[1]; rb = pop[2]; rt = pop[3];

    // the diagonals
	SbLine dbt, dtb;
    dbt.setValue(lb, rt);
    dtb.setValue(lt, rb);
	// takes the ~intersection point(s) and stores them in origin
    SbVec3f origin;
	printf("calcPlane dbt.getClosestPoints(...)\n");
    dbt.getClosestPoints(dtb, origin, origin);
    
    SbRotation r,r2,rr;
    // height of plane (= up/y vector of plane)
    height = lt - lb;
    // width of plane (= x vector of plane)
    width  = rb - lb;
    // rotate y-axis to up vector of plane
    r.setValue(y,height);
    // calc direction of x-axis in rotated space
    r.multVec(x,tmp);
    // rotate x-axis in rotated space to x-axis of plane
    r2.setValue(tmp,width);
    // rotate y-axis to up vector of plane and then
    // rotate x-axis in rotated space to x-axis of plane
    // result = orientation of plane in world/tracker coordinate system
    rr = r*r2;

    float scaleH, scaleV;
   
	// setting the OffAxisCamera
	 // scale according to the coordinates in screen space
	 // referencePointsPlane[marker][coordinate] 
     // 0 - upper left marker        0 - x coordinate
     // 1 - lower left marker        1 - y coordinate
     // 3 - upper right marker
     scaleH = 1.0/(referencePointsPlane[3][0] - referencePointsPlane[0][0]);
     scaleV = 1.0/(referencePointsPlane[0][1] - referencePointsPlane[1][1]);

     SoOffAxisCamera *calcCam = new SoOffAxisCamera;
     calcCam->size.setValue(width.length() *scaleH,
                           height.length()*scaleV);
     calcCam->nearDistance = 0.01;
     calcCam->farDistance = 100;
     calcCam->viewportMapping = SoCamera::LEAVE_ALONE;
     calcCam->aspectRatio.setValue(calcCam->size.getValue()[0]/
                                 calcCam->size.getValue()[1]);
	 // setting position and orientation of display
	 // TODO is this necessary
	 if (viewer->getCameraControlMode()->
			isOfType(SoTrackedViewpointMobileDisplayControlMode::getClassTypeId())){
	 	// for later use without display-tracker 
		// transform back from tracker coordinates
		int displayStation = ((SoTrackedViewpointMobileDisplayControlMode*)viewer->getCameraControlMode())->displayStation.getValue();
		SbVec3f posTracker,posSample;
		SbVec3f v0;
		SbRotation rotTracker;
		// takes tracker data from newest sample
		posTracker   = sampler->samples[displayStation].translations[3];
		rotTracker   = sampler->samples[displayStation].rotations[3];
		(rotTracker.multVec(origin,v0)) ;
		calcCam->position.setValue(v0 + posTracker);
		//calcCam->orientation.setValue(rotTracker*rr);		// wrong 
		calcCam->orientation.setValue(rr*rotTracker);
	}
	else
	{
		// calculated position and orientation of plane are already in world coordinates
		calcCam->position.setValue(origin);
		calcCam->orientation.setValue(rr);
	}

	// fprintf(stderr,"calcPlane before output to files\n");
	// output to files
     newCamera = new SoStereoCameraKit;
     newCamera->ref();

     newCamera->getCameraLeft()->copyFieldValues(calcCam);    
     newCamera->getCameraRight()->copyFieldValues(calcCam);

	 if (viewer->getCameraControlMode()->
			isOfType(SoTrackedViewpointMobileDisplayControlMode::getClassTypeId()))
	 {
		newCamera->displayOffsetLeft=origin;
		newCamera->displayOffsetRight=origin;
		newCamera->displayRotationLeft=rr;
		newCamera->displayRotationRight=rr;
	 }
     char dataFile[1000];
	 printf("calcPlane saving PlaneLog.txt");
     sprintf(dataFile,"%suser%dPlaneLog.txt", 
            dataDirectory.getValue().getString(), userID.getValue());
     sampler->logSampledData(dataFile);
     // save cameraKit to file "user[userID]ProjectionCamera.iv" in 
     // the working directory
     sprintf(dataFile,"%suser%dProjectionCamera.iv", 
             dataDirectory.getValue().getString(), userID.getValue());
     printf("writing camera to %s ...\n", dataFile);
     writeFile(dataFile, newCamera); 
}

//----------------------------------------------------------------------------
//eof

