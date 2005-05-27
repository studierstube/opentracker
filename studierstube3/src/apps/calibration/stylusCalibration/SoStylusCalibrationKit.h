// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoStylusCalibrationKit.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rcs		Rainer Splechtna
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#ifndef _SOSTYLUSCALIBRATIONKIT_H_
#define _SOSTYLUSCALIBRATIONKIT_H_

#include <Inventor/nodes/SoEventCallback.h>

#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/resource/viewer/SoStereoCameraKit.h>

#include "../utils/SoSampler.h"
#include "../utils/SoMessageKit.h"

#include "styluscalibration.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class STYLUSCALIBRATION_API SoStylusCalibrationKit
:
    public SoContextKit
{
    SO_KIT_HEADER(SoStylusCalibrationKit);

    enum calibrationState { START,
                            SAMPLING_STYLUS_DATA,
                            CONTINUE_SAMPLING};


public:
    static void initClass(             //static class initialization
        );
    SoStylusCalibrationKit(                   //constructor
        );
    SoContextKit* factory(             //override this to create new context
        );                             //of a specific type
    ~SoStylusCalibrationKit(
        );
    
private:
    SbBool checkPipGeometry();

    SbBool checkWindowGeometry();

    void checkPipConnections( int uid, SoNode * pipSheetContents );

	void checkPipMasterMode(   //master/slave mode for pip sheet
        SoNode*	 pipSheet,              //override this to implement your
        SbBool masterMode              //behavior
        );               

    static void keyEventCB(
        void *data, 
        SoEventCallback *event
        );

    static void dynamicSampleFilteringCB(
        void *actual
        );
    void dynamicSampleFiltering(
        );

    // scene graph initialization
    void initCalibrationGraph();
    void displayText(const char *s);
    void displayText(const SoMFString &text);

    // control flow methods
    void startStylusCalibration();
    void calibControl(int button);
    void calcStylus(); 

    void setOffset(SbVec3f stylusOffset, SbBool save = FALSE);
    float offsetOptimization(SbVec3f &stylusOffset);
    float sphereCenterOptimization(SbVec3f &stylusOffset);
    SbVec3f SVDStylusOptimization();

    static void buttonTriggered(void *data);

    // auxiliary methods
	bool loadAndConvertData();
    void createData();
    void initSampler();
    void logSampleData(char *dataFile);
    void vis();
	void transformToReferenceFrame();

//-- data members --
	SoSFInt32 referenceStation;
    SoSFFloat epsilon;
    SoSFInt32 minimumNumberOfSamples;
    SoSFString dataDirectory;
	SoSFString offlineData;
    SoSFBool OTreadOnly;

    SoSFFloat angleError;
    SoSFFloat positionError;
    SoSFVec3f idealOffset; 
    SoSFFloat coverage;
    
    SoSeparator *root;
    SoMessageKit *message;
    SoSampler *sampler;
    int pButton;
    int sButton;
    int appState;
      
}; //class SoStylusCalibrationKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOSTYLUSCALIBRATIONKIT_H_

