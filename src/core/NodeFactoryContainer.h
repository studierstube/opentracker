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
/** header file for NodeFactoryContainer class.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/NodeFactoryContainer.h,v 1.4 2001/03/27 06:08:50 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _NODEFACTORYCONTAINER_H
#define _NODEFACTORYCONTAINER_H

#include "../dllinclude.h"

#include "NodeFactory.h"

/**
 * used to keep a list of NodeFactories.
 */
typedef std::vector<NodeFactory*> NodeFactoryVector;

/**
 * An implementation of NodeFactory that contains several NodeFactories and
 * acts like the one providing all node types of the different factories
 * together. When createNode is called it loops through the known
 * NodeFactories and calls createNode on them until it receives a non NULL
 * result. It then returns this node.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API NodeFactoryContainer : public NodeFactory
{

protected:
    /// the list of known NodeFactories
    NodeFactoryVector factories;

public:
    /** Destructor clears the factories list. */
    virtual ~NodeFactoryContainer();
    /**
     * adds a NodeFactory to the container making nodes provided by this
     * factory accessible.
     * @param factory reference to the NodeFactory object */
    void addFactory(NodeFactory& factory);
    /**
     * removes a NodeFactory from the container.
     * @param factory reference to the NodeFactory object */
    void removeFactory(NodeFactory& factory);
    /**
     * implements abstract method from NodeFactory. it loops through the known
     * NodeFactories and calls createNode on them until it receives a non NULL
     * result. It then returns this node. */
    virtual Node * createNode( string& name,  StringMap& attributes);
};

#endif

