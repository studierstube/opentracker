 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
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
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
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


#include "JoystickModule.h"
#include "JoystickSource.h"

#ifdef USE_JOYSTICK

#include <stdio.h>
#include <iostream>    // VisualC++ uses STL based IOStream lib

#pragma comment(lib, "winmm")

using namespace std;


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

        cout << "Build JoystickSource node for ID " << joy_id << endl;
        initialized = 1;
        return source;
    }
    return NULL;
}

// pushes events into the tracker tree.

void JoystickModule::pushState()
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
                source->state = source->tmpState;
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
                cout << "JoystickModule: joystick with ID " << i+1 << 
                    " (" << joyCaps[i].szPname << ") connected.\n";
            }
            else
                cout << "JoystickModule: joystick with ID " << i+1 << 
                " not connected.\n";
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
    State identity, tmp;
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
        tmp.position[0] = (float)actualMovement*2.0/(float)xRange[i];

        // get y
        actualMovement = joyInfoEx.dwYpos;
        if (joyInfoEx.dwYpos < joyCaps[i].wYmin)
            actualMovement = joyCaps[i].wYmin;
        else if (joyInfoEx.dwYpos > joyCaps[i].wYmax)
            actualMovement = joyCaps[i].wYmax;
        actualMovement = actualMovement - joyCaps[i].wYmin - yRange[i]/2;
        tmp.position[1] = (float)actualMovement*2.0/(float)yRange[i];

        // get z
        if (zRange > 0)
        {
            actualMovement = joyInfoEx.dwZpos;
            if (joyInfoEx.dwZpos < joyCaps[i].wZmin)
                actualMovement = joyCaps[i].wZmin;
            else if (joyInfoEx.dwZpos > joyCaps[i].wZmax)
                actualMovement = joyCaps[i].wZmax;
            actualMovement = actualMovement - joyCaps[i].wZmin - zRange[i]/2;
            tmp.position[2] = (float)actualMovement*2.0/(float)zRange[i];
        }
        else
            tmp.position[2] = 0;

        // only four buttons supported
        tmp.button = joyInfoEx.dwButtons & 0x0000000F;

        int update = 0;

        for( NodeVector::iterator it = nodes.begin(); it != nodes.end(); it++ )
        {
            source = (JoystickSource *) *it;
            if (source->id == i+1)
            {
                update = 0;
                if (source->tmpState.button != tmp.button)
                    update = 1;
                for (int j=0; j<3; j++)
                    if (source->tmpState.position[j] != tmp.position[j])
                        update = 1;
                if (update)
                {
                    lock();
                    source->tmpState = tmp;
                    source->changed = 1;
                    unlock();
                }
            }
        }
    } 
}

#endif
