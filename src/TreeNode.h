// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      TreeNode.h
//  TYPE:      h header file
//  PROJECT:   @INSERT_PROJECTNAME
//  CONTENT:   Declaration of class 'TreeNode'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// July 22, 2000 18:55 Gerhard Reitmayr & updated some methods now returning pointers instead of references
//     Update comment header
// ===========================================================================
#ifndef _TREENODE_H
#define _TREENODE_H


//@START_USER1
//@END_USER1


/*@NOTE_66
a node that can contain children. Some types of TreeNodes will only contain on child,
others several. This is not really seperated in the code. However the nodes themselves
will either use only the first child or all of them. Moreover the number of children is
fixed by the DTD of the config file format, so there should be no problems here.
*/
class TreeNode
    : public Node
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    NodeVector children;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    TreeNode();
    virtual ~TreeNode();
    void addChild(Node& node);
    void removeChild(Node& node);
};

#endif


#ifdef CB_INLINES
#ifndef _TREENODE_H_INLINES
#define _TREENODE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
