// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      Node.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'Node'
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
#ifndef _NODE_H
#define _NODE_H


//@START_USER1
//@END_USER1


/*@NOTE_35
a basic tracker tree node
*/
class Node
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

public:
    Node();
    virtual ~Node();
    virtual State* getState() = 0;
};

#endif


#ifdef CB_INLINES
#ifndef _NODE_H_INLINES
#define _NODE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
