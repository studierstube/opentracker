 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for the ConfigNode class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/ConfigNode.h,v 1.4 2001/03/26 22:11:21 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _CONFIGNODE_H
#define _CONFIGNODE_H

#include "../dllinclude.h"

#include <map>

#include "Node.h"

/**
 * maps a string to another string. Mostly used to map element attributes
 * to values.
 */
typedef OPENTRACKER_API std::map<string, string> StringMap;

/**
 * This node implements a simple node containing the elements name and
 * a string map with the attributes. It is used in the node trees passed
 * to the init section of the Modules, to add any more complicated 
 * initialization.
 * @author Gerhard Reitmayr
 * @ingroup core
*/
class OPENTRACKER_API ConfigNode : public Node
{
// Members
protected:
    /// the wrapper element name
    StringMap * attributes;
// Methods
public:
    /**
     * constructor method
     * @param name_ element name of the config node
     * @param map_ string map with attribute values 
     */
    ConfigNode(const string& name_, StringMap * map_)
        : Node()
	    , attributes( map_ )
    { 
        setName( name_ );
    };

    /// destructor method
    virtual ~ConfigNode()
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

	virtual unsigned int countChildren()
	{
		return Node::countChildren();
	}

	virtual Node * getChild( unsigned int index )
	{
		return Node::getChild( index );
	}
};

#endif
