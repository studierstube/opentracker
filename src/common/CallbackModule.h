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
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ========================================================================
  * PROJECT: OpenTracker
  * ======================================================================== */
/** header file for Callback module.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/common/CallbackModule.h,v 1.1 2001/04/29 16:32:41 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

/**
 * @page module_ref Module Reference
 * @section callbackmodule CallbackModule
 * The Callback module provides @ref callbacknode nodes that call registered 
 * callbacks functions for any event they receive. Each node can call only
 * one function. The nodes are addressed by unique names, set in the 
 * element of that node. After reading the initialization file an application
 * can register callback functions on the read nodes. It doesn't have a 
 * configuration element but reserves the name 'CallbackConfig' for it.
 */

#ifndef _CALLBACKMODULE_H
#define _CALLBACKMODULE_H

#include "../OpenTracker.h"
#include "CallbackNode.h"

typedef std::map<string, Node*> NodeMap;

/**
 * The module and factory to drive the callback nodes. It constructs
 * CallbackNode nodes via the NodeFactory interface and registers
 * callbacks with them after parsing
 * @author Gerhard Reitmayr
 * @ingroup common
 */
class OPENTRACKER_API CallbackModule : public Module, public NodeFactory
{
// Members
protected:
    /// map of strings onto nodes
    NodeMap nodes;

// Methods
public:
    /** constructor method. */
    CallbackModule() : Module(), NodeFactory()
    {};
    /** Destructor method, clears nodes member. */
    virtual ~CallbackModule(){};    
    /** This method is called to construct a new Node. It compares
     * name to the TestSource element name, and if it matches
     * creates a new TestSource node.
     * @param name reference to string containing element name
     * @attributes refenrence to StringMap containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringTable& attributes);
 
    /** sets a callback on a certain node. The node is identified by its
     * unique name. Any present callback function in the node is overwritten
     * by the new one.
     * @param name the unique name of the callback node 
     * @param function the new callback function to set on the node
     */
    void setCallback( const string& name, CallbackFunction * function );
};

#endif
