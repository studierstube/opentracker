// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      TreeNode.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'TreeNode'
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
#ifdef __sgi
#include <algo.h>
#else
#include <algorithm>
#endif
//@END_USER2


// Static members


/*@NOTE_3757
Constructor method.
*/
TreeNode::TreeNode() //@INIT_3757
{//@CODE_3757
    ConstructorInclude();

    // Put in your own code
}//@CODE_3757


/*@NOTE_68
Destructor method.
*/
TreeNode::~TreeNode()
{//@CODE_68
    DestructorInclude();

    // Put in your own code
}//@CODE_68


/*@NOTE_122
adds a child to the TreeNode.
*/
void TreeNode::addChild(TreeNode& node)
{//@CODE_122
    TreeNodeVector::iterator result = std::find( children.begin(), children.end(), &node );
    if( result == children.end())
    {
        children.push_back( &node );
    }
}//@CODE_122


/*@NOTE_7057
returns the current state of the node. This method will be overriden in any subclasses
that provide real functionality. All processing of states will be done here.
*/
State* TreeNode::getState()
{//@CODE_7057
   return NULL;
}//@CODE_7057


/*@NOTE_135
removes a child from the TreeNode
*/
void TreeNode::removeChild(TreeNode& node)
{//@CODE_135
    TreeNodeVector::iterator result = std::find( children.begin(), children.end(), &node );    
    if( result != children.end())
    {
        children.erase( result );
    }
}//@CODE_135


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_67
Method which must be called first in a constructor.
*/
void TreeNode::ConstructorInclude()
{
}


/*@NOTE_69
Method which must be called first in a destructor.
*/
void TreeNode::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
