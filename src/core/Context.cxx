 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for Context class
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Context.cxx,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "Context.h"
#include "CommonNodeFactory.h"
#include "TestModule.h"
#include "../output/ConsoleModule.h"
#include "../network/NetworkSinkModule.h"
#include "../network/NetworkSourceModule.h"

// constructor method.

Context::Context()
{
    // Build a parser
    parser = new ConfigurationParser( factory );

    // Instance the default modules and add to factory and parser
    CommonNodeFactory & common = *(new CommonNodeFactory);
    addFactory( common );
    /*
    VideoModule * video = new VideoModule;
    addFactory( *video );
    addModule( "Video", *video );
    */
    NetworkSinkModule * networksink = new NetworkSinkModule;
    addFactory( *networksink );
    addModule( (string)"NetworkSinkConfig", *networksink );
    
    TestModule & test = *(new TestModule);
    addFactory( test );
    // actually it doesn't have a configuration element
    addModule( ( string)"Test", test );

    ConsoleModule & console = *( new ConsoleModule );
    addFactory( console );
    addModule( (string)"ConsoleConfig", console );
    
    NetworkSourceModule * network = new NetworkSourceModule;
    addFactory( * network );
    addModule( (string)"NetworkSourceConfig", *network );    
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
    return 0;
}
