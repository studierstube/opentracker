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
/** header file for NodeFactory interface.
  *
  * @author Gerhard Reitmayr
  *
  * $Header: /scratch/subversion/cvs2svn-0.1236/../cvs/opentracker/src/core/NodeFactory.h,v 1.5 2001/04/08 19:31:09 reitmayr Exp $
  * @file                                                                   */
 /* ======================================================================= */

#ifndef _NODEFACTORY_H
#define _NODEFACTORY_H

#include "../dllinclude.h"

#include "Node.h"
#include "StringTable.h"

/**
 * interface class for node factories. These ruct new nodes based on
 * the xml element definitions. Any new module must provide a factory, so
 * the parser can create nodes for its configuration file elements.
 * @author Gerhard Reitmayr
 * @ingroup core
 */
class OPENTRACKER_API NodeFactory
{
public:
    /**
     * This method tries to create a new Node based on a given
     * element type and the attributes of the element. If it can
     * not create a new Node ( because it doesn't know about the
     * element type for example ), it returns NULL. This method
     * has to be implemented by real NodeFactories.
     * @param name reference to string containing element name
     * @param attributes refenrence to StringTable containing attribute values
     * @return pointer to new Node or NULL. The new Node must be
     *         allocated with new ! */
    virtual Node * createNode( string& name,  StringTable& attributes) = 0;
};

#endif
