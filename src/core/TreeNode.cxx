 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** source file for the TreeNode class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/TreeNode.cxx,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#include "TreeNode.h"

#ifdef __sgi
#include <algo.h>
#else
#include <algorithm>
#endif

/// Destructor method.

TreeNode::~TreeNode()
{
    children.clear();
}

/// adds a child to the Node.

void TreeNode::addChild(Node& node)
{
    NodeVector::iterator result = std::find( children.begin(), children.end(), &node );
    if( result == children.end())
    {
        children.push_back( &node );
    }
}

/// returns a reference to the internal children field

NodeVector& TreeNode::getChildren()
{
    return children;
}

/// removes a child from the Node

void TreeNode::removeChild(Node& node)
{
    NodeVector::iterator result = std::find( children.begin(), children.end(), &node );
    if( result != children.end())
    {
        children.erase( result );
    }
}
