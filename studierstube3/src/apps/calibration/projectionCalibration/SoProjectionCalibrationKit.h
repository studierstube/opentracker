// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoProjectionCalibrationKit.h
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

#ifndef _SOPROJECTIONCALIBRATIONKIT_H_
#define _SOPROJECTIONCALIBRATIONKIT_H_

#include <Inventor/nodes/SoEventCallback.h>

#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/resource/viewer/SoStereoCameraKit.h>
#include <stbapi/tracker/SoTrakEngine.h>
#include <stbapi/window3d/SoWindowKit.h>
#include "../utils/SoSampler.h"
#include "projectionCalibration.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class So2DMenueKit;

class SoProjectionCalibrationKit
:
    public SoContextKit
{
    SO_KIT_HEADER(SoProjectionCalibrationKit);

    enum menues { MAIN = 0,
                  POST_CALIB_DIALOG,
				  DISABLED
				};

    enum calibrationState { START,
                            SAMPLING_PLANE_DATA,
                            SAMPLING_LEFT_EYE_DATA,
                            SAMPLING_RIGHT_EYE_DATA,
                            SAMPLING_FINISHED};

    enum menue1Items {START_FULL = 0,
                     START_EYE,
                     EMPTY,
                     EXIT};

    enum menue2Items {EXIT_KEEP = 0,
                     EXIT_DUMP,
                     RESTART
                     };

public:
    static void initClass(             //static class initialization
        );
    SoProjectionCalibrationKit(                   //constructor
        );
    SoContextKit* factory(             //override this to create new context
        );                             //of a specific type
    ~SoProjectionCalibrationKit(
        );
    
    static void keyEventCB(
        void *data, 
        SoEventCallback *event
        );
	static void mouseButtonEventCB(
        void *data, 
        SoEventCallback *event
        );
private:
    SbBool checkPipGeometry();

    SbBool checkWindowGeometry();

    void checkPipConnections( int uid, SoNode * pipSheetContents );

	void checkPipMasterMode(   //master/slave mode for pip sheet
        SoNode*	 pipSheet,              //override this to implement your
        SbBool masterMode              //behavior
        );               

    // camera manipulations    
    void set2DCamera();
    void remove2DCamera();
    void saveOldCamera();
    void setActiveCamera(SoStereoCameraKit *camKit);
    
    // scene graph initialization and modification methods
    void initCalibrationGraph();
	So2DMenueKit *loadMenue(SbString name);
	void switchMenue(enum menues which);

    //void initMenue(int userID, SbString name);
	
    void displayPoint();
    void displayText(const char *s);
    SoSeparator *build3Dpoint(const SbVec2f &point2D);
    SoSeparator *build3DText(const SoMFString &text);
    SoMaterial *createDistanceSensitiveColor();

    // control flow methods
    void startProjectionPlaneCalibration();
    void startEyePositionCalibration();
    void calibControl(int button);
    void calibFinished();
    //void validateCamera();
    void validateCamera(SbBool valid, SoOffAxisCamera *camera); 
    static void buttonTriggered(void *data);

    // auxiliary methods
    void calcPlane();
    SbBool calcEye();
    void calcRay();
    void initSampler();
    void logSampleData(char *dataFile);
	void samplesIntoDisplayTrackSpace(SbVec3f *trackPosOut, SbRotation *trackRotOut );
	void samplesIntoDisplayTrackSpace();

    static void menueCB(void *data, So2DMenueKit *m);

//-- data members --
    // set the values of these fields within the
    // config-file
    SoMFVec2f referencePointsPlane; // !!a! will be overwritten by ...MobileDisplayC...Mode
    SoMFVec2f referencePointsEye;
    /*  These fields specify the values, which are used to determine
        whether a certain penPosition is valid for sampling or not.
    */
    SoSFFloat markerDistance;
    SoSFFloat epsilon;
    /* Specifys color of marker, when sampling is valid.
    */
    SoSFVec3f enabledColor;
    /* Specifys color of marker, when sampling is not valid.
    */
    SoSFVec3f disabledColor;
    /*  Specifys the maximum allowed distance between any two rays.
        (where a ray is constructed from the two samples associated with
         a reference point).
    */
    SoSFFloat validDistance;
    SoSFFloat minimumAngle;
    /*  This field is used to access the current result of the dynamic validity
        check of the distance between pen and head tracker.
        This field is only updated during the calibration process.
    */
    SoSFBool  validPenPosition; 
    SoMFVec3f raysP0;
    SoMFVec3f raysP1;

    SoSFString dataDirectory;

    SoStudierstubeViewer *viewer;
    int pButton;
    int sButton;
    SbVec2f viewportSize;
    SbBool stereoMode;
    int appState;
    SoStereoCameraKit *oldCamera, *newCamera;
    SoSeparator *root;
    SoSeparator *leftGraph;
    SoSeparator *rightGraph;
    int calibPoint;
    SbBool textMode;
    SoSampler *sampler;

    So2DMenueKit *menues[2];
    SoSeparator *menueHook;

        
}; //class SoProjectionCalibrationKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOPROJECTIONCALIBRATIONKIT_H_

