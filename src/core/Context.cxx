 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for Context class
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Context.cxx,v 1.4 2001/03/24 23:50:02 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */


#include <ace/OS.h>
#include "Context.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#else
#include <windows.h>
#include <sys/timeb.h>
#include <time.h>
#endif
#include <sys/types.h>

// constructor method.

Context::Context()
{
    // Build a parser
    parser = new ConfigurationParser( factory );
}

// Destructor method.

Context::~Context()
{
}

// adds a new newfactory to the NodeFactoryContainer

void Context::addFactory(NodeFactory& newfactory)
{
    factory.addFactory( newfactory );
}

// adds a module to the contexts collection

void Context::addModule(string& name, Module& module)
{
    modules.push_back( &module );
    parser->addModule( name, module );
}

// calls close on all modules to close any resources.

void Context::close()
{
    for( ModuleVector::iterator it = modules.begin(); it != modules.end(); it++ )
    {
        (*it)->close();
    }
}

// parses the file and builds the tree.

void Context::parseConfiguration(string& filename)
{
    rootNode = parser->parseConfigurationFile( filename );
}

// calls pullState on all modules to get data out again.

void Context::pullStates()
{
    for( ModuleVector::iterator it = modules.begin(); it != modules.end(); it++ )
    {
        (*it)->pullState();
    }
}

// This method calls pushState on all modules to get new data into the shared data tree.

void Context::pushStates()
{
    for( ModuleVector::iterator it = modules.begin(); it != modules.end(); it++ )
    {
        (*it)->pushState();
    }
}

// This method implements the main loop and runs until it is stopped somehow.

void Context::run()
{
    start();
    double time = currentTime(), newTime;
    while ( stop() == 0 )
    {
        // push and pull parts of the main loop
        pushStates();
        pullStates();
        newTime = currentTime();
        if( newTime - time < 20 )
        {            
#ifdef WIN32            
            Sleep((DWORD)(newTime - time));
#endif            
            time = newTime;
        }
    }
    close();
}

// calls start on all modules to do some initialization.

void Context::start()
{
    for( ModuleVector::iterator it = modules.begin(); it != modules.end(); it++ )
    {
        (*it)->start();
    }
}

// tests all modules for stopping

int Context::stop()
{
    int value = 0;
    for( ModuleVector::iterator it = modules.begin(); it != modules.end(); it++ )
    {
        value |= (*it)->stop();
    }
    return value;
}

// returns the current time in milliseconds since ...

double Context::currentTime()
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
