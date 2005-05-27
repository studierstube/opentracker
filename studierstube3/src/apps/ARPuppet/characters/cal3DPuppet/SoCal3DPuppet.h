/* ========================================================================
 * Copyright (C) 2000-2003  Vienna University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
 * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
 * Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/* SoCal3DPuppet.h contains the definition of the class SoCal3DPuppet, 
 * a puppet based on the Cal3D library
 *
 * @author Istvan Barakonyi
 *
 * $Id: SoCal3DPuppet.h 3652 2004-09-03 15:41:24Z bara $
 * @file
 */
/* ======================================================================== */

/*
 * -------------------------------------------------------------------------------
 * This code is relying on the Cal3D library and uses some code snippets from the 
 * cally_3d demo examples.
 * 
 * Copyright (C) 2001 Bruno 'Beosil' Heidelberger
 * Copyright notice of Cal3D:
 * -------------------------------------------------------------------------------
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * -------------------------------------------------------------------------------*/

#ifndef _SOCAL3DPUPPET_H_
#define _SOCAL3DPUPPET_H_

#include "cal3DPuppet.h"

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoSFVec3f.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/SbPList.h>

#include <string>
#include <vector>

#include "cal3d/cal3d.h" 

#ifdef __COIN__
#include <Inventor/system/gl.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

class CAL3DPUPPET_API SoCal3DPuppet;

typedef void BoneManipulationCB(void *data,SoCal3DPuppet *cal3dPuppet);

class CAL3DPUPPET_API SoCal3DPuppet : public SoBaseKit
{
    SO_KIT_HEADER(SoCal3DPuppet);

    SO_KIT_CATALOG_ENTRY_HEADER(root);
    SO_KIT_CATALOG_ENTRY_HEADER(bbSwitch);
    SO_KIT_CATALOG_ENTRY_HEADER(bbTrans);
    SO_KIT_CATALOG_ENTRY_HEADER(bbCoord);
    SO_KIT_CATALOG_ENTRY_HEADER(bbMaterial);
    SO_KIT_CATALOG_ENTRY_HEADER(bbMaterialBinding);
    SO_KIT_CATALOG_ENTRY_HEADER(bbLightModel);
    SO_KIT_CATALOG_ENTRY_HEADER(bbDrawStyle);
    SO_KIT_CATALOG_ENTRY_HEADER(bbIndexedFaceSet);

public:

    SoCal3DPuppet();
    ~SoCal3DPuppet();

    SoSFString pathName;
    SoSFString fileName;
    SoSFInt32 lod;
    SoSFBool visible;
    SoSFBool animate;
    SoSFBool skeleton;
    SoSFBool wireframe;
    SoSFBool mesh;
    SoSFVec3f boneColor;
    SoSFVec3f jointColor;
    SoSFBool boneBoundingBoxOn;
    SoSFInt32 boneNum;
    SoMFString boneName;
    SoMFInt32 bbColorIndex;
    SoSFString execute;
    SoSFString state;
    SoMFFloat blend;
    SoMFString blendName;
    SoSFTrigger clear;
    SoSFFloat delayIn;
    SoSFFloat delayOut;
    SoSFBool directManipulation;

    CalModel* getCalModel() { return calModel; }
    void addBoneManipulationFunction(BoneManipulationCB *cb, void *data);
    void removeBoneManipulationFunction(BoneManipulationCB *cb, void *data);

SoINTERNAL public:
    
    static void initClass();

protected:

    virtual void GLRender(SoGLRenderAction *action);
    virtual void computeBBox(SoAction *action,SbBox3f &box, SbVec3f &center);
    virtual void getBoundingBox(SoGetBoundingBoxAction *action);
    virtual void rayPick(SoRayPickAction *action);

    void TestDraw();

private:

    CalCoreModel *calCoreModel;
    CalModel *calModel;

    /// Static information stored for core models already loaded
    static SbPList calCoreModelList;
    static SbPList animationIdList;
    static SbPList animationNameList;

    /// Animation information for the current model
    SbIntList *animationId;
    SbStringList *animationName;

    // storing callback functions and user data
    std::vector<BoneManipulationCB*> boneManipCallback;
    std::vector<void*> boneManipCallbackData;

    bool initialized;
    std::string name;
    int meshId[32];
    int meshCount;
    GLuint textureId[32];
    int textureCount;
    float renderScale;
    float minBounding[3];
    float maxBounding[3];
    float *blendingFactor;
    double previousTime;

    bool initCoreModel(const std::string& strFilename);
    bool initModel();
    int  loadTexture(const std::string& strFilename);
    void renderSkeleton();  
    void renderMesh(SoState* state);
    void setMotionBlending();
    double getCurrentTime();
    bool findCoreModel(SbString *fullFileName);
    void calculateBB();
    void update(float deltaTime);

    SoTimerSensor *timerSensor;
    SoFieldSensor *fileNameSensor;
    SoFieldSensor *lodSensor;
    SoFieldSensor *animateSensor;
    SoFieldSensor *boneBoundingBoxOnSensor;
    SoFieldSensor *bbColorIndexSensor;
    SoFieldSensor *stateSensor;
    SoFieldSensor *executeSensor;
    SoFieldSensor *blendSensor;
    SoFieldSensor *clearSensor;

    /// Setting up field connections
    virtual SbBool setUpConnections(SbBool onOff, SbBool doItAlways = FALSE);

    static void timerSensorCallback(void *data, SoSensor *);
    static void fileNameCB(void *data,SoSensor *sensor);
    static void lodCB(void *data,SoSensor *sensor);
    static void animateCB(void *data,SoSensor *sensor);
    static void boneBoundingBoxOnCB(void *data,SoSensor *sensor);
    static void bbColorIndexCB(void *data,SoSensor *sensor);
    static void executeCB(void *data,SoSensor *sensor);
    static void stateCB(void *data,SoSensor *sensor);
    static void blendCB(void *data,SoSensor *sensor);
    static void clearCB(void *data,SoSensor *sensor);
};

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif // _SOCAL3DPUPPET_H_
