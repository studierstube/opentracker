 /* ========================================================================
  * (C) 2000 Vienna University of Technology
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for NodeFactory interface.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/NodeFactory.h,v 1.2 2001/01/29 17:16:44 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _NODEFACTORY_H
#define _NODEFACTORY_H

#include <map>

using namespace std;

#include "Node.h"

/**
 * maps a string to another string. Mostly used to map element attributes
 * to values.
 */
typedef std::map<string, string> StringMap;

/**
 * interface class for node factories. These ruct new nodes based on
 * the xml element definitions. Any new module must provide a factory, so
 * the parser can create nodes for its configuration file elements.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class NodeFactory
{
public:
    /**
     * This method tries to create a new Node based on a given
     * element type and the attributes of the element. If it can
     * not create a new Node ( because it doesn't know about the
     * element type for example ), it returns NULL. This method
     * has to be implemented by real NodeFactories.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringMap& attributes) = 0;
};

#endif
