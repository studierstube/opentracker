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
/* Director.cxx contains the implementation of Director
 *
 * @author Istvan Barakonyi
 *
 * $Id: Director.cxx 3792 2004-09-29 22:39:35Z bara $ 
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include "Inventor/nodes/SoSeparator.h"
#include "Inventor/engines/SoSelectOne.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// Stb classes
#include <stbapi/misc/SoFileSubgraph.h>

// own classes
#include "SceneManager.h"
#include "Choreographer.h"
#include "Director.h"

SO_KIT_SOURCE(Director);

void Director::initClass()
{
    SO_KIT_INIT_CLASS(Director, SoBaseKit, "BaseKit");
}

Director::Director()
{
    SO_KIT_CONSTRUCTOR(Director);

    SO_KIT_ADD_FIELD(init, ());
    SO_KIT_ADD_FIELD(characterNum, (0));
    SO_KIT_ADD_FIELD(command, (""));
    SO_KIT_ADD_FIELD(finishedCommandData, (""));
    SO_KIT_ADD_FIELD(state, (INITIALIZATION));
    SO_KIT_ADD_FIELD(playerCompletedTile, (SceneManager::INVALID));
    SO_KIT_ADD_FIELD(seekNextTile, (SceneManager::INVALID));
    SO_KIT_ADD_FIELD(hotspotSet, (SceneManager::INVALID));
    SO_KIT_ADD_FIELD(targetReached, (FALSE));
    SO_KIT_ADD_FIELD(nextTileValid, (FALSE));
    SO_KIT_ADD_FIELD(alive, (TRUE));
    SO_KIT_ADD_FIELD(fallenDown, (SceneManager::INVALID));
    SO_KIT_ADD_FIELD(win, (FALSE));

    SO_KIT_INIT_INSTANCE();

    // we need to initialize the component
    initialized=false;

    initSensor=new SoFieldSensor(initCB,this);
    hotspotSetSensor=new SoFieldSensor(hotspotSetCB,this);
    finishedCommandDataSensor=new SoFieldSensor(finishedCommandDataCB,this);
    fallenDownSensor=new SoFieldSensor(fallenDownCB,this);

    setUpConnections(TRUE,TRUE);
}

Director::~Director()
{
    delete initSensor;
    delete hotspotSetSensor;
    delete finishedCommandDataSensor;
    delete fallenDownSensor;
}

SbBool Director::setUpConnections(SbBool onOff, SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    
    if (onOff) {
        initSensor->attach(&init);
        hotspotSetSensor->attach(&hotspotSet);
        finishedCommandDataSensor->attach(&finishedCommandData);
        fallenDownSensor->attach(&fallenDown);
    }
    else {
        initSensor->detach();
        hotspotSetSensor->detach();
        finishedCommandDataSensor->detach();
        fallenDownSensor->detach();
    }

    return !(connectionsSetUp = onOff);
}

void Director::initCB(void *data,SoSensor *sensor)
{
    Director *director=(Director*)data;
    for (int i=0;i<director->characterNum.getValue();i++) {
       director->state.set1Value(i,Director::INITIALIZATION);
       director->alive.set1Value(i,TRUE);
       director->win.set1Value(i,FALSE);
    }
    director->initialized=true;
    printf("DIRECTOR: director initialized\n");
}

void Director::hotspotSetCB(void *data,SoSensor *sensor)
{
    Director *director=(Director*)data;
    for (int i=0;i<director->hotspotSet.getNum();i++){
        int userID=director->hotspotSet[i];
        if (userID>=0) director->stateEngine(userID,HOTSPOTSETEVENT);
    }
}

void Director::finishedCommandDataCB(void *data,SoSensor *sensor)
{
    Director *director=(Director*)data;

    printf("DIRECTOR: finishedCommandData: ");
    if (director!=NULL && director->finishedCommandData.getNum()){
         for (int j=0;j<director->finishedCommandData.getNum();j++){
            if (j) printf("\t");
            printf("---> '%s'\n",director->finishedCommandData[j].getString());
         }
    }
    else printf("---> ' '\n");

    for (int i=0;i<director->finishedCommandData.getNum();i++){        
        // don't deal with empty command feedback strings
        if (!director->finishedCommandData[i].getLength()) continue;

        // extract userID from command feedback
        int userID;
        if (!Choreographer::getUserIdFromString(director->finishedCommandData[i].getString(),userID)){
            printf("DIRECTOR: no or wrong userID was extracted from finished command '%s'\n",
                director->finishedCommandData[i].getString());
            return;
        }
        // run next step in the character's behavior state engine
        else if (director->initialized) director->stateEngine(userID,FINISHEDCOMMANDEVENT);
    }
}

void Director::fallenDownCB(void *data,SoSensor *sensor)
{
    Director *director=(Director*)data;
    
    if (director->fallenDown.getValue()==SceneManager::INVALID) return;
    for (int i=0;i<director->characterNum.getValue();i++){
        char strCmd[255];
        director->state.set1Value(i,GAMEFINISHED);
        if (director->fallenDown.getValue()==i){
            director->alive.set1Value(i,FALSE);
            printf("DIRECTOR: choreographer indicates that character#%d has fallen down and is dead\n",i);
            sprintf(strCmd,"%s%d: fall_water\0",Choreographer::characterIDString.getString(),i);
        }
        else sprintf(strCmd,"%s%d: win\0",Choreographer::characterIDString.getString(),i);
        director->command.set1Value(i,strCmd);
    }
}

void Director::stateEngine(int userID,int eventID)
{
    printf("DIRECTOR: user #%d --> state = %d, event %d\n",userID,state[userID],eventID);
    if (userID<0) return;

    char strCmd[255];
    char hotspotName[255];
    switch (state[userID]) {
        case INITIALIZATION:
            // if hotspots have been initialized, move characters to their initial position and proceed to the next state
            if (eventID==HOTSPOTSETEVENT) {
                // move characters to their start tile
                sprintf(strCmd,"%s%d: setPosition puppeteer(scenemanager).h(center%d).p 100\0",
                    Choreographer::characterIDString.getString(),userID,userID);
                // need to use set1Value to avoid overwriting single command values because all initial stateEngine calls happen almost at the same time
                command.set1Value(userID,strCmd); // ************************* setValue???? ************************
            }
            else {
                // set next state
                state.set1Value(userID,GAMESTARTED);
            }
            break;
        case GAMESTARTED:
            // if the first tile of the user has been placed, the game starts and the character begins to move around                
            if (eventID==HOTSPOTSETEVENT) {                
                // set next state
                state.set1Value(userID,HS_END);
                // set move command
                sprintf(hotspotName,"end%d",userID);
                sprintf(strCmd,"%s%d: goTo puppeteer(scenemanager).h(end%d).p\0",
                    Choreographer::characterIDString.getString(),userID,userID);
                command.setValue(strCmd);                
            }
            break;
        case HS_START:
            if (eventID==FINISHEDCOMMANDEVENT) {
                // set next state
                state.set1Value(userID,CHECK_HS);
                // set move command
                sprintf(hotspotName,"",userID);
                sprintf(strCmd,"%s%d: goTo puppeteer(scenemanager).h(center%d).p\0",
                    Choreographer::characterIDString.getString(),userID,userID);
                command.setValue(strCmd);
                // if the character has left the starting point, we can update the current and next position information
                playerCompletedTile.setValue(userID);
            }
            break;
        case CHECK_HS:
            if (eventID==FINISHEDCOMMANDEVENT) {
                seekNextTile=userID;
                // if character reached the end target, skip the decision part
                if (targetReached[userID]) {
                    state.set1Value(userID,HS_CENTER);
                    stateEngine(userID,FINISHEDCOMMANDEVENT);
                }
                else state.set1Value(userID,DECIDE);
            }
            break;
        case DECIDE:
            if (eventID==HOTSPOTSETEVENT) {
                // set next state
                state.set1Value(userID,HS_CENTER);
                // if currently there is no tile to continue on, play 'shrug' animation
                if (!nextTileValid[userID]){
                    sprintf(strCmd,"%s%d: hesitate\0",Choreographer::characterIDString.getString(),userID);
                    command.setValue(strCmd);
                }
                else stateEngine(userID,FINISHEDCOMMANDEVENT);
                
            }
            break;
        case HS_CENTER:
            if (eventID==FINISHEDCOMMANDEVENT) {
                if (targetReached[userID]){
                    // set new state
                    state.set1Value(userID,GAMEFINISHED);
                    printf("DIRECTOR: user %d has reached the target and won the game\n",userID);
                    // play 'win' animation for winner character, and 'lose' animation for all the others
                    for (int i=0;i<characterNum.getValue();i++){
                        if (i==userID) sprintf(strCmd,"%s%d: win\0",Choreographer::characterIDString.getString(),userID);
                        else {
                            sprintf(strCmd,"%s%d: lose\0",Choreographer::characterIDString.getString(),i);
                            state.set1Value(i,GAMEFINISHED);
                        }
                        command.set1Value(i,strCmd);
                    }
                    win.set1Value(userID,TRUE);
                }
                else {
                    // set next state
                    state.set1Value(userID,HS_END);
                    // set move command
                    sprintf(hotspotName,"end%d",userID);
                    sprintf(strCmd,"%s%d: goTo puppeteer(scenemanager).h(end%d).p\0",
                        Choreographer::characterIDString.getString(),userID,userID);
                    command.setValue(strCmd);
                }
            }
            break;
        case HS_END:
            if (eventID==FINISHEDCOMMANDEVENT) {
                if (!nextTileValid[userID]){
                    // the character has died, set new state
                    state.set1Value(userID,GAMEFINISHED);
                    printf("DIRECTOR: user %d is dead\n",userID);
                    // play 'fall into water' animation for loser character, and 'win' animation for all the others
                    for (int i=0;i<characterNum.getValue();i++){
                       if (i==userID) sprintf(strCmd,"%s%d: fall_water\0",Choreographer::characterIDString.getString(),userID);
                       else {
                           sprintf(strCmd,"%s%d: win\0",Choreographer::characterIDString.getString(),i);
                           state.set1Value(i,GAMEFINISHED);
                       }
                       command.set1Value(i,strCmd);
                    }
                    alive.set1Value(userID,FALSE);
                }
                else {
                    // set new state
                    state.set1Value(userID,HS_START);
                    // set move command
                    sprintf(hotspotName,"start%d",userID);
                    sprintf(strCmd,"%s%d: goTo puppeteer(scenemanager).h(start%d).p\0",
                        Choreographer::characterIDString.getString(),userID,userID);
                    command.setValue(strCmd);
                }
            }
            break;
        case GAMEFINISHED:
            if (eventID==FINISHEDCOMMANDEVENT) {
                printf("DIRECTOR: the game has finished for character %d\n",userID);
            }
            break;
        default:
            printf("DIRECTOR: invalid state %d, cannot proceed\n",state[userID]);
            return;
    }
}