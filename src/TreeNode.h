// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      TreeNode.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'TreeNode'
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
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    TreeNodeVector children;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    TreeNode();
    virtual ~TreeNode();
    void addChild(TreeNode& node);
    virtual State* getState();
    void removeChild(TreeNode& node);
};

#endif


#ifdef CB_INLINES
#ifndef _TREENODE_H_INLINES
#define _TREENODE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
