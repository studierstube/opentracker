// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      TestModule.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'TestModule'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 18, 2000 23:04 gr, changed names and added init flag
//     Added method 'TestModule'
// August 16, 2000 22:10 gerhard reitmayr removed Node and made everything TreeNodes
//     Updated member 'nodes'
// August 16, 2000 21:43 gerhard reitmayr
//     Update comment header
// ===========================================================================
#ifndef _TESTMODULE_H
#define _TESTMODULE_H


//@START_USER1
//@END_USER1


/*@NOTE_4805
the module and factory to drive the test source nodes.
*/
class TestModule
    : public Module
    , public NodeFactory
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    TreeNodeVector nodes;
    int cycle;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    TestModule();
    virtual ~TestModule();
    virtual TreeNode* createNode(char* const name, StringMap& attributes);
    virtual void update();
};

#endif


#ifdef CB_INLINES
#ifndef _TESTMODULE_H_INLINES
#define _TESTMODULE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
