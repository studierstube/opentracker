// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      MaxConfidenceFilter.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'MaxConfidenceFilter'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 16, 2000 22:10 gerhard reitmayr removed Node and made everything TreeNodes
//     Updated code of method 'getState'
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


/*@NOTE_3772
Constructor method.
*/
MaxConfidenceFilter::MaxConfidenceFilter() //@INIT_3772
    : TreeNode()
{//@CODE_3772
    ConstructorInclude();

    // Put in your own code
}//@CODE_3772


/*@NOTE_331
Destructor method.
*/
MaxConfidenceFilter::~MaxConfidenceFilter()
{//@CODE_331
    DestructorInclude();

    // Put in your own code
}//@CODE_331


/*@NOTE_3588
returns the current state of the node. This method will be overriden in any subclasses
that provide real functionality. All processing of states will be done here.
*/
State* MaxConfidenceFilter::getState()
{//@CODE_3588
    State* value, * comp;
    if(children.empty())
    {
        value = NULL;
    } else 
    {
        TreeNodeVector::iterator it = children.begin();
        value = (*it)->getState();
        it++;
        while( it != children.end())
        {
            comp = (*it)->getState();
            if( value == NULL )
            {
                value = comp;
            }
            else 
            {
                if( comp != NULL )
                {
                    if( !value->isValid && comp->isValid )
                    {
                        value = comp;
                    }    
                    else if( comp->isValid && ( value->confidence < comp->confidence ))
                    {
                        value = comp;
                    }   
                }
            }
            it++;
        }
    }
    return value;
}//@CODE_3588


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_330
Method which must be called first in a constructor.
*/
void MaxConfidenceFilter::ConstructorInclude()
{
}


/*@NOTE_332
Method which must be called first in a destructor.
*/
void MaxConfidenceFilter::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
