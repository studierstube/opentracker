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
/** Header file for OSUtils class, a collection of common utility functions.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/OSUtils.h,v 1.2 2001/06/11 22:01:59 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _OSUTILS_H
#define _OSUTILS_H

#include "../dllinclude.h"

/**
 * This class implements some utility functions, which are not provided by
 * other libraries. Currently these include time related functions to get
 * the current time and yield and sleep a specified amount of time.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API OSUtils 
{

public:

    /** returns the current time in milliseconds since 1.1.1970. Someone said 
     * this should probably be a long, I used a double because VRML does too.
     * @note check whether double or long is better 
     * @return double containing milliseconds since ...*/
    static double currentTime();
    
    /** sleeps for the specified amount of milliseconds 
     * and yields the execution to another thread. If 0 is specified, only
     * a yield is performed (implementation based on Sleep (Win32), sginap (SGI)).
     * The implementation is actually not accurate enough, because both systems
     * provide only timeslices of 1/100 of a second. So all sleep times will
     * actually be multiples of 1/100 of a second.
     * @arg time time to sleep in milliseconds
     */
     static void sleep( double time );
};

#endif
