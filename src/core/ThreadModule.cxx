 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for ThreadModule class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/ThreadModule.cxx,v 1.1 2001/03/27 05:36:12 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include <ace/Thread.h>
#include <ace/Synch.h>

#include "ThreadModule.h"

// enters a critical section. 

void ThreadModule::lock()
{
	mutex->acquire();
}

// leaves a critical section. 

void ThreadModule::unlock()
{
	mutex->release();
}

// constructor
        
ThreadModule::ThreadModule()
{
	mutex = new ACE_Thread_Mutex;
}

// starts the thread

void ThreadModule::start()
{
	thread = ACE_Thread::spawn((ACE_THR_FUNC)thread_func, 
		this, 
		THR_NEW_LWP | THR_JOINABLE,
		&thread );
}    

// stops the thread and closes the module

void ThreadModule::close()
{
	ACE_Thread::cancel( thread );
}

