 /* ========================================================================
  * Copyright (C) 2001  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Gerhard Reitmayr under
  * <reitmayr@ims.tuwien.ac.at> or write to Gerhard Reitmayr,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1090 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for the ConfigNode class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/ConfigNode.h,v 1.5 2001/03/27 06:08:50 reitmayr Exp $
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
