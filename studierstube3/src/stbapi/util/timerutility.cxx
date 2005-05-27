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
/** timer class
  *
  * @author  Dieter Schmalstieg
  *
  * $Id: timerutility.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include "timerutility.h"
#include <stbapi/util/os.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

int TimerUtility::silence = 0; // default = silent

//----------------------------------------------------------------------------

TimerUtility::TimerUtility(const char* timerName, double timerInterval)
:
    name(strdup(timerName)),
    interval(timerInterval),
    startTime(osTimeOfDay()),
    counter(0)
{
    // nil
}

//----------------------------------------------------------------------------

TimerUtility::~TimerUtility()
{
    free((void*)name);
}

//----------------------------------------------------------------------------

void
TimerUtility::timeThis()
{
    double currentTime = osTimeOfDay();
    double diff = currentTime - startTime;
    if(diff >= interval)
    {
        double rate = ((double)counter)*1000/diff;
        if(!silence) printf("%s: %6.1f frames/second\n",name,rate);
        counter = 0;
        startTime = currentTime;
    }
    else counter++;    
}

//----------------------------------------------------------------------------

void
TimerUtility::setSilence(int isSilent)
{
    silence = isSilent;
}

//----------------------------------------------------------------------------
//eof

