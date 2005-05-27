// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       cameraOptimizer
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    HMD calibration camera optimization
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    rcs		Rainer Splechtna
// ===========================================================================

#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFRotation.h>

#include <stbapi/resource/viewer/SoOffAxisCamera.h>


SbBool
buildCamera(    SoOffAxisCamera *&optimizedCamera,

                const SoMFVec2f &referencePoints,
                const SoMFVec3f &sampledPenPosition,  
                const SoMFRotation &sampledPenRotation,
                const SoMFVec3f &sampledHeadPosition,
                const SoMFRotation &sampledHeadRotation,

                SbBool relativeSampling = TRUE,
                float averageErrorThreshold = 0.01, // 1 percent
                float validDistance = 0.0125,       // 1.25 cm
                const char *logFileName = "camopt_log.txt",
                // opt flags
                SbBool eyeOpt = FALSE,
                SbBool sizeOpt = TRUE,
                SbBool logging = TRUE,
                SbBool secondOpt = TRUE,
                int badSamplesCount = 2
            );

SbVec3f 
calculateEyepointPosition(SoMFVec3f &sampledPoints, 
                          float validDistance, 
                          SbBool opt);

SbBool
optimizePlane(const SoMFVec3f &samples, 
              SbPlane &plane,
              SoMFVec3f &pointsOnPlane);

 
