// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      NodeFactory.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'NodeFactory'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 25, 2000 13:44 gr added NetworkSource classes
//     Updated return type of method 'createNode'
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
    virtual TreeNode* createNode(char* const name, StringMap& attributes) = 0;
};

#endif


#ifdef CB_INLINES
#ifndef _NODEFACTORY_H_INLINES
#define _NODEFACTORY_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
