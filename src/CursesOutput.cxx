// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      CursesOutput.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'CursesOutput'
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


/*@NOTE_7214
Constructor method.
*/
CursesOutput::CursesOutput(char* comment_) //@INIT_7214
    : TreeNode()
    , comment(comment_)
{//@CODE_7214
    ConstructorInclude();

    // Put in your own code
}//@CODE_7214


/*@NOTE_7208
Destructor method.
*/
CursesOutput::~CursesOutput()
{//@CODE_7208
    DestructorInclude();

    // Put in your own code
}//@CODE_7208


/*@NOTE_7227
returns the comment string
*/
char* CursesOutput::getComment()
{//@CODE_7227
    return comment;
}//@CODE_7227


/*@NOTE_7212
returns the current state of the node. This method will be overriden in any subclasses
that provide real functionality. All processing of states will be done here.
*/
State* CursesOutput::getState()
{//@CODE_7212
    if( !children.empty())
    {
        return (*children.begin())->getState();
    } else {
        return NULL;
    }
}//@CODE_7212


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_7207
Method which must be called first in a constructor.
*/
void CursesOutput::ConstructorInclude()
{
}


/*@NOTE_7209
Method which must be called first in a destructor.
*/
void CursesOutput::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
