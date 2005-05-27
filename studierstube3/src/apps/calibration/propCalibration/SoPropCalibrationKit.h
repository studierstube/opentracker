// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       SoPropCalibrationKit.h
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

#ifndef _SOPROPCALIBRATIONKIT_H_
#define _SOPROPCALIBRATIONKIT_H_

#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoCoordinate3.h>

#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/resource/viewer/SoStereoCameraKit.h>
#include <stbapi/widgets/SoPushButton.h> 

#include "../utils/SoSampler.h"
#include "propcalibration.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


class So2DMenueKit;

class PROPCALIBRATION_API SoPropCalibrationKit
:
    public SoContextKit
{
    SO_KIT_HEADER(SoPropCalibrationKit);

    enum calibrationState { START,
                            SAMPLING_FEATURE_DATA,
                            SAMPLING_FINISHED
                            };

    enum menues { MAIN = 0,
                  POST_CALIB_DIALOG,
				  DISABLED
				};
    enum menue1Items {NEXT_PROP = 0,
                     PREV_PROP,
                     CALIB,
                     EMPTY,
                     EXIT};

    enum menue2Items {SAVE_CALIB = 0,
                     DUMP_CALIB
                     };


    enum pipSheets { FIRST_SHEET,
                     SECOND_SHEET,
                     THIRD_SHEET,
                     EMPTY_SHEET};
    enum buttonIDs { PREVIOUS = 0,
                     NEXT,
                     REGISTER,
                     BACK,
                     CANCEL,
                     SAVE,
                     DUMP };

public:

    SoMFString propNames;
	SoSFString dataDirectory;
    SoSFBool use2DMenue;

    static void initClass(             //static class initialization
        );
    SoPropCalibrationKit(                   //constructor
        );
    SoContextKit* factory(             //override this to create new context
        );                             //of a specific type
    ~SoPropCalibrationKit(
        );
    
    // enable message handling
//    AppMessageCB* getAppMessageCB();
    //static void AppMessageCB(void* userData,int message,int sender,void* data);
    
protected:
    SbBool checkPipGeometry();

    SbBool checkWindowGeometry();

    void checkPipConnections( int uid, SoNode * pipSheetContents );

	void checkPipMasterMode(   //master/slave mode for pip sheet
        SoNode*	 pipSheet,              //override this to implement your
        SbBool masterMode              //behavior
        );               

    SbBool windowEventCB(
        void* data, 
        int messageId,
        SoWindowKit* win,
        int uid, 
        So3DEvent* event,
        SoPath* path);

private:
    static void keyEventCB(
        void *data, 
        SoEventCallback *event
        );
    static void pipSheetButtonCB(        
		void* data,                    //user data
        SoButtonKit* button           //pointer to pressed button
		);

    // scene graph initialization and modifcation methods
    void initCalibrationGraph();

    // control flow methods
    void startPropCalibration();
    void cancelPropCalibration();
    void stepBack();
    void calibControl(int button);
    void calcProp(); 
    void saveProp(); 
    void dumpProp(); 
    static void buttonTriggeredCB(void *data);
    static void removeMessageCB(void *data);

    // auxiliary methods
    void initSampler();
    void logSampleData(char *dataFile);
    void selectProp(int indexOffset);
    void addFeaturePoint();
    void removeFeaturePoint();
    SoTrackedArtifactKit *getTrackedArtifact(SoNode *root);
    void displayMessage(SoMFString &message, int buttonStation = -1, int time = -1);
    void displayMessage(const char *message, int buttonStation = -1, int time = -1);
    void displayMenue(SoMFString &message);
    void removeMessage();

    So2DMenueKit *loadMenue(SbString name);
    void switchMenue(enum menues which);
    void switchPipSheet(enum pipSheets sheetId);
    static void menueCB(void *data, So2DMenueKit *m);

    static void draggerStarted(void* userData, SoDraggerKit*);
    static void draggerFinished(void* userData, SoDraggerKit*);


//-- data members --

    SbBool draggerActive;
    SoStudierstubeViewer *viewer;
    SoSeparator *root;
    SoSeparator *propRoot;
    SoSeparator *propHook;
    SoSeparator *messageHook;
    SoSeparator *menueHook;
    SoDraggerKit *propDragger;

    So2DMenueKit *menues[2];
    
    SoCoordinate3 *featurePoints;
    SoTrackedArtifactKit *trackedProp;
    int currentFeaturePoint;
    int selectedProp;
 
    int pButton;
    int sButton;
    int appState;
    SoSampler *sampler;
    SoSampler *messageButton;
      
}; //class SoPropCalibrationKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOPROPCALIBRATIONKIT_H_

