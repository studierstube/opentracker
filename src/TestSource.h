// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      TestSource.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'TestSource'
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
#ifndef _TESTSOURCE_H
#define _TESTSOURCE_H


//@START_USER1
//@END_USER1


/*@NOTE_4794
implements a simple source that sets its valid flag in regular intervals.
*/
class TestSource
    : public TreeNode
{

//@START_USER2
//@END_USER2

// Members
private:

protected:

public:
    int frequency;
    int offset;
    State state;

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    TestSource();
    virtual ~TestSource();
    virtual State* getState();
};

#endif


#ifdef CB_INLINES
#ifndef _TESTSOURCE_H_INLINES
#define _TESTSOURCE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
