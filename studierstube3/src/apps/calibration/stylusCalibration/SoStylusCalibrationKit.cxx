// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoStylusCalibrationKit.cxx
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

#pragma warning(disable:4786)   // stupid symbol size limitation

#include <io.h>
#include <cassert>

#include "../tnt/tnt.h"
#include "../tnt/jama_svd.h"

#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoMouseButtonEvent.h>

#include <stbapi/widgets/So3DButton.h>
#include <stbapi/util/ivio.h>

#include <stbapi/resource/SoPipSheetKit.h>
#include <stbapi/resource/SoUserManagerKit.h>
#include <stbapi/resource/viewer/SoDisplayMode.h>
#include <stbapi/resource/viewer/SoTrackedViewpointControlMode.h>
#include <stbapi/window3d/SoWindowKit.h>

#include <stbapi/tracker/TrackerOT.h>
#include <stbapi/event3D/Stb3DEventGenerator.h>

#include "../powell/nrutil.h"
#include "../powell/powell.h"

#include <common/StaticTransformation.h>
#include <misc/xml/XMLWriter.h>

//#include "../switchFocusToSuite.h"

#include "SoStylusCalibrationKit.h"


//--initialization of kit structure in Inventor
SO_KIT_SOURCE(SoStylusCalibrationKit);

//--initialization of context in Studierstube
CONTEXT_APPLICATION(SoStylusCalibrationKit);

//----------------------------------------------------------------------------
// static class initialization
//----------------------------------------------------------------------------

