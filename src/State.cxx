// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      State.cxx
//  TYPE:      h header file
//  PROJECT:   @INSERT_PROJECTNAME
//  CONTENT:   Implementation of class 'State'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// July 22, 2000 18:55 Gerhard Reitmayr & updated some methods now returning pointers instead of references
//     Update comment header
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
//@END_USER2


// Static members


/*@NOTE_3789
Constructor method.
*/
State::State(unsigned short button_, int isValid_,
             float confidence_) //@INIT_3789
    : button(button_)
    , isValid(isValid_)
    , confidence(confidence_)
{//@CODE_3789
    ConstructorInclude();
    position[0] = 0;
    position[1] = 0;
    position[2] = 0;
    orientation[0] = 0;
    orientation[1] = 0;
    orientation[2] = 0;
    orientation[3] = 1;
    // Put in your own code
}//@CODE_3789


/*@NOTE_50
Destructor method.
*/
State::~State()
{//@CODE_50
    DestructorInclude();

    // Put in your own code
}//@CODE_50


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_49
Method which must be called first in a constructor.
*/
void State::ConstructorInclude()
{
}


/*@NOTE_51
Method which must be called first in a destructor.
*/
void State::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
