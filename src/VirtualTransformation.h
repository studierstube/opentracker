// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      VirtualTransformation.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'VirtualTransformation'
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
#ifndef _VIRTUALTRANSFORMATION_H
#define _VIRTUALTRANSFORMATION_H


//@START_USER1
//@END_USER1


/*@NOTE_280
a VirtualTransformation acts differently from a common Transformation. It is a local
transformation of the states coordinate system.
*/
class VirtualTransformation
    : public Transformation
{

//@START_USER2
//@END_USER2

// Members
private:

protected:

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:
    virtual State* transformState(State* state);

public:
    VirtualTransformation(float translation_[3], float scale_[3],
                          float rotation_[4]);
    virtual ~VirtualTransformation();
};

#endif


#ifdef CB_INLINES
#ifndef _VIRTUALTRANSFORMATION_H_INLINES
#define _VIRTUALTRANSFORMATION_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
