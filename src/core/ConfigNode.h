 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for the ConfigNode class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/ConfigNode.h,v 1.1 2000/12/11 10:46:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _CONFIGNODE_H
#define _CONFIGNODE_H

#include <map>

#include "TreeNode.h"

/**
 * maps a string to another string. Mostly used to map element attributes
 * to values.
 */
typedef std::map<string, string> StringMap;

/**
 * This node implements a simple node containing the elements name and
 * a string map with the attributes. It is used in the node trees passed
 * to the init section of the Modules, to add any more complicated 
 * initialization.
 * @author Gerhard Reitmayr
*/
class ConfigNode : public TreeNode
{
// Members
protected:
    /// the wrapper element name
    StringMap * attributes;
// Methods
public:
    /**
     * constructor method
     * @param tagName_ the wrapper tag element name for this WrapperNode */
    ConfigNode(string& name_, StringMap * map_)
        : TreeNode()
        , name( name_ )
	, attributes( map_ )
	
    {};
    /// destructor method
    virtual ~WrapperNode()
    {
        delete attributes;
    };
    /**
     * returns a reference to the attribute StringMap
     * @return reference to the attribute StringMap */
    StringMap & getAttributes()
    {
        return *attributes;
    }
};

#endif
