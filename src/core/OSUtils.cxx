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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for OSUtils class
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/OSUtils.cxx,v 1.3 2002/09/26 13:56:25 bornik Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "OSUtils.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#include <limits.h>
#else
#include <windows.h>
#include <sys/timeb.h>
#include <time.h>
#endif
#include <sys/types.h>

// returns the current time in milliseconds since ...
double OSUtils::currentTime()
{
 #ifndef WIN32  // IRIX and Linux code
#ifdef _SGI_SOURCE
    struct timeval tp;
    gettimeofday(&tp);
    return (double)(tp.tv_sec)*1000.0 + (double)(tp.tv_usec)*0.001;
#else //LINUX code
    struct timeval tp;
    gettimeofday(&tp,NULL);
    return (double)(tp.tv_sec)*1000.0 + (double)(tp.tv_usec)*0.001;
#endif
#else  // WIN code
    struct _timeb timeBuffer;
    _ftime(&timeBuffer);
    return (double)(timeBuffer.time)*1000.0 + (double)timeBuffer.millitm;
#endif
}
#if defined (WIN32) || defined (GCC3)
#include <iostream>
#else
#include <iostream.h>
#endif

//sleeps the specified amount of time ...
void OSUtils::sleep( double time )
{
#ifdef WIN32
    Sleep((long) time );
#else
#ifdef _SGI_SOURCE
    sginap((long)( time * CLK_TCK / 1000.0 ));
#else // LINUX Code
    // This is a workaround for a bug in the kernel, that always adds
    // a tick to the specified amount of sleep time.
    if( time >= 10 )
    	time -= 10;
    else 
    	return;
    usleep((unsigned long)(time*1000));
#endif
#endif
}

