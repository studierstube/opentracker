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
/** source file for Context class
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Context.cxx,v 1.13 2001/06/11 03:22:37 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "Context.h"
#include "../OpenTracker.h"

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
Context::Context( int init )
{
    // Build a parser
    parser = new ConfigurationParser( factory );
    if( init != 0 )
    {
        initializeContext( *this );
        cleanUp = 1;
    }
    else {
        cleanUp = 0;
    }
}

// Destructor method.
Context::~Context()
{
    delete parser;
    if( cleanUp == 1 )
    {
        for( ModuleVector::iterator it = modules.begin(); it != modules.end(); it++ )
        {
            delete (*it);
        }
    }
    modules.clear();
}

// adds a new newfactory to the NodeFactoryContainer
void Context::addFactory(NodeFactory& newfactory)
{
    factory.addFactory( newfactory );
}

// adds a module to the contexts collection
void Context::addModule(const string& name, Module& module)
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
void Context::parseConfiguration(const string& filename)
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
    while ( stop() == 0 )
    {
        // push and pull parts of the main loop
        pushStates();
        pullStates();
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
