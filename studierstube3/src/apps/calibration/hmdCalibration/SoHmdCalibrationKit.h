/* ========================================================================
 * Copyright (C) 2000,2001,2002  Vienna University of Technology
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT:
 * ======================================================================== */
/** header file for SoHmdCalibrationKit
 *
 *
 * @author Rainer Splechtna
 *
 * $Id: SoHmdCalibrationKit.h 3426 2004-07-14 12:11:27Z tamer $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _SOHMDCALIBRATIONKIT_H_
#define _SOHMDCALIBRATIONKIT_H_

#include <Inventor/nodes/SoEventCallback.h>

#include <stbapi/workspace/SoContextKit.h>
#include <stbapi/resource/viewer/SoStereoCameraKit.h>

#include <stbapi/tracker/SoTrakEngine.h>

#include <stbapi/widgets/SoPushButton.h> 
#include <stbapi/widgets/SoToggleButton.h>

#include "../utils/SoSampler.h"
#include "hmdcalibration.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class HMDCALIBRATION_API SoHmdCalibrationKit
:
    public SoContextKit
{
    SO_KIT_HEADER(SoHmdCalibrationKit);

    enum pipSheets { FIRST_SHEET,
                     SECOND_SHEET,
                     EMPTY_SHEET};
    enum samplingMode { FULL,
                        SHORT};
    enum calibrationState { START,
                            SAMPLING_LEFT_EYE_DATA,
                            SAMPLING_RIGHT_EYE_DATA,
                            SAMPLING_FINISHED};

public:
    static void initClass(             //static class initialization
        );
    SoHmdCalibrationKit(                   //constructor
        );
    SoContextKit* factory(             //override this to create new context
        );                             //of a specific type
    ~SoHmdCalibrationKit(
        );
    
    static void restartCB(         
        void* data,                    //user data
        SoButtonKit* button           //pointer to pressed button
        );
    static void exitCB(         
        void* data,                    //user data
        SoButtonKit* button           //pointer to pressed button
        );
    static void keepNewCameraCB(         
        void* data,                    //user data
        SoButtonKit* button           //pointer to pressed button
        );
    static void testCB(         
        void* data,                    //user data
        SoButtonKit* button           //pointer to pressed button
        );

    static void startFullCB(         
        void* data,                    //user data
        SoButtonKit* button           //pointer to pressed button
        );
    static void startShortCB(         
        void* data,                    //user data
        SoButtonKit* button           //pointer to pressed button
        );

    static void keyEventCB(
        void *data, 
        SoEventCallback *event
        );
 
    // enable message handling
    AppMessageCB* getAppMessageCB();
    static void AppMessageCBFunc(void* userData,int message,
				 int sender,void* data);

    void startCameraOptimization();
    static void doCameraOptimization(void *data);

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
    
    // scene graph initialization and modifcation methods
    void initCalibrationGraph();
    void displayPoint();
    void displayText(const char *s);
    SoSeparator *build3Dpoint(const SbVec2f &point2D, SbBool near);
    SoSeparator *build3DText(const char *name);
    SoMaterial *createDistanceSensitiveColor(SbBool near);

    // control flow methods
    void startCalibration();
    void calibControl(int button);
    void validateCamera(SbBool valid, SoOffAxisCamera *camera); 
    static void buttonTriggered(void *data);

    // auxiliary methods
    void logSampleData(char *dataFile);

//-- data members --
    /* set the values of these fields within the
       config-file */
    /*  Specifies the screen position of the reference points (i.e. the points, 
	    that are displayed during the calibration process) in normalized 
		screen coordinates.
    */
    SoMFVec2f referencePoints;
    /*  These fields specify the values, which are used to determine
        whether a certain penPosition is valid for sampling or not.
    */
	/* Specifies the distance from the head tracker at which sampling of 
	   "near points" is valid. */
    SoSFFloat nearMarkerDistance;
	/* Specifies the distance from the head tracker at which sampling of 
	   "far points" is valid. */
    SoSFFloat farMarkerDistance;
	/* Specifies the distance epsilon for near and far markerDistance.
	   */
    SoSFFloat epsilon;
    /* Specifies color of marker, when sampling is valid.
    */
    SoSFVec3f enabledColor;
    /* Specifies color of marker, when sampling is not valid.
    */
    SoSFVec3f disabledColor;
    /* Specifies the averageErrorThreshold (max RMS error), given in percentage
	   of screen space.	   
    */
    SoSFFloat averageErrorThreshold;
    /*  Specifies the maximum allowed distance between any two rays.
        (where a ray is constructed from the two samples associated with
         a reference point).
    */
    SoSFFloat validDistance;
    /* Specifies if the video image overlay shall be mirrored (along y-axis) 
       during the calibration process. Useful for achieving better hand-eye 
       coordination ;-)
    */
    SoSFBool  useMirrorMode;
    /*  This field is used to access the current result of the dynamic validity
        check of the distance between pen and head tracker.
        This field is only updated during the calibration process.
		@warning only used internally
    */
    SoSFBool  validPenPosition; 
	/* Specifies the path to the application's data directory */
    SoSFString dataDirectory;


    SoToggleButton *keepCheckBox;
    SoToggleButton *testCheckBox;
    SbBool keepCamera;
    SoSeparator *root;
    SoStudierstubeViewer *viewer;
    int pButton;
    int sButton;
    SbVec2f viewportSize;
    SbBool stereoMode;
    SbBool relativeSampling;
    int appState, samplingMode;
    SoStereoCameraKit *oldCamera, *newCamera;
    SoSeparator *leftGraph;
    SoSeparator *rightGraph;
    int calibPoint;
    SbBool textMode;
    SoSampler *sampler;
    long pid;

protected:
    static void updateCB(void *data, SoSensor *);
    // breiting
    SoEngineOutput *oldConnectedFieldTranslation;
    SoEngineOutput *oldConnectedFieldRotation;

   
}; //class SoHmdCalibrationKit

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif//_SOHMDCALIBRATIONKIT_H_

