// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      Transformation.cxx
//  TYPE:      cxx header file
//  PROJECT:   TrackerServer
//  CONTENT:   Implementation of class 'Transformation'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 16, 2000 22:44 gr, added localState to Transformation
//     Added member 'localState'
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


/*@NOTE_3762
Constructor method.
*/
Transformation::Transformation(float translation_[3], float scale_[3],
                               float rotation_[4]) //@INIT_3762
    : TreeNode()
    , localState()
{//@CODE_3762
    ConstructorInclude();
    for( int i = 0; i < 3; i ++ )
    {
        this->translation[i] = translation_[i];
        this->scale[i] = scale_[i];
        this->rotation[i] = rotation_[i];
    }
    this->rotation[3] = rotation_[3];
    // Put in your own code
}//@CODE_3762


/*@NOTE_258
Destructor method.
*/
Transformation::~Transformation()
{//@CODE_258
    DestructorInclude();

    // Put in your own code
}//@CODE_258


/*@NOTE_4551
returns the rotational part of the Transformation
*/
float* Transformation::getRotation()
{//@CODE_4551
    return rotation;
}//@CODE_4551


/*@NOTE_4544
returns the scaling part of the Transformation
*/
float* Transformation::getScale()
{//@CODE_4544
    return scale;
}//@CODE_4544


/*@NOTE_3557
returns the current state of the node. This method will be overriden in any subclasses
that provide real functionality. All processing of states will be done here.
*/
State* Transformation::getState()
{//@CODE_3557
    if( !children.empty())
    {
        TreeNodeVector::iterator it = children.begin();
        State *value = (*it)->getState();
        if( value != NULL )
        {
            return transformState( value );
        }
    }
    return NULL;
}//@CODE_3557


/*@NOTE_4537
returns the translational part of this Transformation
*/
float* Transformation::getTranslation()
{//@CODE_4537
    return translation;
}//@CODE_4537


/*@NOTE_3367
transforms a state. Simplifies implementing other Transformations as it is only needed to
override this method. It returns the pointer passed to the method.
*/
State* Transformation::transformState(State* state)
{//@CODE_3367
  // float qResult[4];   
    // transform the position of the state
    MathUtils::rotateVector( rotation,  state->position, localState.position );
    localState.position[0] = localState.position[0]*scale[0] + translation[0];
    localState.position[1] = localState.position[1]*scale[1] + translation[1];
    localState.position[2] = localState.position[2]*scale[2] + translation[2];
    // transform the orientation of the state
    MathUtils::multiplyQuaternion( rotation, state->orientation, localState.orientation );
    /*
    state->orientation[0] = qResult[0];
    state->orientation[1] = qResult[1];
    state->orientation[2] = qResult[2];
    state->orientation[3] = qResult[3];
    */
    // copy other state fields
    localState.button = state->button;
    localState.isValid = state->isValid;
    localState.confidence = state->confidence;
    return &localState;    
}//@CODE_3367


//{{AFX DO NOT EDIT CODE BELOW THIS LINE !!!

/*@NOTE_257
Method which must be called first in a constructor.
*/
void Transformation::ConstructorInclude()
{
}


/*@NOTE_259
Method which must be called first in a destructor.
*/
void Transformation::DestructorInclude()
{
}


// Methods for the relation(s) of the class

//}}AFX DO NOT EDIT CODE ABOVE THIS LINE !!!

//@START_USER3
