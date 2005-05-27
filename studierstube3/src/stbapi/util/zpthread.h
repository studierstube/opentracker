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
/**
 * header for zpthread
 *
 * @author  Dieter Schmalstieg, Gerd Hesina, Jan Prikryl
 *
 * $Id: zpthread.h 3425 2004-07-14 12:09:17Z tamer $
 * @file                                                                   */
/* ======================================================================= */
#ifndef _ZPT_H_
#define _ZPT_H_

// architecture specific includes
// windows
#ifdef WIN32
#  include <windows.h>
#  define THREADFUNC_RETURN_TYPE    void
#  define THREADFUNC_NULL
#else
#    include <pthread.h>
#    define THREADFUNC_RETURN_TYPE  void *
#    define THREADFUNC_NULL         NULL
#endif    

#include <stbapi/stbapi.h>

// thread function prototype
typedef THREADFUNC_RETURN_TYPE threadFunction(void*);

// this function creates a thread calling userFunction with data as parameter
long STBAPI_API createThread(threadFunction* userFunction, void* data, int stackLen=0);
void STBAPI_API killThread(long tPid);

// simple semaphore class for thread mutex handling
class Semaphore
{
public:
    STBAPI_API Semaphore();             //construtor
    STBAPI_API ~Semaphore();            //destructor
    void STBAPI_API wait();             //V operation
    void STBAPI_API signal();           //P operation
    static void STBAPI_API initClass(); //call this before first usage

private:

#ifndef WIN32
    pthread_mutex_t semaHandle;       
#else
    HANDLE semaHandle;
#endif
};

#endif//_ZPT_H_
