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
/* SceneManager.cxx contains the implementation of SceneManager
 *
 * @author Istvan Barakonyi
 *
 * $Id: SceneManager.cxx 4209 2005-05-19 09:06:39Z bara $ 
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

// Inventor classes
#include "Inventor/nodes/SoSwitch.h"
#include "Inventor/nodes/SoTransform.h"
#include "Inventor/nodes/SoMaterial.h"
#include "Inventor/nodes/SoTexture2.h"
#include "Inventor/engines/SoSelectOne.h"
#include "Inventor/sensors/SoAlarmSensor.h"

// Stb specific classes
#include <stbapi/interaction/SoDragKit.h>
#include <stbapi/misc/SoFrameBuffer.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// for time function to support the generation of random seeds for the rand() function
#include <time.h> 

#include "SceneManager.h"

#define PI 3.141592653f
#define PI_PER_2 1.57079632f
#define PI_PER_4 0.78539816f

static const SbVec2f playfieldDirection[4]={
    SbVec2f(0,-1), // north
    SbVec2f(1,0),  // east
    SbVec2f(0,1),  // south
    SbVec2f(-1,0)  // west
};

SO_KIT_SOURCE(SceneManager);

void SceneManager::initClass()
{
   SO_KIT_INIT_CLASS(SceneManager, SoPuppeteerKit, "SoPuppeteerKit");
}

SceneManager::SceneManager()
{
    SO_KIT_CONSTRUCTOR(SceneManager);

    // tile info
    // a) parts
    SO_KIT_ADD_CATALOG_ENTRY(tileLibrary, SoSwitch, FALSE, this, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(newTiles, SoSeparator, FALSE, this, \0, FALSE);
    // b) field
    SO_KIT_ADD_FIELD(hotspotOffset, (0.0f,0.0f,0.0f));

    // playfield info
    SO_KIT_ADD_FIELD(maxCoordinate, (0,0));
    SO_KIT_ADD_FIELD(tileSize, (0.0f));
    SO_KIT_ADD_FIELD(globalScale, (1.0f));

    // position, orientation and type info for new tiles to be placed
    SO_KIT_ADD_FIELD(tileToPlacePosition, (0.0f,0.0f,0.0f));
    SO_KIT_ADD_FIELD(tileToPlaceOrientation, (SbRotation::identity()));
    SO_KIT_ADD_FIELD(tileToPlaceType, (INVALID));
    SO_KIT_ADD_FIELD(tileToPlaceTypeRemote, (INVALID));
    SO_KIT_ADD_FIELD(tileSnappedPosition, (0.0f,0.0f,0.0f));
    SO_KIT_ADD_FIELD(tileSnappedOrientation, (SbRotation::identity()));
    SO_KIT_ADD_FIELD(tileSnappedInfoTemp, (INVALID,INVALID,INVALID,INVALID));
    SO_KIT_ADD_FIELD(tileSnappedInfo, (INVALID,INVALID,INVALID,INVALID));
    SO_KIT_ADD_FIELD(tileSnappedInfoRemote, (INVALID,INVALID,INVALID,INVALID));
    SO_KIT_ADD_FIELD(useRemoteInfo, (FALSE));
    SO_KIT_ADD_FIELD(maxTileTypeID, (0));
    SO_KIT_ADD_FIELD(avoidTileType, (SceneManager::INVALID));
    SO_KIT_ADD_FIELD(tileColor, (0.0f,0.0f,0.0f));

    // occupation info
    SO_KIT_ADD_FIELD(playfieldInfo, (INVALID,INVALID));
    SO_KIT_ADD_FIELD(playfieldInitInfo, (INVALID,INVALID,INVALID,INVALID));
    SO_KIT_ADD_FIELD(bonusObjectInfo, (INVALID,INVALID,INVALID,0)); // TODO: implement functionality
    SO_KIT_ADD_FIELD(obstacleTilePosition, (0.0f,0.0f,0.0f));
    SO_KIT_ADD_FIELD(init, ());

    // information for character motion
    SO_KIT_ADD_FIELD(previousTileCoord, (INVALID,INVALID));
    SO_KIT_ADD_FIELD(currentTileCoord, (INVALID,INVALID));
    SO_KIT_ADD_FIELD(nextTileCoord, (INVALID,INVALID));
    SO_KIT_ADD_FIELD(targetReached, (FALSE));
    SO_KIT_ADD_FIELD(playerCompletedTile, (INVALID));
    SO_KIT_ADD_FIELD(seekNextTile, (INVALID));
    SO_KIT_ADD_FIELD(hotspotSet, (INVALID));
    SO_KIT_ADD_FIELD(targetReached, (FALSE));
    SO_KIT_ADD_FIELD(nextTileValid, (FALSE));
    SO_KIT_ADD_FIELD(firstTilePlaced, (FALSE));

    // user info
    SO_KIT_ADD_FIELD(playerNum, (0));
    SO_KIT_ADD_FIELD(tileNum, (0));
    SO_KIT_ADD_FIELD(tileToPlaceNumMax, (0));
    SO_KIT_ADD_FIELD(tileToPlaceNum, (0));
    SO_KIT_ADD_FIELD(tileTotalNum, (0));
    SO_KIT_ADD_FIELD(timeOut, (SbTime(3.0f)));
    SO_KIT_ADD_FIELD(enabled, (FALSE));
    SO_KIT_ADD_FIELD(isOverValidCoord, (FALSE));
    SO_KIT_ADD_FIELD(tilePlacedCursorOn, (FALSE));
    SO_KIT_ADD_FIELD(tilePlacedCursorTimeOut, (SbTime(1.0f)));

    // status/display attributes
    SO_KIT_ADD_FIELD(initError, (0));
    SO_KIT_ADD_FIELD(showPhysicalTiles, (FALSE));
    SO_KIT_ADD_FIELD(physicalTileTexture, (""));

    SO_KIT_INIT_INSTANCE();

    // we need initialization
    initialized=false;

    // turn command loop off for this puppeteer as we won't accept any commands
    timerInterval=SbTime();

    // create field sensors
    tileToPlacePositionSensor=new SoFieldSensor(tilePoseCB,this);
    tileToPlaceOrientationSensor=new SoFieldSensor(tilePoseCB,this);
    tileToPlaceTypeRemoteSensor=new SoFieldSensor(tileToPlaceTypeRemoteCB,this);
    tileSnappedInfoRemoteSensor=new SoFieldSensor(tileSnappedInfoRemoteCB,this);
    playerCompletedTileSensor=new SoFieldSensor(playerCompletedTileCB,this);
    seekNextTileSensor=new SoFieldSensor(seekNextTileCB,this);
    initSensor=new SoFieldSensor(initCB,this);

    // compose scene graph for old tiles
    // 1.) physical tiles
    SoSeparator *physicalTilesRoot=new SoSeparator; physicalTilesRoot->ref();
    SoSwitch *physicalTilesMode=new SoSwitch; physicalTilesMode->ref();
    physicalTiles=new SoSeparator; physicalTiles->ref();
    physicalTilesMode->whichChild.connectFrom(&showPhysicalTiles);
    physicalTilesRoot->addChild(physicalTilesMode);
    physicalTilesRoot->addChild(physicalTiles);
    SoTexture2 *tileTexture=new SoTexture2; tileTexture->ref();
    tileTexture->filename.connectFrom(&physicalTileTexture);
    tileTexture->model=SoTexture2::MODULATE;
    SoFrameBuffer *renderOnlyZbuffer=new SoFrameBuffer;
    renderOnlyZbuffer->mask=SoFrameBuffer::ALPHA;
    physicalTilesMode->addChild(renderOnlyZbuffer);
    physicalTilesMode->addChild(tileTexture);

    // 2.) virtual tiles
    SoSeparator *oldTilesRoot=new SoSeparator; oldTilesRoot->ref();
    oldTiles=new SoSeparator; oldTiles->ref();
    oldTilesRoot->addChild(physicalTilesRoot);
    oldTilesRoot->addChild(oldTiles);

    // compose 'puppets' scene graph
    SoSeparator *tileRoot=new SoSeparator; tileRoot->ref();
    tileRoot->addChild(oldTilesRoot);
    SoDragKit *puppetDragKit=SO_GET_ANY_PART(this,"puppets",SoDragKit);
    SoSeparator *dragKitContent=(SoSeparator*)(puppetDragKit->getPart("content",TRUE));
    dragKitContent->addChild(tileRoot);

    // seed for random number generator
    srand((unsigned)time(NULL));

    setUpConnections(TRUE,TRUE);
}

SceneManager::~SceneManager()
{
    // clean up timer lists
    int i;
    for (i=0;i<timerList.getLength();timerList.remove(0));
    for (i=0;i<tilePlacedTimerList.getLength();tilePlacedTimerList.remove(0));

    // delete field sensors
    delete tileToPlacePositionSensor;
    delete tileToPlaceOrientationSensor;
    delete tileToPlaceTypeRemoteSensor;
    delete tileSnappedInfoRemoteSensor;
    delete playerCompletedTileSensor;
    delete seekNextTileSensor;
    delete initSensor;
}

SbBool SceneManager::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    
    if (onOff) {
        // attach field sensors
        tileToPlacePositionSensor->attach(&tileToPlacePosition);
        tileToPlaceOrientationSensor->attach(&tileToPlaceOrientation);
        tileToPlaceTypeRemoteSensor->attach(&tileToPlaceTypeRemote);
        tileSnappedInfoRemoteSensor->attach(&tileSnappedInfoRemote);
        playerCompletedTileSensor->attach(&playerCompletedTile);
        seekNextTileSensor->attach(&seekNextTile);
        initSensor->attach(&init);
    }
    else {
        // detach field sensors
        tileToPlacePositionSensor->detach();
        tileToPlaceOrientationSensor->detach();
        tileToPlaceTypeRemoteSensor->detach();
        tileSnappedInfoRemoteSensor->detach();
        playerCompletedTileSensor->detach();
        seekNextTileSensor->detach();
        initSensor->detach();
    }

    // set up connections in parent class SoPuppeteerKit
    SoPuppeteerKit::setUpConnections(onOff,doItAlways);

    return !(connectionsSetUp = onOff);
}

void SceneManager::tilePoseCB(void *data,SoSensor *sensor)
{
    SceneManager *sm=(SceneManager*)data;

    // init total number of tiles
    sm->tileTotalNum=sm->tileToPlacePosition.getNum();

    // snap input position elements to playfield grid
    for (int i=0;i<sm->tileTotalNum.getValue();i++){

        SbVec3f normalVec,localPointer;
        SbVec2f oldCoord,newCoord;
        float oldAngle,newAngle;
        SbVec4f info(INVALID,INVALID,INVALID,INVALID);

        // init interruption condition
        bool interrupted=false;

        // determine and set user ID and check whether user is allowed to place the current tile
        int userID,userTileID;
        sm->getUserIdFromTileNum(i,userID,userTileID);
        if (userID==INVALID) return;

        // if the information about tile#i comes from a remote source, skip this tile
        bool remoteInfo=(i<sm->useRemoteInfo.getNum()) ? sm->useRemoteInfo[i] : (sm->useRemoteInfo.getNum() ? sm->useRemoteInfo[sm->useRemoteInfo.getNum()-1] : false);

        interrupted=
                    // check whether information comes from remote source
                    remoteInfo ||
                    // check whether the current player is enabled to place tiles
                    (
                        userID<sm->enabled.getNum() ? 
                            !(sm->enabled[userID]) : sm->enabled.getNum() ? !(sm->enabled[sm->enabled.getNum()-1]) : TRUE
                    ) || 
                    // check whether the current player has a tile/tiles to place which his character hasn't walked across yet
                    (
                        userID<sm->tileToPlaceNum.getNum() ? 
                            userTileID+1>sm->tileToPlaceNum[userID] : 
                            (sm->tileToPlaceNum.getNum() ? userTileID+1>sm->tileToPlaceNum[sm->tileToPlaceNum.getNum()-1] : TRUE)
                    );

        // project tile position vector onto the plane of the playfield and calculate (x,y) playfield coordinates --> newCoord
        if (!interrupted){
            // create normal vector of the playfield
            sm->orientationBase.getValue().multVec(SbVec3f(0,1,0),normalVec);

            // create vector going from the origin of the playfield to the projected point of the tile position vector
            float t=(sm->positionBase.getValue()-sm->tileToPlacePosition[i]).dot(normalVec);
            SbVec3f noSnapPos=sm->tileToPlacePosition[i]+t*normalVec;            
            sm->orientationBase.getValue().inverse().multVec(noSnapPos-sm->positionBase.getValue(),localPointer);

            // snap input position to playfield grid if within playfield, 
            // otherwise interrupt calculations since we can freely move the tile
            for (int j=0;j<2;j++){
                int k=(j==1) ? 2 : 0;
                int subCoord;
                if (fabs(localPointer[k]*2.0f)>sm->maxCoordinate.getValue()[j]*sm->tileSize.getValue()){
                    interrupted=true;
                    break;
                }
                subCoord=(int)(sm->maxCoordinate.getValue()[j]/2.0+(localPointer[k]/sm->tileSize.getValue()/sm->globalScale.getValue()));
                newCoord[j]=(float)subCoord;
            }
        }

        if (!interrupted){
            // set coordinate and userID field for temporary tile info
            info[0]=newCoord[0];
            info[1]=newCoord[1];
            info[2]=(float)userID;

            // check playfield occupancy matrix whether the current candidate field has been already occupied
            SbVec2f testTileInfo=sm->playfieldInfo[sm->getPlayfieldInfoIndex((int)(info[0]),(int)(info[1]))];
            interrupted=true;
            if (floor(testTileInfo[0])<0){ // the tile doesn't belong to any of the users
                // check whether current tile would be a continuation of the player's path, which means there is a tile 
                // to the north, east, south or west of the current tile that belongs to the user and is not an end tile
                for (int j=0;j<4;j++){
                    SbVec2f neighborTileCoord=newCoord+playfieldDirection[j];
                    int neighborTileIndex=sm->getPlayfieldInfoIndex(neighborTileCoord);
                    if (neighborTileIndex!=INVALID){
                        SbVec2f neighborTileInfo=sm->playfieldInfo[neighborTileIndex];
                        if (floor(neighborTileInfo[0])==userID && floor(neighborTileInfo[1])!=ENDTILE){
                            interrupted=false;
                            break;
                        }                        
                    }
                }
            }
        }

        // if outside playfield, move tile freely around
        if (interrupted) {
            // position and orientation unmodified
            sm->tileSnappedPosition.set1Value(i,sm->tileToPlacePosition[i]);
            sm->tileSnappedOrientation.set1Value(
                i, i<sm->tileToPlaceOrientation.getNum() ? 
                sm->tileToPlaceOrientation[i] : 
                    (sm->tileToPlaceOrientation.getNum() ? sm->tileToPlaceOrientation[sm->tileToPlaceOrientation.getNum()-1] : SbRotation::identity())
            );
            info=SbVec4f(INVALID,INVALID,(float)userID,INVALID);
            sm->tileSnappedInfoTemp.set1Value(i,info);

            // unschedule timer measuring hovering over a valid spot
            if (i<sm->timerList.getLength()){
                SoAlarmSensor *currentSensor=(SoAlarmSensor*)(sm->timerList[i]);
                if (currentSensor->isScheduled()) currentSensor->unschedule();
            }
            
            // give feedback that coordinates are invalid, the tile cannot be fixed there
            sm->isOverValidCoord.set1Value(i,FALSE);
            
            // no more business to do for now for this tile
            continue;
        }
        // if inside playfield and survived hitherto tests, snap input tile position and orientation to grid
        else {
            // get previous information about the tile
            SbVec4f tempInfo;
            if (i<sm->tileSnappedInfoTemp.getNum()) tempInfo=sm->tileSnappedInfoTemp[i];
            else tempInfo=SbVec4f(INVALID,INVALID,INVALID,INVALID);

            // retrieve previous coordinates
            oldCoord=SbVec2f(tempInfo[0],tempInfo[1]);

            // retrieve previous angle
            oldAngle=(i<sm->tileSnappedInfoTemp.getNum()) ? tempInfo[3] : 0;

            // set orientation snapped to grid
            SbVec3f localPointerOrient;

            // set relative rotation angle for tile
            if (i<sm->tileToPlaceOrientation.getNum()) sm->tileToPlaceOrientation[i].multVec(SbVec3f(1,0,0),localPointerOrient);
            else sm->tileToPlaceOrientation[sm->tileToPlaceOrientation.getNum()-1].multVec(SbVec3f(1,0,0),localPointerOrient);
            newAngle=(float)(-atan2(localPointerOrient[2],localPointerOrient[0]));
            if (newAngle>=-PI_PER_4 && newAngle<PI_PER_4) newAngle=0;
            else if (newAngle>=PI_PER_4 && newAngle<3*PI_PER_4) newAngle=PI_PER_2;
            else if (newAngle>=3*PI_PER_4 || newAngle<=-3*PI_PER_4) newAngle=PI;
            else if (newAngle<=-PI_PER_4 && newAngle>-3*PI_PER_4) newAngle=3*PI_PER_2;

            // set angle field of the temporary tile info
            info[3]=newAngle; // angle and type info will be properly encoded later in timeOutCB, now only store angle info

            // set position snapped to grid
            SbVec3f pos;
            sm->getTilePositionOnField(info,pos);
            sm->orientationBase.getValue().multVec(pos,pos);
            sm->tileSnappedPosition.set1Value(i,(pos+sm->positionBase.getValue())*sm->globalScale.getValue());

            // set orientation snapped to grid
            SbRotation snapOrient=SbRotation(SbVec3f(0,1,0),newAngle)*sm->orientationBase.getValue();
            sm->tileSnappedOrientation.set1Value(i,snapOrient.getValue());

            // check if the playfield coordinates or the tile angle have changed as a result of moving
            if ((newCoord!=oldCoord || newAngle!=oldAngle) && i<sm->timerList.getLength()){
                SoAlarmSensor *currentSensor=(SoAlarmSensor*)(sm->timerList[i]);
                // if no valid value set for timeout, set default time
                currentSensor->setTimeFromNow(
                    i<sm->timeOut.getNum() ? sm->timeOut[i] : (sm->timeOut.getNum() ? sm->timeOut[sm->timeOut.getNum()-1] : SbTime())
                );
                if (currentSensor->isScheduled()) currentSensor->unschedule();
                currentSensor->schedule();
            }

            // set temporary tile info field
            sm->tileSnappedInfoTemp.set1Value(i,info);

            // give feedback that coordinates are valid and ready for accepting the tile
            sm->isOverValidCoord.set1Value(i,TRUE);
        }
    } // end of for (i)
}

void SceneManager::tileToPlaceTypeRemoteCB(void *data,SoSensor *sensor)
{
    SceneManager *sm=(SceneManager*)data;

    SoSwitch *tl=SO_GET_ANY_PART(sm,"tileLibrary",SoSwitch);
    SoSeparator *nt=SO_GET_ANY_PART(sm,"newTiles",SoSeparator);

    int maxIndex=sm->tileToPlaceTypeRemote.getNum()>nt->getNumChildren() ? nt->getNumChildren() : sm->tileToPlaceTypeRemote.getNum();
    for (int i=0;i<maxIndex;i++){
        bool remoteInfo=(i<sm->useRemoteInfo.getNum()) ? sm->useRemoteInfo[i] : (sm->useRemoteInfo.getNum() ? sm->useRemoteInfo[sm->useRemoteInfo.getNum()-1] : false);
        if (remoteInfo){
            SoSeparator *tileRoot=(SoSeparator*)(nt->getChild(i));
            int tileType=sm->tileToPlaceTypeRemote[i];
            if (tileType>=0 && tileType<tl->getNumChildren()) {
                if (tileRoot->getNumChildren()>1) {
                    SoNode *tileNode=tl->getChild(tileType);
                    tileRoot->replaceChild(2,tileNode);
                }
            }
            else printf("SCENEMANAGER: warning: insufficient number of tiles defined in scene manager or wrong remote tile type (%d > %d)\n",tileType,tl->getNumChildren());
        }
    }
}

void SceneManager::tileSnappedInfoRemoteCB(void *data,SoSensor *sensor)
{
    SceneManager *sm=(SceneManager*)data;

    SbVec4f info=sm->tileSnappedInfoRemote.getValue();
    int tileID=(int)((info[2]-(float)(floor(info[2])))*10.0f);

    // place tile on playfield based on remote information
    if (sm->useRemoteInfo[tileID] && tileID>=0) sm->placeTileOnField(tileID,true);
    else printf("SCENEMANAGER: incorrect remote information (%f %f %f %f)\n",info[0],info[1],info[2],info[3]);
}

void SceneManager::playerCompletedTileCB(void *data,SoSensor *sensor)
{
    SceneManager *sm=(SceneManager*)data;
    int userID=sm->playerCompletedTile.getValue();
    if (userID<0) return;
    sm->previousTileCoord.set1Value(userID,sm->currentTileCoord[userID]);
    sm->currentTileCoord.set1Value(userID,sm->nextTileCoord[userID]);
    
    // increase the number of tiles that may be placed but observe maximum
    int tileTPNum=sm->tileToPlaceNum[userID];
    int maxNum=(userID<sm->tileToPlaceNumMax.getNum()) ? 
        sm->tileToPlaceNumMax[userID] : 
        (sm->tileToPlaceNumMax.getNum() ? sm->tileToPlaceNumMax[sm->tileToPlaceNumMax.getNum()-1] : 0);
    if (tileTPNum+1<=maxNum) sm->tileToPlaceNum.set1Value(userID,tileTPNum+1);
    
    // check winning condition
    int index=sm->getPlayfieldInfoIndex(sm->currentTileCoord[userID]);
    SbVec2f info=sm->playfieldInfo[index];
    if (floor(info[1])==ENDTILE) sm->targetReached.set1Value(userID,TRUE);
}

void SceneManager::seekNextTileCB(void *data,SoSensor *sensor)
{
    SceneManager *sm=(SceneManager*)data;
    int userID=sm->seekNextTile.getValue();
    if (userID<0) return;
    else {
        // determine next tile based on current playfield information --> set nextTile field
        sm->setNextTile(userID);
        // set end, start & center hotspots for each character
        sm->setHotspots(userID);
    }
}

void SceneManager::initCB(void *data,SoSensor *sensor)
{
    SceneManager *sm=(SceneManager*)data;
    sm->initialize();
    // turn on internal init flag
    sm->initialized=true;
}

void SceneManager::timeOutCB(void *data,SoSensor *sensor)
{
    SceneManager *sm=(SceneManager*)data;

    for (int i=0;i<sm->timerList.getLength();i++){
        if (sensor==sm->timerList[i]) sm->placeTileOnField(i,false);
    }
}

void SceneManager::tilePlacedCursorCB(void *data,SoSensor *sensor)
{
    SceneManager *sm=(SceneManager*)data;

    for (int i=0;i<sm->tilePlacedTimerList.getLength();i++){
        // switch of "tile placed" cursor for tile#i
        if (sensor==sm->tilePlacedTimerList[i]) sm->tilePlacedCursorOn.set1Value(i,FALSE);
    }
}

void SceneManager::initialize()
{
    int i,j;

    // init total number of tiles
    tileTotalNum=tileToPlacePosition.getNum();

    // initialize number of placeable tiles to the number of active tiles (maximum number of placeable tiles)
    for (i=0;i<playerNum.getValue();i++){
        tileToPlaceNum.set1Value(
            i,
            i<tileToPlaceNumMax.getNum() ? 
                tileToPlaceNumMax[i] : 
                (tileToPlaceNumMax.getNum() ? tileToPlaceNumMax[tileToPlaceNumMax.getNum()-1] : 0)
        );
    }

    // clear playfield information
    for (i=0;i<maxCoordinate.getValue()[0];i++){
        for (j=0;j<maxCoordinate.getValue()[1];j++){
            // init each (x,y) field to invalid user ID and water type
            playfieldInfo.set1Value(getPlayfieldInfoIndex(i,j),SbVec2f(INVALID,WATER));
        }
    }

    // set initial playfield tiles and obstacle information
    short *initOK=new short[playerNum.getValue()];
    obstacleTilePosition.deleteValues(0);
    for (i=0;i<playerNum.getValue();initOK[i++]=0);
    for (i=0;i<playfieldInitInfo.getNum();i++){
        SbVec4f info=playfieldInitInfo[i];
        int tileType=floor(info[3]);
        playfieldInfo.set1Value(getPlayfieldInfoIndex((int)(info[0]),(int)(info[1])),info[2],info[3]);
        if (tileType==STARTTILE) initOK[(int)(info[2])]+=1;
        else if (tileType==ENDTILE) initOK[(int)(info[2])]+=2;
        else if (tileType>=DECORATION0 && tileType<USERTILE0) {
            SbVec3f pos;
            getTilePositionOnField(SbVec2f((int)(info[0]),(int)(info[1])),pos);
            obstacleTilePosition.set1Value(obstacleTilePosition.getNum(),pos);
        }
    }

    // check whether all users have start and end tiles defined
    for (i=0;i<playerNum.getValue();i++){
        if (initOK[i]!=3) {
            break;
        }
    }
    std::string missingTileText[3]={
        "a start and an end tile",
        "an end tile",
        "a start tile"
    };
    if (i<playerNum.getValue()) {
        printf("SCENEMANAGER: initialization error: user %d hasn't specified %s\n",i,missingTileText[initOK[i]].c_str());
        initError=1;
        return;
    }
    delete[]initOK;

    // remove tiles possibly remaining from the previous game
    int oldTileNum=oldTiles->getNumChildren();
    int physicalTileNum=physicalTiles->getNumChildren();
    SoSeparator *nt=SO_GET_ANY_PART(this,"newTiles",SoSeparator);
    int newTileNum=nt->getNumChildren();
    for (i=0;i<oldTileNum;i++) oldTiles->removeChild(0);
    for (i=0;i<physicalTileNum;i++) physicalTiles->removeChild(0);
    for (i=0;i<newTileNum;i++) nt->removeChild(0);

    // generate old tiles and add them to the scene graph + init current tile coordinates
    for (i=0;i<playfieldInitInfo.getNum();i++){
        SbVec4f info=playfieldInitInfo[i];
        addOldTile(info);
        if ((int)(info[3])==SceneManager::STARTTILE) currentTileCoord.set1Value((int)(info[2]),info[0],info[1]);
    }

    // generate moveable tiles that are attached to the users' cursor(s) and add them to the scene graph
    for (i=0;i<tileTotalNum.getValue();i++) addNewTile(i);

    // reset bonus objects' "picked up" component --> TODO: add bonus object support and implementation
    for (i=0;i<bonusObjectInfo.getNum();i++) {
        SbVec4f info=bonusObjectInfo[i];
        info[3]=0; // component #3 is the "pick-up" indicator
        bonusObjectInfo.set1Value(i,info);
    }

    // set hotspot names
    for (i=0;i<playerNum.getValue();i++){
        SbString directionName[3]={"start","center","end"};
        for (j=0;j<3;j++){
            SbString hsName=directionName[j];
            hsName+=SbString(i);
            hotspotName.set1Value(i*3+j,hsName);
        }
    }

    // reset targetReached parameter and character motion-related fields
    for (i=0;i<playerNum.getValue();i++) {
        targetReached.set1Value(i,FALSE);
        previousTileCoord.set1Value(i,INVALID,INVALID);
        firstTilePlaced.set1Value(i,FALSE);
        // calculate initial character position over the center of the player's start tile
        SbVec3f pos;
        getTilePositionOnField(SbVec4f(currentTileCoord[i][0],currentTileCoord[i][1],SceneManager::INVALID,SceneManager::INVALID),pos);
        int index=getPlayfieldInfoIndex(currentTileCoord[i]);
        int tileType=floor(playfieldInfo[index][1]);
        int hotspotIndex=tileType*5+4;
        // intialize all hotspots to the initial position
        for (j=HS_START;j<=HS_END;j++){
           hotspotCoord.set1Value((HS_END+1)*i+j,(pos+hotspotOffset[hotspotIndex])*globalScale.getValue());
        }
        hotspotSet.set1Value(i,i);
    }

    tileToPlaceTypeRemoteCB(this,NULL);

    // init timer list
    initTimerLists();

    // set init error field
    initError=0;

    printf("SCENEMANAGER: scene manager initialized\n");
}

void SceneManager::initTimerLists()
{
    // maintain timer list (increase or reduce size if necessary)
    // timerList.length() == tilePlacedTimerList.length(), therefore the same cycle can be used to initialize both lists!
    if (timerList.getLength()!=tileTotalNum.getValue()){
        int i;
        if (tileTotalNum.getValue()-timerList.getLength()>0){
            int max=tileTotalNum.getValue()-timerList.getLength();
            for (i=0;i<max;i++){
               SoAlarmSensor *newTimeOutSensor=new SoAlarmSensor(timeOutCB,this);
               SoAlarmSensor *newTilePlacedCursorSensor=new SoAlarmSensor(tilePlacedCursorCB,this);
               // add new items at the end of the lists
               timerList.append(newTimeOutSensor);
               tilePlacedTimerList.append(newTilePlacedCursorSensor);
            }
        }
        else {
            int max=timerList.getLength()-tileTotalNum.getValue();
            for (i=0;i<max;i++){
               // remove last elements from the lists
               timerList.remove(timerList.getLength()-1);
               tilePlacedTimerList.remove(timerList.getLength()-1);
            }
        }
    }
}

void SceneManager::placeTileOnField(int tileID,bool remoteInfo)
{
    int type;
    SbVec4f info;

    if (remoteInfo){
        info=tileSnappedInfoRemote.getValue();
        type=tileToPlaceTypeRemote[tileID];
    }
    else {
        if (tileID<tileToPlaceType.getNum() && tileID<tileSnappedInfoTemp.getNum()) {
            type=tileToPlaceType[tileID];
            info=tileSnappedInfoTemp[tileID];
        }
        else {
            tileSnappedInfo.setValue(SbVec4f(INVALID,INVALID,INVALID,INVALID));
            return;
        }
    }

    // tile number information encoded into tile info
    int userID=floor(info[2]);
    
    // check whether the current player is enabled to place tiles -->
    // This is to avoid processing timers triggered before the end of game otherwise the users would be still 
    // able to place tiles after game is over
    bool interrupted=
            userID>=0 &&
            userID<enabled.getNum() ? 
                !(enabled[userID]) : 
                (enabled.getNum() ? !(enabled[enabled.getNum()-1]) : TRUE);
    if (interrupted) return;

    // if tile info is local, calculate and set final tileSnappedInfo value here
    if (!remoteInfo){
        // encode tile ID into user ID information
        if (info[2]!=INVALID) info[2]=userID+(tileID/10.0f);

        // rotation information encoded into tile type: 
        // enum tile type + (output rotation angle relative to the playfield) % 90' * 0.25
        info[3]=(info[3]/PI_PER_2)*0.25f+type;

        // set output
        tileSnappedInfo.setValue(info);
    }

    // add occupation information to playfield
    playfieldInfo.set1Value(getPlayfieldInfoIndex((int)(info[0]),(int)(info[1])),SbVec2f(info[2],info[3]));

    // set "tile placed" cursor and trigger timer sensor which will hide it after a defined time has passed
    tilePlacedCursorOn.set1Value(tileID,TRUE);
    if (tileID<timerList.getLength()){
        SoAlarmSensor *tilePlacedCursorSensor=(SoAlarmSensor*)(tilePlacedTimerList[tileID]);
        // if no valid value set for timeout, set default time
        tilePlacedCursorSensor->setTimeFromNow(
            tileID<tilePlacedCursorTimeOut.getNum() ? 
                tilePlacedCursorTimeOut[tileID] : 
                (tilePlacedCursorTimeOut.getNum() ? 
                    tilePlacedCursorTimeOut[tilePlacedCursorTimeOut.getNum()-1] : 
                    SbTime()
                )
        );
        if (tilePlacedCursorSensor->isScheduled()) tilePlacedCursorSensor->unschedule();
        tilePlacedCursorSensor->schedule();
    }

    // check neighboring tiles whether they are unoccupied fix tiles that can be occupied by the player
    for (int j=0;j<4;j++){
        SbVec2f neighborTileCoord=SbVec2f(info[0],info[1])+playfieldDirection[j];
        int neighborTileIndex=getPlayfieldInfoIndex(neighborTileCoord);
        if (neighborTileIndex!=INVALID){
            SbVec2f neighborTileInfo=playfieldInfo[neighborTileIndex];
            int neighborTileUserID=floor(neighborTileInfo[0]);
            int neighborTileType=floor(neighborTileInfo[1]);
            // check for unoccupied fix tile neighbors
            if (neighborTileUserID==INVALID && neighborTileType>=FIXTILE0 && neighborTileType<DECORATION0){
                // set own user ID while preserving the tile type --> player occupies field
                playfieldInfo.set1Value(neighborTileIndex,(float)userID,neighborTileInfo[1]);
            }
        }
    }

    // decrease the number of tiles that may be placed (the director will increase the number again 
    // if the character has walked across a tile)
    int tileTPNum=tileToPlaceNum[tileID];
    if (tileTPNum) tileToPlaceNum.set1Value(tileID,tileTPNum-1);

    // fix current tile as old tile and generate new tile in place of it
    replaceTile(tileID);

    // send "hotspot set" event if this is the first tile of the user to place
    if (!firstTilePlaced[tileID] || !nextTileValid[tileID]) {
        if (!firstTilePlaced[tileID]) firstTilePlaced.set1Value(tileID,TRUE);
        // determine next tile based on current playfield information --> set nextTile field
        setNextTile(tileID);
        // set end, start & center hotspots for each character
        setHotspots(tileID);
    }

    // check whether the value in nextTileCoord for user#tileID represents a valid tile on the playfield
    int index=getPlayfieldInfoIndex(nextTileCoord[userID]);
    nextTileValid.set1Value(userID,floor(playfieldInfo[index][0])==userID);
}

void SceneManager::getUserIdFromTileNum(int tileID,int &userID,int &userTileID)
{
    int count=0;
    for (int i=0;i<playerNum.getValue();i++){
        int increment=i<tileNum.getNum() ? tileNum[i] : (tileNum.getNum() ?  tileNum[tileNum.getNum()-1] : 0);
        if (tileID<count+increment) break;
        count+=increment;
    }
    // validate result
    if (i<playerNum.getValue()){
        userID=i;
        userTileID=tileID-count;
    }
    else {
        userID=INVALID;
        userTileID=INVALID;
    }
}

int SceneManager::getPlayfieldInfoIndex(int x,int y) 
{ 
    if (x>=0 && x<maxCoordinate.getValue()[0] && y>=0 && y<maxCoordinate.getValue()[1]) return (int)(maxCoordinate.getValue()[0]*y+x);
    return INVALID;
}

int SceneManager::getPlayfieldInfoIndex(SbVec2f coord)
{
    return getPlayfieldInfoIndex((int)(coord[0]),(int)(coord[1]));
}

void SceneManager::getTilePositionOnField(SbVec4f tileInfo,SbVec3f &position)
{
    getTilePositionOnField(SbVec2f(tileInfo[0],tileInfo[1]),position);
}

void SceneManager::getTilePositionOnField(SbVec2f tileInfo,SbVec3f &position)
{
    position[0]=(-(maxCoordinate.getValue()[0]/2.0f-0.5f)+tileInfo[0])*tileSize.getValue();
    position[1]=0;
    position[2]=(-(maxCoordinate.getValue()[1]/2.0f-0.5f)+tileInfo[1])*tileSize.getValue();
}

void SceneManager::getTileOrientationOnField(SbVec4f tileInfo,SbRotation &orientation)
{
    orientation=SbRotation(SbVec3f(0,1,0),2*PI*(tileInfo[3]-(float)(floor(tileInfo[3]))));
}

int SceneManager::generateTileType(int userID)
{
    // TODO: the current generation algorithm is rather dummy, it's completely random (the info parameter is ignored)
    // --> add something more sophisticated based on each user's history of placed tiles and current game difficulty
    int num;
    bool avoid;
    do {
        avoid=false;
        num=(int)((rand()/(float)(RAND_MAX))*(maxTileTypeID.getValue()));
        // if the generated tile ID is among the "forbidden" IDs 
        // (e.g. it would be a start/end tile, fix tile or decoration element), then regenerate number
        for (int i=0;i<avoidTileType.getNum();i++){
            if (num==avoidTileType[i]) {
                avoid=true;
                break;
            }
        }
    } while (avoid);

    return num;
}

void SceneManager::addOldTile(SbVec4f info)
{
    int tileType=floor(info[3]);
    if (tileType<0) {
        printf("SCENEMANAGER: warning: incorrect tile ID for fix tile (%f)\n",tileType);
        return;
    }

    SoSwitch *tl=SO_GET_ANY_PART(this,"tileLibrary",SoSwitch);
    if (tileType>=tl->getNumChildren()) {
        printf("SCENEMANAGER: there is no model file associated to tile type#%d\n",tileType);
        return;
    }

    // check whether we need to set a user-based material for the tile and whether it's a physical tile
    bool ignoreMaterial=tileType>=FIXTILE0 && tileType<USERTILE0;
    bool isPhysical=tileType==STARTTILE || tileType==ENDTILE || ignoreMaterial;

    // create tile
    SoSeparator *tileRoot=new SoSeparator;

    // create material if userID is valid and tile is not physical (i.e. placed by the user)
    int userID=floor(info[2]);
    if (!ignoreMaterial && userID>=0){
        SoMaterial *tileMaterial=new SoMaterial;
        tileMaterial->diffuseColor.setValue(tileColor[userID]);
        tileRoot->addChild(tileMaterial);
    }

    // create transformation offset
    SoTransform *tileTransform=new SoTransform;
    SbVec3f pos;
    SbRotation rot;
    getTilePositionOnField(info,pos);
    getTileOrientationOnField(info,rot);
    tileTransform->translation=pos*globalScale.getValue();
    tileTransform->rotation=rot;
    tileTransform->scaleFactor.setValue(globalScale.getValue(),globalScale.getValue(),globalScale.getValue());
    tileRoot->addChild(tileTransform);

    // create tile (use reference from tile library)
    if (tileType<tl->getNumChildren()) {
        SoNode *tileNode=tl->getChild(tileType);
        tileRoot->addChild(tileNode);
        // add tile to old tiles
        if (isPhysical) physicalTiles->addChild(tileRoot);
        else oldTiles->addChild(tileRoot);
    }
    else printf("SCENEMANAGER: warning: insufficient number of tiles defined in scene manager (%d > %d)\n",tileType,tl->getNumChildren());
}

void SceneManager::addNewTile(int tileID)
{
    // create tile
    SoSeparator *tileRoot=new SoSeparator;

    // create material
    int userID,userTileID;
    getUserIdFromTileNum(tileID,userID,userTileID);
    SoMaterial *tileMaterial=new SoMaterial;
    tileMaterial->diffuseColor.setValue(tileColor[userID]);
    tileRoot->addChild(tileMaterial);

    // create transformation offset
    SoTransform *tileTransform=new SoTransform;
    SoSelectOne *selectorPos=new SoSelectOne(SoMFVec3f::getClassTypeId());
    SoSelectOne *selectorOrient=new SoSelectOne(SoMFRotation::getClassTypeId());
    selectorPos->input->connectFrom(&tileSnappedPosition);
    selectorOrient->input->connectFrom(&tileSnappedOrientation);
    selectorPos->index=tileID;
    selectorOrient->index=tileID;
    tileTransform->translation.connectFrom(selectorPos->output);
    tileTransform->rotation.connectFrom(selectorOrient->output);
    tileTransform->scaleFactor.setValue(globalScale.getValue(),globalScale.getValue(),globalScale.getValue());
    tileRoot->addChild(tileTransform);

    // create tile (use reference from tile library)
    SoSwitch *tl=SO_GET_ANY_PART(this,"tileLibrary",SoSwitch);
    SoSeparator *nt=SO_GET_ANY_PART(this,"newTiles",SoSeparator);
    bool remoteInfo=(tileID<useRemoteInfo.getNum()) ? useRemoteInfo[tileID] : (useRemoteInfo.getNum() ? useRemoteInfo[useRemoteInfo.getNum()-1] : false);
    int tileType=remoteInfo ? INVALID : generateTileType(userID);    
    tileToPlaceType.set1Value(tileID,tileType);

    if (tileType<tl->getNumChildren()) { // if (tileType==INVALID), it is also < tl->getNumChildren(), no test is necessary
        SoNode *tileNode=remoteInfo ? (new SoSeparator()) : tl->getChild(tileType);
        tileRoot->addChild(tileNode);
        // add tile to new tiles
        nt->addChild(tileRoot);
    }
    else printf("SCENEMANAGER: warning: insufficient number of tiles defined in scene manager (%d > %d)\n",tileType,tl->getNumChildren());
}

void SceneManager::replaceTile(int tileID)
{
    // create tile (use reference from tile library)
    SoSwitch *tl=SO_GET_ANY_PART(this,"tileLibrary",SoSwitch);
    SoSeparator *nt=SO_GET_ANY_PART(this,"newTiles",SoSeparator);

    int userID,userTileID;
    getUserIdFromTileNum(tileID,userID,userTileID);
    
    bool remoteInfo=(tileID<useRemoteInfo.getNum()) ? useRemoteInfo[tileID] : (useRemoteInfo.getNum() ? useRemoteInfo[useRemoteInfo.getNum()-1] : false);
    int tileType=remoteInfo ? INVALID : generateTileType(userID);    
    tileToPlaceType.set1Value(tileID,tileType);
    
    if (tileID<nt->getNumChildren()){
        SoSeparator *tileRoot=(SoSeparator*)(nt->getChild(tileID));
        if (tileType<tl->getNumChildren()) { // if (tileType==INVALID), it is also < tl->getNumChildren(), no test is necessary
            if (tileRoot->getNumChildren()>1) {
                if (!remoteInfo){
                    SoNode *tileNode=tl->getChild(tileType);
                    tileRoot->replaceChild(2,tileNode);
                }
                // add tile to old tiles
                addOldTile(remoteInfo ? tileSnappedInfoRemote.getValue() : tileSnappedInfo.getValue());
            }
        }
        else printf("SCENEMANAGER: warning: insufficient number of tiles defined in scene manager (%d > %d)\n",tileType,tl->getNumChildren());
    }
    else printf("SCENEMANAGER: warning: no tile has been generated for new tile #%d\n",tileID);
}

void SceneManager::setNextTile(int userID)
{
    // ************************* TODO: simplify this function!
    
    int i,index;
    SbVec2f coord;
    // check whether end tile is a neighbor --> enjoys priority!
    for (i=0;i<4;i++){
        coord=currentTileCoord[userID]+playfieldDirection[i];
        index=getPlayfieldInfoIndex(coord);
        if (index!=INVALID){
            SbVec2f info=playfieldInfo[index];
            if (floor(info[0])==userID && floor(info[1])==ENDTILE) {
                nextTileCoord.set1Value(userID,coord);
                printf("SCENEMANAGER: choose next tile for character%d: found end tile neighbor --> (%d,%d)\n",
                    userID,(int)(nextTileCoord[userID][0]),(int)(nextTileCoord[userID][1]));
                return;
            }
        }
    }

    // we have just started the game --> we don't have information about the tile where the character comes from
    if (previousTileCoord[userID]==SbVec2f(INVALID,INVALID)){
        // try to find a neighbor belonging to the same user
        for (i=0;i<4;i++){
            coord=currentTileCoord[userID]+playfieldDirection[i];
            index=getPlayfieldInfoIndex(coord);
            if (index!=INVALID){
                SbVec2f info=playfieldInfo[index];
                if (floor(info[0])==userID) {
                    nextTileCoord.set1Value(userID,coord);
                    printf("SCENEMANAGER: choose next tile for character%d: no previous, found neighbor --> (%d,%d)\n",
                        userID,(int)(nextTileCoord[userID][0]),(int)(nextTileCoord[userID][1]));
                    return;
                }
            }
        }
        // if we reached this point, we couldn't find any tiles occupied by user --> 
        // choose first tile having valid coordinates (i.e. they are within the playfield)
        for (i=0;i<4;i++){
            coord=currentTileCoord[userID]+playfieldDirection[i];
            index=getPlayfieldInfoIndex(coord);
            if (index!=INVALID){
                nextTileCoord.set1Value(userID,coord);
                printf("SCENEMANAGER: choose next tile for character%d: no previous, no neighbor found --> (%d,%d)\n",
                    userID,(int)(nextTileCoord[userID][0]),(int)(nextTileCoord[userID][1]));
                return;
            }
        }
    }
    else {
        SbVec2f offsetVector=currentTileCoord[userID]-previousTileCoord[userID];
        SbVec2f straightDirectionCoord=currentTileCoord[userID]+offsetVector;
        int straightIndex=getPlayfieldInfoIndex(straightDirectionCoord);
        // give priority to straight direction --> if there is a valid tile that belongs to the user, we're done
        if (straightIndex!=INVALID && floor(playfieldInfo[straightIndex][0])==userID){
            nextTileCoord.set1Value(userID,straightDirectionCoord);
            printf("SCENEMANAGER: choose next tile for character%d: previous found, found neighbor in straight direction --> (%d,%d)\n",
                userID,(int)(nextTileCoord[userID][0]),(int)(nextTileCoord[userID][1]));
            return;
        }
        else {
            // try to find a neighbor belonging to the same user
            for (i=0;i<4;i++){
                coord=currentTileCoord[userID]+playfieldDirection[i];
                // we already checked whether current user has a valid tile in the straight direction, don't bother here
                if (straightDirectionCoord==coord) continue;
                index=getPlayfieldInfoIndex(coord);
                if (index!=INVALID){
                    SbVec2f info=playfieldInfo[index];
                    if (coord!=previousTileCoord[userID] && floor(info[0])==userID){
                        nextTileCoord.set1Value(userID,coord);
                        printf("SCENEMANAGER: choose next tile for character%d: previous found, found neighbor --> (%d,%d)\n",
                            userID,(int)(nextTileCoord[userID][0]),(int)(nextTileCoord[userID][1])
                        );
                        return;
                    }
                }
            }
            // if we reached this point, we couldn't find any tiles occupied by user --> choose straight direction if tile is within 
            // playfield, otherwise choose first tile having valid coordinates (i.e. they are within the playfield)
            if (straightIndex!=INVALID){
                nextTileCoord.set1Value(userID,straightDirectionCoord);
                printf("SCENEMANAGER: choose next tile for character%d: previous found, no neighbor found, go straight --> (%d,%d)\n",
                    userID,(int)(nextTileCoord[userID][0]),(int)(nextTileCoord[userID][1]));
                return;
            }
            for (i=0;i<4;i++){
                SbVec2f coord=currentTileCoord[userID]+playfieldDirection[i];
                // we already checked whether there is a valid tile in the straight direction, don't bother here
                if (straightDirectionCoord==coord) continue;
                int index=getPlayfieldInfoIndex(coord);
                if (index!=INVALID){
                    SbVec2f info=playfieldInfo[index];
                    if (coord!=previousTileCoord[userID]){
                        nextTileCoord.set1Value(userID,coord);
                        printf("SCENEMANAGER: choose next tile for character%d: previous found, no neighbor found --> (%d,%d)\n",
                            userID,(int)(nextTileCoord[userID][0]),(int)(nextTileCoord[userID][1])
                        );
                        return;
                    }
                }
            }
        }
    }
}

void SceneManager::setHotspots(int userID)
{
    int i;

    // determine offset vector of last character movement
    SbVec2f offsetCoord=nextTileCoord[userID]-currentTileCoord[userID];
    int directionIndex=INVALID;
    for (i=0;i<4;i++){
        if (playfieldDirection[i]==offsetCoord){
            directionIndex=i;
            break;
        }
    }
    if (directionIndex<0){
        printf("SCENEMANAGER: the generated next tile (%d %d) for user %d is incorrect\n",
            (int)(nextTileCoord[userID][0]),(int)(nextTileCoord[userID][1]),userID
        );
        return;
    }

    // generate tile end, start and center hotspot information for the user's character
    SbVec3f pos,posOffset;
    int hotspotIndex,rotationInfo;
    bool interrupt=false;
    int nextTileIndex=getPlayfieldInfoIndex(nextTileCoord[userID]);
    int nextTileUserID=nextTileIndex<0 ? INVALID : floor(playfieldInfo[nextTileIndex][0]);
    int nextTileType=nextTileIndex<0 ? INVALID : floor(playfieldInfo[nextTileIndex][1]);
    if (nextTileType<0 || nextTileUserID!=userID) {
        nextTileValid.set1Value(userID,FALSE);
        interrupt=true;
        printf("SCENEMANAGER: the currently chosen next tile for user %d is invalid\n",userID);
    }
    int currentTileIndex=getPlayfieldInfoIndex(currentTileCoord[userID]);
    int currentTileUserID=currentTileIndex<0 ? INVALID : floor(playfieldInfo[currentTileIndex][0]);
    int currentTileType=currentTileIndex<0 ? INVALID : floor(playfieldInfo[currentTileIndex][1]);
    if (currentTileUserID<0 || currentTileType<0) {
        nextTileValid.set1Value(userID,FALSE);
        interrupt=true;
        printf("SCENEMANAGER: invalid current tile [%d %d] for user %d, something is quite wrong here!\n",
            floor(currentTileCoord[userID][0]),floor(currentTileCoord[userID][1]),userID
        );
    }
    
    for (i=HS_START;i<=HS_END;i++){
        if (i!=HS_END && interrupt) continue;
        switch (i) {
            case HS_START: // set start hotspot
                getTilePositionOnField(nextTileCoord[userID],pos);
                rotationInfo=(int)((playfieldInfo[nextTileIndex][1]-(float)nextTileType)*4.0f);
                hotspotIndex=nextTileType*5+(directionIndex+rotationInfo+2)%4; // +2 because of mirroring about the center
                break;
            case HS_CENTER: // set center hotspot
                getTilePositionOnField(nextTileCoord[userID],pos);
                hotspotIndex=nextTileType*5+4; // 4 = code of the center
                break;
            case HS_END: // set end hotspot
                getTilePositionOnField(currentTileCoord[userID],pos);
                rotationInfo=(int)((playfieldInfo[currentTileIndex][1]-(float)currentTileType)*4.0f);
                hotspotIndex=currentTileType*5+(directionIndex+rotationInfo)%4;
                break;
            default:
                break;
        }

        // rotate hotspot
        SbRotation(SbVec3f(0,1,0),rotationInfo*PI_PER_2).multVec(hotspotOffset[hotspotIndex],posOffset);

        // set hotspot#i's value
        hotspotCoord.set1Value((HS_END+1)*userID+i,(pos+posOffset)*globalScale.getValue());
    }

    // generate event for characters' state engines in director
    hotspotSet.setValue(userID);

}

void SceneManager::positionChanged(SoNode *node)
{
    tilePoseCB(this,NULL);
}

void SceneManager::orientationChanged(SoNode *node)
{
    tilePoseCB(this,NULL);
}
