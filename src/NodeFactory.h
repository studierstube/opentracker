// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      NodeFactory.h
//  TYPE:      h header file
//  PROJECT:   @INSERT_PROJECTNAME
//  CONTENT:   Declaration of class 'NodeFactory'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// July 22, 2000 18:56 Gerhard Reitmayr & updated some methods now returning pointers instead of references
//     Update comment header
// ===========================================================================
#ifndef _NODEFACTORY_H
#define _NODEFACTORY_H


//@START_USER1
//@END_USER1


/*@NOTE_999
super class for node factories. These construct new nodes based on the xml
element definitions.
*/
class NodeFactory
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
    NodeFactory();
    virtual ~NodeFactory();
    virtual Node* createNode(char* const name, StringMap& attributes) = 0;
};

#endif


#ifdef CB_INLINES
#ifndef _NODEFACTORY_H_INLINES
#define _NODEFACTORY_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
