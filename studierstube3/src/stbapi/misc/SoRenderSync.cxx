// ===========================================================================
//  (C) 2003 Vienna University of Technology
// ===========================================================================
//  NAME:       SoRenderSync.h
//  TYPE:       c++ header
//  PROJECT:    Studierstube
//  CONTENT:    class for synchronisation path traveral in Open Inventor
//  VERSION:    1.0
// ===========================================================================
//  AUTHORS:    ge             Gottfried Eibner
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#define SYNC_TIMEOUT_MSEC 1000000

#include "ace/OS.h"
#include "ace/Thread.h"

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/SbLinear.h>
#include <Inventor/fields/SoFieldContainer.h>

#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif

#include "SoRenderSync.h"

SO_NODE_ABSTRACT_SOURCE(SoRenderSync);

void
SoRenderSync::initClass(void)
{
    SO_NODE_INIT_ABSTRACT_CLASS(SoRenderSync, SoNode, "Node");
}

SoRenderSync::SoRenderSync(void) :
   frameNumber(1), hasChanged(true), threadID(NULL)
{
    SO_NODE_CONSTRUCTOR(SoRenderSync);

//-- default values of fields
    SO_NODE_ADD_FIELD(clientNum       ,(0));

    SO_NODE_ADD_FIELD(synchronized    ,(TRUE));

//-- set up sensors
    clientNumSensor = new SoFieldSensor(serverAndClientChangedCB, (void *)this);

    clientNumSensor->attach(&clientNum);
}

SoRenderSync::~SoRenderSync(void)
{
    if (threadID)
    {
        ACE_Thread::cancel(*((ACE_thread_t *)threadID));
    }
}

void
SoRenderSync::setupServerAndClients(void)
{
    if (threadID)
    {
        ACE_Thread::cancel(*((ACE_thread_t *)threadID));
        delete (ACE_thread_t *)threadID;
        threadID = NULL;
    }

    if (-1 == clientNum.getValue())
    {
        threadID = (void *)new ACE_thread_t;
        ACE_Thread::spawn((ACE_THR_FUNC)renderSyncServerThread,this,THR_NEW_LWP,(ACE_thread_t *)threadID);
    }
}

void
SoRenderSync::serverAndClientChangedCB(void *data, SoSensor *)
{
    ((SoRenderSync *)data)->hasChanged = true;
}

void
SoRenderSync::doAction(SoAction *action)
{
    (void *)action->getState();
}

void
SoRenderSync::callback(SoCallbackAction *action)
{
    SoRenderSync::doAction(action);
}

/** Thread that acts as synchronization server */
void *SoRenderSync::renderSyncServerThread(void *data)
{
    ((SoRenderSync *)data)->serverCode();

    return 0;
}
