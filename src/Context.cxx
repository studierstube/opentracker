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
  
    VideoSource * video = new VideoSource;
    addFactory( *video );
    addModule( "Video", *video );
    
    NetworkDriver * driver = new NetworkDriver;
    addFactory( *driver );
    addModule( "Network", *driver );

    TestModule * test = new TestModule;
    addFactory( *test );
    // actually it doesn't have a configuration element
    addModule( "Test", *test );
    
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
    rootNodes = parser->parseConfigurationFile( filename );
}//@CODE_4765


/*@NOTE_4768
This method implements the main loop and runs until it is stopped somehow.
Then it calls close() on all modules.
*/
void Context::run()
{//@CODE_4768
    while (1)
    {
        ModuleVector::iterator it = modules.begin();
        
        // begin update cycle
        while( it != modules.end())
        {
            (*it)->beginUpdate();
            it++;
        }
        
        // update cycle
        it = modules.begin();
        while( it != modules.end())
        {
            (*it)->update();
            it++;
        }
        
        // end update cycle
        it = modules.begin();
        while( it != modules.end())
        {
            (*it)->endUpdate();
            it++;
        }
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
