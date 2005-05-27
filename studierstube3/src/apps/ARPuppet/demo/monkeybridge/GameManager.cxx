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
/* GameManager.cxx contains the implementation of GameManager
 *
 * @author Istvan Barakonyi
 *
 * $Id: GameManager.cxx 3737 2004-09-20 18:09:01Z bara $
 * @file
 */
/* ======================================================================== */

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#include "Inventor/nodes/SoSeparator.h"
#include "Inventor/nodes/SoTexture2.h"
#include "Inventor/nodes/SoMaterial.h"
#include "Inventor/nodes/SoFont.h"
#include "Inventor/nodes/SoText3.h"

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

// own classes
#include "GameManager.h"

SO_KIT_SOURCE(GameManager);

void GameManager::initClass()
{
   SO_KIT_INIT_CLASS(GameManager, SoBaseKit, "BaseKit");
}

GameManager::GameManager()
{
    SO_KIT_CONSTRUCTOR(GameManager);

    SO_KIT_ADD_CATALOG_ENTRY(root, SoSeparator, FALSE, this, \0, FALSE);
    SO_KIT_ADD_CATALOG_ENTRY(texture, SoTexture2, TRUE, root, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(material, SoMaterial, TRUE, root, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(font, SoFont, FALSE, root, \0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(text, SoText3, FALSE, root, \0, TRUE);

    // general game parameters
    SO_KIT_ADD_FIELD(init, ());
    SO_KIT_ADD_FIELD(started, (FALSE));
    SO_KIT_ADD_FIELD(countdownFinished, (FALSE));
    SO_KIT_ADD_FIELD(countdownFrom, (5));
    SO_KIT_ADD_FIELD(speed, (1.0f));
    SO_KIT_ADD_FIELD(countdown, (0));
    SO_KIT_ADD_FIELD(displayTime, (5.0f));
    
    // user information
    SO_KIT_ADD_FIELD(alive, (TRUE));
    SO_KIT_ADD_FIELD(win, (0));
    SO_KIT_ADD_FIELD(enabled, (FALSE));

    SO_KIT_INIT_INSTANCE();

    // time counter object
    counter=new SoTimeCounter;
    counter->step=1;
    counter->min=0;
    counter->on=FALSE;

    displayTimer=new SoAlarmSensor(displayTimerCB,this);

    // set text justification
    SoText3 *text=SO_GET_ANY_PART(this,"text",SoText3);
    text->justification=SoText3::CENTER;

    // field sensor
    initSensor=new SoFieldSensor(initCB,this);
    countdownSensor=new SoFieldSensor(countdownCB,this);
    aliveSensor=new SoFieldSensor(aliveCB,this);
    winSensor=new SoFieldSensor(winCB,this);

    setUpConnections(TRUE,TRUE);
}

GameManager::~GameManager()
{
    if (displayTimer->isScheduled()) displayTimer->unschedule();

    delete initSensor;
    delete countdownSensor;
    delete aliveSensor;
    delete winSensor;
    delete displayTimer;
}

SbBool GameManager::setUpConnections(SbBool onOff,SbBool doItAlways)
{
    if (!doItAlways && connectionsSetUp == onOff)
        return onOff;
    
    if (onOff) {
        countdown.connectFrom(&counter->output);

        initSensor->attach(&init);
        countdownSensor->attach(&countdown);
        aliveSensor->attach(&alive);
        winSensor->attach(&win);
    }
    else {
        initSensor->detach();
        countdownSensor->detach();
        aliveSensor->detach();
        winSensor->detach();

        counter->min.disconnect();
        counter->frequency.disconnect();
    }

    return !(connectionsSetUp = onOff);
}

void GameManager::initCB(void *data,SoSensor *sensor)
{
    GameManager *gm=(GameManager*)data;

    if (!gm->started.getValue()){
        gm->counter->max=gm->countdownFrom.getValue()+1;
        gm->counter->frequency=1.0f/(gm->countdownFrom.getValue()+1.0f)*gm->speed.getValue();
        gm->counter->on.setValue(TRUE);
        gm->counter->syncIn.setValue();
        // Set enabled to FALSE if a) the game has just started and we are waiting for the countdown to finish.
        // It's enough to set only one value since we don't know how many users there are.
        // In the scene manager this value will be correctly interpreted and set for all users.
        gm->enabled.setValue(FALSE);
        gm->started.setValue(TRUE);
    }
    else {
        gm->counter->on.setValue(FALSE);
        gm->finish(""); // the enabled parameter will be disabled in the finish function, we don't need to set it here
        gm->started.setValue(FALSE);
    }
}

void GameManager::countdownCB(void *data,SoSensor *sensor)
{    
    GameManager *gm=(GameManager*)data;

    if (!gm->counter->on.getValue()) return;

    SoText3 *text=SO_GET_ANY_PART(gm,"text",SoText3);
    if (gm->countdown.getValue()==gm->countdownFrom.getValue()+1) {
        text->string="";
        gm->counter->on.setValue(FALSE);
    }
    else if (gm->countdown.getValue()==gm->countdownFrom.getValue()){
        text->string="START";
        // We can allow the placement of tiles since the countdown has finished, the game has started.
        // It's enough to set only one value since we don't know how many users there are. 
        // In the scene manager this value will be correctly interpreted and set for all users.
        gm->enabled.setValue(TRUE);
        // Indicate that the game has started
        gm->countdownFinished.setValue(TRUE);
    }
    else text->string=SbString(gm->countdownFrom.getValue()-gm->countdown.getValue());
}

void GameManager::aliveCB(void *data,SoSensor *sensor)
{
    GameManager *gm=(GameManager*)data;

    for (int i=0;i<gm->alive.getNum();i++){
        if (!gm->alive[i]) {
            char str[255];
            sprintf(str,"User %d lost",i+1);
            gm->finish(str);
            break;
        }
    }
}

void GameManager::winCB(void *data,SoSensor *sensor)
{
    GameManager *gm=(GameManager*)data;

    for (int i=0;i<gm->win.getNum();i++){
        if (gm->win[i]) {
            char str[255];
            sprintf(str,"User %d won",i+1);
            gm->finish(str);
            break;
        }
    }
}

void GameManager::displayTimerCB(void *data,SoSensor *sensor)
{
    GameManager *gm=(GameManager*)data;
    SoText3 *text=SO_GET_ANY_PART(gm,"text",SoText3);

    text->string="";
}

void GameManager::finish(const char *str)
{
    SoText3 *text=SO_GET_ANY_PART(this,"text",SoText3);

    // don't set text if we started the game for the first time
    text->string.set1Value(0,"Game Over");
    text->string.set1Value(1,str);

    // indicate that the game is over/stopped
    countdownFinished.setValue(FALSE);

    // indicate end of game
    started.setValue(FALSE);

    // The game has finished, disable the placement of tiles. It's enough to set only one value since we don't 
    // know how many users there are. In the scene manager this value will be correctly interpreted and set for all users.
    enabled.setValue(FALSE);

    // start timer that turns of "Game Over" message after a specific time
    displayTimer->setTimeFromNow(displayTime.getValue());
    if (!displayTimer->isScheduled()) displayTimer->schedule();
}
