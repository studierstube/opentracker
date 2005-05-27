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
//  PROJECT:    Studierstube
//  CONTENT:    timer class
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
// ===========================================================================
//  HISTORY:
//
// ===========================================================================

#ifndef _TIMERUTILITY_H_
#define _TIMERUTILITY_H_

#include <stbapi/stbapi.h>

class STBAPI_API TimerUtility
{
public:
    TimerUtility(                      // constructor
        const char* timerName,         // description of timer
        double timerInterval = 1000.0  // default = 1 sec interval
        );
	~TimerUtility(                     // destructor
        );
    void timeThis(                     // call this every frame
        );
    static void setSilence(            // turn of timers
        int isSilent                   // true -> silent, false -> verbose
        );

private:
    static int silence;
    const char* name;
    double interval;
    double startTime;
    int counter;
};

#endif//_TIMERUTILITY_H_

