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
/** header file for ThreadModule class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/ThreadModule.h,v 1.10 2003/02/18 02:12:51 tamer Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _THREADMODULE_H
#define _THREADMODULE_H

#include "../dllinclude.h"

class ACE_Thread_Mutex;

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
    /// handle to module specific thread. This is a little bit tricky as we
    /// don't use the ACE definition but the same type.
   void * thread;

    /// Mutex to implement lock, unlock behavior
    ACE_Thread_Mutex * mutex;
        
//methods
protected:    
    /** enters a critical section. Use this method to protect your operations
     * from another thread. This is not a recursive lock, do not call it
     * several times without unlocking !*/
    void lock();
    /** leaves a critical section. Use this method to release the protection.
     */
    void unlock();
    /** the work method for the module thread. This is executed by the new
     * module thread. In this class it does nothing but subclasses should
     * override it to add their implementation. */
    virtual void run()
    {};    
    /** static thread function passed to the actual thread. This calls
     * then run on the right instance. Do not use this yourself. */
    static void thread_func( void * data )
    {
        ((ThreadModule *)data)->run();
    };

public:
    /** constructor */
	ThreadModule();
    /** destructor */
    virtual ~ThreadModule();

    /**
     * This method is called after initialisation is finished and before the
     * main loop is started. In this implementation it starts the thread. Be
     * sure to call this method from your subclasses start method to start the
     * thread !*/
    virtual void start();    
    /**
     * closes the module. In this implementation it stops the thread. Be sure
     * to call this method from your subclasses close method to stop the
     * thread !*/
    virtual void close();            
};

#endif
