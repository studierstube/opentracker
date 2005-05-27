// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoPropCalibrationKit.cxx
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
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoFile.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/actions/SoSearchAction.h>

#include <stbapi/util/ivio.h>

#include <stbapi/workspace/StbWorkspace.h>
#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/resource/viewer/SoDisplayMode.h>
#include <stbapi/resource/viewer/SoTrackedViewpointControlMode.h>

#include <stbapi/window3d/SoWindowKit.h>

#include <stbapi/interaction/SoDraggerKit.h>

#include "../powell/nrutil.h"
#include "../powell/powell.h"

#include "SoPropCalibrationKit.h"
#include "../utils/SoMessageKit.h"
#include "../utils/So2DMenueKit.h"
//#include "../switchFocusToSuite.h"


SoNode*
myFindNode(SoNode* root, SbString name)
{
    SoSearchAction sa;
	sa.setSearchingAll(TRUE);
    sa.setName(name);
    sa.apply(root);
    SoFullPath* path = (SoFullPath*)sa.getPath();
    if (path == NULL)
        return NULL;
    else
        return path->getTail();
}



//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoPropCalibrationKit);

//--initialization of context in Studierstube
CONTEXT_APPLICATION(SoPropCalibrationKit);

//----------------------------------------------------------------------------
// static class initialization
//----------------------------------------------------------------------------

