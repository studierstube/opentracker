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
/** Zero Problem Threads: Threads'n'Semaphores
  *
  * @author  Dieter Schmalstieg, Gerd Hesina
  *
  * $Id: zpthread.cxx 3425 2004-07-14 12:09:17Z tamer $
  * @file                                                                   */
 /* ======================================================================= */

#include <cassert>
#include <cerrno>
#include <cstdio>

#ifdef WIN32
#  include <windows.h>
#else
#  
#    include <signal.h>
#    include <pthread.h>
#endif


#include "zpthread.h"

#ifndef WIN32 // Unix Code ( Either Irix or Linux )

threadFunction * tempFunction; // to store user function for createThread
pthread_t        _thread;

long 
createThread(threadFunction* userFunction, void* data, int stackLen)
{
  int threadPid = pthread_create(&_thread,
				 NULL,
				 userFunction,
				 data);
  return ((long)threadPid);
}

void killThread(long tPid)
{
  if (kill((int)tPid, SIGKILL)==-1)
    perror("NetworkTracker: kill");
  //assert(0); //why assert? - dieter
}

Semaphore::Semaphore()
{
  pthread_mutex_init(&semaHandle, NULL);
}

Semaphore::~Semaphore()
{
  pthread_mutex_unlock(&semaHandle);
  pthread_mutex_destroy(&semaHandle);
}


void 
Semaphore::wait()
{
  pthread_mutex_lock(&semaHandle);
}


void 
Semaphore::signal()
{
  pthread_mutex_unlock(&semaHandle);
}

void
Semaphore::initClass()
{

}

#else  // WIN code

long 
createThread(threadFunction* userFunction, void* data, int stackLen)
{
  DWORD threadPid;
  HANDLE threadHandle;
        
  threadHandle = CreateThread(NULL, 
                              stackLen, 
                              (LPTHREAD_START_ROUTINE)userFunction, 
                              data, 
                              0, 
                              &threadPid
                              );
  
  if (threadHandle == NULL)
  {
    LPVOID lpMsgBuf;
    
    FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL 
        );

    // Display the string.
    MessageBox(
        NULL,
        (LPCSTR)lpMsgBuf,
        "GetLastError",
        MB_OK|MB_ICONINFORMATION
        );
        
    // Free the buffer.
    LocalFree( lpMsgBuf );
    assert(0);
  }

  return ((long)threadHandle);
}

void killThread(long tPid)
{
    TerminateThread((HANDLE)tPid,0);
}

Semaphore::Semaphore()
{
    semaHandle = CreateSemaphore(NULL, 1, 200, NULL);
    assert(semaHandle != NULL);
}

Semaphore::~Semaphore()
{
    CloseHandle(semaHandle);
}


void 
Semaphore::wait()
{
    DWORD result = WaitForSingleObject(semaHandle, INFINITE);

    assert(result !=  WAIT_FAILED);
}


void 
Semaphore::signal()
{
    LONG prevCount;
	BOOL result = ReleaseSemaphore(semaHandle, 1, &prevCount);
	assert(result);
}

void
Semaphore::initClass()
{
}

#endif

//eof

