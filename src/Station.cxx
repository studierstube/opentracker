// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      Station.cxx
//  TYPE:      h header file
//  PROJECT:   @INSERT_PROJECTNAME
//  CONTENT:   Implementation of class 'Station'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// July 22, 2000 18:56 Gerhard Reitmayr & updated some methods now returning pointers instead of references
//     Update comment header
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
#include<string.h>
//@END_USER2


// Static members


/*@NOTE_3774
Constructor method.
*/
Station::Station(int stationNumber_, const char* name_) //@INIT_3774
    : TreeNode()
    , stationNumber(stationNumber_)
{//@CODE_3774
    ConstructorInclude();
    strcpy( name, name_ );    
}//@CODE_3774


/*@NOTE_3149
Destructor method.
*/
Station::~Station()
{//@CODE_3149
    DestructorInclude();

    // Put in your own code
}//@CODE_3149


/*@NOTE_3547
returns the name of the station
*/
char* Station::getName()
{//@CODE_3547
    return name;
}//@CODE_3547


/*@NOTE_3590
returns the current state of the node. This method will be overriden in any subclasses
that provide real functionality. All processing of states will be done here.
*/
State* Station::getState()
{//@CODE_3590
    if( !children.empty())
    {
        return (*children.begin())->getState();
    } else {
        return NULL;
    }
}//@CODE_3590


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_3148
Method which must be called first in a constructor.
*/
void Station::ConstructorInclude()
{
}


/*@NOTE_3150
Method which must be called first in a destructor.
*/
void Station::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
