// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      Context.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'Context'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
//@END_USER2


// Static members


/*@NOTE_4782
Constructor method.
*/
Context::Context() //@INIT_4782
  
{//@CODE_4782
    ConstructorInclude();

    // Build a parser
    parser = new ConfigurationParser( &factory );
    
    // Instance the default modules and add to factory and parser
    CommonNodeFactory * common = new CommonNodeFactory;
    addFactory( *common );
  
    VideoModule * video = new VideoModule;
    addFactory( *video );
    addModule( "Video", *video );
    
    NetworkModule * driver = new NetworkModule;
    addFactory( *driver );
    addModule( "Network", *driver );

    TestModule * test = new TestModule;
    addFactory( *test );
    // actually it doesn't have a configuration element
    addModule( "Test", *test );

	NetworkSourceModule * network = new NetworkSourceModule;
	addFactory( * network );
	addModule( "NetworkSourceModule", *network );
#ifndef WIN32
    CursesModule * output = new CursesModule;
    addFactory( *output );
    addModule( "CursesModule", *output );
#endif
    
}//@CODE_4782


/*@NOTE_4748
Destructor method.
*/
Context::~Context()
{//@CODE_4748
    DestructorInclude();

    // Put in your own code
}//@CODE_4748


/*@NOTE_4761
adds a new newfactory to the NodeFactoryContainer
*/
void Context::addFactory(NodeFactory& newfactory)
{//@CODE_4761
    factory.addFactory( newfactory );
}//@CODE_4761


/*@NOTE_4756
adds a module to the contexts collection
*/
void Context::addModule(const char* name, Module& module)
{//@CODE_4756
    modules.push_back( &module );
    parser->addModule( name, module );
}//@CODE_4756


/*@NOTE_4765
This method initialies the context with a tracker tree described by a configuration file.
It parses the file and builds the tree. 
*/
void Context::parseConfiguration(const char* filename)
{//@CODE_4765
    rootNode = parser->parseConfigurationFile( filename );
}//@CODE_4765


/*@NOTE_4768
This method implements the main loop and runs until it is stopped somehow.
Then it calls close() on all modules.
*/
void Context::run()
{//@CODE_4768
    int stop = 0;
    
    ModuleVector activeModules;
    for( ModuleVector::iterator it = modules.begin(); it != modules.end(); it++ )
    {
        if( (*it)->isInitialized())
        {
            activeModules.push_back( (*it));
        }
    }
    
    while ( stop == 0)
    {              
        // begin update cycle
        for( it = activeModules.begin(); it != activeModules.end(); it ++ )  
        {
            (*it)->beginUpdate();            
        }
        
        // update cycle
        for( it = activeModules.begin(); it != activeModules.end(); it ++ )
        {
            (*it)->update();
        }
        
        // end update cycle
        for( it = activeModules.begin(); it != activeModules.end(); it ++ )
        {
            (*it)->endUpdate();         
        }
        
        // test for exit 
        for( it = activeModules.begin(); it != activeModules.end(); it ++ )
        {
            if( (*it)->stop() == 1 )
            {
                stop = 1;
            }     
        }
    }
    for( it = activeModules.begin(); it != activeModules.end(); it++ )
    {
        (*it)->close();
    }   
}//@CODE_4768


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4747
Method which must be called first in a constructor.
*/
void Context::ConstructorInclude()
{
}


/*@NOTE_4749
Method which must be called first in a destructor.
*/
void Context::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
