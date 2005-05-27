// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       roentgen
//  TYPE:       c++ source code
//  PROJECT:    Studierstube
//  CONTENT:    Demo application for workspace - roentgen
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    hw      Hermann Wurnig
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 17:17:27  gh      last modification
// ===========================================================================
#include <stdlib.h>
#include <stdio.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/sensors/SoFieldSensor.h>

#include <stbapi/window3d/SoWindowKit.h>
#include <stbapi/tracker/SoTrakEngine.h>
#include <stbapi/workspace/StbCommunicator.h>
#include <stbapi/resource/SoUserKit.h>
#include <stbapi/workspace/appdef.h>
#include <Inventor/actions/SoWriteAction.h>
#include <stbapi/seams/SoSeamKit.h>
#include <stbapi/util/ivio.h>
#ifdef _SGI_SOURCE
#include <unistd.h>
#endif

#include <stbapi/util/zpthread.h>
#include "roentgen.h"

void transformChangeCB(void *data, SoSensor *)
{
    penTransformStruct *para = (penTransformStruct *) data;

    para->borderTransform->translation =
        para->penTransform->translation.getValue()-
        para->roentgenWindow->getPosition();

    para->borderTransform->rotation =
        para->penTransform->rotation.getValue()*
        para->roentgenWindow->getRotation().inverse();
}

void setupRoentgenGeometry(geometryLoadedMsg *msg)
{
    SoSeparator *clientVolumeSep = msg->roentgenWindow->getClientVolumeSeparator();

    clientVolumeSep->addChild(msg->maleSeparator);
    clientVolumeSep->addChild(msg->penSeparator);
}

//-------------------------------------------------------------------
// loads the geometry of the male, the skull and the skeleton
// using a thread
// when finished, the app sends a message to itself to add the loaded
// geometry to the client area of the window
//-------------------------------------------------------------------
void loadGeometry(SoWindowKit *window, StbCommunicator * comMan, int userID)
{
    int i;

    SoFieldSensor *transformSensor;

    SoTransform  *userTransform;
    SoSwitch     *lensOnOffSwitch;

    SoSeparator *maleSep;
    SoSeparator *penSep;
    SoSeparator *seamSep;
    SoSeparator *skeletonSep;

    SoTransform *borderTransform;
    SoTransform *seamTransform;
    SoSeparator *secondaryWorldSep;

    SoTrakEngine *userTrakEngine;

        // save references to node sensors for cleanup

    nodeSensorStruct *sensors = (nodeSensorStruct * )
        malloc(sizeof(nodeSensorStruct));

    for (i=0; i<MAX_USERS; i++)
    {
        sensors->usedSensor[i] = NULL;
        sensors->seamSep[i] = NULL;
    }

        // create list of all lens switches

    lensSwitchStruct *switches = (lensSwitchStruct * )
        malloc(sizeof(lensSwitchStruct));

    for (i=0; i<MAX_USERS; i++) switches->lensSwitch[i] = NULL;

        // construct filenames for geometry to load
        // construct filenames for geometry to load

    SbString malePath       = comMan->workingDirectory;
    SbString penPath        = comMan->workingDirectory;
    SbString skeletonPath   = comMan->workingDirectory;

    malePath               += "male.iv";
    penPath                += "pen.iv";
    skeletonPath           += "skeleton.iv";

        // read in male geometry

    maleSep = readFile(malePath.getString());
    if (maleSep == NULL)
    {
        printf("Couldn't load male geometry\n");
        return;
    }


    skeletonSep = readFile(skeletonPath.getString());
    if (skeletonSep == NULL)
    {
        printf("Couldn't load male geometry\n");
        return;
    }


    penSep = new SoSeparator;
    penSep->ref();

   const int32_t *userIDs = comMan->getUserIDs();
    for (i=0; i<comMan->getUserNumber(); i++)
    {
        if (comMan->getPenFromUser(userIDs[i]) != -1)
        {
            seamSep = readFile(penPath.getString());
            if (seamSep == NULL)
            {
                printf("Couldn't load pen geometry\n");
                return;
            }

            borderTransform     = (SoTransform *) SoNode::getByName("penBorderTransform");
            seamTransform       = (SoTransform *) SoNode::getByName("penSeamTransform");
            secondaryWorldSep   = (SoSeparator *) SoNode::getByName("seamWorldSeparator");

            borderTransform->setName("");
            seamTransform->setName("");
            secondaryWorldSep->setName("");

            secondaryWorldSep->addChild(skeletonSep);

            seamTransform->translation.connectFrom(&borderTransform->translation);
            seamTransform->rotation.connectFrom(&borderTransform->rotation);

                // get trak engine of pen, connect a transform node and setup
                // a sensor to set the seam transformation in the window

            userTrakEngine =
             comMan->getSoTrakEngine(comMan->getPenFromUser(userIDs[i]));

            userTransform = new SoTransform;
            userTransform->translation.connectFrom(&userTrakEngine->translation);
            userTransform->rotation.connectFrom(&userTrakEngine->rotation);
            userTransform->ref();

            penTransformStruct *penXfStruct =
                (penTransformStruct *) malloc(sizeof(penTransformStruct));

            penXfStruct->borderTransform = borderTransform;
            penXfStruct->penTransform = userTransform;
            penXfStruct->roentgenWindow = window;

            transformSensor = new SoFieldSensor(transformChangeCB, (void *) penXfStruct);
            transformSensor->attach(&userTransform->translation);

            sensors->usedSensor[i] = transformSensor;
            sensors->seamSep[i] = seamSep;

            lensOnOffSwitch = new SoSwitch;
            lensOnOffSwitch->addChild(seamSep);

            if (SoUserKit::getUserKitIndex(userID) == i)
                lensOnOffSwitch->whichChild = 0;
            else
                lensOnOffSwitch->whichChild = -1;

            switches->lensSwitch[i] = lensOnOffSwitch;

            penSep->addChild(lensOnOffSwitch);

        }
    }

        // inform application that geometry is loaded and ready to be
        // added to the window and HMDs

    geometryLoadedMsg *msg =
        (geometryLoadedMsg *) malloc(sizeof(geometryLoadedMsg));

    strcpy(msg->messageID, "ROENTGEN_GEOMETRY_LOADED");
    msg->maleSeparator        = maleSep;
    msg->penSeparator         = penSep;
    msg->communicationManager = comMan;
    msg->roentgenWindow       = window;

        // send message

    comMan->sendMessage(MSG_ROENTGEN_GEOMETRY_LOADED,
                        comMan->appID, comMan->appID, (void *) msg);
    //setupRoentgenGeometry(msg);

    comMan->setExitData((void *) sensors);
    comMan->setMessageData((void *) switches);

}



