// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      VirtualTransformation.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'VirtualTransformation'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 10, 2000 10:22 Gerhard Reitmayr
//     Update comment header
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
//@END_USER2


// Static members


/*@NOTE_3764
Constructor method.
*/
VirtualTransformation::VirtualTransformation(float translation_[3],
                                             float scale_[3],
                                             float rotation_[4]) //@INIT_3764
    : Transformation(translation_, scale_, rotation_ )
{//@CODE_3764
    ConstructorInclude();

    // Put in your own code
}//@CODE_3764


/*@NOTE_282
Destructor method.
*/
VirtualTransformation::~VirtualTransformation()
{//@CODE_282
    DestructorInclude();

    // Put in your own code
}//@CODE_282


/*@NOTE_3567
transforms a state. Simplifies implementing other Transformations as it is only needed to
override this method. It returns the pointer passed to the method.
*/
State* VirtualTransformation::transformState(State* state)
{//@CODE_3567
    
    return state;
}//@CODE_3567


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_281
Method which must be called first in a constructor.
*/
void VirtualTransformation::ConstructorInclude()
{
}


/*@NOTE_283
Method which must be called first in a destructor.
*/
void VirtualTransformation::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
