// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      TestSource.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'TestSource'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 25, 2000 13:44 gr added NetworkSource classes
//     Update comment header
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
//@END_USER2


// Static members


/*@NOTE_4801
default constructor
*/
TestSource::TestSource() //@INIT_4801
    : TreeNode()
    
{//@CODE_4801
    ConstructorInclude();

    // Put in your own code
}//@CODE_4801


/*@NOTE_4796
Destructor method.
*/
TestSource::~TestSource()
{//@CODE_4796
    DestructorInclude();

    // Put in your own code
}//@CODE_4796


/*@NOTE_4803
returns the current state of the node. This method will be overriden in any subclasses
that provide real functionality. All processing of states will be done here.
*/
State* TestSource::getState()
{//@CODE_4803
   return &state;
}//@CODE_4803


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_4795
Method which must be called first in a constructor.
*/
void TestSource::ConstructorInclude()
{
}


/*@NOTE_4797
Method which must be called first in a destructor.
*/
void TestSource::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
