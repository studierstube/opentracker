// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      TestModule.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'TestModule'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 16, 2000 22:10 gerhard reitmayr removed Node and made everything TreeNodes
//     Updated code of method 'createNode'
//     Updated member 'nodes'
// August 16, 2000 21:43 gerhard reitmayr
//     Update comment header
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
#include <string>
#include <stdio.h>
#ifdef WIN32
#include <iostream>    // VisualC++ has two incompatible iostreams libraries !
#else
#include <iostream.h>
#endif
//@END_USER2


// Static members


/*@NOTE_4807
Destructor method.
*/
TestModule::~TestModule()
{//@CODE_4807
    DestructorInclude();

    // Put in your own code
}//@CODE_4807


/*@NOTE_4813
This method is called to construct a new Node. It takes the element name and
any attribute name, value pairs and returns the new Node, if it is able to 
construct the corresponding type. Otherwise it returns NULL.
*/
TreeNode* TestModule::createNode(char* const name, StringMap& attributes)
{//@CODE_4813
    std::string strName( name );
    if( strName.compare("TestSource") == 0 )        
    {
        int frequency;
		int offset;
        int num = sscanf( attributes["frequency"], " %i", &frequency );
        if( num == 0 ){
            frequency = 1;
        }
		num = sscanf( attributes["offset"], " %i", &offset );
        if( num == 0 ){
            offset = 0;
        }
		TestSource * source = new TestSource;
		source->frequency = frequency;
		source->offset = offset;
        nodes.push_back( source );
        cout << "Build source node " << endl;
        return source;        
    }
    return NULL;
}//@CODE_4813


/*@NOTE_4812
updates the tracker source. This method is where most work is done. It should do
whatever the source needs to do to update any nodes in the tree with new data.

It may seem overkill to have three methods called per update cycle. However the
different functionality of the modules might call for that. For example, any sources 
should do their work in update, whereas the NetworkDriver will gather the
stations updates in endUpdate to be sure that the new data was processed.
This makes the modules independed of the order of their invocations.
*/
void TestModule::update()
{//@CODE_4812
    for( TreeNodeVector::iterator it = nodes.begin(); it != nodes.end(); it ++)
    {
		TestSource * source = (TestSource *)(*it);
		if(((cycle - source->offset ) % source->frequency ) == 0 )
		{
			source->getState()->isValid = 1;
		}
		else
		{
			source->getState()->isValid = 0;
		}
    }
    cycle++;
}//@CODE_4812


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4806
Method which must be called first in a constructor.
*/
void TestModule::ConstructorInclude()
{
}


/*@NOTE_4808
Method which must be called first in a destructor.
*/
void TestModule::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
