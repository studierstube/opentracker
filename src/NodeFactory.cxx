// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      NodeFactory.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'NodeFactory'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 25, 2000 13:44 gr added NetworkSource classes
//     Updated return type of method 'createNode'
//     Update comment header
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
//@END_USER2


// Static members


/*@NOTE_3796
Constructor method.
*/
NodeFactory::NodeFactory() //@INIT_3796
{//@CODE_3796
    ConstructorInclude();

    // Put in your own code
}//@CODE_3796


/*@NOTE_1001
Destructor method.
*/
NodeFactory::~NodeFactory()
{//@CODE_1001
    DestructorInclude();

    // Put in your own code
}//@CODE_1001


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_1000
Method which must be called first in a constructor.
*/
void NodeFactory::ConstructorInclude()
{
}


/*@NOTE_1002
Method which must be called first in a destructor.
*/
void NodeFactory::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
