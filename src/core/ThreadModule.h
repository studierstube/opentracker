 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for ThreadModule class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/ThreadModule.h,v 1.3 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _THREADMODULE_H
#define _THREADMODULE_H

#include "../dllinclude.h"

#include <ace/Thread.h>
#include <ace/Synch.h>

#include "Module.h"

/**
 * ThreadModule is an abstract superclass for modules that require a 
 * multithreaded implementation. It provides a thread method that should be
 * overriden by subclasses to add their own thread implementation and
 * provides lock and unlock methods to synchronize access to shared resources
 * between the main thread and the module thread. More complicated 
 * concurrent access control has to be implemented by the subclasses.
 *
 * This class provides empty implementations for all its methods instead of
 * pure virtual functions, so that classes inheriting from it don't have to
 * implement all methods in a trivial way.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API ThreadModule : public Module
{
//members
protected:
    /// handle to module specific thread;
    ACE_thread_t thread;
    /// Mutex to implement lock, unlock behavior
    ACE_Thread_Mutex mutex;
        
//methods
protected:    
    /** enters a critical section. Use this method to protect your operations
     * from another thread. This is not a recursive lock, do not call it
     * several times without unlocking !*/
    void lock()
    {
        mutex.acquire();
    };
    /** leaves a critical section. Use this method to release the protection.
     */
    void unlock()
    {
        mutex.release();
    };
    /** the work method for the module thread. This is executed by the new
     * module thread. In this class it does nothing but subclasses should
     * override it to add their implementation. */
    virtual void run()
    {};    
    /** static thread function passed to the actual thread. This calls
     * then run on the right instance. Do not use this yourself. */
    static void thread_func( void * data ){
        ((ThreadModule *)data)->run();
    }
        
public:
    /**
     * This method is called after initialisation is finished and before the
     * main loop is started. In this implementation it starts the thread. Be
     * sure to call this method from your subclasses start method to start the
     * thread !*/
    virtual void start()
    {
        thread = ACE_Thread::spawn((ACE_THR_FUNC)thread_func, 
                                   this, 
                                   THR_NEW_LWP | THR_JOINABLE,
                                   &thread );
    };    
    /**
     * closes the module. In this implementation it stops the thread. Be sure
     * to call this method from your subclasses close method to stop the
     * thread !*/
    virtual void close()
    {
        ACE_Thread::cancel( thread );
    };            
};

#endif
