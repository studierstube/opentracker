// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      Marker.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'Marker'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 16, 2000 21:43 gerhard reitmayr
//     Update comment header
// ===========================================================================
//@START_USER1
//@END_USER1


// Master include file
#include "TrackerServer.h"


//@START_USER2
//@END_USER2


// Static members


/*@NOTE_3750
Constructor method.
*/
Marker::Marker(int markerId_, double vertex_[4][2]) //@INIT_3750
    : TreeNode()
    , markerId(markerId_)
{//@CODE_3750
    ConstructorInclude();
    for( int i = 0; i < 4; i ++ )
    {
    	this->vertex[i][0] = vertex_[i][0];
	    this->vertex[i][1] = vertex_[i][1];
    }
    // Put in your own code
}//@CODE_3750


/*@NOTE_1598
Destructor method.
*/
Marker::~Marker()
{//@CODE_1598
    DestructorInclude();

    // Put in your own code
}//@CODE_1598


/*@NOTE_4436
returns the markerId of the Marker
*/
int Marker::getMarkerId()
{//@CODE_4436
    return markerId;
}//@CODE_4436


/*@NOTE_3550
returns the current state of the node. This method will be overriden in any subclasses
that provide real functionality. All processing of states will be done here.
*/
State* Marker::getState()
{//@CODE_3550
    return &state;
}//@CODE_3550


/*@NOTE_4438
returns the array of vertex positions
*/
double** Marker::getVertex()
{//@CODE_4438
    return (double **)vertex;
}//@CODE_4438


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_1597
Method which must be called first in a constructor.
*/
void Marker::ConstructorInclude()
{
}


/*@NOTE_1599
Method which must be called first in a destructor.
*/
void Marker::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
