// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       zpthread
//  TYPE:       c++ code
//  PROJECT:    Studierstube
//  CONTENT:    Zero Problem Threads: Threads'n'Semaphores
//  VERSION:    2.0
// ===========================================================================
//  AUTHORS:    ds      Dieter Schmalstieg
//              gh      Gerd Hesina
// ===========================================================================
//  HISTORY:
//
//  27-aug-99 16:59:18  gh      last modification
//  08-aug-97 09:50:52  ds      last modification
//  08-aug-97 09:50:52  ds      created
// ===========================================================================
//zpt.c++

#include <assert.h>
#include <errno.h>
#include <stdio.h>

#ifdef _SGI_SOURCE
#include <ulocks.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/prctl.h>
#include <signal.h>
#else
#include <windows.h>
#endif
#include "zpthread.h"

#ifdef _SGI_SOURCE  // IRIX code

threadFunction* tempFunction; //to store user function for createThread

usptr_t* arenaHandle;

void 
controlThread(void* data, size_t)
{
    prctl(PR_TERMCHILD); // terminate child if parent dies
    tempFunction(data);
}


long 
createThread(threadFunction* userFunction, void* data, int stackLen)
{
    if(stackLen == 0) stackLen = prctl(PR_GETSTACKSIZE);
    tempFunction = userFunction; //store the user function
    int threadPid = sprocsp((controlThread),PR_SALL,(void*)data,NULL,stackLen);
    if(threadPid == -1)
    {
        printf("createThread failed, errno=%d\n",errno);
        assert(0);
    }
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
    semaHandle = (void*)usnewsema(arenaHandle, 1);
    assert(semaHandle != NULL);
}

Semaphore::~Semaphore()
{
    usfreesema(semaHandle, arenaHandle);
}


void 
Semaphore::wait()
{
    int result = uspsema((usptr_t*)semaHandle);
    assert(result != -1);
}


void 
Semaphore::signal()
{
    int result = usvsema((usptr_t*)semaHandle);
    assert(result != -1);
}

void
Semaphore::initClass()
{
    int result;
    result = usconfig(CONF_ARENATYPE,US_SHAREDONLY); //remove file!
    assert(result != -1);
    result = usconfig(CONF_INITUSERS, 200); //allow 200 threads
    assert(result != -1);
    arenaHandle = usinit(mktemp("/tmp/XXXXXX"));
    assert(arenaHandle != NULL);
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

    if(threadHandle == NULL)
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
        MessageBox( NULL, (LPCSTR)lpMsgBuf, "GetLastError", MB_OK|MB_ICONINFORMATION );
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
    semaHandle = CreateSemaphore(NULL,
		                         1,
                                 200,
								 NULL
								 );
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

