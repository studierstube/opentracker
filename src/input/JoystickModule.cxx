 /* ========================================================================
  * Copyright (c) 2006,
  * Institute for Computer Graphics and Vision
  * Graz University of Technology
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are
  * met:
  *
  * Redistributions of source code must retain the above copyright notice,
  * this list of conditions and the following disclaimer.
  *
  * Redistributions in binary form must reproduce the above copyright
  * notice, this list of conditions and the following disclaimer in the
  * documentation and/or other materials provided with the distribution.
  *
  * Neither the name of the Graz University of Technology nor the names of
  * its contributors may be used to endorse or promote products derived from
  * this software without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
  * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
  * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
  * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
  * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
  * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
  * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for JoystickModule module.
  *
  * @author Rainer Splechtna
  *
  * $Id$
  * @file                                                                   */
 /* ======================================================================= */

// this will remove the warning 4786
#include "../tool/disable4786.h"

#include "JoystickModule.h"
#include "JoystickSource.h"

#ifdef USE_JOYSTICK

#include <stdio.h>

#ifndef _WIN32_WCE
#pragma comment(lib, "winmm")
#endif //_WIN32_WCE

//using namespace std;


namespace ot {

// Destructor method

JoystickModule::~JoystickModule()
{
    nodes.clear();
}


// This method is called to construct a new Node.

Node * JoystickModule::createNode( const string& name, StringTable& attributes)
{
    if( name.compare("JoystickSource") == 0 )
    {
        int joy_id;

        int ret = sscanf(attributes.get("id").c_str(), " %d", &joy_id);
        if( ret == 0 )
            joy_id = 1;

        JoystickSource * source = new JoystickSource(joy_id);
        nodes.push_back( source );

        ACE_LOG_INFO("ot:Build JoystickSource node for ID %d\n", joy_id );
        initialized = 1;
        return source;
    }
    return NULL;
}

// pushes events into the tracker tree.

void JoystickModule::pushEvent()
{
    JoystickSource *source;

    if( isInitialized() == 1 )
    {   
        for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
        {
            source = (JoystickSource *) *it;
            lock();
            if (source->changed == 1)
            {
                source->event = source->tmpEvent;
                source->changed = 0;
                unlock();
                source->push();
            }
            else
                unlock();
        }
    }
}


// Tests for available joysticks and starts the polling-thread.

void JoystickModule::start()
{
    if( isInitialized() && !nodes.empty() )
    {
        JOYINFOEX joyInfoEx;
        
        for (int i=0; i<2; i++)
        {
            // test if joystick with id i is connected
            memset(&joyInfoEx,0,sizeof(joyInfoEx));
            joyInfoEx.dwSize = sizeof(joyInfoEx);
            joyInfoEx.dwFlags = JOY_RETURNALL;
            joyPresent[i] = (joyGetPosEx(i, &joyInfoEx) == JOYERR_NOERROR);
            
            if (joyPresent[i])
            {
                getCaps(i);
                ACE_LOG_INFO("ot:JoystickModule: joystick with ID %d(%s) connected.\n", i+1, joyCaps[i].szPname);
            }
            else
                ACE_LOG_INFO("ot:ot:JoystickModule: joystick with ID %d not connected.\n", i+1 );
        }
        ThreadModule::start();
    }
}


// Retrieves capabilities of a joystick, specified by id.

void JoystickModule::getCaps(int id)
{
    if (joyPresent[id])
    {
        // test this
        joyGetDevCaps(id, &joyCaps[id], sizeof(joyCaps[id]));

        xRange[id] = joyCaps[id].wXmax-joyCaps[id].wXmin;
        yRange[id] = joyCaps[id].wYmax-joyCaps[id].wYmin;
        if (joyCaps[id].wCaps & JOYCAPS_HASZ)
            zRange[id] = joyCaps[id].wZmax-joyCaps[id].wZmin;
        else 
            zRange[id] = 0;
    }

}

// This is the method executed in its own thread. It polls the joysticks.

void JoystickModule::run()
{
    while(1)
    {
        if( stop == 1 )
            break;

        pollJoysticks();
        OSUtils::sleep(10);
    }
}

// Sets the stop flag.

void JoystickModule::close()
{
    // stop thread
    lock();
    stop = 1;
    unlock();
}


// Polls the joysticks. 

void JoystickModule::pollJoysticks()
{
    // number of joysticks supported by winmm: 2
    // hence number of joystick directly coded
    JOYINFOEX joyInfoEx;
    Event identity, tmp;
    JoystickSource *source;
    int jp;
    int actualMovement;

    for (int i=0; i<2; i++)
    {
        // test if joystick with id i is connected
        joyInfoEx.dwSize = sizeof(joyInfoEx);
        joyInfoEx.dwFlags = JOY_RETURNALL;
        jp = (joyGetPosEx(i, &joyInfoEx) == JOYERR_NOERROR);

        if (jp)
            if (!joyPresent[i])
                getCaps(i);

        joyPresent[i] = jp;

        if (!joyPresent[i])
            continue;

        // get x
        actualMovement = joyInfoEx.dwXpos;
        if (joyInfoEx.dwXpos < joyCaps[i].wXmin)
            actualMovement = joyCaps[i].wXmin;
        else if (joyInfoEx.dwXpos > joyCaps[i].wXmax)
            actualMovement = joyCaps[i].wXmax;
        actualMovement = actualMovement - joyCaps[i].wXmin - xRange[i]/2;
        tmp.getPosition()[0] = (float)actualMovement*2.0/(float)xRange[i];

        // get y
        actualMovement = joyInfoEx.dwYpos;
        if (joyInfoEx.dwYpos < joyCaps[i].wYmin)
            actualMovement = joyCaps[i].wYmin;
        else if (joyInfoEx.dwYpos > joyCaps[i].wYmax)
            actualMovement = joyCaps[i].wYmax;
        actualMovement = actualMovement - joyCaps[i].wYmin - yRange[i]/2;
        tmp.getPosition()[1] = (float)actualMovement*2.0/(float)yRange[i];

        // get z
        if (zRange > 0)
        {
            actualMovement = joyInfoEx.dwZpos;
            if (joyInfoEx.dwZpos < joyCaps[i].wZmin)
                actualMovement = joyCaps[i].wZmin;
            else if (joyInfoEx.dwZpos > joyCaps[i].wZmax)
                actualMovement = joyCaps[i].wZmax;
            actualMovement = actualMovement - joyCaps[i].wZmin - zRange[i]/2;
            tmp.getPosition()[2] = (float)actualMovement*2.0/(float)zRange[i];
        }
        else
            tmp.getPosition()[2] = 0;

        // only four buttons supported
        tmp.getButton() = joyInfoEx.dwButtons & 0x0000000F;

        int update = 0;

        for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
        {
            source = (JoystickSource *) *it;
            if (source->id == i+1)
            {
                update = 0;
                if (source->tmpEvent.getButton() != tmp.getButton())
                    update = 1;
                for (int j=0; j<3; j++)
                    if (source->tmpEvent.getPosition()[j] != tmp.getPosition()[j])
                        update = 1;
                if (update)
                {
                    lock();
                    source->tmpEvent = tmp;
                    source->changed = 1;
                    unlock();
                }
            }
        }
    } 
}

} // namespace ot


#else
#pragma message(">>> no joystick support")
#endif
