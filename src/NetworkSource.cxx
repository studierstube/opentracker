// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      NetworkSource.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'NetworkSource'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 25, 2000 14:28 gr added StateVector type
//     Updated code of method 'getState'
// August 25, 2000 13:44 gr added NetworkSource classes
//     Added method 'DestructorInclude'
//     Added method 'ConstructorInclude'
//     Added method 'getStationNumber'
//     Added method 'getState'
//     Added method '~NetworkSource'
//     Added method 'NetworkSource'
//     Added inheritance 'TreeNode'
//     Added member 'stationNumber'
//     Added member 'state'
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
//@END_USER2


// Static members


/*@NOTE_13352
Constructor method.
*/
NetworkSource::NetworkSource(int stationNumber_) //@INIT_13352
    : TreeNode()
    , stationNumber(stationNumber_)
{//@CODE_13352
    ConstructorInclude();

    // Put in your own code
}//@CODE_13352


/*@NOTE_12986
Destructor method.
*/
NetworkSource::~NetworkSource()
{//@CODE_12986
    DestructorInclude();

    // Put in your own code
}//@CODE_12986


/*@NOTE_13000
returns the current state of the node. This method will be overriden in any subclasses
that provide real functionality. All processing of states will be done here.
*/
State* NetworkSource::getState()
{//@CODE_13000
   return &state;
}//@CODE_13000


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_12985
Method which must be called first in a constructor.
*/
void NetworkSource::ConstructorInclude()
{
}


/*@NOTE_12987
Method which must be called first in a destructor.
*/
void NetworkSource::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
