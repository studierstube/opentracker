// ===========================================================================
//  (C) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:       zpthread
//  TYPE:       c++ header
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
//  08-aug-97 09:49:41  ds      last modification
//  08-aug-97 09:49:41  ds      created
// ===========================================================================

#ifndef _ZPT_H_
#define _ZPT_H_

#ifndef _SGI_SOURCE
#include <windows.h>
#endif

// #include <stbapi/stbapi.h>

// thread function prototype
typedef void threadFunction(void*);

// this function creates a thread calling userFunction with data as parameter
long createThread(threadFunction* userFunction, void* data, int stackLen=0);
void killThread(long tPid);

// simple semaphore class for thread mutex handling
class Semaphore
{
public:
    Semaphore();             //construtor
    ~Semaphore();            //destructor
    void wait();             //V operation
    void signal();           //P operation
    static void initClass(); //call this before first usage

private:
#ifdef _SGI_SOURCE
    void* semaHandle;
#else
	HANDLE semaHandle;
#endif
};

#endif//_ZPT_H_