void
SoStylusCalibrationKit::initClass(void)
{
    SO_KIT_INIT_CLASS(SoStylusCalibrationKit, SoContextKit, "SoContextKit");
    SoMessageKit::initClass();
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoContextKit*
SoStylusCalibrationKit::factory()
{
    return new SoStylusCalibrationKit;  
}

//----------------------------------------------------------------------------
// constructor
//----------------------------------------------------------------------------

SoStylusCalibrationKit::SoStylusCalibrationKit()
:
root(NULL),appState(START),pButton(0),sButton(1),sampler(NULL),message(NULL)
{
    SO_KIT_CONSTRUCTOR(SoStylusCalibrationKit);

    SO_KIT_ADD_FIELD(epsilon, (0.005));
    SO_KIT_ADD_FIELD(minimumNumberOfSamples, (200));
    SO_KIT_ADD_FIELD(dataDirectory, ("../apps/calibration/stylusCalibration/"));
	SO_KIT_ADD_FIELD(offlineData, (""));
    SO_KIT_ADD_FIELD(OTreadOnly, (TRUE));

	SO_KIT_ADD_FIELD(referenceStation, (-1));
    SO_KIT_ADD_FIELD(angleError, (0.0));
    SO_KIT_ADD_FIELD(positionError, (0.0));
    SO_KIT_ADD_FIELD(coverage, (0.5));
    SO_KIT_ADD_FIELD(idealOffset, (0,0,0));
        
    SO_KIT_INIT_INSTANCE();
}

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

SoStylusCalibrationKit::~SoStylusCalibrationKit()
{
    if (sampler)
        delete(sampler);

    //switchFocusToSuite(userID.getValue());
}


//----------------------------------------------------------------------------
// Reads the pip sheet and sets the button callbacks
//----------------------------------------------------------------------------

SbBool
SoStylusCalibrationKit::checkPipGeometry()
{
    return TRUE;
}
    
//----------------------------------------------------------------------------

SbBool 
SoStylusCalibrationKit::checkWindowGeometry()
{
    SoStudierstubeViewer *viewer = SoUserManagerKit::getInstance()->getUserKit(userID.getValue())->
        getDisplay()->getViewer();
	viewer->setViewing(false);

    //fprintf(stderr, "window for user no. %d\n", userID.getValue());
        // get and store the primary and secondary button of the pen
    pButton = SoUserManagerKit::getInstance()->getUserKit(userID.getValue())
        ->getPen()->primaryButton.getValue();
    sButton = SoUserManagerKit::getInstance()->getUserKit(userID.getValue())
        ->getPen()->secondaryButton.getValue();

    assert(getWindow(0));
    // setup basic application graph
    root = getWindow(0)->getClientVolume();

    initCalibrationGraph();
    startStylusCalibration();

    return TRUE;
}

//----------------------------------------------------------------------------

void
SoStylusCalibrationKit::checkPipConnections( int uid, SoNode * pipSheetContents )
{
    if (uid != userID.getValue())
        return;
}

//----------------------------------------------------------------------------

void 
SoStylusCalibrationKit::checkPipMasterMode( SoNode * pipSheetContents, SbBool masterMode )
{
    // only the user, who started the application gets a sheet,
    // hence nothing has to be done in this method
}

//----------------------------------------------------------------------------

void 
SoStylusCalibrationKit::initCalibrationGraph()
{
    SoEventCallback *event = new SoEventCallback;
    root->addChild(event);
    event->addEventCallback(SoKeyboardEvent::getClassTypeId(), keyEventCB, this);              
	event->addEventCallback(SoMouseButtonEvent::getClassTypeId(), keyEventCB, this);

    message = (SoMessageKit *)readNodeFromFile
        ("message.iv", dataDirectory.getValue().getString());
    if (message == NULL)
        message = new SoMessageKit();

    root->addChild(message);
}

//----------------------------------------------------------------------------

void
SoStylusCalibrationKit::initSampler()
{
    if (sampler)
        sampler->clearAllSamples();
    else
    {
        SoMFInt32 stations;
        int penStation;
        stations.setValue(penStation = SoUserManagerKit::getInstance()->
            getPenFromUser(userID.getValue()));
		// ADDED
		if (referenceStation.getValue() > -1 && 
			referenceStation.getValue() != penStation)
	        stations.set1Value(1, referenceStation.getValue());
        sampler = new SoSampler;
        sampler->setStationsToSample(stations);
        sampler->initButtonTriggering(penStation, pButton, FALSE, FALSE,
                                      buttonTriggered, this);
    }
}

//----------------------------------------------------------------------------

float gR(int seed = 1.0)
{
    return 2.0*(float)rand()/(float)RAND_MAX-1.0;
}

void
SoStylusCalibrationKit::createData()
{
    SbVec3f fixedPosition(gR()*1000.0,gR()*1000.0,gR()*1000.0);

    int penStation = SoUserManagerKit::getInstance()->
            getPenFromUser(userID.getValue());
    int sampleCount = minimumNumberOfSamples.getValue();

    float a;
    float asum = 0.0;
    float vsum = 0.0;
    SbVec3f tmp, eVec;
    SbRotation r;

	// ADDED
	SbRotation rref(SbVec3f(gR(),gR(),gR()),gR()*M_PI);
	SbVec3f tref(gR()*1000.0,gR()*1000.0,gR()*1000.0);

    for (int i=0; i<sampleCount; i++) 
    {
        r = SbRotation(SbVec3f(gR(),gR(),gR()),gR()*M_PI*coverage.getValue());        
        r.multVec(idealOffset.getValue(), tmp);
        eVec = SbVec3f(gR(),gR(),gR());
        eVec.normalize();
        eVec *= positionError.getValue()*gR();
        vsum += eVec.length();
        for (int j=0; j<i; j++)
            if ((fixedPosition-tmp-sampler->samples[penStation].translations[j]).length() < epsilon.getValue()) 
                break;

        if (i!=j)
        {
            i--; continue;
        }

        a = gR()*angleError.getValue()*M_PI/180.0;
        asum += fabs(a);
        r *= SbRotation(SbVec3f(gR(),gR(),gR()), a);
        sampler->samples[penStation].translations.set1Value(i, fixedPosition-tmp+eVec);
        sampler->samples[penStation].rotations.set1Value(i, r);

		// ADDED
		if (referenceStation.getValue() > -1 && referenceStation.getValue() != penStation)
		{
	        sampler->samples[referenceStation.getValue()].translations.set1Value(i, tref);
		    sampler->samples[referenceStation.getValue()].rotations.set1Value(i, rref);
		}

	}
    fprintf(stderr, "avg. angle deviation: %f\n", (180.0/M_PI)*asum/sampleCount);
    fprintf(stderr, "avg. position deviation: %f\n", vsum/sampleCount);
}


bool 
SoStylusCalibrationKit::loadAndConvertData()
{
	if (offlineData.getValue() == "")
		return false;

    SbString logName = dataDirectory.getValue().getString();
    logName += offlineData.getValue();
	FILE *datalog = fopen(logName.getString(), "r");
	if (datalog == NULL)
		return false;
    SbVec3f offset;
    int sampleCount;
	fscanf(datalog, "%d",&sampleCount);
	float x,y,z;
    float rx,ry,rz,rw;
    SbVec3f axis;
    SbRotation r;
	int i;

    int penStation = SoUserManagerKit::getInstance()->
            getPenFromUser(userID.getValue());

	for (i=0; i<sampleCount; i++) 
    {
	    if (fscanf(datalog, "%f %f %f", &x,&y,&z) == 3) 
            sampler->samples[penStation].translations.set1Value(i,SbVec3f(x,y,z));
        else
        {
            printf("format error occured reading from file %s\n",logName.getString());
            exit(-1); 
        }
        if (fscanf(datalog, "%f %f %f %f", &rx,&ry,&rz,&rw) == 4) 
            sampler->samples[penStation].rotations.set1Value(i, SbRotation(rx,ry,rz,rw));
        else
        {
            printf("format error occured reading from file %s\n",logName.getString());
            exit(-1); 
        }
	}
	fclose(datalog);
	return true;
}

void 
SoStylusCalibrationKit::startStylusCalibration()
{
    // set offset of pen to reflect hotspot position 
    if (appState != START)
    {
        sampler->stopButtonTriggering();
        sampler->stopContinousSampling();
        appState = START;
    }

    displayText("Press button to start stylus calibration!");
    initSampler();

    if (idealOffset.getValue() != SbVec3f(0,0,0))
    {
	    createData();
	    calcStylus();
        vis();
        exit(0);
    }

	if (loadAndConvertData())
	{
		calcStylus();
        vis();
		exit(0);
	}
	else
		sampler->startButtonTriggering();
    // reset any previous set offset
    setOffset(SbVec3f(0,0,0));
}

//----------------------------------------------------------------------------

void
SoStylusCalibrationKit::buttonTriggered(void *data)
{
    SoStylusCalibrationKit *actual = (SoStylusCalibrationKit *)data;
    actual->calibControl(actual->pButton);  
}

//----------------------------------------------------------------------------

void 
SoStylusCalibrationKit::keyEventCB(void *data, SoEventCallback *myEvent) 
{
    const SoEvent *event = myEvent->getEvent();
    SoKeyboardEvent::Key pressed;
 
    SoStylusCalibrationKit *actual = (SoStylusCalibrationKit *)data;

    if (SoKeyboardEvent::isKeyPressEvent(event, SoKeyboardEvent::ANY)) {
        pressed = ((SoKeyboardEvent *)event)->getKey(); 
        switch (pressed)
        {
            case SoKeyboardEvent::S:
                actual->startStylusCalibration();
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
    if (SoMouseButtonEvent::isButtonPressEvent(event, SoMouseButtonEvent::BUTTON1))
    {
		actual->calibControl(actual->pButton);
	}
    myEvent->setHandled();
}

//----------------------------------------------------------------------------

void 
SoStylusCalibrationKit::calibControl(int button) 
{
    switch (appState)
    {
        case START: case CONTINUE_SAMPLING: 
            displayText("Move stylus around ...");
            sampler->startContinousSampling(dynamicSampleFilteringCB, this);
            appState = SAMPLING_STYLUS_DATA;
            break;
        case SAMPLING_STYLUS_DATA:
            sampler->stopButtonTriggering();
            sampler->stopContinousSampling();
            calcStylus();
			//vis();
            break;
        default: break;
    }
}

//----------------------------------------------------------------------------

void 
SoStylusCalibrationKit::displayText(const char *text)
{
    message->message.setValue(text);
}

//----------------------------------------------------------------------------

void 
SoStylusCalibrationKit::displayText(const SoMFString &text)
{
    message->message.setValues(0,text.getNum(),text.getValues(0));
}

//---------------------------------------------------------------------------

void
SoStylusCalibrationKit::dynamicSampleFilteringCB(void *actual) 
{
    ((SoStylusCalibrationKit *)actual)->dynamicSampleFiltering();
}

//---------------------------------------------------------------------------

void
SoStylusCalibrationKit::dynamicSampleFiltering() 
{
    SoMFVec3f &samT = sampler->samples[SoUserManagerKit::getInstance()->
            getPenFromUser(userID.getValue())].translations;
    SoMFRotation &samR = sampler->samples[SoUserManagerKit::getInstance()->
            getPenFromUser(userID.getValue())].rotations;

    int testSample = samT.getNum()-1;

    for (int i=0; i<samT.getNum()-1; i++)
        if ((samT[testSample] - samT[i]).length() < epsilon.getValue()) 
        {
            sampler->stepBack();
            return;
        }

    SoMFString text;
    char buffer[50];
    text.setValue("Move stylus around ...");
    sprintf(buffer, "  Valid samples: %4d", samT.getNum());
    text.set1Value(1, buffer);
    sprintf(buffer, "(Samples needed: %4d)", minimumNumberOfSamples.getValue());
    text.set1Value(2, buffer);
    if (samT.getNum() >= minimumNumberOfSamples.getValue())
        text.set1Value(3, "Press button to stop stylus calibration !");
    displayText(text);
}

//---------------------------------------------------------------------------

void logRotation(FILE *logFile, SbRotation rot) 
{
    assert(logFile);
	float q1,q2,q3,q4;
    rot.getValue(q1,q2,q3,q4);
	fprintf(logFile, "%1.16f %1.16f %1.16f %1.16f\n", q1,q2,q3,q4);
}

//---------------------------------------------------------------------------

void logVector(FILE *logFile, SbVec3f vec) 
{
    assert(logFile);
	float x,y,z;
	vec.getValue(x,y,z);
	fprintf(logFile, "%1.20f %1.20f %1.20f\n", x,y,z);
}

//---------------------------------------------------------------------------
// optimizing offset (in stylus tracker coordinates)
//---------------------------------------------------------------------------

class stylusTestData
{
    const SoMFVec3f &translations;
    const SoMFRotation &rotations;
public:
    stylusTestData(const SoMFVec3f &translations,
                   const SoMFRotation &rotations) :
                   translations(translations),
                   rotations(rotations)
    {};
    friend float testStylusOffset(float []);
    friend bool sphereFit (SbVec3f& rkCenter, float& rfRadius);
} *testData;

//---------------------------------------------------------------------------

float
testStylusOffset(float t[]) 
{
	float fret = 0;	
    SbVec3f offset(t[1],t[2],t[3]);
	
    // calc mean stylus hotspot
    SbVec3f hotspot,wpT, help;
    SbRotation wpR; 
    hotspot.setValue(0,0,0);
    int n = testData->translations.getNum();
	for (int i=0; i < n; i++) 
	{
        wpR = testData->rotations[i];
        wpT = testData->translations[i];
        wpR.multVec(offset, help);
        hotspot += help + wpT;
	}

    hotspot /= (float)n;
    
    // calc variance (th - thm)
    for (i=0; i < n; i++) 
    {
        wpR = testData->rotations[i];
        wpT = testData->translations[i];
        wpR.multVec(offset, help);
        help += wpT;
        float tmp;
		tmp = (help - hotspot).length();
        fret += tmp*tmp;
    }

    fret /= (float)(n);
	// return standard deviation
    return sqrt(fret);
}

//---------------------------------------------------------------------------

// This function is  based on MgcAppr3DSphereFit from
// Magic Software, Inc.
// http://www.magic-software.com

bool 
sphereFit(SbVec3f& rkCenter, float& rfRadius)
{
    // user-selected parameters
    const int iMaxIterations = 5000;
    const float fTolerance = 0.00000001;//1e-06;

    // compute the average of the data points stylus = kAverage
    SbVec3f kAverage(0,0,0);
    
    for (int i0=0;i0<testData->translations.getNum();i0++)
        kAverage += testData->translations[i0];
    float fInvQuantity = 1.0/testData->translations.getNum();
    kAverage *= fInvQuantity;

    // initial guess
    rkCenter = kAverage;

    int i1;
    for (i1 = 0; i1 < iMaxIterations; i1++)
    {
        // update the iterates
        SbVec3f kCurrent = rkCenter;

        // compute average L, dL/da, dL/db, dL/dc
        float fLAverage = 0.0;
        SbVec3f kDerLAverage(0,0,0);
        for (i0 = 0; i0 < testData->translations.getNum() ; i0++)
        {
            SbVec3f kDiff = testData->translations[i0] - rkCenter;
            float fLength = kDiff.length();
            if ( fLength > fTolerance )
            {
                fLAverage += fLength;
                float fInvLength = 1.0/fLength;
                kDerLAverage -= fInvLength*kDiff;
            }
        }
        fLAverage *= fInvQuantity;
        kDerLAverage *= fInvQuantity;

        rkCenter = kAverage + fLAverage*kDerLAverage;
        rfRadius = fLAverage;

        if ( fabs(rkCenter[0] - kCurrent[0]) <= fTolerance
        &&   fabs(rkCenter[1] - kCurrent[1]) <= fTolerance
        &&   fabs(rkCenter[2] - kCurrent[2]) <= fTolerance )
        {
            break;
        }
    }

    //printf("iterations: %d\n",i1);
    return i1 < iMaxIterations;
}

//---------------------------------------------------------------------------
// calculate optimized th-offset

float
SoStylusCalibrationKit::offsetOptimization(SbVec3f &stylusOffset)
{
    int penStation = SoUserManagerKit::getInstance()->
            getPenFromUser(userID.getValue());
    int numberOfSamples = sampler->samples[penStation].translations.getNum();

    SbVec3f help, pwT;
    SbRotation pwR;

    // init data for powell
	const int n = 3;
	float start[n+1] = {0,stylusOffset[0],stylusOffset[1],stylusOffset[2]};

    float *p = ::vector(1,n);
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
	powell(p, xi,n,0.0000001,&iter,&fret,testStylusOffset);
	stylusOffset.setValue(p[1],p[2],p[3]);

	free_vector(p,1,n);
	free_matrix(xi,1,n,1,n);

    return fret;
}

//---------------------------------------------------------------------------

float
SoStylusCalibrationKit::sphereCenterOptimization(SbVec3f &stylusOffset) 
{
    int penStation = SoUserManagerKit::getInstance()->
            getPenFromUser(userID.getValue());
    int numberOfSamples = sampler->samples[penStation].translations.getNum();

    float mean = 0.0;
    for (int i=0; i < numberOfSamples; i++) 
        mean += (sampler->samples[penStation].translations[i] - stylusOffset).length();
    mean /= (float)numberOfSamples;
	SbVec3f sphereCenter = stylusOffset;      
    sphereFit(sphereCenter, mean);
    //printf("radius of sphere: %f\n",mean);

    // calc variance
    float fret = 0;
    float t;
    for (i=0; i < numberOfSamples; i++) 
    {
        t = (sampler->samples[penStation].translations[i] - sphereCenter).length() - mean;
        fret += t*t;
    }
    fret /= (float)(numberOfSamples-1);

    // calc offset in stylus-tracker space
    stylusOffset.setValue(0,0,0);
    SbVec3f tmp;
	float length = 0.0;

    for (i=0; i < numberOfSamples; i++) 
    {
        sampler->samples[penStation].rotations[i].inverse().multVec(
                 sphereCenter - sampler->samples[penStation].translations[i] , tmp);
		length += tmp.length();
		stylusOffset += tmp;
    }
    stylusOffset /= (float)numberOfSamples;
	
	length /= (float)numberOfSamples;
	  
    return fret;
}

//---------------------------------------------------------------------------
// fills TNT array2d with transpose of SbMatrix
Array2D<double> generateMatrix(SbMatrix in)
{
	Array2D<double> out(3,3);
	for (int m=0; m<3; m++)
		for (int n=0; n<3; n++)
			out[m][n] = in[n][m];
	return out;

}


void
SoStylusCalibrationKit::transformToReferenceFrame()
{
	int penStation = SoUserManagerKit::getInstance()->
		getPenFromUser(userID.getValue());

	if (referenceStation.getValue() > -1 && 
		referenceStation.getValue() != penStation &&
        offlineData.getValue() == "")
	{
        FILE *lf;
        SbString logName = dataDirectory.getValue().getString();
        logName += "stylusCalibrationData.txt";
        lf = fopen(logName.getString(), "w");

	    int numberOfSamples = sampler->samples[penStation].translations.getNum();
        fprintf(lf, "%d\n", numberOfSamples);

		SbVec3f vtmp;
		SbRotation rtmp;
		for (int i=0; i<numberOfSamples; i++) 
		{
			// get
			vtmp = sampler->samples[penStation].translations[i];
			rtmp = sampler->samples[penStation].rotations[i];

			// to reference frame space
			vtmp -= sampler->samples[referenceStation.getValue()].translations[i];
			sampler->samples[referenceStation.getValue()].rotations[i].inverse().multVec(vtmp, vtmp);
			rtmp *= sampler->samples[referenceStation.getValue()].rotations[i].inverse();

			// put
			sampler->samples[penStation].translations.set1Value(i, vtmp);
			sampler->samples[penStation].rotations.set1Value(i, rtmp);

            // log
            logVector(lf, sampler->samples[penStation].translations[i]);
            logRotation(lf, sampler->samples[penStation].rotations[i]);

		}
        fclose(lf);
	}
}


void 
SoStylusCalibrationKit::calcStylus()
{
    displayText("Calculating stylus offset ...");

    int penStation = SoUserManagerKit::getInstance()->
            getPenFromUser(userID.getValue());
    int numberOfSamples = sampler->samples[penStation].translations.getNum();

    if (numberOfSamples < minimumNumberOfSamples.getValue())
    {
        SoMFString text;
        char buffer[50];
        sprintf(buffer, "  Valid samples: %4d", numberOfSamples);
        text.setValue("Press button to continue stylus calibration !");
        text.set1Value(1, buffer);
        sprintf(buffer, "(Samples needed: %4d)", minimumNumberOfSamples.getValue());
        text.set1Value(2, buffer);
        displayText(text);
        appState = CONTINUE_SAMPLING;
        sampler->startButtonTriggering();
        return;
    }

	// log raw data
    FILE *lfraw;
    SbString logNameRaw = dataDirectory.getValue().getString();
    logNameRaw += "stylusCalibrationDataRaw.txt";
    lfraw = fopen(logNameRaw.getString(), "w");
    fprintf(lfraw, "%d\n", numberOfSamples);
    for (int i=0; i<numberOfSamples; i++)
    {
        logVector(lfraw, sampler->samples[penStation].translations[i]);
        logRotation(lfraw, sampler->samples[penStation].rotations[i]);
    }
    fclose(lfraw);

	transformToReferenceFrame();

    FILE *logfile;
    SbString logName = dataDirectory.getValue().getString();
    logName += "stylusCalibrationLog.txt";
    logfile = fopen(logName.getString(), "a");

    SbVec3f stylusOffset;
    stylusOffset.setValue(0,0,0);
  	// calculate starting point(th-offset) for optimization
    for (i=0; i<numberOfSamples; i++)
        stylusOffset += sampler->samples[penStation].translations[i];

    stylusOffset /= numberOfSamples;

    testData = new stylusTestData(sampler->samples[penStation].translations,
                             sampler->samples[penStation].rotations);

    // sphere center calculation
    float fretS = sphereCenterOptimization(stylusOffset);
    float t[4];
    for (i=0; i<3; i++)
        t[i+1] = stylusOffset.getValue()[i];
    float fretSO = testStylusOffset(t);

    /*printf("filter epsilon: %f\nnumber of samples: %d\nresidual: %1.20f\n",
            epsilon.getValue(), numberOfSamples, fretS);
    printf("stylus-offset: %f %f %f\n", stylusOffset[0], stylusOffset[1], stylusOffset[2]);
    */

    fprintf(logfile, "\n------------------------------------------------------------------\n");
    fprintf(logfile, "filter epsilon: %f\nnumber of samples: %d\nresiduals:\nsphere %f \noffset %f \n",
            epsilon.getValue(), numberOfSamples, sqrt(fretS), fretSO);
    fprintf(logfile, "stylus-offset: %f %f %f\n", stylusOffset[0], stylusOffset[1], stylusOffset[2]);
	fprintf(logfile, "stylus-length: %f \n", stylusOffset.length());
    
    // direct offset calculation
    SbVec3f start = stylusOffset; 
    float fretDO = offsetOptimization(start);

    fprintf(logfile, "\ndirect offset calculation:\n");
	fprintf(logfile, "residuals: %f \n", fretDO);
    fprintf(logfile, "stylus-offset: %f %f %f\n", start[0], start[1], start[2]);
	fprintf(logfile, "stylus-length: %f \n", start.length());

    // SVD calculation
	SbVec3f svdOffset(0,0,0);
    svdOffset = SVDStylusOptimization();
  
	for (i=0;i<3; i++)
		t[i+1] = svdOffset[i];
    float fretSVD = testStylusOffset(t);

    fprintf(logfile, "\nSVD offset calculation:\n");
	fprintf(logfile, "residuals: %f \n", fretSVD);
    fprintf(logfile, "stylus-offset: %f %f %f\n", svdOffset[0], svdOffset[1], svdOffset[2]);
	fprintf(logfile, "stylus-length: %f \n", svdOffset.length());

	fclose(logfile);

	delete(testData);

	/*if (fretDO < fretSO && fretDO < fretSVD)
		setOffset(start, TRUE);
	else if (fretSO < fretSVD)
		setOffset(stylusOffset, TRUE);
	else
		setOffset(svdOffset, TRUE);
		*/
	setOffset(svdOffset, TRUE);

    scheduleDelete();
}



const float cepsilon = 0.0000001;

//---------------------------------------------------------------------------

SbVec3f 
SoStylusCalibrationKit::SVDStylusOptimization()
                                     
{
	int penStation = SoUserManagerKit::getInstance()->
            getPenFromUser(userID.getValue());
    int n = sampler->samples[penStation].translations.getNum();

	SbVec3f accumTrans(0,0,0);
	Array2D<double> accumRot(3,3,0.0);
	SbMatrix rm;
	for (int i=0; i<n; i++) 
	{
		accumTrans += sampler->samples[penStation].translations[i];
		sampler->samples[penStation].rotations[i].getValue(rm);
		accumRot = matadd(accumRot, generateMatrix(rm));
	}

	Array2D<double> b(3*n,1,0.0);
	Array2D<double> A(3*n,3,0.0); 
	Array2D<double> Atmp(3,3,0.0); 
	Array2D<double> x(3,1,0.0);

    SbVec3f bv;
	// b = A*x
    for (i=0; i<n; i++)
    {
        // build b = trans_accum - (n * trans_i)
        bv =  accumTrans - n*sampler->samples[penStation].translations[i];
        for (int j=0; j<3; j++) 
            b[i*3+j][0]=bv[j];
        
        // build A = (rot_i * n) - rot_accum
        sampler->samples[penStation].rotations[i].getValue(rm);
        Atmp = matadd(multscalar(generateMatrix(rm), double(n)), multscalar(accumRot, -1.0));
        for (j=0; j<3; j++)
            for (int k=0; k<3; k++)
                A[i*3+j][k] = Atmp[j][k];
    }

	// Do SVD
	JAMA::SVD<double> svd(A);
	Array2D<double> U,V,S;
	svd.getU(U); svd.getV(V); svd.getS(S);

	for (i=0; i<3; i++)
		S[i][i] = S[i][i] > cepsilon ? 1.0/S[i][i] : 0.0;

	// x = V * (S_inv * (U_trans * b));
	x = matmult(V, matmult(S, matmult(U.transpose(),b)));

	SbVec3f out;
	for (i=0; i<3; i++) 
		out[i]=x[i][0];

	return out;
}

//---------------------------------------------------------------------------

void 
SoStylusCalibrationKit::setOffset(SbVec3f stylusOffset, SbBool save)
{
    int penStation = SoUserManagerKit::getInstance()->
            getPenFromUser(userID.getValue());

    Node *newNode, *parent, *tmp, *root;
    TrackerOT *ot = Stb3DEventGenerator::getTracker();
    assert(ot);
    Context *ctx = ot->getContext();
    root   = ctx->getRootNode();

    // search for StbSink associated with stylus
    for (int i=0; i<root->countChildren(); i++) 
    {
        tmp = root->getChild(i);
        if (tmp->getType().compare("StbSink") == 0)
            if (((StbSink *)tmp)->state.station == penStation)
                break;
    }
    // if StbSink was found then update currently used OT-tree
    if (i < root->countChildren())
    {
        parent = tmp;
        // StbSink can only have one child, but test it anyway
        assert(parent->countChildren() == 1);
        // if a virtual offset for this station was set just remove it
        if (parent->getChild(0)->getType().compare("EventVirtualPositionTransform") == 0)
        {
            tmp = parent->getChild(0)->getChild(0);
            parent->removeChild(*parent->getChild(0));
            parent->addChild(*tmp);
            //StaticTransformation *tNode = (StaticTransformation *);
            //tNode->setTranslation((float *)stylusOffset.getValue());
        }
        // create new EventVirtualPositionTransform node and insert it
        //else
        {
            StringTable nodeConfig;
            nodeConfig.put("translation", stylusOffset.getValue(), 3);
            newNode = ctx->createNode("EventVirtualPositionTransform", nodeConfig);
            if (newNode)
            {
                tmp = parent->getChild(0);
                parent->removeChild(*tmp);
                parent->addChild(*newNode);
                newNode->addChild(*tmp);
            }
        }
        // save stylus offset in OT-config file
        if (save)
        {
            XMLWriter xw = XMLWriter(*ctx);
            if (OTreadOnly.getValue())
            {
                int i=0;
                char buffer[80];
                SbString name = ot->getFilename();
                int len = name.getLength();
                SbString fname = name.getSubString(0, len-5); //extension is always .xml
                do
                {
                    sprintf(buffer, "%s_%02d%s",fname.getString(), i++, ".xml");
                }
                while(!(access(buffer,0) == -1));
                xw.write(buffer);
            }
            else
                xw.write(ot->getFilename());
            printf("stylus-offset: %f %f %f\n", stylusOffset[0], stylusOffset[1], stylusOffset[2]);
        }
    }
    // no StbSink for pen found TODO obsolete ???
    else
    {
        fprintf(stderr, "virtual transformation for the pen station couldn't be set\n");
        printf("stylus-offset: %f %f %f\n", stylusOffset[0], stylusOffset[1], stylusOffset[2]);
    }

}


void 
SoStylusCalibrationKit::vis()
{
    char ivFileName[100];
    strcpy(ivFileName, "visualization.iv");
 
	FILE *ivFile = fopen(ivFileName, "w");
	if (!ivFile) {
		printf("can't create %s\n", ivFileName);
		exit(1);
	}

    fprintf(ivFile, "#Inventor V2.1 ascii\n\n");

    int penStation = SoUserManagerKit::getInstance()->
            getPenFromUser(userID.getValue());
    int numberOfSamples = sampler->samples[penStation].translations.getNum();



	for (int i=0; i<numberOfSamples; i++) 
    {
        SbVec3f stylusSamples = sampler->samples[penStation].translations[i];
        fprintf(ivFile, "SoSeparator { \n");
        fprintf(ivFile, "SoTransform { translation %f %f %f }\n", stylusSamples[0],stylusSamples[1],stylusSamples[2]);
		fprintf(ivFile, "SoMaterial { diffuseColor 0.9 0.9 0.9}\n");
        //fprintf(ivFile, "SoCube { height 0.0005 width 0.0005 depth 0.0005}}\n");
		fprintf(ivFile, "SoSphere { radius 1}}\n");
         
        //stylusSamplesRot[i].getValue(axis,a);
/*        fprintf(ivFile, "SoSeparator { \n");
        fprintf(ivFile, "SoTransform { translation %f %f %f \n", stylusSamples[i][0],stylusSamples[i][1],stylusSamples[i][2]);
        fprintf(ivFile, "              rotation %f %f %f %f}\n", axis[0], axis[1], axis[2],a);
        fprintf(ivFile, "SoTransform { translation %f %f %f }\n", offset[0],offset[1],offset[2]);
        fprintf(ivFile, "SoMaterial { diffuseColor 0.4 0.4 0.8}\n");
        fprintf(ivFile, "SoCube { height 0.0001 width 0.0001 depth 0.0001}}\n");
*/
    }
}

//----------------------------------------------------------------------------
//eof