//-------------------------------------------------------------------
// message handling routine, waits until message arrives that
// geometry has been successfully loaded and then adds it to the
// window
//-------------------------------------------------------------------
ROENTGEN_API void Stb_getMessage(void *userData, int message, int sender, void *messageData)
{

    if (message == MSG_ROENTGEN_GEOMETRY_LOADED)
    {
        geometryLoadedMsg *msg = (geometryLoadedMsg *) messageData;

        if (strcmp(msg->messageID, "ROENTGEN_GEOMETRY_LOADED") == 0)
        {
            StbCommunicator * comMan = msg->communicationManager;

            if (comMan->appID == sender)
                setupRoentgenGeometry(msg);
        }
    }
    else if (message == AM_APP_TO_FOREGROUND)
    {
        int userID = (int) messageData;

        if (userData == NULL) return;

        ((lensSwitchStruct *) userData)->
          lensSwitch[SoUserKit::getUserKitIndex(userID)]->whichChild = 0;
    }
    else if (message == AM_APP_TO_BACKGROUND)
    {
        int userID = (int) messageData;

        if (userData == NULL) return;

        ((lensSwitchStruct *) userData)->
          lensSwitch[SoUserKit::getUserKitIndex(userID)]->whichChild = -1;
    }

}


ROENTGEN_API void Stb_init(int userID, StbCommunicator *comMan)
{
    // create window in which the male geometry is shown

    WindowCreate wc;

    wc.position.setValue(0,0,0);
    wc.orientation.setValue(SbVec3f(1.0, 0.0, 0.0), 0);
    wc.size.setValue(0.4, 0.3, 0.8);
    wc.style = WS_NOTITLE+WS_CLIENTVOLUMEINVISIBLE+WS_MOVEABLE+WS_RESIZEABLE;
    wc.state = BM_MAXIMIZED;
    wc.title = SbString("");
    wc.eventMask = Base3D::BM_ALL_EVENTS;
    wc.clientEventMask = Base3D::BM_ALL_EVENTS;

    SoWindowKit * windowKit = comMan->createWindow(
                        NULL, &wc, NULL, NULL);
    windowKit->setBorderThickness(0.01);
    windowKit->setIconSize(SbVec3f(0.05, 0.05, 0.05));

        // get client area of window

    SoSeparator *clientVolumeSep = windowKit->getClientVolumeSeparator();
        // setup thread for loading the geometry

    loadGeometry(windowKit, comMan, userID);


}

ROENTGEN_API SbBool Stb_exit(void * data)
{
    nodeSensorStruct *sensors = (nodeSensorStruct *) data;

    for (int i=0; i<MAX_USERS;i++)
    {
        if(sensors->usedSensor[i] != NULL)
        {
            if(sensors->usedSensor[i]->getAttachedField())
            {
                SoTransform *trans=
                ((SoTransform*)
                (sensors->usedSensor[i]->getAttachedField()->getContainer()));
                if(trans)
                {
                    trans->translation.disconnect();
                    trans->rotation.disconnect();
                }
            }
            delete (sensors->usedSensor[i]);
        }
        if(sensors->seamSep[i] != NULL)
            sensors->seamSep[i]->removeAllChildren();
    }
    return TRUE;
}
