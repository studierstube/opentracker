 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for the WrapperNode class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/Attic/WrapperNode.h,v 1.2 2001/01/29 17:16:44 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _WRAPPERNODE_H
#define _WRAPPERNODE_H

#include "TreeNode.h"

/**
 * This node implements a wrapper node. Its just there to signal the existence
 * of a wrapper tag. Wrapper tags capsulate the nodes children into several
 * groups. The children are then added to the node with the
 * addWrappedChild method, to tell the node to which group the child belongs.
 * @note write explanation of wrapper tags here
 * @author Gerhard Reitmayr
 * @ingroup core
*/
class WrapperNode : public TreeNode
{
// Members
protected:
    /// the wrapper element name
    string tagName;
// Methods
public:
    /**
     * ructor method
     * @param tagName_ the wrapper tag element name for this WrapperNode */
    WrapperNode(string& tagName_)
        : TreeNode()
        , tagName(tagName_)
    {};
    /// destructor method
    virtual ~WrapperNode()
    {};
    /** tests whether the given node is a wrapper node.
     * @return always returns 1 */
    virtual WrapperNode * isWrapperNode()
    {
        return this;
    };
    /** returns the value of the wrapper tag
     * @return string containing wrapper tag value */
     string& getTagName()
    {
        return tagName;
    };
};

#endif
