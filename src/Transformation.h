// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      Transformation.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'Transformation'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 16, 2000 22:44 gr, added localState to Transformation
//     Added member 'localState'
// August 16, 2000 21:43 gerhard reitmayr
//     Update comment header
// ===========================================================================
#ifndef _TRANSFORMATION_H
#define _TRANSFORMATION_H


//@START_USER1
//@END_USER1


/*@NOTE_256
A Transform is a simple full coordinate transformation.

Its element has the following attributes :

translation   0 0 0    (T)

scale           1 1 1    (S)

rotationtype (quaternion|matrix|euler) quaternion

rotation        0 0 0 1 (R)

the Transformation is specified by T*S*R.
*/
class Transformation
    : public TreeNode
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    float translation[3];
    float scale[3];
    float rotation[4];
    State localState;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:
    virtual State* transformState(State* state);

public:
    Transformation(float translation_[3], float scale_[3], float rotation_[4]);
    virtual ~Transformation();
    float* getRotation();
    float* getScale();
    virtual State* getState();
    float* getTranslation();
};

#endif


#ifdef CB_INLINES
#ifndef _TRANSFORMATION_H_INLINES
#define _TRANSFORMATION_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
