// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      State.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'State'
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
#ifndef _STATE_H
#define _STATE_H


//@START_USER1
//@END_USER1


/*@NOTE_48
a single state passing through the tree. It encodes the full information of a tracked object.
Not all information has to be used at a certain time.
*/
class State
{

//@START_USER2
//@END_USER2

// Members
private:

protected:

public:
    float position[3];
    float orientation[4];
    unsigned short button;
    int isValid;
    float confidence;

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    State(unsigned short button_ = 0, int isValid_ = 0, float confidence_ = 0);
    virtual ~State();
};

#endif


#ifdef CB_INLINES
#ifndef _STATE_H_INLINES
#define _STATE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
