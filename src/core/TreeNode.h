 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for the TreeNode class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/TreeNode.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _TREENODE_H
#define _TREENODE_H

#include <vector>

using namespace std;

#include "Node.h"

/**
 * a Vector of Node pointers. Very useful to implement a simple
 * container of Nodes such as a parent node, or to keep pointers
 * to several nodes around.
 */
typedef std::vector<Node*> NodeVector;

/**
 * A node that has a number of children. Some types of TreeNodes will only
 * contain only one child, others several. This class implements the
 * storage part for classes that store several children. The number and
 * type of these is not restricted by the implementation. This is fixed by the
 * DTD of the config file format, so there should be no problems here.
 * @author Gerhard Reitmayr
*/
class TreeNode : public Node
{
// Members
protected:
    /// stores the children of this node
    NodeVector children;

// Methods
public:
    /** ructor method.*/
    TreeNode() : Node()
    {};
    /** Destructor method clears the children member */
    virtual ~TreeNode();
    /** adds a child to the Node.
     * @param node the new child node */
    virtual void addChild( Node& node);
    /** returns a reference to the internal children field,
     * so that the parser can read the children.
     * @return reference to internal children field */
    NodeVector& getChildren() ;
    /** removes a child from the Node
     * @param node reference to the child to be removed */
    void removeChild( Node& node);
};

#endif