void
SoPropCalibrationKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoPropCalibrationKit, SoContextKit, "SoContextKit");
    SoMessageKit::initClass();
    So2DMenueKit::initClass();
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoContextKit*
SoPropCalibrationKit::factory()
{
    return new SoPropCalibrationKit;  
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoPropCalibrationKit::SoPropCalibrationKit()
:
root(NULL), propRoot(NULL), trackedProp(NULL), appState(START), viewer(NULL), 
pButton(0), sButton(1), sampler(NULL),
selectedProp(0), currentFeaturePoint(0), featurePoints(NULL), menueHook(NULL),
messageHook(NULL), draggerActive(false), propDragger(NULL)
{
    SO_KIT_CONSTRUCTOR(SoPropCalibrationKit);
    // These fields' values may be read from file
    SO_KIT_ADD_FIELD(propNames, (""));
	SO_KIT_ADD_FIELD(dataDirectory, ("../apps/calibration/propCalibration/"));
    SO_KIT_ADD_FIELD(use2DMenue, (TRUE));
    //SO_KIT_ADD_FIELD(use2DMenue, (FALSE));
    
    SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoPropCalibrationKit::~SoPropCalibrationKit()
{
    if (sampler)
        delete(sampler);

    if (menueHook)
        SoUserManagerKit::getInstance()->getUserKit(userID.getValue())
            ->exclusiveGeometryRoot->removeChild(menueHook);
    if (messageHook)
        SoUserManagerKit::getInstance()->getUserKit(userID.getValue())
            ->exclusiveGeometryRoot->removeChild(messageHook);
    if (propDragger)
        propDragger->unref();

	//switchFocusToSuite(userID.getValue());
}

//----------------------------------------------------------------------------

void
SoPropCalibrationKit::pipSheetButtonCB(void *data,SoButtonKit *button)
{
    SoPropCalibrationKit *actual = (SoPropCalibrationKit *)data;
	SbString s = button->getName().getString();
    switch (atoi(s.getSubString(1,1).getString()))
    {
    case PREVIOUS:
        actual->selectProp(-1);
        break;
    case NEXT:
        actual->selectProp(1);
        break;
    case REGISTER:
        actual->startPropCalibration();
        break;
    case CANCEL:
        actual->cancelPropCalibration();
        break;
    case BACK:
        actual->stepBack();
        break;
    case SAVE:
        actual->saveProp();
        break;
    case DUMP:
        actual->dumpProp();
        break;
    default: break;
    }
}


//----------------------------------------------------------------------------
// Reads the pip sheet and sets the button callbacks
//----------------------------------------------------------------------------


SbBool
SoPropCalibrationKit::checkPipGeometry()
{
    if (use2DMenue.getValue())
    {
        menues[0] = loadMenue("menue1.iv");
        menues[1] = loadMenue("menue2.iv");
        
        switchMenue(MAIN);

        /*SoUserManagerKit::getInstance()->getUserKit(userID.getValue())->
        getPip();
        SoUserManagerKit::getInstance()->getUserKit(userID.getValue())->
        getPip()->updatePip();
        */

    }
    return TRUE;
}
    
//----------------------------------------------------------------------------

SbBool 
SoPropCalibrationKit::checkWindowGeometry()
{
	propRoot = new SoSeparator;
	SoSeparator *prop;
	for (int i=0; i < propNames.getNum(); i++)
	{
		assert(prop = readFile(propNames[i].getString()));
		propRoot->addChild(prop);
		propNames.set1Value(i, propNames[i]);
		writeFile(propNames[i].getString(), propRoot->getChild(i));
	}

    // get and store the primary and secondary button of the pen
    pButton = SoUserManagerKit::getInstance()->getUserKit(userID.getValue())->
        getPen()->primaryButton.getValue();
    sButton = SoUserManagerKit::getInstance()->getUserKit(userID.getValue())->
        getPen()->secondaryButton.getValue();

    // setup basic application graph
    assert(getWindow(0)), root = getWindow(0)->getClientVolume();

    initCalibrationGraph();


    return TRUE;
}

//----------------------------------------------------------------------------

void
SoPropCalibrationKit::checkPipConnections( int uid, SoNode * pipSheetContents )
{
    if (uid != userID.getValue())
        return;

    SoNode *psk = pipSheetContents;

    // PIP SHEET 1 
    SoPushButton* b = (SoPushButton*)findNode(psk,"PREVIOUS_BUTTON");
    b->setName(SbString(PREVIOUS));
    b->setPressCallback(pipSheetButtonCB, this);
    b = (SoPushButton*)findNode(psk,"NEXT_BUTTON");
    b->setName(SbString(NEXT));
    b->setPressCallback(pipSheetButtonCB, this);
    b = (SoPushButton*)findNode(psk,"REGISTER_BUTTON");
    b->setName(SbString(REGISTER));
    b->setPressCallback(pipSheetButtonCB, this);

    // PIP SHEET 2
    b = (SoPushButton*)findNode(psk,"BACK_BUTTON");
    b->setName(SbString(BACK));
    b->setPressCallback(pipSheetButtonCB, this);
    b = (SoPushButton*)findNode(psk,"CANCEL_BUTTON");
    b->setName(SbString(CANCEL));
    b->setPressCallback(pipSheetButtonCB, this);

    // PIP SHEET 3
    b = (SoPushButton*)findNode(psk,"SAVE_BUTTON");
    b->setName(SbString(SAVE));
    b->setPressCallback(pipSheetButtonCB, this);
    b = (SoPushButton*)findNode(psk,"DUMP_BUTTON");
    b->setName(SbString(DUMP));
    b->setPressCallback(pipSheetButtonCB, this);
}

//----------------------------------------------------------------------------

void 
SoPropCalibrationKit::checkPipMasterMode( SoNode * pipSheetContents, SbBool masterMode )
{
    // only the user, who started the application gets a sheet,
    // hence nothing has to be done in this method
}

//----------------------------------------------------------------------------

SbBool 
SoPropCalibrationKit::windowEventCB(void* data, int messageId, SoWindowKit* win,
                                    int uid, So3DEvent* event, SoPath* path)
{
    if((appState == SAMPLING_FEATURE_DATA) &&
       (messageId == WM_EVENT) &&
       (event->getType() == STB_PRIMARY_BUTTON_EVENT) &&
       (event->getButton(STB_PRIMARY_BUTTON) == TRUE))
    {
        calibControl(pButton);
        return TRUE;
    }
    if((appState == SAMPLING_FEATURE_DATA) &&
       (messageId == WM_EVENT) &&
       (event->getType() == STB_SECONDARY_BUTTON_EVENT) &&
       (event->getButton(STB_SECONDARY_BUTTON) == 2))
    {
        if (appState == SAMPLING_FEATURE_DATA)
            stepBack();
        return TRUE;
    }
    return FALSE;
}

//----------------------------------------------------------------------------

void 
SoPropCalibrationKit::initCalibrationGraph()
{
    SoEventCallback *event = new SoEventCallback;
    root->addChild(event);
    event->addEventCallback(SoKeyboardEvent::getClassTypeId(), keyEventCB, this);              

    propHook = new SoSeparator;
    root->addChild(propHook);
    messageHook = new SoSeparator;
    messageButton = new SoSampler;
    messageButton->initButtonTriggering(SoUserManagerKit::getInstance()->
										getPenFromUser(userID.getValue()),
                                         pButton, TRUE, FALSE,
                                         removeMessageCB, this);

	SoUserManagerKit::getInstance()->getUserKit(userID.getValue())->
		exclusiveGeometryRoot->addChild(messageHook);
    selectProp(0);
}

//----------------------------------------------------------------------------

void
SoPropCalibrationKit::initSampler()
{
    if (sampler)
        delete(sampler);

    SoMFInt32 stations;
    int penStation;
    stations.setValue(penStation = SoUserManagerKit::getInstance()->
										getPenFromUser(userID.getValue()));
    if (trackedProp)
        stations.set1Value(1,trackedProp->station.getValue());
    sampler = new SoSampler;
    sampler->setStationsToSample(stations);
}

//----------------------------------------------------------------------------

SoTrackedArtifactKit *
SoPropCalibrationKit::getTrackedArtifact(SoNode *root)
{
    // search for SoTrackedArtifactKit nodes
    SoSearchAction sAction;
    sAction.setType(SoTrackedArtifactKit::getClassTypeId());
    sAction.setSearchingAll(TRUE);
    sAction.setInterest(SoSearchAction::FIRST);
    sAction.apply(root);
    SoPath *path = sAction.getPath();
    if (path)
        return (SoTrackedArtifactKit *)path->getTail();
    else 
        return NULL;
}

//----------------------------------------------------------------------------
// indexOffset


void 
SoPropCalibrationKit::draggerFinished(void* userData, SoDraggerKit*)
{
    SoPropCalibrationKit *actual = (SoPropCalibrationKit *)userData;
    actual->draggerActive = false;
    printf("fini\n");
}


void 
SoPropCalibrationKit::draggerStarted(void* userData, SoDraggerKit*)
{
    SoPropCalibrationKit *actual = (SoPropCalibrationKit *)userData;
    actual->draggerActive = true;
    printf("start\n");
}


void
SoPropCalibrationKit::selectProp(int indexOffset)
{
    selectedProp += indexOffset;
    selectedProp %= propRoot->getNumChildren();
    if (selectedProp < 0)
        selectedProp += propRoot->getNumChildren();
	SbVec3f oldTrans(0,0,0);
	SbRotation oldRot(0,0,0,1);
	if (propDragger != NULL)
	{
		oldTrans = propDragger->translation.getValue();
		oldRot = propDragger->rotation.getValue();
        propDragger->unref();
        propDragger = NULL;
	}
    propHook->removeAllChildren();
    draggerActive = false;
    propDragger = new SoDraggerKit();
    propDragger->ref();
    propDragger->setStartCallback(draggerStarted, this);
    propDragger->setFinishCallback(draggerFinished, this);
	propDragger->translation = oldTrans;
	propDragger->rotation = oldRot;
    if (trackedProp = getTrackedArtifact(propRoot->getChild(selectedProp)))
    {
        if (!trackedProp->getGeometry()->isOfType(SoSeparator::getClassTypeId()))
        {
            SoSeparator *tag = new SoSeparator;
            tag->addChild(trackedProp->getGeometry());
            trackedProp->setGeometry(tag);
        }
		propDragger->setPart("normalGeometry", (SoSeparator *)trackedProp->getGeometry());
    }
    else
		propDragger->setPart("normalGeometry", propRoot->getChild(selectedProp));

    SoSeparator *highlightedContent = new SoSeparator;
    SoDrawStyle *ds = new SoDrawStyle;
    ds->style = SoDrawStyle::LINES;
    ds->lineWidth = 4;
    SoMaterial *m = new SoMaterial;
    m->emissiveColor = SbColor(1.0,0,0);
	highlightedContent->addChild(propDragger->getPart("normalGeometry", FALSE));
    highlightedContent->addChild(ds);
    highlightedContent->addChild(m);
    highlightedContent->addChild(propDragger->getPart("normalGeometry", FALSE));

	propDragger->setPart("highlightedGeometry", highlightedContent);
	propDragger->setPart("draggedGeometry", highlightedContent);
	SoTransform *xf = (SoTransform *)propDragger->
		getPart("constrainedTransform", FALSE);
	assert(xf);
	xf->translation.connectFrom(&propDragger->translation);
	xf->rotation.connectFrom(&propDragger->rotation);
	propDragger->dragPoint = SoDraggerKit::PICKPOINT;

    propHook->addChild(propDragger);

    featurePoints = (SoCoordinate3 *)myFindNode(propRoot->getChild(selectedProp),
                                              "FEATURE_POINTS");
    /*if ((featurePoints == NULL) || (featurePoints->point.getNum() == 0))
        displayMessage("The prop has no feature points");
    else 
        displayMessage("");
        */

}

//----------------------------------------------------------------------------

void 
SoPropCalibrationKit::addFeaturePoint()
{
    SoSeparator *s = new SoSeparator;
    SoTranslation *t = new SoTranslation;
    t->translation = featurePoints->point[currentFeaturePoint];
    
    SbString name;
    name = dataDirectory.getValue().getString();
    name += "featureMarker.iv";
    SoNode *marker = readFile(name.getString());
    if (marker == NULL) 
        exit(-1);
    s->addChild(t);
    s->addChild(marker);

    SoSeparator *dg = (SoSeparator *)propDragger->getPart("normalGeometry", false);
    dg->insertChild(s, 0);
    dg = (SoSeparator *)propDragger->getPart("highlightedGeometry", false);
    dg->insertChild(s, 0);
}

//----------------------------------------------------------------------------

void 
SoPropCalibrationKit::removeFeaturePoint()
{
    SoSeparator *dg = (SoSeparator *)propDragger->getPart("normalGeometry", false);
    dg->removeChild(0);
    dg = (SoSeparator *)propDragger->getPart("highlightedGeometry", false);
    dg->removeChild(0);
}

//----------------------------------------------------------------------------

void 
SoPropCalibrationKit::startPropCalibration()
{
    if ((featurePoints == NULL) || (featurePoints->point.getNum() == 0))
        return;

    switchPipSheet(SECOND_SHEET);
    switchMenue(DISABLED);
    appState = SAMPLING_FEATURE_DATA;
    currentFeaturePoint = 0;
    addFeaturePoint();
    initSampler();
}


//----------------------------------------------------------------------------

void 
SoPropCalibrationKit::cancelPropCalibration()
{
    removeFeaturePoint();
    appState = START;
    switchPipSheet(FIRST_SHEET);
    switchMenue(MAIN);
}

//----------------------------------------------------------------------------

void 
SoPropCalibrationKit::stepBack()
{
    if (currentFeaturePoint > 0)
    {
        sampler->stepBack();
        currentFeaturePoint -= 2;
        calibControl(pButton);
    }
}

//----------------------------------------------------------------------------

void
SoPropCalibrationKit::buttonTriggeredCB(void *data)
{
    SoPropCalibrationKit *actual = (SoPropCalibrationKit *)data;
    actual->calibControl(actual->pButton);  
}

//----------------------------------------------------------------------------

void
SoPropCalibrationKit::removeMessageCB(void *data)
{
    SoPropCalibrationKit *actual = (SoPropCalibrationKit *)data;
    actual->removeMessage();
}

//----------------------------------------------------------------------------

void 
SoPropCalibrationKit::keyEventCB(void *data, SoEventCallback *myEvent) 
{

    const SoEvent *event = myEvent->getEvent();
    SoKeyboardEvent::Key pressed;
 
    SoPropCalibrationKit *actual = (SoPropCalibrationKit *)data;

    if (SoKeyboardEvent::isKeyPressEvent(event, SoKeyboardEvent::ANY)) {
        pressed = ((SoKeyboardEvent *)event)->getKey(); 
			printf("key event %d\n",pressed );
        switch (pressed)
        {
        
            case SoKeyboardEvent::S:
                if (actual->appState == START)
                    actual->startPropCalibration();
                else if (actual->appState == SAMPLING_FINISHED)
                    actual->saveProp();
                break;

            case SoKeyboardEvent::N:
                if (actual->appState == START)
                    actual->selectProp(1);
                break;
            case SoKeyboardEvent::P:
                if (actual->appState == START)
                    actual->selectProp(-1);
                break;

            case SoKeyboardEvent::A:
                if (actual->appState == SAMPLING_FEATURE_DATA)
                    actual->cancelPropCalibration();
                else if (actual->appState == SAMPLING_FINISHED)
                    actual->dumpProp();
                break;

            case SoKeyboardEvent::ENTER:
            case SoKeyboardEvent::SPACE:
				printf("state: %d must be %d to trigger somthing\n", actual->appState ,SAMPLING_FEATURE_DATA);
                if (actual->appState == SAMPLING_FEATURE_DATA)
                    actual->calibControl(actual->pButton);
                break;
            case SoKeyboardEvent::BACKSPACE:
                if (actual->appState == SAMPLING_FEATURE_DATA)
                    actual->stepBack();
                break;
            default: break;
        }
    }

    myEvent->setHandled();
}

//----------------------------------------------------------------------------

SbBool
checkSampleValidity()
{
    return FALSE;
}

void 
SoPropCalibrationKit::calibControl(int button) 
{
	if (button != pButton)
		return;
    switch (appState)
    {
        case START: break;
        case SAMPLING_FEATURE_DATA:
			// TLP
			printf("data sampled\n");
            sampler->sample();
            if (++currentFeaturePoint < featurePoints->point.getNum())
            {
                removeFeaturePoint();
                addFeaturePoint();
                appState = SAMPLING_FEATURE_DATA;
            }
            else
            {
                removeFeaturePoint();
                appState = SAMPLING_FINISHED;
                calcProp();
                switchPipSheet(THIRD_SHEET);
                switchMenue(POST_CALIB_DIALOG);
            }
            break;
        default: break;
    }
}

//----------------------------------------------------------------------------

void 
SoPropCalibrationKit::displayMessage(SoMFString &message, int buttonStation, int time)
{
    // userID of user who started this application
    SoMessageKit *m = new SoMessageKit();
    m->fontSize = 40.0;
    m->message = message;
    removeMessage();
    messageHook->addChild(m);
    messageButton->startButtonTriggering();
}

//----------------------------------------------------------------------------  

void
SoPropCalibrationKit::menueCB(void *data, So2DMenueKit *m)
{
    SoPropCalibrationKit *actual = (SoPropCalibrationKit *)data;
    if (actual->draggerActive)
        return;
	
    if (actual->appState == START)
        switch (m->getSelectedEntry())
        {
            case NEXT_PROP:
                actual->selectProp(1);
                break;
            case PREV_PROP:
                actual->selectProp(-1);
                break;
            case CALIB:
                actual->startPropCalibration();
                break;
            case EXIT:
                printf("exit\n");
                actual->scheduleDelete();
                break;
            default: break;
        }
    else if (actual->appState == SAMPLING_FINISHED)
        switch (m->getSelectedEntry())
        {
            case SAVE_CALIB:
                actual->saveProp();
                break;
            case DUMP_CALIB:
                actual->dumpProp();
                break;
            default: break;
        }
}


So2DMenueKit * 
SoPropCalibrationKit::loadMenue(SbString name)
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

void 
SoPropCalibrationKit::switchMenue(enum menues which)
{
    if (!use2DMenue.getValue())
        return;

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

void SoPropCalibrationKit::switchPipSheet(enum pipSheets sheetId)
{
    //if (!use2DMenue.getValue())
    ((SoPipSheetKit *)getPipSheet(userID.getValue()))->switchPipSheet(sheetId);
}


//----------------------------------------------------------------------------

void
SoPropCalibrationKit::displayMessage(const char *message, int buttonStation, int time)
{
    SoMFString help;
    help.setValue(message);
    displayMessage(help);
}

//----------------------------------------------------------------------------

void 
SoPropCalibrationKit::removeMessage()
{
    messageHook->removeAllChildren();
}

//---------------------------------------------------------------------------
// optimizing transformation
//---------------------------------------------------------------------------

class propTestData
{
    const SoMFVec3f &featurePoints;
    const SoMFVec3f &sampledPoints;
public:
    propTestData(const SoMFVec3f &featurePoints,
                 const SoMFVec3f &sampledPoints) :
                 featurePoints(featurePoints),
                 sampledPoints(sampledPoints)
    {};
    friend float testPropOffset(float []);
} *testData;

//---------------------------------------------------------------------------

float
testPropOffset(float t[]) 
{
	float fret = 0;	
	SbVec3f translation(t[1],t[2],t[3]);
    SbRotation rotation(SbVec3f(t[4],t[5],t[6]),t[7]);
    SbVec3f tfp;
    float distance;

    for (int i=0; i<testData->featurePoints.getNum(); i++)    
    {
        rotation.multVec(testData->featurePoints[i], tfp);
        tfp += translation;
        distance = (testData->sampledPoints[i] - tfp).length();
        fret += distance * distance;
    }
    
    return sqrt(fret/(float)testData->featurePoints.getNum());
}

//---------------------------------------------------------------------------

void
SoPropCalibrationKit::saveProp()
{
    printf("saving registered prop to: %s\n", propNames[selectedProp].getString());
    writeFile(propNames[selectedProp].getString(), propHook);
    dumpProp();
}

//---------------------------------------------------------------------------

void
SoPropCalibrationKit::dumpProp()
{
    selectProp(0);
    switchPipSheet(FIRST_SHEET);
    switchMenue(MAIN);
    appState = START;
}

//---------------------------------------------------------------------------
// calculate optimized prop-offset

void 
SoPropCalibrationKit::calcProp()
{
    SoTransform *offset = new SoTransform;

    //displayMessage("Calculating prop offset ...");

    int penStation = SoUserManagerKit::getInstance()->
										getPenFromUser(userID.getValue());
    int numberOfSamples = sampler->samples[penStation].translations.getNum();

    if (trackedProp)
    {
        int propStation = trackedProp->station.getValue();
        SbVec3f tmp;
        for (int i=0; i<numberOfSamples; i++)
        {
            sampler->samples[propStation].rotations[i].inverse().multVec(
                sampler->samples[penStation].translations[i] - 
                sampler->samples[propStation].translations[i], tmp);
            sampler->samples[penStation].translations.set1Value(i, tmp);
        }
    }
    SoMFVec3f &sampledFeaturePoints = sampler->samples[penStation].translations;

    SbVec3f v1, v2;
    SbVec3f n1, n2;
    SbPlane p1, p2;
    SbRotation r1, r2;

    switch (featurePoints->point.getNum())
    {
    case 1: offset->translation = sampledFeaturePoints[0] - featurePoints->point[0];
            break;
    case 2: v1 = featurePoints->point[1] - featurePoints->point[0];    
            v2 = sampledFeaturePoints[1] - sampledFeaturePoints[0];    
            offset->rotation = SbRotation(v1, v2);
            v2 = sampledFeaturePoints[0] + v2*0.5f;
            offset->rotation.getValue().multVec(featurePoints->point[0] + v1*0.5f, v1);
            offset->translation = v2 - v1;
            break;
    default: p1 = SbPlane(featurePoints->point[0],      
                          featurePoints->point[1],      
                          featurePoints->point[2]);      
             p2 = SbPlane(sampledFeaturePoints[0],      
                          sampledFeaturePoints[1],      
                          sampledFeaturePoints[2]);
             n1 = p1.getNormal();
             n2 = p2.getNormal();
             r1 = SbRotation(n1, n2);
             v1 = featurePoints->point[1] - featurePoints->point[0];    
             v2 = sampledFeaturePoints[1] - sampledFeaturePoints[0];    
             r1.multVec(v1, v1);
             r2 = SbRotation(v1, v2);
             offset->rotation = r1*r2;
             offset->rotation.getValue().multVec(featurePoints->point[0], v1);
             offset->translation = sampledFeaturePoints[0] - v1;
             break; 
    }

    testData = new propTestData(featurePoints->point, sampledFeaturePoints);

    // init data for powell
    // TODO for feature point num == 2 ?
	if (featurePoints->point.getNum() > 2)
	{
		SbVec3f translation,axis;
		float angle;
		translation = offset->translation.getValue();
		offset->rotation.getValue().getValue(axis,angle);
		const int n = 7;
		float start[n+1] = {0, translation[0], translation[1], translation[2],
			axis[0], axis[1], axis[2], angle};
		
		float *p = vector(1,n);
		float **xi = matrix(1,n,1,n);
		
		// init p and xi
		int r,c;
		for (r=1; r<=n; r++) {
			p[r] = start[r];
			for (c=1; c<=n; c++) 
				xi[r][c] = r==c ? 1 : 0;
		}
		
		int iter;
		float fret;	
		// call POWELL
		powell(p, xi,n,0.00001,&iter,&fret,testPropOffset);
		
		offset->translation = SbVec3f(p[1],p[2],p[3]);
		offset->rotation    = SbRotation(SbVec3f(p[4],p[5],p[6]),p[7]);
		
		delete(testData);
		free_vector(p,1,n);
		free_matrix(xi,1,n,1,n);
		
		printf("TRE: %f\n", fret);
	}

    propHook->removeAllChildren();
    if (trackedProp)
    {
        trackedProp->getOffset()->translation = offset->translation;
        trackedProp->getOffset()->rotation = offset->rotation;
        offset->ref();
        offset->unref();
    }
    else
        propHook->addChild(offset);
    propHook->addChild(propRoot->getChild(selectedProp));
}

//----------------------------------------------------------------------------
//eof

