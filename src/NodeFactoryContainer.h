// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      NodeFactoryContainer.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'NodeFactoryContainer'
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
#ifndef _NODEFACTORYCONTAINER_H
#define _NODEFACTORYCONTAINER_H


//@START_USER1
//@END_USER1


/*@NOTE_1081
another kind of NodeFactory that collects several NodeFactories and acts
like the one providing all node types of the different factories together. When createNode
is called it loops through the known NodeFactories and calls createNode on them
until it receives a non NULL result. It then returns this node.
*/
class NodeFactoryContainer
    : public NodeFactory
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    NodeFactoryVector factories;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    NodeFactoryContainer();
    virtual ~NodeFactoryContainer();
    void addFactory(NodeFactory& factory);
    virtual TreeNode* createNode(char* const name, StringMap& attributes);
    void removeFactory(NodeFactory& factory);
};

#endif


#ifdef CB_INLINES
#ifndef _NODEFACTORYCONTAINER_H_INLINES
#define _NODEFACTORYCONTAINER_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
